#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QByteArray>
#include <QSqlTableModel>
#include <QVector>
#include <QTextCodec>
#include <QString>

#include "filelogger.h"
class TcpClient : public QObject
{
    Q_OBJECT
public:
    //explicit TcpClient(QObject *parent = nullptr);
    static TcpClient* Instance(QString = 0,QString = 0);

    QString getIP();
    QString getPort();
    int getAccessLevel();
    /// @return result of connecting
    bool connectToServer(QString = 0, QString = 0);

    void changeIPPORT(QString,QString);

protected:
    TcpClient(QString,QString,QObject *parent = nullptr);

private:

    void translateMessage(int code, QVector<char*> data);
    void authHandler(QVector<QString> data);
    void answerReceiver(QVector<QString> data);
    void tableRemoveHandler(QVector<QString> data);

private:
    static TcpClient* _tcpClient;
    QString IP=0, PORT=0;

    //Socket variables
    QTcpSocket* pTcpSocket;
    quint16 nextBlockSize;

    int accessLevel = 0;
private slots:
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
    void slotSendToServer(int code,QVector<QString> args);
    void slotConnected();
signals:
    void sendMessageSignal(int, QVector<QString> args);
    void succesfulAuth(QVector<QString>);
    void failedAuth();
    void connectionLost();
    void deletingResult(QVector<QString>);
};

#endif // TCPCLIENT_H
