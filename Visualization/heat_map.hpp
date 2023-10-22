#ifndef HEAT_MAP_HPP
#define HEAT_MAP_HPP

#include <thread>

#include <QPainter>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QtWidgets>

#define COUNT_THREAD 4


class QColor;
class PropagationModel;

class HeatMap
{
public:
    HeatMap(int posx, int posy, int x, int y);
    ~HeatMap();
    int add_object(int posx, int posy, int sizex, int sizey, int type);
    int delete_object(int posx, int posy, int sizex, int sizey);
    int** get_map();
    double** get_map_signal();

    void set_point(int x, int y);
    void calculation_signal_by_Brezenham(int x1, int y2);
    int get_size_x();
    int get_size_y();
    double get_value_f();
    void set_value_f(double f);

    void signal_calculation();
    void draw();
    QGraphicsView *get_view();
private:
    int **map;
    double **map_signal;
    int size_x, size_y;
    int pos_point_x = 100, pos_point_y = 100;
    double tx_power = 23;//mw
    double ant_power = 6;//коэф усиления
    double val_f = 2.4;

    std::thread threads[COUNT_THREAD];
    QColor dbm_in_color(double dbm);


    QPixmap *pixmap;
    QGraphicsScene* scene;
    QGraphicsView *view;

    PropagationModel *propModel;


};

#endif // HEAT_MAP_HPP
