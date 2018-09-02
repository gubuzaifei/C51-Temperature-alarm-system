#include "myserver.h"
#include "mythread.h"

QList <QTcpSocket*> socket_list;
QList <QString> IP_list;
QList <QString> User_list;

myserver::myserver(QObject *parent) : QTcpServer(parent)
{
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("kuku.db");
    QString create_sql_reg = "create table reg(id int primary key,name varchar(30),password varchar(30))";
    QString create_sql_online = "create table online(id int primary key,name varchar(30),IP varchar(30))";
    if(database.open())
    {
        QSqlQuery sql_qury;
        sql_qury.prepare(create_sql_reg);
        if(!sql_qury.exec())
        {
            qDebug()<<QObject::tr("regist table create failed");
            qDebug()<<sql_qury.lastError();
        }
        else
        {
            qDebug()<<"regist table created";
        }

        sql_qury.prepare(create_sql_online);
        if(!sql_qury.exec())
        {
            qDebug()<<QObject::tr("online table create failed");
            qDebug()<<sql_qury.lastError();
        }
        else
        {
            qDebug()<<"online user table created";
        }
    }
}


void myserver::startserver()
{
    int port = 9090;
    if(!this->listen(QHostAddress::Any,port))
    {
        qDebug()<<"could not start server";
    }
    else
    {
        qDebug()<<"listening to port"<<port<<"....";
    }
}

void myserver::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<socketDescriptor<<"connecting...";
    mythread *thread = new mythread(socketDescriptor,this);
    connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));
    thread->start();

}
