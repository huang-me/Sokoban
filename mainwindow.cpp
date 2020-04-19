#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <iostream>
#include <QAction>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // make buttons no use of arrow keys
    ui->RestartBtn->setFocusPolicy(Qt::NoFocus);
    ui->NextlevelBtn->setFocusPolicy(Qt::NoFocus);

    set_level1();

    // connect menu buttons
    connect(ui->actionStart_Game,SIGNAL(triggered()),this,SLOT(set_level1()));
    connect(ui->Level1,SIGNAL(triggered()),this,SLOT(set_level1()));
    connect(ui->Level2,SIGNAL(triggered()),this,SLOT(set_level2()));
    connect(ui->actionQuit_Game,SIGNAL(triggered()),this,SLOT(restartLevel()));
    connect(ui->RestartBtn,SIGNAL(clicked()),this,SLOT(restartLevel()));
    connect(ui->NextlevelBtn,SIGNAL(clicked()),this,SLOT(nextLevel()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
    // if key_esc pressed, restart the game
    if(e->key() == Qt::Key_Escape) {
        restartLevel();
    }

    // if the game haven't done
    if(player_x > 0 && player_y > 0) {
        int tmpx = player_x, tmpy = player_y;

        if(e->key() == Qt::Key_Right) {
            player_x += 60; // move right
            step_count++;
        }
        else if(e->key() == Qt::Key_Up) {
            player_y -= 60; // move up
            step_count++;
        }
        else if(e->key() == Qt::Key_Left) {
            player_x -= 60; // move left
            step_count++;
        }
        else if(e->key() == Qt::Key_Down) {
            player_y += 60; // move down
            step_count++;
        }

        // push the movable boxes
        for(int i=0;i<len_move;i++) {
            if(player_x == move[i][0] && player_y == move[i][1]) {
                move[i][0] += (player_x-tmpx);
                move[i][1] += (player_y-tmpy);

                for(int j=0;j<len_move;j++) {
                    if(i == j) continue;
                    else if(move[j][0] == move[i][0] && move[j][1] == move[i][1]) {
                        move[i][0] -= (player_x-tmpx);
                        move[i][1] -= (player_y-tmpy);
                        player_x = tmpx;
                        player_y = tmpy;
                        step_count--;
                    }
                }

                for(int j=0;j<len_unmove;j++) {
                    if(unmove[j][0]*60 == move[i][0] && unmove[j][1]*60 == move[i][1]) {
                        move[i][0] -= (player_x-tmpx);
                        move[i][1] -= (player_y-tmpy);
                        player_x = tmpx;
                        player_y = tmpy;
                        step_count--;
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
                step_count--;
            }
        }

        ui->player->move(player_x,player_y);
        ui->steps->setNum(step_count);

        // player in the position of bomb
        if(player_x == bomb_x && player_y == bomb_y) {
            toggleMap();
        }

        if(testfinish() == 1) showfinish();
    }
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

    // all the movable boxes are at the end position
    if(done == len_move) return 1;
    else return 0;
}

void MainWindow::showfinish() {
    if(level_num == 1) {
        set_level2();
    }
    else {
        ui->player->move(420,0);
        player_x = -1;
        player_y = -1;
        ui->congrates->setVisible(true);
        ui->game->setEnabled(false);
        ui->unmove->setEnabled(false);
    }
}

void MainWindow::set_level1() {
    // initialize constants
    len_move = level1_len_move;
    len_unmove = level1_len_unmove;
    step_count = 0;
    level_num = 1;
    bomb_x = level1_bombx;
    bomb_y = level1_bomby;

    ui->congrates->setVisible(false);
    ui->game->setEnabled(true);
    ui->steps->setNum(step_count);
    ui->noNext->setVisible(false);

    // import images
    QPixmap background("/home/pd2/Desktop/2020-pd2-sokoban/imgs/back.jpg");
    QPixmap player_pic("/home/pd2/Desktop/2020-pd2-sokoban/imgs/player_front.png");
    QPixmap movable("/home/pd2/Desktop/2020-pd2-sokoban/imgs/movable.png");
    QPixmap unmovable("/home/pd2/Desktop/2020-pd2-sokoban/imgs/unmovable.png");
    QPixmap end_pic("/home/pd2/Desktop/2020-pd2-sokoban/imgs/endPoint.png");
    QPixmap bomb("/home/pd2/Desktop/2020-pd2-sokoban/imgs/bomb.png");

    // setup the bomb
    ui->bomb->setPixmap(bomb.scaled(60,60));
    ui->bomb->setGeometry(bomb_x,bomb_y,60,60);

    // set up the background size and position
    ui->label_bg->setPixmap(background.scaled(480,540));
    ui->player->setPixmap(player_pic.scaled(60,60));

    // put the player in the initial position
    player_x = 120;
    player_y = 120;
    ui->player->setGeometry(player_x,player_y,60,60);
    ui->player->setEnabled(true);

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
    ui->end1->setVisible(true);
    ui->end2->setPixmap(end_pic.scaled(20,20));
    ui->end2->setGeometry(60+20,240+20,20,20);
    ui->end2->setVisible(true);
    ui->end3->setPixmap(end_pic.scaled(20,20));
    ui->end3->setGeometry(180+20,360+20,20,20);
    ui->end3->setVisible(true);
    ui->end4->setPixmap(end_pic.scaled(20,20));
    ui->end4->setGeometry(240+20,300+20,20,20);
    ui->end4->setVisible(true);
    ui->end5->setPixmap(end_pic.scaled(20,20));
    ui->end5->setGeometry(240+20,420+20,20,20);
    ui->end5->setVisible(true);
    ui->end6->setPixmap(end_pic.scaled(20,20));
    ui->end6->setGeometry(300+20,180+20,20,20);
    ui->end6->setVisible(true);
    ui->end7->setPixmap(end_pic.scaled(20,20));
    ui->end7->setGeometry(360+20,360+20,20,20);
    ui->end7->setVisible(true);

    // put unmovable boxes where they should be
    for(int i=0; i<len_unmove; i++) {
        unmove[i][0] = level1_unmove[i][0];
        unmove[i][1] = level1_unmove[i][1];
    }
    ui->unmove1->setPixmap(unmovable.scaled(60,60));
    ui->unmove1->setGeometry(unmove[0][0]*60,unmove[0][1]*60,60,60);
    ui->unmove2->setPixmap(unmovable.scaled(60,60));
    ui->unmove2->setGeometry(unmove[1][0]*60,unmove[1][1]*60,60,60);
    ui->unmove3->setPixmap(unmovable.scaled(60,60));
    ui->unmove3->setGeometry(unmove[2][0]*60,unmove[2][1]*60,60,60);
    ui->unmove4->setPixmap(unmovable.scaled(60,60));
    ui->unmove4->setGeometry(unmove[3][0]*60,unmove[3][1]*60,60,60);
    ui->unmove5->setPixmap(unmovable.scaled(60,60));
    ui->unmove5->setGeometry(unmove[4][0]*60,unmove[4][1]*60,60,60);
    ui->unmove6->setPixmap(unmovable.scaled(60,60));
    ui->unmove6->setGeometry(unmove[5][0]*60,unmove[5][1]*60,60,60);
    ui->unmove7->setPixmap(unmovable.scaled(60,60));
    ui->unmove7->setGeometry(unmove[6][0]*60,unmove[6][1]*60,60,60);
    ui->unmove8->setPixmap(unmovable.scaled(60,60));
    ui->unmove8->setGeometry(unmove[7][0]*60,unmove[7][1]*60,60,60);
    ui->unmove9->setPixmap(unmovable.scaled(60,60));
    ui->unmove9->setGeometry(unmove[8][0]*60,unmove[8][1]*60,60,60);
    ui->unmove10->setPixmap(unmovable.scaled(60,60));
    ui->unmove10->setGeometry(unmove[9][0]*60,unmove[9][1]*60,60,60);
    ui->unmove11->setPixmap(unmovable.scaled(60,60));
    ui->unmove11->setGeometry(unmove[10][0]*60,unmove[10][1]*60,60,60);
    ui->unmove12->setPixmap(unmovable.scaled(60,60));
    ui->unmove12->setGeometry(unmove[11][0]*60,unmove[11][1]*60,60,60);
    ui->unmove13->setPixmap(unmovable.scaled(60,60));
    ui->unmove13->setGeometry(unmove[12][0]*60,unmove[12][1]*60,60,60);
    ui->unmove14->setPixmap(unmovable.scaled(60,60));
    ui->unmove14->setGeometry(unmove[13][0]*60,unmove[13][1]*60,60,60);
    ui->unmove15->setPixmap(unmovable.scaled(60,60));
    ui->unmove15->setGeometry(unmove[14][0]*60,unmove[14][1]*60,60,60);
    ui->unmove16->setPixmap(unmovable.scaled(60,60));
    ui->unmove16->setGeometry(unmove[15][0]*60,unmove[15][1]*60,60,60);
    ui->unmove17->setPixmap(unmovable.scaled(60,60));
    ui->unmove17->setGeometry(unmove[16][0]*60,unmove[16][1]*60,60,60);
    ui->unmove18->setPixmap(unmovable.scaled(60,60));
    ui->unmove18->setGeometry(unmove[17][0]*60,unmove[17][1]*60,60,60);
    ui->unmove19->setPixmap(unmovable.scaled(60,60));
    ui->unmove19->setGeometry(unmove[18][0]*60,unmove[18][1]*60,60,60);
    ui->unmove20->setPixmap(unmovable.scaled(60,60));
    ui->unmove20->setGeometry(unmove[19][0]*60,unmove[19][1]*60,60,60);
    ui->unmove21->setPixmap(unmovable.scaled(60,60));
    ui->unmove21->setGeometry(unmove[20][0]*60,unmove[20][1]*60,60,60);
    ui->unmove22->setPixmap(unmovable.scaled(60,60));
    ui->unmove22->setGeometry(unmove[21][0]*60,unmove[21][1]*60,60,60);
    ui->unmove23->setPixmap(unmovable.scaled(60,60));
    ui->unmove23->setGeometry(unmove[22][0]*60,unmove[22][1]*60,60,60);
    ui->unmove24->setPixmap(unmovable.scaled(60,60));
    ui->unmove24->setGeometry(unmove[23][0]*60,unmove[23][1]*60,60,60);
    ui->unmove25->setPixmap(unmovable.scaled(60,60));
    ui->unmove25->setGeometry(unmove[24][0]*60,unmove[24][1]*60,60,60);
    ui->unmove26->setPixmap(unmovable.scaled(60,60));
    ui->unmove26->setGeometry(unmove[25][0]*60,unmove[25][1]*60,60,60);
    ui->unmove27->setPixmap(unmovable.scaled(60,60));
    ui->unmove27->setGeometry(unmove[26][0]*60,unmove[26][1]*60,60,60);
    ui->unmove28->setPixmap(unmovable.scaled(60,60));
    ui->unmove28->setGeometry(unmove[27][0]*60,unmove[27][1]*60,60,60);
    ui->unmove29->setPixmap(unmovable.scaled(60,60));
    ui->unmove29->setGeometry(unmove[28][0]*60,unmove[28][1]*60,60,60);
    ui->unmove30->setPixmap(unmovable.scaled(60,60));
    ui->unmove30->setGeometry(unmove[29][0]*60,unmove[29][1]*60,60,60);
    ui->unmove31->setPixmap(unmovable.scaled(60,60));
    ui->unmove31->setGeometry(unmove[30][0]*60,unmove[30][1]*60,60,60);
    ui->unmove32->setPixmap(unmovable.scaled(60,60));
    ui->unmove32->setGeometry(unmove[31][0]*60,unmove[31][1]*60,60,60);
    ui->unmove33->setPixmap(unmovable.scaled(60,60));
    ui->unmove33->setGeometry(unmove[32][0]*60,unmove[32][1]*60,60,60);
    ui->unmove34->setPixmap(unmovable.scaled(60,60));
    ui->unmove34->setGeometry(unmove[33][0]*60,unmove[33][1]*60,60,60);
    ui->unmove35->setPixmap(unmovable.scaled(60,60));
    ui->unmove35->setGeometry(unmove[34][0]*60,unmove[34][1]*60,60,60);

    // set the value of movable boxes
    for(int i=0;i<len_move;i++) {
        move[i][0] = level1_move[i][0];
        end[i][0] = level1_end[i][0];
        move[i][1] = level1_move[i][1];
        end[i][1] = level1_end[i][1];
    }
}

void MainWindow::set_level2() {

    // initialize constants
    len_move = level2_len_move;
    len_unmove = level2_len_unmove;
    step_count = 0;
    level_num = 2;
    bomb_x = level2_bombx;
    bomb_y = level2_bomby;

    ui->congrates->setVisible(false);
    ui->game->setEnabled(true);
    ui->unmove->setEnabled(true);
    ui->noNext->setVisible(false);
    ui->steps->setNum(step_count);

    // import images
    QPixmap background("/home/pd2/Desktop/2020-pd2-sokoban/imgs/back.jpg");
    QPixmap player_pic("/home/pd2/Desktop/2020-pd2-sokoban/imgs/player_front.png");
    QPixmap movable("/home/pd2/Desktop/2020-pd2-sokoban/imgs/movable.png");
    QPixmap unmovable("/home/pd2/Desktop/2020-pd2-sokoban/imgs/unmovable.png");
    QPixmap end_pic("/home/pd2/Desktop/2020-pd2-sokoban/imgs/endPoint.png");
    QPixmap bomb("/home/pd2/Desktop/2020-pd2-sokoban/imgs/bomb.png");

    // set up the background size and position
    ui->label_bg->setPixmap(background.scaled(480,540));
    ui->player->setPixmap(player_pic.scaled(60,60));

    // setup the bomb
    ui->bomb->setPixmap(bomb.scaled(60,60));
    ui->bomb->setGeometry(bomb_x,bomb_y,60,60);

    // put the player in the initial position
    player_x = 240;
    player_y = 420;
    ui->player->setGeometry(player_x,player_y,60,60);
    ui->player->setEnabled(true);

    // put movable boxes initial place
    ui->box1->setPixmap(movable.scaled(60,60));
    ui->box1->setGeometry(120,120,60,60);
    ui->box2->setPixmap(movable.scaled(60,60));
    ui->box2->setGeometry(120,240,60,60);
    ui->box3->setPixmap(movable.scaled(60,60));
    ui->box3->setGeometry(240,120,60,60);
    ui->box4->setPixmap(movable.scaled(60,60));
    ui->box4->setGeometry(300,240,60,60);
    ui->box5->setPixmap(movable.scaled(60,60));
    ui->box5->setGeometry(300,360,60,60);
    ui->box6->setPixmap(movable.scaled(60,60));
    ui->box6->setGeometry(240,360,60,60);
    ui->box7->setPixmap(movable.scaled(60,60));
    ui->box7->setGeometry(180,360,60,60);

    // set end point
    ui->end1->setPixmap(end_pic.scaled(20,20));
    ui->end1->setGeometry(60+20,180+20,20,20);
    ui->end1->setVisible(true);
    ui->end2->setPixmap(end_pic.scaled(20,20));
    ui->end2->setGeometry(60+20,300+20,20,20);
    ui->end2->setVisible(true);
    ui->end3->setPixmap(end_pic.scaled(20,20));
    ui->end3->setGeometry(180+20,180+20,20,20);
    ui->end3->setVisible(true);
    ui->end4->setPixmap(end_pic.scaled(20,20));
    ui->end4->setGeometry(180+20,300+20,20,20);
    ui->end4->setVisible(true);
    ui->end5->setPixmap(end_pic.scaled(20,20));
    ui->end5->setGeometry(300+20,180+20,20,20);
    ui->end5->setVisible(true);
    ui->end6->setPixmap(end_pic.scaled(20,20));
    ui->end6->setGeometry(300+20,300+20,20,20);
    ui->end6->setVisible(true);
    ui->end7->setPixmap(end_pic.scaled(20,20));
    ui->end7->setGeometry(360+20,240+20,20,20);
    ui->end7->setVisible(true);

    // put unmovable boxes where they should be
    for(int i=0; i<len_unmove; i++) {
        unmove[i][0] = level2_unmove[i][0];
        unmove[i][1] = level2_unmove[i][1];
    }

    ui->unmove1->setPixmap(unmovable.scaled(60,60));
    ui->unmove1->setGeometry(unmove[0][0]*60,unmove[0][1]*60,60,60);
    ui->unmove2->setPixmap(unmovable.scaled(60,60));
    ui->unmove2->setGeometry(unmove[1][0]*60,unmove[1][1]*60,60,60);
    ui->unmove3->setPixmap(unmovable.scaled(60,60));
    ui->unmove3->setGeometry(unmove[2][0]*60,unmove[2][1]*60,60,60);
    ui->unmove4->setPixmap(unmovable.scaled(60,60));
    ui->unmove4->setGeometry(unmove[3][0]*60,unmove[3][1]*60,60,60);
    ui->unmove5->setPixmap(unmovable.scaled(60,60));
    ui->unmove5->setGeometry(unmove[4][0]*60,unmove[4][1]*60,60,60);
    ui->unmove6->setPixmap(unmovable.scaled(60,60));
    ui->unmove6->setGeometry(unmove[5][0]*60,unmove[5][1]*60,60,60);
    ui->unmove7->setPixmap(unmovable.scaled(60,60));
    ui->unmove7->setGeometry(unmove[6][0]*60,unmove[6][1]*60,60,60);
    ui->unmove8->setPixmap(unmovable.scaled(60,60));
    ui->unmove8->setGeometry(unmove[7][0]*60,unmove[7][1]*60,60,60);
    ui->unmove9->setPixmap(unmovable.scaled(60,60));
    ui->unmove9->setGeometry(unmove[8][0]*60,unmove[8][1]*60,60,60);
    ui->unmove10->setPixmap(unmovable.scaled(60,60));
    ui->unmove10->setGeometry(unmove[9][0]*60,unmove[9][1]*60,60,60);
    ui->unmove11->setPixmap(unmovable.scaled(60,60));
    ui->unmove11->setGeometry(unmove[10][0]*60,unmove[10][1]*60,60,60);
    ui->unmove12->setPixmap(unmovable.scaled(60,60));
    ui->unmove12->setGeometry(unmove[11][0]*60,unmove[11][1]*60,60,60);
    ui->unmove13->setPixmap(unmovable.scaled(60,60));
    ui->unmove13->setGeometry(unmove[12][0]*60,unmove[12][1]*60,60,60);
    ui->unmove14->setPixmap(unmovable.scaled(60,60));
    ui->unmove14->setGeometry(unmove[13][0]*60,unmove[13][1]*60,60,60);
    ui->unmove15->setPixmap(unmovable.scaled(60,60));
    ui->unmove15->setGeometry(unmove[14][0]*60,unmove[14][1]*60,60,60);
    ui->unmove16->setPixmap(unmovable.scaled(60,60));
    ui->unmove16->setGeometry(unmove[15][0]*60,unmove[15][1]*60,60,60);
    ui->unmove17->setPixmap(unmovable.scaled(60,60));
    ui->unmove17->setGeometry(unmove[16][0]*60,unmove[16][1]*60,60,60);
    ui->unmove18->setPixmap(unmovable.scaled(60,60));
    ui->unmove18->setGeometry(unmove[17][0]*60,unmove[17][1]*60,60,60);
    ui->unmove19->setPixmap(unmovable.scaled(60,60));
    ui->unmove19->setGeometry(unmove[18][0]*60,unmove[18][1]*60,60,60);
    ui->unmove20->setPixmap(unmovable.scaled(60,60));
    ui->unmove20->setGeometry(unmove[19][0]*60,unmove[19][1]*60,60,60);
    ui->unmove21->setPixmap(unmovable.scaled(60,60));
    ui->unmove21->setGeometry(unmove[20][0]*60,unmove[20][1]*60,60,60);
    ui->unmove22->setPixmap(unmovable.scaled(60,60));
    ui->unmove22->setGeometry(unmove[21][0]*60,unmove[21][1]*60,60,60);
    ui->unmove23->setPixmap(unmovable.scaled(60,60));
    ui->unmove23->setGeometry(unmove[22][0]*60,unmove[22][1]*60,60,60);
    ui->unmove24->setPixmap(unmovable.scaled(60,60));
    ui->unmove24->setGeometry(unmove[23][0]*60,unmove[23][1]*60,60,60);
    ui->unmove25->setPixmap(unmovable.scaled(60,60));
    ui->unmove25->setGeometry(unmove[24][0]*60,unmove[24][1]*60,60,60);
    ui->unmove26->setPixmap(unmovable.scaled(60,60));
    ui->unmove26->setGeometry(unmove[25][0]*60,unmove[25][1]*60,60,60);
    ui->unmove27->setPixmap(unmovable.scaled(60,60));
    ui->unmove27->setGeometry(unmove[26][0]*60,unmove[26][1]*60,60,60);
    ui->unmove28->setPixmap(unmovable.scaled(60,60));
    ui->unmove28->setGeometry(unmove[27][0]*60,unmove[27][1]*60,60,60);
    ui->unmove29->setPixmap(unmovable.scaled(60,60));
    ui->unmove29->setGeometry(unmove[28][0]*60,unmove[28][1]*60,60,60);
    ui->unmove30->setPixmap(unmovable.scaled(60,60));
    ui->unmove30->setGeometry(unmove[29][0]*60,unmove[29][1]*60,60,60);
    ui->unmove31->setPixmap(unmovable.scaled(60,60));
    ui->unmove31->setGeometry(unmove[30][0]*60,unmove[30][1]*60,60,60);
    ui->unmove32->setPixmap(unmovable.scaled(60,60));
    ui->unmove32->setGeometry(unmove[31][0]*60,unmove[31][1]*60,60,60);
    ui->unmove33->setPixmap(unmovable.scaled(60,60));
    ui->unmove33->setGeometry(unmove[32][0]*60,unmove[32][1]*60,60,60);
    ui->unmove34->setPixmap(unmovable.scaled(60,60));
    ui->unmove34->setGeometry(unmove[33][0]*60,unmove[33][1]*60,60,60);
    ui->unmove35->setVisible(false);

    // set the value of movable boxes
    for(int i=0;i<len_move;i++) {
        move[i][0] = level2_move[i][0];
        end[i][0] = level2_end[i][0];
        move[i][1] = level2_move[i][1];
        end[i][1] = level2_end[i][1];
    }
}

void MainWindow::restartLevel() {
    if(level_num == 1)
        set_level1();
    else
        set_level2();
}

void MainWindow::toggleMap() {
    if(level_num == 1) set_level2();
    else set_level1();
}

void MainWindow::nextLevel() {
    if(level_num == 1) set_level2();
    else ui->noNext->setVisible(true);
}
