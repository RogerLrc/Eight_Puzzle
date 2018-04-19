#include "inputdialog.h"
#include "ui_inputdialog.h"
#include <string>
#include <vector>
#include "warning.h"
#include "widget.h"
using namespace std;
InputDialog::InputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputDialog)
{
    ui->setupUi(this);
}

InputDialog::~InputDialog()
{
    delete ui;
}

void InputDialog::on_pushButton_clicked()
{
    vector<int> read;
    Widget* father = (Widget*)parentWidget();

    read.push_back(stoi(ui->num1->toPlainText().toStdString()));
    read.push_back(stoi(ui->num2->toPlainText().toStdString()));
    read.push_back(stoi(ui->num3->toPlainText().toStdString()));
    read.push_back(stoi(ui->num4->toPlainText().toStdString()));
    read.push_back(stoi(ui->num5->toPlainText().toStdString()));
    read.push_back(stoi(ui->num6->toPlainText().toStdString()));
    read.push_back(stoi(ui->num7->toPlainText().toStdString()));
    read.push_back(stoi(ui->num8->toPlainText().toStdString()));
    read.push_back(stoi(ui->num9->toPlainText().toStdString()));

    if(!father->isValid(read)){
        Warning* w = new Warning(this);
        w->show();
        ui->num1->clear();
        ui->num2->clear();
        ui->num3->clear();
        ui->num4->clear();
        ui->num5->clear();
        ui->num6->clear();
        ui->num7->clear();
        ui->num8->clear();
        ui->num9->clear();
    }


    for(int i = 0; i != 3; i++)
        for(int j = 0; j != 3; j++){
            father->matrix[i][j] = read[3*i + j];
            if(!father->matrix[i][j]){
                father->blank_x = j;
                father->blank_y = i;
            }
        }
    accept();
}
