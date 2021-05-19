#include "ipeditwindow.h"
#include "ui_ipeditwindow.h"

IpEditWindow::IpEditWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IpEditWindow)
{
    ui->setupUi(this);
    setWindowTitle("Изменение настроек подключения");
}

IpEditWindow::~IpEditWindow()
{
    delete ui;
}

void IpEditWindow::on_buttonBox_accepted()
{
    QString IP = ui->IP->text();
    QString PORT = ui->PORT->text();
    //Проверка валидности IP адреса
    QHostAddress address(IP);

    if(QAbstractSocket::IPv4Protocol == address.protocol())
    {
       qDebug("Valid IPv4 address");
       bool portConversion;
       int uintPort = PORT.toUInt(&portConversion);
       if(uintPort <= 65535 && portConversion)
       {
           qDebug("%d",uintPort);
           emit acceptAddress(address.toString(),PORT);
           return;
       }
    }
    else
       qDebug("Invalid IPv4 address");
       QMessageBox::critical(this,"Ошибка","Введены некорректные значения!");
}

