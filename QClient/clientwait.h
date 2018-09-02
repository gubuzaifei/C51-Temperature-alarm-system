#ifndef CLIENTWAIT_H
#define CLIENTWAIT_H
#include <QThread>

class clientwait : public QThread
{
    void run();
public:
    clientwait();
};

#endif // CLIENTWAIT_H
