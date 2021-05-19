#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //Настройка UI
    ui->setupUi(this);
    //setupBckgrd();
    ui->centralwidget->setWindowTitle("Авторизация");
    IP = new QString;
    PORT = new QString;

    configureEditFunction();

    //Открытие файла с сохраненными IP и портом
    ipFile = new QFile("ipconf");

    //Файл с логами
//    logFile = new QFile("log.txt");
//    if(logFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
//    {
//        canLogging = true;
//    }
//    else
//        qDebug("Cant logging actions");

    while(!m_open());

    if(ipFile->size() == 0)
    {
        qDebug("Файл пуст. Установка значений по умолчанию");

        logging("Невозможно найти сохраненный адрес. Установка значений по умолчанию...");
        *IP = "127.0.0.1";
        *PORT = "2323";
        saveCurrentAddr();
    }
    else
    {
        *IP = ipFile->readLine();
        //убирает \n В конце
        IP->resize(IP->size()-1);
        *PORT = ipFile->readLine();
        ipFile->close();
    }
    //Вывод текущего сохраненного IP/PORT
    ui->IPStatus->setText(*IP+"/"+*PORT);

    // Настройка клиента
    tcpClient = tcpClient->Instance(*IP,*PORT);
    connect(tcpClient,&TcpClient::succesfulAuth,this,&MainWindow::slotAuthResultTrue);
    connect(tcpClient,&TcpClient::failedAuth,this,&MainWindow::slotAuthResultFalse);

    //Настройка вовзращения в главное меню

}


void MainWindow::editIP()
{
    logging("Изменение IP...");
    IpEditWindow ipEditWindow;
    connect(&ipEditWindow,&IpEditWindow::acceptAddress,this,&MainWindow::newAddress);
    ipEditWindow.setModal(true);
    ipEditWindow.exec();
}

void MainWindow::newAddress(QString ip,QString port){
    *IP = ip;
    *PORT = port;
    ui->IPStatus->setText(*IP+"/"+*PORT);
    tcpClient->changeIPPORT(*IP,*PORT);
    saveCurrentAddr();
}

bool MainWindow::m_open()
{
    //Очистка файла
//    ipFile->open(QIODevice::Truncate | QIODevice::WriteOnly);
//    ipFile->close();

    if(!ipFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug("File open error...");
        if(!ipFile->exists())
        {
            qDebug("File does not exist...");
            qDebug("Trying to create a file...");
            if(!ipFile->open(QIODevice::WriteOnly | QIODevice::Text))
            {
                  qDebug("File creating error...");
            }
            else
            {
                ipFile->close();
                qDebug("File was succesfully created");
            }
        }
        return false;
    }
    else
    {
        qDebug("File was opened!");
        return true;
    }

}

void MainWindow::saveCurrentAddr()
{
    if(ipFile->isOpen())
       ipFile->close();
    ipFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
    ipFile->write(IP->toUtf8()+'\n');
    ipFile->write(PORT->toUtf8());
    ipFile->close();
    logging("Новый IP адрес сохранен.");
}

void MainWindow::logging(QString str)
{
    QTime time;
    QDate date;
    ui->logText->append('['+date.currentDate().toString("dd.MM.yyyy")+' '+time.currentTime().toString()+"]: "+str);
    FileLogger::log(str);
}

void MainWindow::setupBckgrd()
{
    QPixmap bg("C:/Users/silen/Desktop/stolb.jpg");
    bg = bg.scaled(size(),Qt::IgnoreAspectRatio);
    QPalette p = palette();
    p.setBrush(QPalette::Background,bg);
    setPalette(p);
}

void MainWindow::on_connectButton_clicked()
{
    if(tcpClient->connectToServer(*IP,*PORT))
    {
        QVector<QString> vec;
        vec.push_back(ui->loginEdit->text().toLocal8Bit().data());
        vec.push_back(ui->passwordEdit->text().toLocal8Bit().data());
        emit tcpClient->sendMessageSignal(1,vec);
    }
}

void MainWindow::configureEditFunction()
{
    //Настройка кнопки изменения IP
    ipPortSettings = new QAction("Настройка Адреса");
    ipPortSettings->setText("Изменить адрес");

    //Горячие клавиши изменения IP - CTRL+E
    ipPortSettings->setShortcut(QKeySequence("CTRL+E"));
    ipPortSettings->setStatusTip("Изменить IP и порт сервера");
    ui->adrMenu->addAction(ipPortSettings);
    connect(ipPortSettings,SIGNAL(triggered()),SLOT(editIP()));
}
MainWindow::~MainWindow()
{
    if(ipFile->isOpen())
    {
        ipFile->close();
        qDebug("ip file was closed.");
    }
    if(logFile->isOpen())
    {
        logFile->close();
        qDebug("log file was closed");
    }
    delete ui;
}

void MainWindow::slotAuthResultFalse()
{

    QMessageBox::warning(this,"Авторизация","Логин или пароль введен неверно!");
    logging("Логин или пароль введен неверно.");
}

void MainWindow::slotAuthResultTrue(QVector<QString> data)
{
    this->hide();
    logging("Авторизация пользователя " + data[1]);
    dbWindow = new DBWindow();
    connect(dbWindow, &DBWindow::returnBackSignal,this, &MainWindow::slotReturnThisWindow);
    if(!dbWindow->configureDatabase(data))
    {
        logging("База данных не была открыта!");
    }
    else
        dbWindow->show();
    //dbWindow.exec();
}
void MainWindow::slotReturnThisWindow()
{
    this->show();
    dbWindow->deleteLater();
}
