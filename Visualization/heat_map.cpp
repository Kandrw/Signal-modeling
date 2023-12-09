#include "heat_map.hpp"

#include <iostream>
#include <cmath>

#include "objectmap.hpp"
#include "propagation_model.hpp"


int distance_points(int x1, int y1, int x2, int y2){
    return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

double final_signal(double txp, double antp, double pl){
    return txp + antp - pl;
    return pl;

}
HeatMap::HeatMap(int posx, int posy, int x, int y) : size_x(x), size_y(y)
{
    map = new int*[size_x];
    map_signal = new double*[size_x];
    for(int i = 0; i < size_y; ++i){
        map[i] = new int[size_x]{};
        map_signal[i] = new double[size_x]{};
    }
    pixmap = new QPixmap(size_x, size_y);
    scene = new QGraphicsScene();
    view = new QGraphicsView(scene);
    view->setGeometry(posx, posy, size_x, size_y);

    propModel = new PropagationModel(UMaLOS, 20, 25);

}
HeatMap::~HeatMap(){

    for(int i = 0; i < size_y; ++i){
        delete[] map[i];
        delete[] map_signal[i];
    }
    delete[] map;
    delete[] map_signal;
    delete pixmap;
    delete view;
    delete scene;
}
QColor HeatMap::dbm_in_color(double dbm){
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


void HeatMap::calculation_signal_by_Brezenham(int x1, int y1)
{
    int x0 = pos_point_x;
    int y0 = pos_point_y;
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
            break;
        default:
            f += attenuation_material[map[y][x]](val_f);
            break;
        }
    }
    get_map_signal()[y][x] = (f * -1.0) + (double)final_signal(tx_power, ant_power, propModel->PL_func(distance_points(x0, y0, x, y), val_f));

}




int HeatMap::add_object(int posx, int posy, int sizex, int sizey, int type){

    if(posx < 0 || posy < 0){
        return -1;
    }
    if(sizex + posx >= this->size_x || sizey + posy >= this->size_y){
        return -1;
    }
    int x, y;
    for(y = posy; y < sizey + posy; ++y){
        for(x = posx; x < sizex + posx; ++x){
            map[y][x] = type;
        }
    }
    return 0;
}
int HeatMap::delete_object(int posx, int posy, int sizex, int sizey){

    if(posx < 0 || posy < 0){
        return -1;
    }
    if(sizex + posx >= this->size_x || sizey + posy >= this->size_y){
        return -1;
    }
    int x, y;
    for(y = posy; y < sizey + posy; ++y){
        for(x = posx; x < sizex + posx; ++x){
            map[y][x] = (int) type_material::NO_MATERIAL;
        }
    }
    return 0;
}
int** HeatMap::get_map(){
    return map;
}
double** HeatMap::get_map_signal(){
    return map_signal;
}
void HeatMap::set_point(int x, int y){
    pos_point_x = x;
    pos_point_y = y;
}
int HeatMap::get_size_x(){
    return size_x;
}
int HeatMap::get_size_y(){
    return size_y;
}
double HeatMap::get_value_f(){
    return val_f;
}
void HeatMap::set_value_f(double f){
    val_f = f;
}
QGraphicsView *HeatMap::get_view(){
    return view;
}
void thread_signal_calculation(HeatMap *heatmap, int start_pos, int step){
    int x, y;
    for(y = 0; y < heatmap->get_size_y(); ++y){

        for(x = start_pos; x < heatmap->get_size_x(); x += step){
            switch(heatmap->get_map()[y][x]){
            case (int)type_material::NO_MATERIAL:
                heatmap->calculation_signal_by_Brezenham(x, y);
                break;
            default:
                break;
            }
        }
    }
}

void HeatMap::signal_calculation(){
    for(int i = 0; i < COUNT_THREAD; ++i){
        threads[i] = std::thread(thread_signal_calculation, this, i, COUNT_THREAD);
    }
    for(int i = 0; i < COUNT_THREAD; ++i){
        threads[i].join();
    }
}


void HeatMap::draw(){
    pixmap->fill( Qt::black);
    QPainter painter(pixmap);
    signal_calculation();
    for(int y = 0; y < size_y; ++y){
        for(int x = 0; x < size_x; ++x){

            switch(get_map()[y][x]){
            case (int)type_material::NO_MATERIAL:
                painter.setPen(dbm_in_color( get_map_signal()[y][x] ));
                painter.drawPoint(x, y);
                break;
            default:
                painter.setPen(color_material[ get_map()[y][x] ]);
                painter.drawPoint(x, y);
                break;
            }
        }
    }
    painter.end();

    //scene->addPixmap(*pixmap);
    pixmap->save(filename_map);

}

char *HeatMap::get_file_texture(){
    return filename_map;
}






