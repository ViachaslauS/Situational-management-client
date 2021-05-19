//#include "OLDtcpclient.h"


//TcpClient::TcpClient(QString ip, QString port)
//{
//    IP = ip;
//    PORT = port;

//    pTcpSocket = new QTcpSocket();

//    pTcpSocket->connectToHost(IP,PORT.toUInt());

//    connect(pTcpSocket,SIGNAL(connected()),SLOT(slotConnected()));
//    connect(pTcpSocket,SIGNAL(readyRead()),SLOT(slotReadyRead()));
//    connect(pTcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(slotError(QAbstractSocket::SocketError)));

//    connect(this,&TcpClient::sendMessageSignal,&TcpClient::slotSendToServer);
//}

//TcpClient* TcpClient::_tcpClient = 0;

//TcpClient* TcpClient::Instance(QString ip, QString port)
//{
//    if(_tcpClient == 0 && ip != 0 && port !=0)
//    {
//        _tcpClient = new TcpClient(ip,port);
//    }
//    return _tcpClient;
//}

//QString TcpClient::getIP()
//{
//    return IP;
//}

//QString TcpClient::getPort()
//{
//    return PORT;
//}

//void TcpClient::slotReadyRead()
//{
//    //Поток, в который записываются получаемые данные
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

//        QString str;

//        in >> str;

//        nextBlockSize = 0;
//        qDebug() << in;
//    }
//}
//void TcpClient::slotError(QAbstractSocket::SocketError err)
//{
//    QString strError =
//            "Error: " + (err == QAbstractSocket::HostNotFoundError ?
//                "The host was not found." :
//                err == QAbstractSocket::RemoteHostClosedError ?
//                "The remote host is closed." :
//                err == QAbstractSocket::ConnectionRefusedError ?
//                "The connection was refused." :
//                QString(pTcpSocket->errorString())
//                );
//    qDebug() << strError;
//}
//void TcpClient::slotSendToServer()
//{
//    QByteArray arrayBlock;
//    QDataStream outStream(&arrayBlock, QIODevice::WriteOnly);
//    outStream.setVersion(QDataStream::Qt_5_15);
//    outStream << quint16(0) << "sosi";

//    outStream.device()->seek(0);

//    outStream << quint16(arrayBlock.size() - sizeof (quint16));

//    pTcpSocket->write(arrayBlock);
//}
//void TcpClient::slotConnected()
//{
//    qDebug()<<"Conected.";
//}
