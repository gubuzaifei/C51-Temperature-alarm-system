#ifndef SERVER_SHOW_H
#define SERVER_SHOW_H

#include <QMainWindow>
#include "myserver.h"

namespace Ui {
class server_show;
}

class server_show : public QMainWindow
{
    Q_OBJECT

public:
    explicit server_show(QWidget *parent = 0);
    ~server_show();

private slots:
    void on_exit_ser_clicked();

    void on_dis_bt_clicked();

private:
    Ui::server_show *ui;
    myserver *ser;

};

#endif // SERVER_SHOW_H
