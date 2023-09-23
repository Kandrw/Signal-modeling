#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <cmath>

#include <QPainter>



//#include <QGraphicsScene>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->w_width, this->w_height);

    map = new QPixmap(size_map_x, size_map_y);
    scene = new QGraphicsScene();
    view = new QGraphicsView(scene);
    //view->resize(size_map_x, size_map_y);
    view->setGeometry(pos_map_x, pos_map_y, size_map_x, size_map_y);
    this->layout()->addWidget(view);
    draw();
    draw_gradient();
    ui->lineEdit_f->setText(QString::number(val_f));
    ui->lineEdit_f->setInputMask("9.99");



}

MainWindow::~MainWindow()
{
    delete map;
    delete scene;
    delete ui;
}
int distance_points(int x1, int y1, int x2, int y2){
    return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}
double PL_func(double d, double f){
    return 28.0 + 22.0 * log10(d * 100.0) + 20.0 * log10(f);
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
void MainWindow::draw(){
    QPainter painter(map);
    int x, y;
    painter.setPen(QColor(100, 10, 100, 127));
    for(y = 0; y < size_map_y; ++y){
        for(x = 0; x < size_map_x; ++x){
            double d = distance_points(pos_point_x, pos_point_y, x, y);
            double pl = PL_func(d, val_f);
            double fs = final_signal(tx_power, ant_power, pl);
            painter.setPen(dbm_in_color(fs));
            painter.drawPoint(x, y);
        }
    }
    painter.end();
    scene->addPixmap(*map);
    //view->add


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
