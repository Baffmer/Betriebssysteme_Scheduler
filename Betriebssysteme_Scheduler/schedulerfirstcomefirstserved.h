#ifndef SCHEDULERFIRSTCOMEFIRSTSERVED_H
#define SCHEDULERFIRSTCOMEFIRSTSERVED_H

#include <QObject>
#include <QDebug>
#include <QTimer>

#include "processtable.h"

class SchedulerFirstComeFirstServed : QObject
{
    Q_OBJECT
public:
    SchedulerFirstComeFirstServed();

    void startFirstComeFirstServedSheduling();

signals:
    void signalUpdateProcessTable();

private slots:
    void timerEvent() {
        emit signalUpdateProcessTable();
        qDebug() << "tick";
    };

private:
    QTimer timer;
};

#endif // SCHEDULERFIRSTCOMEFIRSTSERVED_H
