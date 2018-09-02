#include "chat.h"
#include "ui_chat.h"

chat::chat(QTcpSocket *socket2, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::chat)
{
    ui->setupUi(this);
    mClient = socket2;
    connect(mClient,SIGNAL(readyRead()),this,SLOT(client_ReadMessage()));

    thread = new QThread();
    thread->start();
}

chat::~chat()
{
    delete ui;
}

void chat::on_back_bt_clicked()
{
    this->parentWidget()->show();
    this->hide();
    mClient->write("31\n");
}

void chat::on_send_bt_clicked()
{
    int row = ui->user_lw->currentRow(); //获取你选中的IP行数
    QString ip_select;
    if(row < 0)
        QMessageBox::warning(this,"warning","no select ip");
    if(row >= 0)
    {
        ip_select=ui->user_lw->currentItem()->text(); //获取选中的IP
        qDebug()<< "select:" <<ip_select;

        QString msg = ui->chat_te->toPlainText();

        QListWidgetItem *item = new QListWidgetItem();
        item->setTextAlignment(Qt::AlignRight);
        item->setText(QTime::currentTime().toString() + "\n" +msg);
        msg.insert(0,"21\n");        //chat message flag
        msg.append("2\n" + ip_select);

        ui->chat_lw->addItem(item);
        ui->chat_lw->scrollToBottom();
        ui->chat_te->clear();
        mClient->write(msg.toStdString().c_str(),strlen(msg.toStdString().c_str()));
    }
}

void chat::client_ReadMessage()
{
    QString data;
    QStringList llist;
    QListWidgetItem *item = new QListWidgetItem();
    item->setTextAlignment(Qt::AlignLeft);

    llist = mmessage.split("1\n");
    if(llist[0] == "3")         //online user message
    {
        qDebug() << llist[1];
        if(llist[1] == "0")     //clear online user flag
        {
            ui->user_lw->clear();
        }
        else
        {
            item->setText(llist[1]);
            ui->user_lw->addItem(item);
        }
    }
    if(llist[0] == "4")         //chat message
    {
        item->setText(QTime::currentTime().toString() + "\n" + llist[1]);
        ui->chat_lw->addItem(item);
        ui->chat_lw->scrollToBottom();
    }
}

void chat::on_sendToolBtn_clicked()
{
    if(ui->user_lw->selectedItems().isEmpty())
    {
        QMessageBox::warning(0,tr("选择用户"),
                             tr("请先从用户列表选择要传送的用户!"),QMessageBox::Ok);
        return;
    }
    server->show();
    server->initServer();
}
