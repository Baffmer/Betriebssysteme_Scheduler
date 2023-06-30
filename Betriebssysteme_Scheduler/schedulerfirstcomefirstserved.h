#ifndef SCHEDULERFIRSTCOMEFIRSTSERVED_H
#define SCHEDULERFIRSTCOMEFIRSTSERVED_H

#include <QObject>
#include <QDebug>
#include <QTimer>

#include "processtable.h"

class SchedulerFirstComeFirstServed : public QObject
{
    Q_OBJECT
public:

    enum SchedulingStatus{
        INIT,
        PROZESS,
        PROZESSWECHSEL,
        ENDE
    };

    SchedulerFirstComeFirstServed();

    void handleFirstComeFirstServedSheduling();

signals:
    void signalUpdateProcessTable(qint64 processPointer, qint64 processCounter);

private slots:
    void timerEvent() {
        handleFirstComeFirstServedSheduling();
        emit signalUpdateProcessTable(this->m_prozessPointer, this->m_prozessCounter);
    };

private:
    QTimer m_timer;

    // Simulationsgeschwindigkeit
    qint64 m_tick = 500; // normal

    SchedulingStatus m_schedulingStatus = INIT;
    qint64 m_prozessPointer = 0;
    qint64 m_prozessCounter = 0;
};

#endif // SCHEDULERFIRSTCOMEFIRSTSERVED_H
