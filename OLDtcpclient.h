//#ifndef OLDTCPCLIENT_H
//#define OLDTCPCLIENT_H

//#include <QString>
//#include <QTcpSocket>
//#include <QWidget>
///**
// * @brief The TcpClient class. It is Singleton
// */
//class TcpClient : QObject
//{
//    Q_OBJECT
//public:
//    static TcpClient* Instance(QString = 0,QString = 0);

//    QString getIP();
//    QString getPort();

//    /// @return result of connecting
//    bool connectToServer();
//protected:
//    TcpClient(QString,QString);

//private:
//    static TcpClient* _tcpClient;
//    QString IP, PORT;

//    //Socket variables
//    QTcpSocket* pTcpSocket;
//    quint16 nextBlockSize;

//private slots:
//    void slotReadyRead();
//    void slotError(QAbstractSocket::SocketError);
//    void slotSendToServer();
//    void slotConnected();

//signals:
//    void sendMessageSignal();
//};

//#endif // OLDTCPCLIENT_H
