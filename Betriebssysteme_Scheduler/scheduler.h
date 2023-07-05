#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QList>

#include "processtable.h"

/**
 * @brief Scheduler Oberklasse
 */
class Scheduler : public QObject
{
    Q_OBJECT

public:

    /**
     * @brief The SchedulingStatus enum
     */
    enum SchedulingStatus{
        INIT,
        PROZESS,
        PROZESSWECHSEL,
        ENDE,
        PAUSIERT
    };

    /**
     * @brief handleSheduling
     * Führt das Scheduling aus.
     */
    virtual void handleSheduling() = 0;

    /**
     * @brief pauseTimer
     * Pausiert den Timer und somit das Scheduling.
     */
    virtual void pauseTimer() = 0;

    /**
     * @brief reset
     * Setzt das Scheduling auf die Standardwerte zurück
     */
    virtual void reset() = 0;

    /**
     * @brief setSimSpeed
     * @param value
     * Setzt die Simulationsgeschwindigkeit (0=langsam, 1=normal, 2=schnell)
     */
    virtual void setSimSpeed(qint64 value) = 0;

    /**
     * @brief timerEvent
     * Callbackfunktion für den Timer
     */
    virtual void timerEvent() = 0;

    /**
     * @brief Scheduler
     * Konstruktor
     */
    Scheduler();

    // Timer für Interrupts
    QTimer m_timer; ///< QTimer für die Interrupts

    // Simulationsgeschwindigkeit
    qint64 m_tick = 250; ///< Simulationsgeschwindigkeit (0=langsam, 1=normal, 2=schnell)

    SchedulingStatus m_schedulingStatus = INIT; ///< Scheduler Status
    qint64 m_prozessPointer = 0; ///< enthält den Index des zur Zeit bearbeiteten Prozesses
    qint64 m_prozessCounter = 0; ///< enthält den Index vom Zeitstrahl des bearbeiteten Prozesses
};

#endif // SCHEDULER_H
