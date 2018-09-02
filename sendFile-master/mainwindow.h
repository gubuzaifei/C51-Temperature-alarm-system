#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QProgressDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QLineEdit>
#include <QMenuBar>
#include <QDialog>
#include <QWidget>
#include <QLayout>
#include <QRegExp>
#include <QLabel>
#include <QDebug>
#include <QString>
#include <QMenu>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void on(void (*sendFileFun)(const QString&, const QString&));
private slots:
    void openFileEvent();
    void exitEvent();
    void aboutEvent();
    void sendFileEvent();
    void showErrorEvent( const QString& message );
private:
    QMenuBar*       menuBar;
    QMenu*          fileMenu;
    QMenu*          helpMenu;
    QGridLayout*    MainLayout;
    QVBoxLayout*    middleLayout;
    QHBoxLayout*    topLayout;
    QHBoxLayout*    bottomLayout;
    QLineEdit*      fileInput;
    QLineEdit*      ipAddrInput;
    QPushButton*    browerBtn;
    QPushButton*    sendBtn;
    QString         fileName;
    QString         remoteIpaddr;
    void (*sendFileFun)(const QString&, const QString&);

    bool            checkUserInput();
    void            createMenu();
    void            createMainUI();
};

class AboutDialog: public QDialog {
    Q_OBJECT
public:
    AboutDialog(QWidget* parent);
    ~AboutDialog();
private:
    QGridLayout*        aboutLayout;
    QLabel*             label[6];
    QPushButton*        button;
private slots:
    void closeDialog();
};

#endif // MAINWINDOW_H
