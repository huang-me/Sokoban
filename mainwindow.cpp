#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->congrates->setVisible(false);

    // import images
    QPixmap background("/home/pd2/Desktop/2020-pd2-sokoban/imgs/back.jpg");
    QPixmap player_pic("/home/pd2/Desktop/2020-pd2-sokoban/imgs/player_front.png");
    QPixmap movable("/home/pd2/Desktop/2020-pd2-sokoban/imgs/movable.png");
    QPixmap unmovable("/home/pd2/Desktop/2020-pd2-sokoban/imgs/unmovable.png");
    QPixmap end_pic("/home/pd2/Desktop/2020-pd2-sokoban/imgs/endPoint.png");

    // set up the background size and position
    ui->label_bg->setPixmap(background.scaled(480,540));
    ui->player->setPixmap(player_pic.scaled(60,60));

    // put the player in the initial position
    player_x = 120;
    player_y = 120;
    ui->player->setGeometry(player_x,player_x,60,60);

    // unable the buttons
    ui->btns->setEnabled(false);

    // put movable boxes initial place
    ui->box1->setPixmap(movable.scaled(60,60));
    ui->box1->setGeometry(180,120,60,60);
    ui->box2->setPixmap(movable.scaled(60,60));
    ui->box2->setGeometry(240,180,60,60);
    ui->box3->setPixmap(movable.scaled(60,60));
    ui->box3->setGeometry(240,240,60,60);
    ui->box4->setPixmap(movable.scaled(60,60));
    ui->box4->setGeometry(180,360,60,60);
    ui->box5->setPixmap(movable.scaled(60,60));
    ui->box5->setGeometry(60,360,60,60);
    ui->box6->setPixmap(movable.scaled(60,60));
    ui->box6->setGeometry(240,360,60,60);
    ui->box7->setPixmap(movable.scaled(60,60));
    ui->box7->setGeometry(300,360,60,60);

    // set end point
    ui->end1->setPixmap(end_pic.scaled(20,20));
    ui->end1->setGeometry(60+20,120+20,20,20);
    ui->end2->setPixmap(end_pic.scaled(20,20));
    ui->end2->setGeometry(60+20,240+20,20,20);
    ui->end3->setPixmap(end_pic.scaled(20,20));
    ui->end3->setGeometry(180+20,360+20,20,20);
    ui->end4->setPixmap(end_pic.scaled(20,20));
    ui->end4->setGeometry(240+20,300+20,20,20);
    ui->end5->setPixmap(end_pic.scaled(20,20));
    ui->end5->setGeometry(240+20,420+20,20,20);
    ui->end6->setPixmap(end_pic.scaled(20,20));
    ui->end6->setGeometry(300+20,180+20,20,20);
    ui->end7->setPixmap(end_pic.scaled(20,20));
    ui->end7->setGeometry(360+20,360+20,20,20);

    // put unmovable boxes where they should be
    for(int i=0; i<len_unmove; i++) {
        QLabel *tmp = new QLabel(ui->blocks);
        tmp->setPixmap(unmovable.scaled(60,60));
        tmp->setGeometry(unmove[i][0]*60,unmove[i][1]*60,60,60);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
    int tmpx = player_x, tmpy = player_y;
    int memo[10][2];
    // memorize the movable location
    for(int j=0;j<len_move;j++) {
        memo[j][0] = move[j][0];
        memo[j][1] = move[j][1];
    }

    if(e->key() == Qt::Key_Right) {
        player_x += 60; // move right
    }
    else if(e->key() == Qt::Key_Up) {
        player_y -= 60; // move up
    }
    else if(e->key() == Qt::Key_Left) {
        player_x -= 60; // move left
    }
    else if(e->key() == Qt::Key_Down) {
        player_y += 60; // move down
    }

    // push the movable boxes
    for(int i=0;i<len_move;i++) {
        if(player_x == move[i][0] && player_y == move[i][1]) {
            move[i][0] += (player_x-tmpx);
            move[i][1] += (player_y-tmpy);

            int tmpi = i;

            for(int j=0;j<len_move;j++) {
                if(tmpi == j) continue;
                else if(move[j][0] == move[tmpi][0] && move[j][1] == move[tmpi][1]) {
                    move[j][0] += (player_x-tmpx);
                    move[j][1] += (player_y-tmpy);
                    tmpi = j;
                    j = 0;
                }
            }

            // test if movable box is bumped into unmovable box
            for(int j=0;j<len_unmove;j++) {
                if(move[tmpi][0] == unmove[j][0]*60 && move[tmpi][1] == unmove[j][1]*60) {

                    for(int k=0;k<len_move;k++) {
                        move[k][0] = memo[k][0];
                        move[k][1] = memo[k][1];
                    }

                    player_x = tmpx;
                    player_y = tmpy;
                }
            }

            ui->box1->move(move[0][0],move[0][1]);
            ui->box2->move(move[1][0],move[1][1]);
            ui->box3->move(move[2][0],move[2][1]);
            ui->box4->move(move[3][0],move[3][1]);
            ui->box5->move(move[4][0],move[4][1]);
            ui->box6->move(move[5][0],move[5][1]);
            ui->box7->move(move[6][0],move[6][1]);
        }
    }

    // test if player is bumped into unmovable box
    for(int i=0;i<len_unmove;i++) {
        if(player_x == unmove[i][0]*60 && player_y == unmove[i][1]*60) {
            player_x = tmpx;
            player_y = tmpy;
        }
    }

    ui->player->move(player_x,player_y);

    if(testfinish() == 1) showfinish();
}

int MainWindow::testfinish() {
    int done = 0;
    int test[10] = {0,0,0,0,0,0,0,0,0,0};
    for(int i=0;i<len_move;i++) {
        for(int j=0;j<len_move;j++) {
            if(move[i][0] == end[j][0]*60 && move[i][1] == end[j][1]*60) {
                done++;
                test[j] = 1;
            }
        }
    }

    for(int i=0;i<len_move;i++) {
            if(test[i] == 1) {
                switch (i) {
                case 0:
                    ui->end1->setVisible(false);
                    continue;
                case 1:
                    ui->end2->setVisible(false);
                    continue;
                case 2:
                    ui->end3->setVisible(false);
                    continue;
                case 3:
                    ui->end4->setVisible(false);
                    continue;
                case 4:
                    ui->end5->setVisible(false);
                    continue;
                case 5:
                    ui->end6->setVisible(false);
                    continue;
                case 6:
                    ui->end7->setVisible(false);
                    continue;
                }
            }
            else {
                switch (i) {
                case 0:
                    ui->end1->setVisible(true);
                    continue;
                case 1:
                    ui->end2->setVisible(true);
                    continue;
                case 2:
                    ui->end3->setVisible(true);
                    continue;
                case 3:
                    ui->end4->setVisible(true);
                    continue;
                case 4:
                    ui->end5->setVisible(true);
                    continue;
                case 5:
                    ui->end6->setVisible(true);
                    continue;
                case 6:
                    ui->end7->setVisible(true);
                    continue;
                }
            }
       }

//    if(done == 2) return 1;
    if(done == len_move) return 1;
    else return 0;
}

void MainWindow::showfinish() {
    ui->player->move(420,0);
    ui->congrates->setVisible(true);
}
