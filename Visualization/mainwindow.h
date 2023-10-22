#ifndef MAINWINDOW_H
#define MAINWINDOW_H



#include <QMainWindow>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QtWidgets>


class HeatMap;

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
    

    QLabel *label_;
    QGroupBox *gradientGroupBox;
    HeatMap *map;

    void mousePressEvent(QMouseEvent *mouse);
    void draw();
    void draw_gradient_panel();

    
};



#endif // MAINWINDOW_H
