#include "mythread.h"
#include "myserver.h"
#include <QStringList>

mythread::mythread(qintptr ID, QObject *parent) :
    QThread(parent)
{
    this->socketDescriptor = ID;

    connect(this,SIGNAL(user_offline(QTcpSocket*)),this,SLOT(delete_from_onlinelist(QTcpSocket*)));
}

void mythread::run()
{
    qDebug() << " Thread started";
    socket = new QTcpSocket();
    if(!socket->setSocketDescriptor(this->socketDescriptor))
    {
        emit error(socket->error());
        return;
    }
    connect(socket, SIGNAL(readyRead()), this, SLOT(read_message()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    qDebug() << socketDescriptor << " Client connected";
    exec();
}

void mythread::read_message()
{
    bool login_flag = false;
    QStringList Data_List;              //flag and message
    QString Data = socket->readAll();
    qDebug() << Data;
    Data_List = Data.split("1\n");

    if(Data_List[0] == "0")             //regist flag---------------------------------------------------------------
    {
        QStringList reglist;
        QString reg_message;
        reg_message = Data_List[1];     //ID2\nName3\nCode
        reglist = reg_message.split("2\n");     //ID    Name3\nCode

        QString reg_id;
        QString reg_name;
        QString reg_psw;

        reg_id = reglist[0];

        reg_message = reglist[1];       //Name3\nCode
        reglist = reg_message.split("3\n");     //Name  Code
        reg_name = reglist[0];
        reg_psw = reglist[1];

        QSqlDatabase database;
        if(QSqlDatabase::contains("qt_sql_default_connection"))
            database = QSqlDatabase::database("qt_sql_default_connection");
        else
            database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("kuku.db");

        if(database.open())
        {
            //qDebug() << "hello";
            QSqlQuery reg_query;
            QString insert_reg = "insert into reg values(?,?,?)";
//            QString select_all_reg = "select * from reg";

            reg_query.prepare(insert_reg);

            QVariantList groupid;
            groupid.append(reg_id.toInt());

            QVariantList groupusername;
            groupusername.append(reg_name);

            QVariantList grouppsw;
            grouppsw.append(reg_psw);

            reg_query.addBindValue(groupid);
            reg_query.addBindValue(groupusername);
            reg_query.addBindValue(grouppsw);

            if(!reg_query.execBatch())
            {
                //qDebug() << "hello";
                socket->write("41\n");
                qDebug()<<reg_query.lastError();
                database.close();
            }
            else
            {
                socket->write("51\n");
                qDebug()<<"insert record success";
            }

//            reg_query.prepare(select_all_reg);
//            int id;
//            QString name;
//            QString password;
//            if(!reg_query.exec())
//            {
//                qDebug()<<reg_query.lastError();
//            }
//            else
//            {
//                while(reg_query.next())
//                {
//                    id = reg_query.value(0).toInt();
//                    name = reg_query.value(1).toString();
//                    password = reg_query.value(2).toString();
//                    qDebug() << QString("ID:%1  Name:%2  Password:%3").arg(id).arg(name).arg(password);
//                }
//            }
        }
        database.close();
    }

    if(Data_List[0] == "1")  //login----------------------------------------------------------------------------------------
    {
        QSqlDatabase database;
        if(QSqlDatabase::contains("qt_sql_default_connection"))
            database = QSqlDatabase::database("qt_sql_default_connection");
        else
            database = QSqlDatabase::addDatabase("QSQLITE");

        database.setDatabaseName("kuku.db");
        if(database.open())
        {
            QSqlQuery reg_query;
            QString online_user;
            QStringList online_user_ip;

            QString login_id;
            QString login_psw;
            QStringList loginlist;
            QString login;
            login = Data_List[1];               //ID2\nCode
            loginlist = login.split("2\n");
            login_id = loginlist[0];
            login_psw = loginlist[1];
            QString select_all_reg = "select * from reg";
            reg_query.prepare(select_all_reg);
            if(!reg_query.exec())
            {
                qDebug()<<reg_query.lastError();
            }
            else
            {
                while(reg_query.next())
                {
                    if(login_id == reg_query.value(0).toString())
                    {
                        if(login_psw == reg_query.value(2))
                        {
                            login_flag  = true;
                            socket->write("21\n");              //to client login success flag
                            socket->waitForBytesWritten(100);
                            usleep(10000);
                            socket_list.append(socket);
                            IP_list.append(socket->peerAddress().toString());

                            online_user_ip = socket->peerAddress().toString().split("ff:");
                            //emit send_onlinelist();

//                            QString insert_reg = "insert into  values(?,?,?)";
                //            QString select_all_reg = "select * from reg";

//                            reg_query.prepare(insert_reg);

//                            QVariantList groupid;
//                            groupid.append(reg_id.toInt());

//                            QVariantList groupusername;
//                            groupusername.append(reg_name);

//                            QVariantList grouppsw;
//                            grouppsw.append(reg_psw);

//                            reg_query.addBindValue(groupid);
//                            reg_query.addBindValue(groupusername);
//                            reg_query.addBindValue(grouppsw);

//                            if(!reg_query.execBatch())
//                            {
//                                //qDebug() << "hello";
//                                socket->write("41\n");
//                                qDebug()<<reg_query.lastError();
//                                database.close();
//                            }
                            if(socket_list.size() == 1)
                            {
                                for(int j = 0;j < socket_list.size();j++)
                                {
                                    for(int i = 0;i < IP_list.size();i++)
                                    {
                                    qDebug() << "***";
                                    qDebug() << socket_list.size();
                                    qDebug() << IP_list.size();
                                    online_user_ip =  IP_list.at(i).split("ff:");
                                    online_user = "31\n"+online_user_ip[1];
                                    socket_list.at(j)->write(online_user.toStdString().c_str());    //send online list
                                    qDebug() << online_user.toStdString().c_str()<< "aaabbb";
                                    socket_list.at(j)->waitForBytesWritten(100);
                                    usleep(40000);
                                    }
                                }
                            }
                            if(socket_list.size() == 2)
                            {
                                for(int j = 0;j < socket_list.size();j++)
                                {
                                socket_list.at(j)->write("31\n0");          //clear client user_lw;
                                socket_list.at(j)->waitForBytesWritten(100);

                                usleep(5000);
                                qDebug() << "**--";
                                qDebug() << socket_list.size();
                                }
                                for(int j = 0;j < socket_list.size();j++)
                                {
                                    for(int i = 0;i < IP_list.size();i++)
                                    {
                                    qDebug() << "***";
                                    qDebug() << socket_list.size();
                                    qDebug() << IP_list.size();
                                    online_user_ip =  IP_list.at(i).split("ff:");
                                    online_user = "31\n"+online_user_ip[1];
                                    socket_list.at(j)->write(online_user.toStdString().c_str());    //send online list
                                    qDebug() << online_user.toStdString().c_str()<< "aaabbb";
                                    socket_list.at(j)->waitForBytesWritten(100);
                                    usleep(100000);
                                    }
                                }

                            }
                            if(socket_list.size() > 2)
                            {
                                for(int j = 0;j < socket_list.size();j++)
                                {
                                socket_list.at(j)->write("31\n0");          //clear client user_lw;
                                socket_list.at(j)->waitForBytesWritten(100);

                                usleep(5000);
                                qDebug() << "**--";
                                qDebug() << socket_list.size();
                                }

                                for(int j = 0;j < socket_list.size();j++)
                                {
                                    for(int i = 0;i < IP_list.size();i++)
                                    {
                                    qDebug() << "***";
                                    qDebug() << socket_list.size();
                                    qDebug() << IP_list.size();
                                    online_user_ip =  IP_list.at(i).split("ff:");
                                    online_user = "31\n"+online_user_ip[1];
                                    socket_list.at(j)->write(online_user.toStdString().c_str());    //send online list
                                    qDebug() << online_user.toStdString().c_str()<< "aaabbb";
                                    socket_list.at(j)->waitForBytesWritten(100);
                                    usleep(40000);


                                    }
                                }
                            }
                        }
                        else
                        {
                            socket->write("01\n");  //to client password error flag
                            return;
                        }
                    } 
                }
            }
            if(!login_flag)
            {
                socket->write("11\n");      //to client ID no exist flag
                return ;
            }
        }
    }
    if(Data_List[0] == "2")                 //chat message flag--------------------------------------------------------------
    {
        QStringList msglist;
        QString msg = Data_List[1];
        qDebug() << "data_list:" <<Data_List[1];

        QString send_msg;
        QStringList send_IP;
        msglist = msg.split("2\n");
        int i,loc;
        for(i = 0;i < IP_list.size();++i)   //find position
        {
            //qDebug() << IP_list.at(i);
            send_IP = IP_list.at(i).split("ff:");
            if(msglist[1] == send_IP[1])
            {
                qDebug() << msglist[1] << send_IP[1];
                loc = i;
                break;
            }
        }
        send_msg = "41\n"+ msglist[0];
//        qDebug() << "send_msg:" << send_msg <<loc << i;
        socket_list.at(loc)->write(send_msg.toStdString().c_str());
        socket_list.at(loc)->waitForBytesWritten(30000);
    }
    if(Data_List[0] == "3")         //client offline back to login window flag
    {
        emit user_offline(socket);
    }
}

void mythread::delete_from_onlinelist(QTcpSocket *socket)
{
    int loc;
    int j;
    QString online_user;
    QStringList online_user_ip;


    for(j = 0;j < socket_list.size();j++)
    {
        if(socket_list.at(j) == socket)
        {
            loc = j;
            break;
        }
    }

    socket_list.removeOne(socket);
    IP_list.removeAt(loc);

    for(int j = 0;j < socket_list.size();j++)
    {
        socket_list.at(j)->write("31\n0");  //clear client user_lw;
        socket_list.at(j)->waitForBytesWritten(100);
        usleep(50000);
    }

    for(int j = 0;j < socket_list.size();j++)               //update online user list
    {
        for(int i = 0;i < IP_list.size();i++)
        {
            online_user_ip =  IP_list.at(i).split("ff:");
            online_user = "31\n"+online_user_ip[1];
            socket_list.at(j)->write(online_user.toStdString().c_str());
            socket_list.at(j)->waitForBytesWritten(30000);
            usleep(10000);
        }
    }
}

void mythread::disconnected()       //remove offline user from socket_list and IP_list
{
    qDebug() << socketDescriptor << " Disconnected";

    emit user_offline(socket);

    socket->deleteLater();
    exit(0);
}
