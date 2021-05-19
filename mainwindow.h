#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QTime>
#include <QDate>
#include <QSqlQuery>
#include <QTableView>
#include <QMessageBox>

#include "ipeditwindow.h"
#include "tcpclient.h"
#include "dbwindow.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /**
     * @return успешность открытия файла ipconf
     */
    bool m_open();
    /**
     * @brief MainWindow::saveCurrentAddr
     * @brief Очищает файл ipconf и сохраняет в него текущие значение IP и PORT
     */
    void saveCurrentAddr();
    void logging(QString);
    void setupBckgrd();
    void sampleBDConnect();
    /**
     * @brief configureEditFunction
     * Настраивает QAction, отвечающий за изменение IP-PORT
     */
    void configureEditFunction();
//    void resizeEvent(QResizeEvent *event);
private:
    Ui::MainWindow *ui;
    QAction* ipPortSettings;
    ///Переменная, хранящая IP адрес подкюлчения
    QString* IP;
    ///Переменная, хранящая port подкюлчения
    QString* PORT;
    ///Файл с сохраненным адресом
    QFile* ipFile;
    ///Файл логирования действий
    QFile* logFile;
    bool canLogging = false;

    TcpClient* tcpClient;

    DBWindow* dbWindow;

private slots:

    void editIP();
    void newAddress(QString,QString);

    void on_connectButton_clicked();

    void slotAuthResultTrue(QVector<QString>);
    void slotAuthResultFalse();

    void slotReturnThisWindow();
};
#endif // MAINWINDOW_H
