#include "dbwindow.h"
#include "ui_dbwindow.h"

DBWindow::DBWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DBWindow)
{
    ui->setupUi(this);
    setWindowTitle("Рабочее окно");
    updateAction = new QAction(QIcon("C:/Users/silen/Desktop/QtProject/Icons/updateIcon.png"),"Обновить");
    updateAction->setShortcut(QKeySequence("CTRL+R"));
    connect(updateAction,&QAction::triggered,this, &DBWindow::configureTabTable);
    ui->toolBar->addAction(updateAction);

}

DBWindow::~DBWindow()
{
    query->clear();
    model->disconnect();
    database.close();
    delete ui;
}

bool DBWindow::configureDatabase(QVector<QString> data)
{
    tcpClient = tcpClient->Instance();
    //qDebug() << "IP from inst is " << tcpClient->getIP();
    connect(tcpClient,&TcpClient::connectionLost,this,&DBWindow::slotDisconnect);
    connect(tcpClient,&TcpClient::deletingResult,this,&DBWindow::slotDeletingResult);
    configureUIAtLevel();
    //qDebug() << "DB form was configured!";
    database = QSqlDatabase::addDatabase("QMYSQL");

    username = data[1];
    password = data[2];
    IP = data[3];
    port = data[4];

    database.setHostName(IP);
    database.setUserName(username);
    database.setPassword(password);

    qDebug() << database.isValid();
    if(!database.open())
    {
        qDebug() << "Database is not open!";
        FileLogger::log("Database is not open!");
        return false;
    }

    query = new QSqlQuery(database);
    //query->exec("SET NAMES 'cp1251'");//ВОТ ОНА!!!! ЭТА СТРОКА!!!!
    query->exec("USE "+_DB_NAME_);
    //query->exec("SELECT")
    model = new QSqlRelationalTableModel(nullptr,database);
    model->setEditStrategy(QSqlTableModel::EditStrategy::OnFieldChange);
    configureTabTable();
    return true;
}

void DBWindow::configureTabTable()
{
    QStringList tablesList = database.tables(QSql::TableType::Tables);
    for(auto a: tabWidgets)
    {
        delete a->layout();
        delete a;
    }
    tabWidgets.clear();
    tabWidgets.squeeze();           // Сомнительно

    ui->tabTablesWidget->clear();

    //qDebug() <<tablesList;
    for(int i = 0; i < tablesList.size(); i++)
    {
        tabWidgets.push_back(new QWidget());
        QTableView* childView = new QTableView(tabWidgets.last());

        QVBoxLayout *layout = new QVBoxLayout();

        QPushButton* addStrButton = new QPushButton("Добавить строку");
        connect(addStrButton,SIGNAL(clicked()),this,SLOT(slotOnAddStrButtonClicked()));

        layout->addWidget(addStrButton);

        layout->addWidget(childView);
        tabWidgets.last()->setLayout(layout);

        QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        tabWidgets.last()->setSizePolicy(policy);
        //childView->setSizePolicy(policy);
        //childView->setGeometry(0,0,800,600);
        childView->setModel(model);
        //childView->setItemDelegate(new QSqlRelationalDelegate(childView));
        childView->show();
        ui->tabTablesWidget->addTab(tabWidgets.last(),tablesList[i]);
        tabWidgets.last()->setToolTip(QString::number(i));
    }
    currentTable = (QTableView*)ui->tabTablesWidget->currentWidget();
    makeRelationsForTables();
}

void DBWindow::makeRelationsForTables()
{
//    QTableView *slave, *master;
//    for(int i=0; i< ui->tabTablesWidget->tabBar()->count(); i++)
//    {
//        if(ui->tabTablesWidget->tabBar()->tabText(i) == _V_C_TABLE)
//        {}

//    }
}
void DBWindow::on_tabTablesWidget_currentChanged(int index)
{
//    query->exec("SHOW VARIABLES LIKE 'char%'");
//    query->exec("SHOW VARIABLES LIKE 'coll%'");
    model->setTable(ui->tabTablesWidget->tabBar()->tabText(index));
    model->select();
    currentTable = (QTableView*)ui->tabTablesWidget->currentWidget();
}

void DBWindow::configureUIAtLevel()
{

    switch(tcpClient->getAccessLevel())
    {
    case 1: configAdminUI();
    case 2: configUserUI();
    case 3: configSpectatorUI();
    }
}

void DBWindow::configAdminUI()
{
    QMenu *admMenus = new QMenu();
    admMenus->setTitle("Администратор");
    ui->menubar->addMenu(admMenus);
    Aactions[addingUser] = new QAction(QIcon("C:/Users/silen/Desktop/QtProject/Icons/add_User.png"),"Добавление пользователя");
    Aactions[createTable] = new QAction(QIcon("C:/Users/silen/Desktop/QtProject/Icons/add_Table.png"),"Добавление таблицы");
    Aactions[editTable] = new QAction(QIcon("C:/Users/silen/Desktop/QtProject/Icons/edit_Table.png"),"Изменение текущей таблицы");
    Aactions[deleteTable] = new QAction(QIcon("C:/Users/silen/Desktop/QtProject/Icons/delete--v2.png"),"Удаление текущей таблицы");

    ui->toolBar->addAction(Aactions[addingUser]);
    ui->toolBar->addAction(Aactions[createTable]);
    ui->toolBar->addAction(Aactions[editTable]);
    ui->toolBar->addAction(Aactions[deleteTable]);
    //admMenus->addAction(actions[addingUser]);

    connect(Aactions[addingUser],SIGNAL(triggered()),SLOT(slotAddNewUser()));
    connect(Aactions[createTable],SIGNAL(triggered()),SLOT(slotAddTable()));
    connect(Aactions[editTable],SIGNAL(triggered()),SLOT(slotEditCurrentTable()));
    connect(Aactions[deleteTable],SIGNAL(triggered()),SLOT(slotDeleteTable()));
    admMenus->addActions(Aactions.values());
}

