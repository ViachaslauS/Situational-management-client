#include "addtablewindow.h"
#include "ui_addtablewindow.h"

AddTableWindow::AddTableWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTableWindow)
{
    ui->setupUi(this);
    setWindowTitle("Добавление таблицы");
    //Добавляет скроллинг
    ui->scrollArea->setWidget(ui->formLayoutWidget);
    columnsNames.push_back(new QLineEdit());
    boxes.push_back(new QComboBox());
    configBox(boxes.last());

    ui->formLayout->addRow(columnsNames.last(),boxes.last());

}

AddTableWindow::~AddTableWindow()
{
    delete ui;
}

void AddTableWindow::on_columnsCount_valueChanged(int arg1)
{
    if(columnsNames.size() < arg1)
    {
        columnsNames.push_back(new QLineEdit());
        boxes.push_back(new QComboBox());
        configBox(boxes.last());
        ui->formLayout->addRow(columnsNames.last(),boxes.last());

    }
    else
    {
        delete columnsNames.last();
        delete boxes.last();
        columnsNames.pop_back();
        boxes.pop_back();
        qDebug() << columnsNames.count();
    }

}
void AddTableWindow::configBox(QComboBox *box)
{
    box->addItems(dbTypes);
}

void AddTableWindow::on_buttonBox_accepted()
{
    TcpClient* tcpClient = 0;
    tcpClient = tcpClient->Instance();
    QVector<QString> vec = buildedMessage();
    if(vec.size() > 0)
        emit tcpClient->sendMessageSignal(3,buildedMessage());
    else
    {
        QMessageBox::critical(this,"Ошибка","Значения введены не полностью!");
    }
}

QVector<QString> AddTableWindow::buildedMessage()
{
    QVector<QString> vec;

    if(ui->TableName->text() == 0)
    {
        return vec;
    }
    vec.push_back(ui->TableName->text());
    for(int i = 0;i<columnsNames.size();i++)
    {
        vec.push_back(columnsNames[i]->text());
        vec.push_back(boxes[i]->currentText());
        if(columnsNames[i]->text() == 0)
        {
            vec.clear();
            return vec;
        }

    }
    return vec;
}
