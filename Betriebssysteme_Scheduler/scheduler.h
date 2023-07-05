#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QList>

#include "processtable.h"

class Scheduler : public QObject
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

    virtual void handleSheduling() = 0;

    virtual void pauseTimer() = 0;

    virtual void reset() = 0;

    virtual void setSimSpeed(qint64 value) = 0;

    virtual void timerEvent() = 0;

    Scheduler();

    // Timer für Interrupts
    QTimer m_timer;

    // Simulationsgeschwindigkeit
    qint64 m_tick = 250; // normal

    SchedulingStatus m_schedulingStatus = INIT;
    qint64 m_prozessPointer = 0;
    qint64 m_prozessCounter = 0;
};

#endif // SCHEDULER_H
