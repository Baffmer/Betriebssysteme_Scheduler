#ifndef SCHEDULERFIRSTCOMEFIRSTSERVED_H
#define SCHEDULERFIRSTCOMEFIRSTSERVED_H

#include "scheduler.h"

class SchedulerFirstComeFirstServed : public Scheduler
{
    Q_OBJECT
public:

    SchedulerFirstComeFirstServed() : Scheduler(){
        connect(&m_timer, SIGNAL(timeout()), this, SLOT(timerEvent()));

        if(ProcessTable::instance()->simSpeed() == 0){
            this->m_tick = 500;
        } else if (ProcessTable::instance()->simSpeed() == 1) {
            this->m_tick = 250;
        } else {
            this->m_tick = 100;
        }

        m_timer.setSingleShot(true);
    }

    virtual void handleSheduling();

    virtual void pauseTimer();

    virtual void reset();

    virtual void setSimSpeed(qint64 value);

signals:
    void signalUpdateProcessTable(qint64 processPointer, qint64 processCounter);
    void signalShedulingFinished(qint64 sheduler);
    void signalMessageStatusBar(QString message, qint64 timeout);

private slots:
    virtual void timerEvent() {
        handleSheduling();
        emit signalUpdateProcessTable(this->m_prozessPointer, this->m_prozessCounter);
    };
};

#endif // SCHEDULERFIRSTCOMEFIRSTSERVED_H
