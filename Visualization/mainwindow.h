#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QtWidgets>

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
    void mousePressEvent(QMouseEvent *mouse);
    unsigned int w_height = 1000, w_width = 1200;
    unsigned int pos_map_x = 180, pos_map_y = 0;
    unsigned int size_map_x = 1000, size_map_y = 800;
    unsigned int pos_point_x = 1, pos_point_y = 1;
    double tx_power = 23;//mw
    double ant_power = 6;//коэф усиления
    double val_f = 2.4;
    void draw();
    void draw_gradient();
    QPixmap *map;
    QGraphicsScene* scene;
    QGraphicsView *view;

    QLabel *label_;
    QGroupBox *gradientGroupBox;
};



#endif // MAINWINDOW_H
