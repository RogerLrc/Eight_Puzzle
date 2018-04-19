#ifndef WIDGET_H
#define WIDGET_H
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <vector>
#include "inputdialog.h"
const int TOP_MARGIN = 100;
const int BLOCK_SIZE = 120;
const int COUNT = 3;
const int SIDE_MARGIN = 20;
const int FILLER = 10;

enum Direction{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    virtual void paintEvent(QPaintEvent *event);
    virtual void timerEvent(QTimerEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

    bool isValid(std::vector<int>& v);
    ~Widget();
    std::vector<std::vector<int> > matrix;
    int blank_x, blank_y;

private:
    Ui::Widget *ui;
    QLabel* title;
    QLabel* tip;
    QPushButton* answer_btn;
    QPushButton* restart_btn;
    QPushButton* input_btn;
    QDialog* input;


    void move(Direction d);
    //bool move;
    //int move_count;
    std::vector<Direction> path;
    int iter;


    int fresh_timer;
    int move_timer;
    int fresh_ms;
    int move_ms;

public slots:
    void Restart();
    void ShowAnswer();
    void Input();
};

#endif // WIDGET_H
