#include "widget.h"
#include "ui_widget.h"
#include <time.h>
#include <QMessageBox>
#include <QDebug>
#include <QPainter>
#include <QKeyEvent>
#include <QLabel>
#include <algorithm>
#include <QPushButton>
#include <vector>
#include <time.h>
#include <cstdlib>      // std::rand, std::srand
#include "search_algorithm.h"
#include <iostream>
#include "warning.h"

using namespace std;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    blank_x(1),blank_y(1),
    fresh_ms(30),move_ms(200),matrix{{7,2,4},{5,0,6},{8,3,1}}

{


    ui->setupUi(this);
    fresh_timer = startTimer(fresh_ms);
    move_timer = startTimer(move_ms);

    QFont font;
    font.setFamily("Consolas");
    font.setBold(true);
    font.setPixelSize(30);

    title = new QLabel("Eight Puzzle",this);
    title->setGeometry(SIDE_MARGIN,SIDE_MARGIN,200, 60);
    title->setFont(font);
    title->setAlignment(Qt::AlignCenter);

    font.setPixelSize(17);
    font.setBold(false);
    tip = new QLabel("tip:you can use keyboard to move the blank",this);
    tip->setFont(font);
    tip->setGeometry(SIDE_MARGIN + 5,TOP_MARGIN + 4 * FILLER + COUNT * BLOCK_SIZE + 5,
                     400,20);
    tip->setAlignment(Qt::AlignLeft);

    font.setPixelSize(15);
    font.setBold(false);
    answer_btn = new QPushButton("Answer", this);
    answer_btn->setFont(font);
    answer_btn->setGeometry(SIDE_MARGIN + 3 * FILLER + 2 * BLOCK_SIZE,
                             10, BLOCK_SIZE - 10,20);
    connect(answer_btn,SIGNAL(clicked(bool)),this,SLOT(ShowAnswer()));

    restart_btn = new QPushButton("Random", this);
    restart_btn->setFont(font);
    restart_btn->setGeometry(SIDE_MARGIN + 3 * FILLER + 2 * BLOCK_SIZE,
                             40, BLOCK_SIZE - 10,20);
    connect(restart_btn,SIGNAL(clicked()),this,SLOT(Restart()));

    input_btn = new QPushButton("Reset",this);
    input_btn->setFont(font);
    input_btn->setGeometry(SIDE_MARGIN + 3 * FILLER + 2 * BLOCK_SIZE,
                           70, BLOCK_SIZE - 10,20);
    connect(input_btn,SIGNAL(clicked(bool)),this,SLOT(Input()));

    resize(2*SIDE_MARGIN + 4 * FILLER + COUNT * BLOCK_SIZE,
           TOP_MARGIN + 4 * FILLER + COUNT * BLOCK_SIZE + SIDE_MARGIN + 20);
}
void Widget::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(255,250,240));
    painter.drawRect(rect());
    painter.setBrush(QColor(210,180,140));
    painter.drawRect(SIDE_MARGIN,TOP_MARGIN,4 * FILLER + COUNT * BLOCK_SIZE,
                     4 * FILLER + COUNT * BLOCK_SIZE);
    painter.setBrush(QColor(250,235,215));
    for(int i = 0; i != 3; i++){
        for(int j = 0; j != 3; j++){
            painter.drawRect(SIDE_MARGIN + FILLER + i * (BLOCK_SIZE + FILLER),
                             TOP_MARGIN + FILLER + j * (BLOCK_SIZE + FILLER),
                             BLOCK_SIZE,BLOCK_SIZE);
        }
    }

    QFont font;
    font.setFamily("Consolas");
    font.setPixelSize(40);
    font.setBold(true);
    painter.setFont(font);
    painter.setPen(QColor::fromRgb(0, 0, 0));
    for(int i = 0; i != 3; i++){
        for(int j = 0; j != 3; j++)
            if(matrix[i][j]){
                painter.drawText(QRectF(SIDE_MARGIN + FILLER + j * (BLOCK_SIZE + FILLER),
                                        TOP_MARGIN + FILLER + i * (BLOCK_SIZE + FILLER),
                                        BLOCK_SIZE,BLOCK_SIZE),Qt::AlignCenter,
                                 QString::number(matrix[i][j]));
             }
    }


}
void Widget::timerEvent(QTimerEvent *event){
    if(event->timerId() == fresh_timer)
        update();
    if(event->timerId() == move_timer)
        if(iter < int(path.size())){
            move(path[iter]);
            iter++;
        }

}

void Widget::keyPressEvent(QKeyEvent *event){
    switch(event->key()) {
    case Qt::Key_Up:
        move(UP);
        break;
    case Qt::Key_Down:
        move(DOWN);
        break;
    case Qt::Key_Left:
        move(LEFT);
        break;
    case Qt::Key_Right:
        move(RIGHT);
        break;
    default:
        break;
    }
}

Widget::~Widget()
{
    delete ui;
}
bool myRandom(int i){return rand() % i;}
bool Widget::isValid(vector<int>& v){
    vector<int> v1(v);
    sort(v1.begin(), v1.end());
    for(int i = 0; i != v1.size(); i++)
        if(v1[i] != i)
            return false;
    int sum = 0;
    for(int i = 0; i != v.size() - 1; i++){
        if(v[i] == 0)
            continue;
        for(int j = i + 1; j != v.size(); j++)
            if(v[i] < v[j])
                sum++;
    }
    return (sum + 1) % 2;
}

void Widget::Restart(){
    srand(time(0));
    vector<int> original{7,2,4,5,0,6,8,3,1};
    do{
        random_shuffle(original.begin(), original.end(),myRandom);
    }while(!isValid(original));

    for(int i = 0; i != 3; i++)
        for(int j = 0; j != 3; j++){
            matrix[i][j] = original[3*i + j];
            if(!matrix[i][j]){
                blank_x = j;
                blank_y = i;    
            }
        }

}
void Widget::Input(){
    InputDialog* input = new InputDialog(this);
    input->show();

}

void Widget::ShowAnswer(){
    iter = 0;
    path = solve(matrix);
}

void Widget::move(Direction d){
     switch(d){
        case UP:
            if(blank_y != 0){
                matrix[blank_y][blank_x] = matrix[blank_y - 1][blank_x];
                matrix[blank_y - 1][blank_x] = 0;
                blank_y--;
            }
            break;

        case DOWN:
        if(blank_y != 2){
            matrix[blank_y][blank_x] = matrix[blank_y + 1][blank_x];
            matrix[blank_y + 1][blank_x] = 0;
            blank_y++;
        }
        break;

        case LEFT:
        if(blank_x != 0){
            matrix[blank_y][blank_x] = matrix[blank_y][blank_x - 1];
            matrix[blank_y][blank_x - 1] = 0;
            blank_x--;
        }
        break;

        case RIGHT:
        if(blank_x != 2){
            matrix[blank_y][blank_x] = matrix[blank_y][blank_x + 1];
            matrix[blank_y][blank_x + 1] = 0;
            blank_x++;
        }
        break;

     default:
        break;
    }
}







