#ifndef REGIST_H
#define REGIST_H

#include <QMainWindow>
#include <QMessageBox>
#include "cclient.h"
#include <QTcpSocket>

namespace Ui {
class regist;
}

class regist : public QMainWindow
{
    Q_OBJECT

public:
    explicit regist(QTcpSocket *socket1,QWidget *parent = 0);

    ~regist();

private slots:

    void on_back_bt_clicked();
    void client_ReadMessage();

    void on_submit_bt_clicked();

private:
    Ui::regist *ui;
    QTcpSocket *mClient;
};

#endif // REGIST_H
