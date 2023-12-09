#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <cmath>


#include <QPainter>

#include <QtDataVisualization/Q3DTheme>
#include <QtDataVisualization/QCustom3DItem>
#include <QtDataVisualization/QCustom3DLabel>
#include <QtGui/QImage>



#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QHeightMapSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>
#include <QtCore/QPropertyAnimation>

#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>

#include <QGraphicsSceneMouseEvent>
//#include <Qt3DRender/QMouseDevice>
//#include <Qt3DRender/QMouseEventHandler>

#include <Qt3DExtras>
#include <Qt3DExtras/QOrbitCameraController>

#include "heat_map.hpp"
#include "objectmap.hpp"

#include "map_3d.hpp"

using namespace QtDataVisualization;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->w_width, this->w_height);

    map = new HeatMap(pos_map_x, pos_map_y, size_map_x, size_map_y);

    map_3d = new Map_3D(200, 50, 800, 800, size_map_x, size_map_y);

    /*
    map->add_object(100, 400, 60, 5, (int) type_material::GLASS);
    map->add_object(100, 300, 60, 5, (int) type_material::WOOD);
    map->add_object(200, 300, 60, 20, (int) type_material::WOOD);
    map->add_object(100, 200, 60, 5, (int) type_material::DRYWALL);

    map->add_object(500, 600, 60, 5, (int) type_material::IRR_GLASS);
    */
    this->layout()->addWidget(map->get_view());
    draw();

    draw_gradient_panel();

    ui->lineEdit_f->setText(QString::number(map->get_value_f()));
    ui->lineEdit_f->setInputMask("9.99");

    this->layout()->addWidget(map_3d->get_container());
    map_3d->render(map->get_file_texture());
    //QObject::connect(checkboxThreeRight, &QCheckBox::stateChanged,
    //                 modifier, &CustomItemGraph::toggleShadows);
    //Q3DSurface *graph = new Q3DSurface();
    //Qt3DExtras::QMouseEventHandler *mouseEventHandler = new Qt3DExtras::QMouseEventHandler(sceneRoot);
//    QObject::connect(map_3d->get_QSurface3DSeries(), &QSurface3DSeries::selectedPointChanged, this, [layerOneSeries](const QPoint &pos)
//        {
//            if ((pos.x() >= 0) && (pos.y() >= 0)) {
//                QVector3D vector = map_3d->get_QSurface3DSeries()->dataProxy()->itemAt(pos)->position();
//                qDebug() << vector;
//            }
//        });
//    connect(map_3d->graph, &QSurface3DSeries::selectedPointChanged, this, [=](const QString &label){
//            float x, y, z;
//            // Получите координаты x, y, z выбранной точки из label
//            //emit pointSelected(x, y, z);
//        });
    //QMainWindow::
    //connect(map_3d->graph, &QAbstract3DGraph::selectedElementChanged, this, &Map_3D::clicked);
    //connect(map_3d->graph, &QAbstract3DGraph::selectedElementChanged, this, &MainWindow::clicked);
    //connect(map_3d->graph, &QAbstract3DGraph::selectedElementChanged, this, &Map_3D::clicked);


//    connect(ui->pushButton, &QPushButton::clicked, [=](){

//        m_surface->removeCustomItem(m_label);
//        createLabel(ui->lineEdit->text());
//    });
}

MainWindow::~MainWindow()
{
    if(map)
        delete map;
    if(map_3d){
        //delete map_3d;
    }
    delete ui;
}


void MainWindow::draw(){
    if(map){

        map->draw();
        map_3d->render(map->get_file_texture());
    }

}
void MainWindow::mousePressEvent(QMouseEvent *mouse){

    if(mouse->buttons() == Qt::RightButton){
        if( (mouse->pos().x() >= pos_map_x && mouse->pos().x() <= pos_map_x + size_map_x) &&\
            (mouse->pos().y() >= pos_map_y && mouse->pos().y() <= pos_map_y + size_map_y))
        {
            map->set_point(mouse->pos().x() - pos_map_x, mouse->pos().y() - pos_map_y);
            draw();
            //map_3d->render(map->get_file_texture());
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
        //map_3d->render(map->get_file_texture());
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

void MainWindow::clicked(){
    std::cerr<<"Test\n";
    float x, y, z;
    //pos.x();
    //emit pointSelected(x, y, z);
}







