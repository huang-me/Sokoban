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

    // player position
    int player_x, player_y;
    // level now
    int level_num;
    // boxes and steps now
    int len_unmove, len_move, step_count;
    int unmove[35][2], move[7][2], end[7][2];
    // position of the bomb
    int bomb_x, bomb_y;

    // Level_1 map
    int level1_len_unmove = 35, level1_len_move = 7;
    int level1_unmove[35][2] = {{2,0},{3,0},{4,0},{5,0},{6,0},{6,1},{6,2},{6,3},{6,4},{6,5}, \
                         {7,5},{7,6},{7,7},{7,8},{6,8},{5,8},{4,8},{3,8},{2,8},{1,8}, \
                         {0,8},{0,7},{0,6},{0,5},{0,4},{0,3},{0,2},{0,1},{1,1},{2,1}, \
                         {1,3},{2,3},{2,4},{2,5},{3,4}
                        };
    int level1_move[7][2] = {{180,120},{240,180},{240,240},{180,360},{60,360},{240,360},{300,360}
                     };
    int level1_end[7][2] = {
        {1,2},{1,4},{3,6},{4,5},{4,7},{5,3},{6,6}
    };
    int level1_bombx = 360, level1_bomby = 420;

    // Level_2 map
    int level2_len_unmove = 34, level2_len_move = 7;
    int level2_unmove[35][2] = {{0,0},{0,1},{0,2},{0,3},{0,4},{0,5},{0,6},{1,0},{1,6},{2,0}, \
                                {2,3},{2,5},{2,6},{2,7},{2,8},{3,0},{3,8},{4,0},{4,3},{4,5}, \
                                {4,8},{5,0},{5,8},{6,0},{6,1},{6,5},{6,6},{6,7},{6,8},{7,1}, \
                                {7,2},{7,3},{7,4},{7,5}
                        };
    int level2_move[7][2] = {{120,120},{120,240},{240,120},{300,240},{300,360},{240,360},{180,360}
                     };
    int level2_end[7][2] = {
        {1,3},{1,5},{3,3},{3,5},{5,3},{5,5},{6,4}
    };
    int level2_bombx = 360, level2_bomby = 120;

signals:

public slots:
    void set_level1();
    void set_level2();
    void restartLevel();
    void toggleMap();
    void nextLevel();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
