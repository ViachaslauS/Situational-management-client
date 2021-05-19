#include "disconnectwindow.h"
#include "ui_disconnectwindow.h"

DisconnectWindow::DisconnectWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DisconnectWindow)
{
    ui->setupUi(this);
}

DisconnectWindow::~DisconnectWindow()
{
    delete ui;
}

void DisconnectWindow::badTry()
{
    ui->errLabel->setText("Ошибка подключения");
}

void DisconnectWindow::on_buttonBox_clicked(QAbstractButton *button)
{
    emit retryConnect();
}
