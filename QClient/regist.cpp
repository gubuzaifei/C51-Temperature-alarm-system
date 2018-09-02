#include "regist.h"
#include "ui_regist.h"

regist::regist(QTcpSocket *socket1,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::regist)
{
    ui->setupUi(this);
    mClient = socket1;
    connect(mClient,SIGNAL(readyRead()),this,SLOT(client_ReadMessage()));
}

regist::~regist()
{
    delete ui;
}


void regist::on_back_bt_clicked()
{
    this->parentWidget()->show();
    this->hide();
}

void regist::client_ReadMessage()
{
    QStringList llist;

    llist = mmessage.split("1\n");
    if(llist[0] == "4")
    {
        QMessageBox::warning(this,"WARNING","ID already existed!");
        ui->id_le->clear();
        ui->name_le->clear();
        ui->code_le->clear();
    }
    else if(llist[0] == "5")
    {
        QMessageBox::information(this,"Regist","upload finished!");
        ui->id_le->clear();
        ui->name_le->clear();
        ui->code_le->clear();
    }
}

void regist::on_submit_bt_clicked()
{

    QString msg = "01\n";
    msg.append(ui->id_le->text() + "2\n");
    msg.append(ui->name_le->text() + "3\n");
    msg.append(ui->code_le->text());
    if(ui->id_le->text().isEmpty() || ui->name_le->text().isEmpty() || ui->code_le->text().isEmpty())
    {
        QMessageBox::warning(this,"Warning","ID or user name or code is empty,enter again!");
        return;
    }
    mClient->write(msg.toStdString().c_str());
}
