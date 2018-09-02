#include "serverstartwindow.h"
#include "ui_mainwindow.h"
#include "myserver.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Welcome");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_start_bt_clicked()
{

    next = new server_show();
    next->show();
    this->close();
}

void MainWindow::on_exit_bt_clicked()
{
    this->close();
}
