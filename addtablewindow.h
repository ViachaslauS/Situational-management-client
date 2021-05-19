#ifndef ADDTABLEWINDOW_H
#define ADDTABLEWINDOW_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QDebug>
#include <QAbstractButton>
#include <QMessageBox>

#include "tcpclient.h"

namespace Ui {
class AddTableWindow;
}

class AddTableWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddTableWindow(QWidget *parent = nullptr);
    ~AddTableWindow();

private:
    void configBox(QComboBox* box);
    QVector<QString> buildedMessage();
private slots:
    void on_columnsCount_valueChanged(int arg1);


    void on_buttonBox_accepted();

private:
    Ui::AddTableWindow *ui;

    QVector<QLineEdit*> columnsNames;
    QVector<QComboBox*> boxes;

    const QStringList dbTypes =
    {"CHAR","VARCHAR(20)","TEXT","BOOL","INT",
     "INT UNSIGNED","FLOAT","DOUBLE","DATE",
     "TIME","DATETIME"};


};

#endif // ADDTABLEWINDOW_H
