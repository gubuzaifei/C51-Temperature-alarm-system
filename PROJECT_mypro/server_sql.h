#ifndef SERVER_SQL_H
#define SERVER_SQL_H

#include <QObject>

class server_sql : public QObject
{
    Q_OBJECT
public:
    explicit server_sql(QObject *parent = 0);

signals:

public slots:
//    QString regist_insert(QString ID);
};

#endif // SERVER_SQL_H
