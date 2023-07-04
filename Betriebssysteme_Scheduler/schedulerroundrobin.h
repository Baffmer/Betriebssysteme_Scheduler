#ifndef SCHEDULERROUNDROBIN_H
#define SCHEDULERROUNDROBIN_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QList>

#include "processtable.h"

class SchedulerRoundRobin : public QObject
{
    Q_OBJECT
public:

    enum SchedulingStatus{
        INIT,
        PROZESS,
        PROZESSWECHSEL,
        ENDE,
        PAUSIERT
    };

    SchedulerRoundRobin();

    void handleRoundRobinSheduling();

    void pauseTimer();

    void reset();

    void setSimSpeed(qint64 value);

signals:
    void signalUpdateProcessTable(qint64 processPointer, qint64 processCounter);
    void signalShedulingFinished(qint64 sheduler);
    void signalMessageStatusBar(QString message, qint64 timeout);

private slots:
    void timerEvent() {
        handleRoundRobinSheduling();
        emit signalUpdateProcessTable(this->m_prozessPointer, this->m_prozessCounterList.at(this->m_prozessPointer));
    };

private:
    QTimer m_timer;

    // Simulationsgeschwindigkeit
    qint64 m_tick = 250; // normal

    SchedulingStatus m_schedulingStatus = INIT;
    qint64 m_prozessPointer = 0;
    qint64 m_prozessCounter = 0;

    // RoundRobin Quantum
    qint64 m_quantumCounter = 0;
    QList<qint64> m_prozessCounterList;

    qint64 m_processesFinishedCounter = 0;

    void incrementProcessCounterAt(qint64 pos);
};

#endif // SCHEDULERROUNDROBIN_H
