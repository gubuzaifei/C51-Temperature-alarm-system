#ifndef CCLIENT_H
#define CCLIENT_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include "regist.h"
#include "chat.h"

extern QString mmessage;

namespace Ui {
class Cclient;
}

class regist;
class chat;

class Cclient : public QMainWindow
{
    Q_OBJECT

public:
    explicit Cclient(QWidget *parent = 0);
    ~Cclient();

    QTcpSocket *mClient;

private slots:
    void on_register_bt_clicked();

    void on_login_bt_clicked();

    void on_cancel_bt_clicked();
    void client_ReadMessage();

private:
    Ui::Cclient *ui;

    regist *ui_reg;
    chat *ui_chat;
};

#endif // CCLIENT_H
