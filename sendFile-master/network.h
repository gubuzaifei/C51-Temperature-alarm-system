#ifndef NETWORK_H
#define NETWORK_H

#include <QList>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include "mainwindow.h"

class FileServerClient: public QTcpSocket {
    // 文件客户端类
    Q_OBJECT
public:
    FileServerClient(QObject *parent);
private slots:
    void disConnect();
    void recvFile();
private:
    FILE* fp = NULL;
    QString filePath = "";
};

class FileClient: public QTcpSocket {
    // 发送文件客户端类
    Q_OBJECT
public:
    FileClient(QObject *parent, const QString& fileName, const QString& ipaddr);
private slots:
    void sendFile();
    void connErr();
private:
    QString fileName = "";
};

class FileServer: public QTcpServer {
    // 文件服务器类
    Q_OBJECT
public:
    FileServer(QObject* parent=Q_NULLPTR);
    ~FileServer();
    bool start();
    static QList<FileServerClient*> fileClientList;
    static QList<FileClient*> fileCltList;
    static MainWindow* mainWindow;
private:
    // 继承虚函数
    void incomingConnection(int socketDescriptor);
};

void sendFileFun( const QString& fileName, const QString& ipaddr );

#endif // NETWORK_H
