#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <cmath>


#include <QPainter>

#include "map.hpp"
#include "objectmap.hpp"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->w_width, this->w_height);

    pixmap = new QPixmap(size_map_x, size_map_y);
    scene = new QGraphicsScene();
    view = new QGraphicsView(scene);

    map = new Map(size_map_x, size_map_y);
    map->add_object(100, 400, 60, 5, (int) type_material::GLASS);
    map->add_object(100, 300, 60, 5, (int) type_material::WOOD);
    map->add_object(200, 300, 60, 20, (int) type_material::WOOD);
    map->add_object(100, 200, 60, 5, (int) type_material::DRYWALL);

    map->add_object(500, 600, 60, 5, (int) type_material::IRR_GLASS);

    view->setGeometry(pos_map_x, pos_map_y, size_map_x, size_map_y);
    this->layout()->addWidget(view);
    draw();
    draw_gradient();

    ui->lineEdit_f->setText(QString::number(val_f));
    ui->lineEdit_f->setInputMask("9.99");

}

MainWindow::~MainWindow()
{
    delete pixmap;
    delete scene;
    delete ui;
}


int distance_points(int x1, int y1, int x2, int y2){
    return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}
double PL_func(double d, double f){
    return 28.0 + 22.0 * log10(d * 100) + 20.0 * log10(f);
}
double final_signal(double txp, double antp, double pl){
    return txp + antp - pl;
    return pl;

}

QColor dbm_in_color(double dbm){
    if(dbm > -44.0){
        return QColor(255, 0, 0);
    }
    else if(dbm > -79.0){
        return QColor(255, (-1.0 * (dbm + 44) * 7.2), 0);
    }
    else if(dbm > -99.0){
        return QColor(255 + (12.75 * (dbm+79)), 255, 0);
    }
    else if(dbm > -119.0){
        return QColor(0, 255 + (3.6 * (dbm + 99)), (12.75 * -1.0 * (dbm+99)));
    }
    else if(dbm > -144.0){
        return QColor(0, 183 + (7.32 * (dbm + 119)), 255);
    }
    else{
        return QColor(0, 0, 255);
    }
}
void signal_atten(int **map, double **signal_map, double tx_power,
                  double ant_power, double val_f, int x0, int y0, int x1, int y1)
{
    int x = x0;
    int y = y0;
    int dx = abs(x1 - x);
    int dy = abs(y1 - y);
    int sx = x < x1 ? 1 : -1;
    int sy = y < y1 ? 1 : -1;
    int err = dx - dy;
    double f = 0;
    while (x != x1 || y != y1) {

        int err2 = err * 2;
        if (err2 > -dy) {
            err -= dy;
            x += sx;

        }
        if (err2 < dx) {
            err += dx;
            y += sy;

        }
        switch(map[y][x]){
        case (int)type_material::NO_MATERIAL:
            signal_map[(int)y][x] = (f * -1.0) + (double)final_signal(tx_power, ant_power, PL_func(distance_points(x0, y0, x, y), val_f));

            break;
        default:

            f += attenuation_material[map[y][x]](val_f) / 10.0;
            break;
        }
    }
    signal_map[y][x] = (f * -1.0) + (double)final_signal(tx_power, ant_power, PL_func(distance_points(x0, y0, x, y), val_f));

}

void thread_draw_map(MainWindow *main_win, int start_pos, int step){
    int x, y;

    for(y = 0; y < main_win->size_map_y; ++y){

        for(x = start_pos; x < main_win->size_map_x; x += step){
            switch(main_win->map->get_map()[y][x]){
            case (int)type_material::NO_MATERIAL:
                signal_atten(main_win->map->get_map(), main_win->map->get_map_signal(), main_win->tx_power,
                             main_win->ant_power, main_win->val_f, main_win->pos_point_x, main_win->pos_point_y, x, y);
                break;
            default:
                break;

            }
        }
    }

}

void MainWindow::draw(){

    pixmap->fill( Qt::black);
    QPainter painter(pixmap);
    for(int i = 0; i < COUNT_THREAD; ++i){
        threads[i] = std::thread(thread_draw_map, this, i, COUNT_THREAD);

    }
    for(int i = 0; i < COUNT_THREAD; ++i){
        threads[i].join();
    }
    for(int y = 0; y < size_map_y; ++y){
        for(int x = 0; x < size_map_x; ++x){

            switch(map->get_map()[y][x]){
            case (int)type_material::NO_MATERIAL:
                painter.setPen(dbm_in_color(map->get_map_signal()[y][x]));
                painter.drawPoint(x, y);
                break;

            default:
                painter.setPen(color_material[ map->get_map()[y][x] ]);
                painter.drawPoint(x, y);

                break;

            }
        }
    }
    painter.end();
    scene->addPixmap(*pixmap);

}

void MainWindow::mousePressEvent(QMouseEvent *mouse){

    if(mouse->buttons() == Qt::RightButton){
        if( (mouse->pos().x() >= pos_map_x && mouse->pos().x() <= pos_map_x + size_map_x) &&\
            (mouse->pos().y() >= pos_map_y && mouse->pos().y() <= pos_map_y + size_map_y))
        {
            pos_point_x = mouse->pos().x() - pos_map_x;
            pos_point_y = mouse->pos().y() - pos_map_y;
            draw();
        }
    }
}
void MainWindow::on_lineEdit_f_textEdited(const QString &arg1)
{
    bool ok;
    double f = arg1.toDouble(&ok);
    if(ok && f > 0.0){
        val_f = f;
        draw();
    }

}
void MainWindow::draw_gradient(){
    gradientGroupBox = new QGroupBox(QStringLiteral("Signal Strength"));
    gradientGroupBox->setGeometry(0, 150, 150, 500);
    int height = 400;
    int width = 150;
    int border = 10;
    QLinearGradient gr(0, 0, 1, height - 2 * border);
    gr.setColorAt(1.0f, Qt::blue);
    gr.setColorAt(0.8f, Qt::cyan);
    gr.setColorAt(0.6f, Qt::green);
    gr.setColorAt(0.4f, Qt::yellow);
    gr.setColorAt(0.0f, Qt::red);

    //blue,cyan,green,yellow,red
    QPixmap pm(width, height);
    pm.fill(Qt::transparent);
    QPainter pmp(&pm);
    pmp.setBrush(QBrush(gr));
    pmp.setPen(Qt::NoPen);
    pmp.drawRect(border, border, 35, height - 2 * border);
    pmp.setPen(Qt::black);
    int step = (height - 2 * border) / 10;
    for (int i = 0; i < 11; i++) {
        int yPos = i * step + border;
        pmp.drawLine(border, yPos, 55, yPos);
        pmp.drawText(60, yPos + 2, QString("%1 [dBm]").arg(-44 - (i * 10)));
    }

    label_ = new QLabel();
    label_->setPixmap(pm);

    QVBoxLayout *colorMapVBox = new QVBoxLayout;
    colorMapVBox->addWidget(label_);
    gradientGroupBox->setLayout(colorMapVBox);
    this->layout()->addWidget(gradientGroupBox);
}









