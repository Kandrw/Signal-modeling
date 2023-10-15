#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <thread>

#include <QMainWindow>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QtWidgets>


#define COUNT_THREAD 6


#define METHOD2

class Map;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_lineEdit_f_textEdited(const QString &arg1);

private:
    Ui::MainWindow *ui;

public:

    int w_height = 1000, w_width = 1200;
    int pos_map_x = 180, pos_map_y = 0;
    int size_map_x = 1000, size_map_y = 1000;
    int pos_point_x = 100, pos_point_y = 100;
    double tx_power = 23;//mw
    double ant_power = 6;//коэф усиления
    double val_f = 2.4;

    QPixmap *pixmap;
    QGraphicsScene* scene;
    QGraphicsView *view;

    QLabel *label_;
    QGroupBox *gradientGroupBox;
    Map *map;

    void mousePressEvent(QMouseEvent *mouse);
    void draw();
    void draw_gradient();

    std::thread threads[COUNT_THREAD];
};



#endif // MAINWINDOW_H
