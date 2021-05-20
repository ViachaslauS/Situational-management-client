#ifndef DELETEROWWINDOW_H
#define DELETEROWWINDOW_H

#include <QDialog>

namespace Ui {
class DeleteRowWindow;
}

class DeleteRowWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteRowWindow(QWidget *parent = nullptr);
    ~DeleteRowWindow();

    void setMaximumRows(int count);
private:
    Ui::DeleteRowWindow *ui;
private slots:
    void on_buttonBox_accepted();
signals:
    void rowToDelete(int);
};

#endif // DELETEROWWINDOW_H
