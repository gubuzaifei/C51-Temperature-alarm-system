#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    this->MainLayout = new QGridLayout(this);
    this->setWindowTitle("sendFile");
    this->createMenu();
    this->createMainUI();
    this->setLayout(this->MainLayout);
}

MainWindow::~MainWindow()
{
    delete this->fileMenu;
    delete this->helpMenu;
    delete this->menuBar;
    delete this->fileInput;
    delete this->ipAddrInput;
    delete this->sendBtn;
    delete this->browerBtn;
    delete this->topLayout;
    delete this->middleLayout;
    delete this->bottomLayout;
    delete this->MainLayout;
}

void MainWindow::createMenu() {
    this->menuBar = new QMenuBar(this);
    this->fileMenu = new QMenu("文件", this);
    this->helpMenu = new QMenu("帮助", this);

    this->menuBar->setGeometry(0, 0, 330, 22);
    QAction* openFileAct = this->fileMenu->addAction("打开文件");
    this->fileMenu->addSeparator();
    QAction* exitAct = this->fileMenu->addAction("退出");

    QAction* aboutAct= this->helpMenu->addAction("About");

    this->menuBar->addMenu(this->fileMenu);
    this->menuBar->addMenu(this->helpMenu);

    // 绑定事件
    connect(openFileAct, SIGNAL(triggered()), this, SLOT(openFileEvent()));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(exitEvent()));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(aboutEvent()));
}

void MainWindow::createMainUI() {
    // 创建主程序框架
    this->fileInput = new QLineEdit(this);
    this->ipAddrInput = new QLineEdit(this);
    this->sendBtn = new QPushButton("发送", this);
    this->browerBtn = new QPushButton("..", this);
    this->topLayout = new QHBoxLayout(this);
    this->middleLayout = new QVBoxLayout(this);
    this->bottomLayout = new QHBoxLayout(this);

    /* 修改输入框属性 */
    this->fileInput->setReadOnly(true);
    this->fileInput->setPlaceholderText("file path..");
    this->ipAddrInput->setPlaceholderText("remote ip address..");

    /* 按钮属性 */
    this->browerBtn->setFixedSize(22, 20);
    this->sendBtn->setFixedSize(80, 32);

    /* 上层框架 */
    this->topLayout->setSpacing(10);
    this->topLayout->addWidget(this->fileInput);
    this->topLayout->addWidget(this->browerBtn);

    /* 中间框架 */
    this->middleLayout->setMargin(20);
    this->middleLayout->addLayout(this->topLayout);
    this->middleLayout->addWidget(this->ipAddrInput);

    /* 下层框架 */
    this->bottomLayout->addWidget(this->sendBtn);

    /* 主框架 */
    this->MainLayout->setSpacing(15);
    this->MainLayout->setMargin(30);
    this->MainLayout->addLayout(this->middleLayout, 0, 0);
    this->MainLayout->addLayout(this->bottomLayout, 1, 0);
    this->MainLayout->setSizeConstraint(QLayout::SetFixedSize);

    /* 事件绑定 */
    connect(this->browerBtn, SIGNAL(clicked()), this, SLOT(openFileEvent()));
    connect(this->sendBtn, SIGNAL(clicked()), this, SLOT(sendFileEvent()));
}

void MainWindow::openFileEvent() {
    // 打开文件操作
    QString tmpFileName;
    tmpFileName = QFileDialog::getOpenFileName(this);
    if ( tmpFileName != "" ) {
        this->fileInput->setText(tmpFileName);
        this->fileName = tmpFileName;
    }
}

void MainWindow::exitEvent() {
    // 退出程序操作
    this->close();
}

void MainWindow::aboutEvent() {
    // 显示帮助框
    AboutDialog aboutDialog(this);
    aboutDialog.exec();
}

bool MainWindow::checkUserInput() {
    // 检查用户输入是否正确
    // 简陋的匹配IP地址的规则
    QString pattern = "^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}$";
    QRegExp re(pattern);
    QString sendFileName = this->fileInput->text();
    QString remoteIpaddr = this->ipAddrInput->text();
    if ( sendFileName == "" ){
        this->showErrorEvent("输入文件不能为空");
        goto ERR;
    }

    if ( remoteIpaddr == "" ) {
        this->showErrorEvent("IP地址不能为空");
        goto ERR;
    }

    if ( !re.exactMatch(remoteIpaddr) ) {
        this->showErrorEvent("IP地址不合法");
        goto ERR;
    }
    this->fileName = sendFileName;
    this->remoteIpaddr = remoteIpaddr;
    return true;
ERR:
    return false;
}

void MainWindow::on(void (*sendFileFun)(const QString&, const QString&)) {
    // 绑定发送文件事件
    this->sendFileFun = sendFileFun;
}

void MainWindow::sendFileEvent() {
    if ( !this->checkUserInput() ) {
        // 如果用户输入有误
        return;
    }
    this->sendFileFun(this->fileName, this->remoteIpaddr);
}

void MainWindow::showErrorEvent( const QString& message ) {
    QMessageBox::critical(this, "Error", message, "确定");
}

AboutDialog::AboutDialog(QWidget* parent): QDialog(parent) {
    int i = 0;
    QString message[] = {"Introduction: ", "这是一个局域网传输文件的小工具", "Author: ", "Hejs", "Mail: ", "240197153@qq.com"};
    this->resize(300, 200);
    this->aboutLayout = new QGridLayout(this);
    this->button = new QPushButton("确定", this);
    for (i=0; i<6; i++) {
        this->label[i] = new QLabel(this);
        this->label[i]->setText(message[i]);
    }
    this->button->resize(80, 32);

    this->aboutLayout->setColumnStretch(1, 3);
    this->aboutLayout->addWidget(this->label[0], 0, 0, Qt::AlignRight);
    this->aboutLayout->addWidget(this->label[1], 0, 1, Qt::AlignLeft);
    this->aboutLayout->addWidget(this->label[2], 1, 0, Qt::AlignRight);
    this->aboutLayout->addWidget(this->label[3], 1, 1, Qt::AlignLeft);
    this->aboutLayout->addWidget(this->label[4], 2, 0, Qt::AlignRight);
    this->aboutLayout->addWidget(this->label[5], 2, 1, Qt::AlignLeft);
    this->aboutLayout->addWidget(this->button, 3, 1, Qt::AlignRight);
    this->aboutLayout->setSizeConstraint(QLayout::SetFixedSize);
    this->setLayout(this->aboutLayout);

    // 绑定事件
    connect(this->button, SIGNAL(clicked()), this, SLOT(closeDialog()));
}

AboutDialog::~AboutDialog() {
    int i = 0;
    for (i=0; i<6; i++) {
        delete this->label[i];
    }
    delete this->button;
    delete this->aboutLayout;
}

void AboutDialog::closeDialog() {
    this->close();
}
