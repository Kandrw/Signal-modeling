#include "map_3d.hpp"

#include <iostream>

#include <QtDataVisualization/Q3DTheme>
#include <QtDataVisualization/QCustom3DItem>
#include <QtDataVisualization/QCustom3DLabel>
#include <QtGui/QImage>
#include <QtDataVisualization/QAbstract3DGraph>


#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QHeightMapSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>
#include <QtCore/QPropertyAnimation>

#include <QtWidgets>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>




using namespace QtDataVisualization;



Map_3D::Map_3D(int pos_x, int pos_y, int size_x, int size_y, int size_map_x, int size_map_y)\
    : pos_x(pos_x), pos_y(pos_y), size_x(size_x), size_map_x(size_map_x), size_map_y(size_map_y)
{
    graph = new Q3DSurface();
    graph->setSelectionMode(QAbstract3DGraph::SelectionRowAndColumn);
    /*
    graph->axisX()->setLabelFormat("%.1f N");
    graph->axisZ()->setLabelFormat("%.1f E");
    graph->axisX()->setRange(34.0f, 40.0f);
    graph->axisY()->setRange(0.0f, 200.0f);
    graph->axisZ()->setRange(18.0f, 24.0f);*/
    graph->axisY()->setRange(90.0f, 500.0f);
    container = QWidget::createWindowContainer(graph);
    //container->pos().setX(200);
    //container->pos().setY(200);
    container->setGeometry(pos_x, pos_y, size_x, size_y);
    //container->setMinimumSize(QSize(800, 600));
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);
    //QCustom3DItem()

    //QVector3D positionOne = QVector3D(39.0f, 77.0f, 19.2f);
    QVector3D positionOne = QVector3D(0, 0, 0);

    QImage color = QImage(2, 2, QImage::Format_RGB32);
    color.fill(Qt::red);

    QCustom3DItem *item = new QCustom3DItem(filename1, positionOne,
                                            QVector3D(0.025f, 0.025f, 0.025f),
                                            QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, 45.0f),
                                            color);
    graph->addCustomItem(item);
    QImage layerOneHMap(filename2);
    QHeightMapSurfaceDataProxy *layerOneProxy = new QHeightMapSurfaceDataProxy(layerOneHMap);
    //layerOneProxy->
    layerOneSeries = new QSurface3DSeries(layerOneProxy);
    layerOneSeries->setItemLabelFormat(QStringLiteral("(@xLabel, @zLabel): @yLabel"));
    //layerOneProxy->setValueRanges(34.0f, 40.0f, 18.0f, 24.0f);
    layerOneProxy->setValueRanges(0, size_map_x, 0, size_map_y);

    layerOneSeries->setDrawMode(QSurface3DSeries::DrawSurface);
    layerOneSeries->setFlatShadingEnabled(false);

    graph->addSeries(layerOneSeries);



}


QWidget *Map_3D::get_container(){

    return container;
}
void Map_3D::render(char *file_texture){

    layerOneSeries->setTextureFile("");
    layerOneSeries->setTextureFile(file_texture);
    //layerOneSeries->
    //layerOneSeries->setDrawMode(QSurface3DSeries::DrawSurface);
    //layerOneSeries->setFlatShadingEnabled(false);

}
QSurface3DSeries *Map_3D::get_QSurface3DSeries(){
    return layerOneSeries;
}

void Map_3D::clicked(int type){
    std::cerr<<"Clicked "<<type<<"\n";
}



















