#ifndef SCHEDULERROUNDROBIN_H
#define SCHEDULERROUNDROBIN_H

#include "scheduler.h"

class SchedulerRoundRobin : public Scheduler
{
    Q_OBJECT
public:

    SchedulerRoundRobin() : Scheduler(){
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
        emit signalUpdateProcessTable(this->m_prozessPointer, this->m_prozessCounterList.at(this->m_prozessPointer));
    };

private:

    // RoundRobin Quantum
    qint64 m_quantumCounter = 0;
    QList<qint64> m_prozessCounterList;

    qint64 m_processesFinishedCounter = 0;

    void incrementProcessCounterAt(qint64 pos);
};

#endif // SCHEDULERROUNDROBIN_H
