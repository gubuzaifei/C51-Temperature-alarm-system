#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "servermenuwindow.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_start_bt_clicked();

    void on_exit_bt_clicked();

private:
    Ui::MainWindow *ui;
    server_show *next;
};

#endif // MAINWINDOW_H
