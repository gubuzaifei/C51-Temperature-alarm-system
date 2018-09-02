#include <QFile>
#include <QHostAddress>
#include "network.h"

/* 文件服务器类 */
QList<FileServerClient*>    FileServer::fileClientList;
QList<FileClient*>          FileServer::fileCltList;
MainWindow*                 FileServer::mainWindow;

FileServer::FileServer(QObject* parent): QTcpServer(parent) {}

FileServer::~FileServer() {
    // 关闭FileServer
    int i = 0;
    int fileCltLen = FileServer::fileCltList.count();
    int fileClientLen = FileServer::fileClientList.count();
    if ( this->isListening() ) {
        this->close();
    }
    for (i=0; i<fileClientLen; i++) {
        delete FileServer::fileClientList.at(i);
    }

    for (i=0; i<fileCltLen; i++) {
        delete FileServer::fileCltList.at(i);
    }
    delete FileServer::mainWindow;
}

bool FileServer::start() {
    FileServer::mainWindow = new MainWindow();
    int ret = listen(QHostAddress::LocalHost, 12346);
    if ( !ret ) {
        // 监听失败
        QMessageBox::critical(FileServer::mainWindow, "Error", "监听12346端口失败。请关闭12346端口再开启此程序。", "确定");
        return false;
    }
    FileServer::mainWindow->show();
    FileServer::mainWindow->on(sendFileFun);

    return true;
}

void FileServer::incomingConnection(int socketDescriptor) {
    // 当有客户端连接的时候， 系统自动调用这个函数
    FileServerClient* fileClient = new FileServerClient(this);
    // 设置文件描述符
    fileClient->setSocketDescriptor(socketDescriptor);
    // 添加进 连接列表
    FileServer::fileClientList.append(fileClient);
}

/* end */

/* 文件客户端类 */
FileServerClient::FileServerClient(QObject* parent): QTcpSocket(parent) {
    connect(this, SIGNAL(disconnected()), this, SLOT(disConnect()));
    int userChoose = QMessageBox::question(FileServer::mainWindow, "接受文件", "是否接受文件？", "确定", "取消");
    if ( userChoose == 0 ) {
        // 用户选择了同意
        // 选择保存的目录
        this->filePath = QFileDialog::getExistingDirectory(FileServer::mainWindow);
        connect(this, SIGNAL(readyRead()), this, SLOT(recvFile()));
    }else {
        // 用户选择了取消
        this->close();
    }
}

void FileServerClient::disConnect() {
    if ( this->isOpen() ) {
        this->close();
    }
    if ( this->fp != NULL ) {
        fclose( this->fp );
    }
    QMessageBox::information(FileServer::mainWindow, "Success", "接受成功", "确定");
}

void FileServerClient::recvFile() {
    // 接受文件 数据格式 fileName\r\n\r\ncontent
    int     recvLen             = 0;
    int     fileNameLen         = 0;        // 文件名字长度
    char    recvBuf[1024]       = { 0 };
    char    recvFileName[1024]  = { 0 };    // 接受文件的名字
    char*   index               = NULL;
    char*   saveFile            = NULL;

    while( (recvLen = read(recvBuf, 1024)) > 0 ) {
        if ( this->fp == NULL ) {
            // 当文件还没有建立的时候
            index = strstr(recvBuf, "\r\n\r\n");
            if ( index == NULL ) {
                // 没有找到分割点
                QMessageBox::critical(FileServer::mainWindow, "Error", "接受文件格式错误。", "确定");
                break;
            }else {
                // 成功找到分割点
                // 获取名字长度
                // 首先询问客户是否需要接受文件
                fileNameLen = index - recvBuf;
                // 复制名字
                strncpy(recvFileName, recvBuf, fileNameLen);
                this->filePath += "/"+QString(QLatin1String(recvFileName));
                QByteArray tmp = this->filePath.toLatin1();
                saveFile = tmp.data();
                this->fp = fopen(saveFile, "wb");
                fwrite(index+4, recvLen-fileNameLen-4, 1, fp);
            }
        }else {
            fwrite(recvBuf, recvLen, 1, fp);
        }
        memset(recvBuf, 0, 1024);
    }
}
/* end */

/* 发送文件客户端类 */
FileClient::FileClient(QObject* parent, const QString& fileName, const QString& ipaddr): QTcpSocket(parent) {
    this->fileName = fileName;
    QHostAddress serIp;
    serIp.setAddress(ipaddr);
    this->connectToHost(serIp, 12346);
    connect(this, SIGNAL(connected()), this, SLOT(sendFile()));
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connErr()));
}

void FileClient::sendFile() {
    FILE* fp = NULL;
    char readBuf[1024] = { 0 };
    int readLen = 0;
    char* sendFileName = NULL;
    QString sendHeader = "";
    int headerLen = 0;
    QStringList fileNameList;

    QByteArray tmp = this->fileName.toLatin1();
    sendFileName = tmp.data();
    fp = fopen(sendFileName, "rb");
    if ( fp == NULL ) {
        QMessageBox::critical(FileServer::mainWindow, "Error", "打开文件失败", "确定");
        return;
    }
    // 取出文件名
    fileNameList = this->fileName.split("/");
    sendHeader = fileNameList.last()+"\r\n\r\n";
    headerLen = sendHeader.length();
    tmp = sendHeader.toLatin1();

    this->write(tmp.data(), headerLen);
    while( (readLen = fread(readBuf, 1, 1024, fp)) > 0 ) {
        this->write(readBuf, readLen);
        memset(readBuf, 0, 1024);
    }
    fclose(fp);
    this->close();
    QMessageBox::information(FileServer::mainWindow, "Success", "发送成功", "确定");
}

void FileClient::connErr() {
    QMessageBox::critical(FileServer::mainWindow, "Error", "连接失败", "确定");
}
/* end */

/* 发送文件函数 */
void sendFileFun( const QString& fileName, const QString& ipaddr ) {
    FileClient* fileClt = new FileClient(FileServer::mainWindow, fileName, ipaddr);
    FileServer::fileCltList.append(fileClt);
}
/* end */
