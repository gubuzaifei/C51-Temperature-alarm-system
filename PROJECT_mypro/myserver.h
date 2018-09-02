#ifndef MYSERVER_H
#define MYSERVER_H

#include <QObject>
#include <QTcpServer>
#include "mythread.h"
#include <QtSql>
#include <QDebug>

extern QList <QTcpSocket*> socket_list;
extern QList <QString> IP_list;
extern QList <QString> User_list;

class myserver : public QTcpServer
{
    Q_OBJECT
public:
    explicit myserver(QObject *parent = 0);
    void startserver();
signals:


public slots:

protected:
    void incomingConnection(qintptr socketDescriptor);
};

#endif // MYSERVER_H
