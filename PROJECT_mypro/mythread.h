#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <unistd.h>

class mythread : public QThread
{
    Q_OBJECT
public:
    explicit mythread(qintptr ID,QObject *parent = 0);
    void run();

signals:
    void error(QTcpSocket::SocketError socketerror);
    void user_offline(QTcpSocket *socket);
    void send_onlinelist();
public slots:
    void read_message();
    void delete_from_onlinelist(QTcpSocket *socket);
    void disconnected();
private:
    QTcpSocket *socket;
    qintptr socketDescriptor;

};

#endif // MYTHREAD_H
