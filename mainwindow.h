#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QPixmap>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent *e);
    int testfinish();
    void showfinish();
    ~MainWindow();

    int player_x, player_y;
    int len_unmove = 35;
    int len_move = 7,step_count;
    int unmove[50][2] = {{2,0},{3,0},{4,0},{5,0},{6,0},{6,1},{6,2},{6,3},{6,4},{6,5}, \
                         {7,5},{7,6},{7,7},{7,8},{6,8},{5,8},{4,8},{3,8},{2,8},{1,8}, \
                         {0,8},{0,7},{0,6},{0,5},{0,4},{0,3},{0,2},{0,1},{1,1},{2,1}, \
                         {1,3},{2,3},{2,4},{2,5},{3,4}
                        };
    int move[10][2] = {{180,120},{240,180},{240,240},{180,360},{60,360},{240,360},{300,360}
                     };
    int end[10][2] = {
        {1,2},{1,4},{3,6},{4,5},{4,7},{5,3},{6,6}
    };

signals:


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
