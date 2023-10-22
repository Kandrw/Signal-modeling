#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <cmath>


#include <QPainter>

#include "heat_map.hpp"
#include "objectmap.hpp"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->w_width, this->w_height);

    map = new HeatMap(pos_map_x, pos_map_y, size_map_x, size_map_y);
    map->add_object(100, 400, 60, 5, (int) type_material::GLASS);
    map->add_object(100, 300, 60, 5, (int) type_material::WOOD);
    map->add_object(200, 300, 60, 20, (int) type_material::WOOD);
    map->add_object(100, 200, 60, 5, (int) type_material::DRYWALL);

    map->add_object(500, 600, 60, 5, (int) type_material::IRR_GLASS);

    this->layout()->addWidget(map->get_view());
    draw();
    draw_gradient_panel();

    ui->lineEdit_f->setText(QString::number(map->get_value_f()));
    ui->lineEdit_f->setInputMask("9.99");
}

MainWindow::~MainWindow()
{
    delete map;
    delete ui;
}


void MainWindow::draw(){
    map->draw();
}
void MainWindow::mousePressEvent(QMouseEvent *mouse){
    if(mouse->buttons() == Qt::RightButton){
        if( (mouse->pos().x() >= pos_map_x && mouse->pos().x() <= pos_map_x + size_map_x) &&\
            (mouse->pos().y() >= pos_map_y && mouse->pos().y() <= pos_map_y + size_map_y))
        {
            map->set_point(mouse->pos().x() - pos_map_x, mouse->pos().y() - pos_map_y);
            draw();
        }
    }
}


void MainWindow::on_lineEdit_f_textEdited(const QString &arg1)
{
    bool ok;
    double f = arg1.toDouble(&ok);
    if(ok && f > 0.0){
        map->set_value_f(f);
        draw();
    }

}
void MainWindow::draw_gradient_panel(){
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









