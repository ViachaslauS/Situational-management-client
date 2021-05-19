#ifndef DISCONNECTWINDOW_H
#define DISCONNECTWINDOW_H

#include <QDialog>
#include <QAbstractButton>
namespace Ui {
class DisconnectWindow;
}

class DisconnectWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DisconnectWindow(QWidget *parent = nullptr);
    ~DisconnectWindow();

    void badTry();
private:
    Ui::DisconnectWindow *ui;

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
signals:
    void retryConnect();
};

#endif // DISCONNECTWINDOW_H
