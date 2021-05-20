#ifndef DBWINDOW_H
#define DBWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QTabBar>
#include <QTableView>
#include <QSizePolicy>
#include <QPushButton>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>

#include <QSqlError>
#include <QSqlResult>

#include "tcpclient.h"
#include "adduserwindow.h"
#include "disconnectwindow.h"
#include "filelogger.h"
#include "addtablewindow.h"
#include "deleterowwindow.h"

#define _V_C_TABLE "voinskaya chast"

namespace Ui {
class DBWindow;
}

class DBWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DBWindow(QWidget *parent = nullptr);
    ~DBWindow();

    bool configureDatabase(QVector<QString> data);


private:
    Ui::DBWindow *ui;

    QSqlDatabase database;
    QString IP, port;
    QString username, password;
    QString dbName, tableName;

    const QString _DB_NAME_ = "testbase";

    QSqlQuery *query;
    //QSqlTableModel* model;
    QSqlRelationalTableModel *model;

    QVector<QWidget*> tabWidgets;

    TcpClient* tcpClient;

    enum AactDefining{
        addingUser,
        createTable,
        editTable,
        deleteTable,
        deleteRow
    };
    enum UactDefining{
        returnToMainMenu,
        updateTables,

    };

    QMap<AactDefining,QAction*> Aactions;
    QMap<UactDefining,QAction*> Uactions;
    //QMenu* admMenus;
    DisconnectWindow dWindow;

    QAction* updateAction;

    QTableView* currentTable;

    int currentTabTable = 0;
private:
    void configureTabTable(uint setCurrent = 0);
    void configureUIAtLevel();
    void configAdminUI();
    void configUserUI();
    void configSpectatorUI();
    void makeRelationsForTables();
private slots:
    void on_tabTablesWidget_currentChanged(int index);
    void slotAddNewUser();
    void slotSendNewUser(QVector<QString>);
    void slotDisconnect();
    void slotReconnect();
    void slotOnAddStrButtonClicked();
    void slotAddTable();
    void slotEditCurrentTable();
    void slotUpdate();
    void slotDeleteTable();
    void slotDeletingTableResult(QVector<QString>);
    void slotAcceptDeletingTable();
    void slotDeleteRow();
    void slotAcceptDeletingRow(int);

signals:
    void returnBackSignal();
};

#endif // DBWINDOW_H