void DBWindow::configUserUI()
{
    QMenu *userMenus = new QMenu();
    userMenus->setTitle("Действия");
    ui->menubar->addMenu(userMenus);
    Uactions[returnToMainMenu] = new QAction(QIcon("C:/Users/silen/Desktop/QtProject/Icons/exit.png"),"Вернуться в главное меню");
    Uactions[updateTables] = updateAction;
    Uactions[deleteRow] = new QAction(QIcon("C:/Users/silen/Desktop/QtProject/Icons/deleteRow.png"),"Удалить строку");

    ui->toolBar->addActions(Uactions.values());
    //ui->toolBar->addAction(Uactions[returnToMainMenu]);


    connect(Uactions[returnToMainMenu],&QAction::triggered,this,&DBWindow::slotDisconnect);
    connect(Uactions[deleteRow], &QAction::triggered,this, &DBWindow::slotDeleteRow);

    userMenus->addActions(Uactions.values());
}

void DBWindow::configSpectatorUI()
{

}

void DBWindow::slotAddNewUser()
{
    qDebug() << "Adding!";
    model->insertRow(model->rowCount());
    AddUserWindow window;
    connect(&window,&AddUserWindow::acceptData,this,&DBWindow::slotSendNewUser);
    window.setModal(true);
    window.exec();
}

void DBWindow::slotSendNewUser(QVector<QString> vec)
{
    emit tcpClient->sendMessageSignal(2,vec);
}

void DBWindow::slotDisconnect()
{
    qDebug() <<"disconnected";
    emit returnBackSignal();
    this->close();
    database.close();
}

void DBWindow::slotReconnect()
{
    if(tcpClient->connectToServer())
    {
        dWindow.close();
        ui->centralwidget->setEnabled(true);
        ui->menubar->setEnabled(true);
    }
    else
    {
        dWindow.badTry();
        qDebug() << "bad try reconnect";
    }
}

void DBWindow::slotOnAddStrButtonClicked()
{
    model->insertRow(model->rowCount());
    //model->insertColumn(model->columnCount());
    //ui->tabTablesWidget->currentWidget()->update();
}
void DBWindow::slotAddTable()
{
    AddTableWindow addTableWindow;
    addTableWindow.setModal(true);
    addTableWindow.exec();
    qDebug() << "add table";

}
void DBWindow::slotEditCurrentTable()
{
    qDebug() << "edit table";
}

void DBWindow::slotOnUpdateButtonClicked()
{
    tabWidgets[ui->tabTablesWidget->currentIndex()]->update();
}

void DBWindow::slotDeleteTable()
{
    QString title = ui->tabTablesWidget->tabBar()->tabText(ui->tabTablesWidget->currentIndex());
    QMessageBox *msg = new QMessageBox(QMessageBox::Question, "Удаление таблицы", "Вы уверены что хотите удалить таблицу " + title + "?", QMessageBox::Yes| QMessageBox::No, this);
    connect(msg, &QMessageBox::accepted,this,&DBWindow::slotAcceptDeleting);
    msg->show();
    //slotAcceptDeleting();
}
void DBWindow::slotAcceptDeleting()
{
    QVector<QString> message;
    QString title = ui->tabTablesWidget->tabBar()->tabText(ui->tabTablesWidget->currentIndex());
    message.push_back(title);
    emit tcpClient->sendMessageSignal(4,message);
}
void DBWindow::slotDeletingResult(QVector<QString> data)
{
    switch (data[0].toInt()) {
    case 0: QMessageBox::warning(this,"Удаление таблицы","Таблица не удалена.\n" + data[1]); FileLogger::log(data[1]); break;
    case 1: QMessageBox::information(this,"Удаление таблицы","Таблица удалена успешно."); configureTabTable(); break;
    }
}
void DBWindow::slotDeleteRow()
{
//    QModelIndex index;
//    QTableView* tableView = currentTable;
//    QItemSelectionModel* selectModel = (tableView)->selectionModel();
//    QModelIndexList indexes = selectModel->selectedIndexes();
//    int row;
//    foreach( index, indexes )
//    {
//        row = index.row();
//        QString tmp = tableView->model()->data(tableView->model()->index(row,0)).toString();

//        if ( row != -1 )
//        {
//            qDebug() << row; //делаем что-то
//        }
//        break;
//    }
    if(currentTable->selectionModel() == nullptr)
    {
        qDebug() << "table model is null";
    }
    else
    {
         qDebug() << "not null";
            QModelIndex index;
            QTableView* tableView = currentTable;
            QItemSelectionModel* selectModel = (tableView)->selectionModel();
            QModelIndexList indexes = selectModel->selectedIndexes();
            int row;
            foreach( index, indexes )
            {
                row = index.row();
                QString tmp = tableView->model()->data(tableView->model()->index(row,0)).toString();

                if ( row != -1 )
                {
                    qDebug() << row; //делаем что-то
                }
                break;
            }

    }
    model->select();
    configureTabTable();
}
