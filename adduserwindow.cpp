#include "adduserwindow.h"
#include "ui_adduserwindow.h"

AddUserWindow::AddUserWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddUserWindow)
{
    ui->setupUi(this);
    setWindowTitle("Добавление пользователя");
}

AddUserWindow::~AddUserWindow()
{
    delete ui;
}

void AddUserWindow::on_buttonBox_accepted()
{
    QVector<QString> vec;
    vec.push_back(ui->loginLine->text());
    vec.push_back(ui->passLine->text());
    vec.push_back(ui->accesLevelLine->text());
    emit acceptData(vec);
}
