#ifndef ADDUSERWINDOW_H
#define ADDUSERWINDOW_H

#include <QDialog>

namespace Ui {
class AddUserWindow;
}

class AddUserWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddUserWindow(QWidget *parent = nullptr);
    ~AddUserWindow();

private slots:
    void on_buttonBox_accepted();

signals:
    void acceptData(QVector<QString>);

private:
    Ui::AddUserWindow *ui;
};

#endif // ADDUSERWINDOW_H
