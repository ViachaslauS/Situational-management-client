#ifndef IPEDITWINDOW_H
#define IPEDITWINDOW_H

#include <QDialog>
#include <QHostAddress>
#include <QMessageBox>
namespace Ui {
class IpEditWindow;
}

class IpEditWindow : public QDialog
{
    Q_OBJECT

public:
    explicit IpEditWindow(QWidget *parent = nullptr);
    ~IpEditWindow();

signals:
    void acceptAddress(QString,QString);


private slots:
    void on_buttonBox_accepted();

private:
    Ui::IpEditWindow *ui;
};

#endif // IPEDITWINDOW_H
