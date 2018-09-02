#include "servermenuwindow.h"
#include "ui_server_show.h"
#include "myserver.h"
#include <QListWidgetItem>


server_show::server_show(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::server_show)
{
    ui->setupUi(this);
    ser = new myserver();
    ser->startserver();
//    int msg = server.socketDescriptor();
//    ui->list_ser->addItem(msg);
}

server_show::~server_show()
{
    delete ui;
}


void server_show::on_exit_ser_clicked()
{
    this->close();
}

void server_show::on_dis_bt_clicked()
{
    qDebug()<<"exit";
}
