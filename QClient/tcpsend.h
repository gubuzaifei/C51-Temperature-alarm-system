#ifndef TCPSEND_H
#define TCPSEND_H

#include <QTcpServer>
#include <QMessageBox>
#include <QFileDialog>
#include <QTcpSocket>
#include <QDataStream>
#include <QTime>

namespace Ui {
class tcpsend;
}

class QTcpServer;
class QTcpSocket;
class QFile;

class tcpsend : public QDialog
{
    Q_OBJECT

public:
    explicit tcpsend(QWidget *parent = 0);
    ~tcpsend();

    void initServer();
    void refused();

private:
    Ui::tcpsend *ui;

    qint16 tcpPort;
    QTcpServer *tcpServer;
    QString fileName;
    QString theFileName;
    QFile *localFile;
    qint64 payloadSize;
    qint64 totalBytes;
    qint64 bytesWritten;
    qint64 bytesToWrite;
    QByteArray outBlock;

    QTcpSocket *clientConnection;

    QTime time;

private slots:
    void sendMessage();
    void updateClientProgress(qint64 numBytes);
    void on_serverOpenBtn_clicked();
    void on_serverSendBtn_clicked();
    void on_serverCloseBtn_clicked();

signals:
    void sendFileName(QString fileName);
};

#endif // TCPSEND_H
