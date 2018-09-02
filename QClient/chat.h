#ifndef CHAT_H
#define CHAT_H

#include <QMainWindow>
#include <QTime>
#include <QMessageBox>
#include <QLabel>
#include "cclient.h"
#include <QThread>
#include <QTcpServer>
#include "tcpsend.h"

namespace Ui {
class chat;
}
class tcpsend;
//class wwait:public QThread
//{
//public:

//    virtual void run();
//};

class chat : public QMainWindow
{
    Q_OBJECT

public:
    explicit chat(QTcpSocket *socket2, QWidget *parent = 0);
    ~chat();

    //virtual void run();

private slots:
    void on_back_bt_clicked();

    void on_send_bt_clicked();

    void client_ReadMessage();


    void on_sendToolBtn_clicked();

private:
    Ui::chat *ui;
    QTcpSocket *mClient;
    QThread *thread;
    tcpsend *server;
};

#endif // CHAT_H
