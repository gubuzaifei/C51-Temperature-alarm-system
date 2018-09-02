#include "cclient.h"
#include "ui_cclient.h"

QString mmessage;

Cclient::Cclient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Cclient)
{
    ui->setupUi(this);
    mClient = new QTcpSocket(this);
    mClient->connectToHost(QHostAddress("192.168.0.44"),9090);
    //mClient->connectToHost(QHostAddress("192.168.0.123"),9090);
    connect(mClient,SIGNAL(readyRead()),this,SLOT(client_ReadMessage()));
}

Cclient::~Cclient()
{
    delete ui;
}

void Cclient::on_register_bt_clicked()
{
    ui_reg = new regist(mClient,this);
    ui_reg->show();
    this->hide();
}

void Cclient::on_login_bt_clicked()
{
    QString login_msg;
    login_msg.insert(0,"11\n");        //login message flag
    login_msg.append(ui->ID_le->text() + "2\n");
    login_msg.append(ui->code_le->text());

    ui->ID_le->clear();
    ui->code_le->clear();

    mClient->write(login_msg.toStdString().c_str());
//    ui_chat = new chat(mClient,this);
//    ui_chat->show();
//    this->hide();
}

void Cclient::on_cancel_bt_clicked()
{
    close();
}

void Cclient::client_ReadMessage()
{
    QStringList msglist;
    mmessage = mClient->readAll();
    qDebug() << mmessage;
    msglist = mmessage.split("1\n");
    if(msglist[0] == "0")
    {
        QMessageBox::warning(this,"ERROR","code is error,\nplease try again!");
    }
    else if(msglist[0] == "1")
    {
        QMessageBox::warning(this,"WARNING","ID no exist,\nplease regist!");
    }
    else if(msglist[0] == "2")
    {
        ui_chat = new chat(mClient,this);
        ui_chat->show();
        this->hide();
    }
}
