#include "deleterowwindow.h"
#include "ui_deleterowwindow.h"

DeleteRowWindow::DeleteRowWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeleteRowWindow)
{
    ui->setupUi(this);
    ui->spinBox->setMinimum(1);
    ui->spinBox->setMaximum(1);
}

void DeleteRowWindow::setMaximumRows(int count)
{
    ui->spinBox->setMaximum(count);
}

DeleteRowWindow::~DeleteRowWindow()
{
    delete ui;
}

void DeleteRowWindow::on_buttonBox_accepted()
{
    emit rowToDelete(ui->spinBox->value());
}
