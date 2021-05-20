#include "tcpclient.h"
#include <QTime>
//TcpClient::TcpClient(QObject *parent) : QObject(parent)
//{

//}
TcpClient::TcpClient(QString ip, QString port, QObject* parent) : QObject(parent)
{
    pTcpSocket = new QTcpSocket();
    connect(pTcpSocket,SIGNAL(connected()),SLOT(slotConnected()));
    connect(pTcpSocket,SIGNAL(readyRead()),SLOT(slotReadyRead()));
    connect(pTcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(slotError(QAbstractSocket::SocketError)));
    connect(this,&TcpClient::sendMessageSignal,&TcpClient::slotSendToServer);

    if(ip.size() == 0 || port.size() == 0)
        return;

    connectToServer(ip,port);

}

TcpClient* TcpClient::_tcpClient = 0;

TcpClient* TcpClient::Instance(QString ip, QString port)
{
    if(_tcpClient == 0)
    {
        _tcpClient = new TcpClient(ip,port);
    }
    return _tcpClient;
}

QString TcpClient::getIP()
{
    return IP;
}

QString TcpClient::getPort()
{
    return PORT;
}

void TcpClient::slotReadyRead()
{
    //Поток, в который записываются получаемые данные

    QByteArray receive;
    QDataStream stream(&receive,QIODevice::ReadOnly);
    stream.setVersion(QDataStream::Qt_5_15);

    while(pTcpSocket->bytesAvailable())
    {
        receive = pTcpSocket->readAll();
    }

    QVector<char*> result(10);
    quint16 fullSize;
    uint code, blockSize;

    stream >> fullSize >> code;
    int i = 0;
    while(!stream.atEnd())
    {
        stream >> blockSize;
        stream.readBytes(result[i],blockSize);
        i++;
    }
    //qDebug() << result;
    //qDebug() << code;


    translateMessage(code,result);
//    QDataStream in(pTcpSocket);
//    in.setVersion(QDataStream::Qt_5_15);
//    while(true)
//    {
//        if(!nextBlockSize)
//        {
//            if(pTcpSocket->bytesAvailable()<sizeof(quint16))
//            {
//                break;
//            }
//            in >> nextBlockSize;
//        }

//        if(pTcpSocket->bytesAvailable() < nextBlockSize)
//        {
//            break;
//        }
//        QTime time;
//        QString str;

//        in >>time>> str;

//        nextBlockSize = 0;
//        //qDebug() << in;
//    }
}
void TcpClient::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
            "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                "The host was not found." :
                err == QAbstractSocket::RemoteHostClosedError ?
                "The remote host is closed." :
                err == QAbstractSocket::ConnectionRefusedError ?
                "The connection was refused." :
                QString(pTcpSocket->errorString())
                );
    FileLogger::log(strError);
    emit connectionLost();
}

void TcpClient::slotSendToServer(int code,QVector<QString> args)
{

    QByteArray arrayBlock;
    QDataStream outStream(&arrayBlock, QIODevice::WriteOnly);
    outStream.setVersion(QDataStream::Qt_5_15);

    outStream << quint16(0);

    //Запись кода операции
    outStream << code;
    for(uint i = 0; i<args.size();i++)
    {
        outStream << args[i].size() << args[i].toUtf8();
         //qDebug() <<args[i].size();

    }

    //Запись размера сообщения
    outStream.device()->seek(0);
    outStream << quint16(arrayBlock.size() - sizeof(quint16));

    ///Запись данных в сокет
    pTcpSocket->write(arrayBlock);

    // ПРОВЕРКА ВАЛИДНОСТИ ОТПРАВКИ
//qDebug() << arrayBlock.size();
//    quint16 num;
//    outStream.device()->seek(0);
//    outStream >> num;
//    uint num2, num3;
//    outStream >> num2 >> num3;
//    //num3/=2;
//    //char* str3 = new char(num3);
//    QVector<char*> str3(10);

//    outStream.readBytes(str3[0],num3);
//    qDebug() << "num = " << num <<" code is "<< num2<<"bytes is "<<num3<<" str is: " << str3[0];
//    outStream >> num3;
//    outStream.readBytes(str3[1],num3);
//    qDebug() << num3<<" str is: " << str3[1];

//    qDebug() << str3;

}
void TcpClient::slotConnected()
{
    FileLogger::log("Connected socket to server");
}

void TcpClient::translateMessage(int code, QVector<char *> data)
{
    QVector<QString> formattedData(data.size());
    for(int i=0;i<data.size();i++)
    {
        formattedData[i] = data[i];
    }
    //qDebug() << "after formatting new data is:";
    //qDebug() << formattedData;
    qDebug() << "Code from server about operation is " << code;
    //qDebug() << "decoder";
    switch(code)
    {

    case 1: authHandler(formattedData); break;                  // Авторизация
    case 2: answerReceiver(formattedData); break;               // Регистрация
    case 3: answerReceiver(formattedData); break;               // Добавление таблицы
    case 4: tableRemoveHandler(formattedData); break;           // Удаление таблицы
    }
}
void TcpClient::authHandler(QVector<QString> data)
{
    //qDebug() << "handler";
    //qDebug() << data;
    if(data[0] == "0")
    {
        emit failedAuth();
    }
    else
    {
        accessLevel = data[5].toInt();
        emit succesfulAuth(data);
    }
}

void TcpClient::changeIPPORT(QString ip, QString port)
{
    IP = ip;
    PORT = port;
}

int TcpClient::getAccessLevel()
{
    return accessLevel;
}

bool TcpClient::connectToServer(QString ip, QString port)
{
    pTcpSocket->disconnectFromHost();
    pTcpSocket->close();
    if(ip.size() != 0 && port.size() !=0)
    {
        IP = ip;
        PORT = port;
    }
    pTcpSocket->connectToHost(IP,PORT.toUInt());
    if((pTcpSocket->state() == QAbstractSocket::ConnectingState) || (pTcpSocket->state() == QAbstractSocket::ConnectedState))
        return true;
    else
        return false;
}
void TcpClient::answerReceiver(QVector<QString> data)
{
    FileLogger::log(data[1]);
}

void TcpClient::tableRemoveHandler(QVector<QString> data)
{
    emit deletingResult(data);

}
