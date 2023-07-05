#ifndef SCHEDULERFIRSTCOMEFIRSTSERVED_H
#define SCHEDULERFIRSTCOMEFIRSTSERVED_H

#include "scheduler.h"

class SchedulerFirstComeFirstServed : public Scheduler
{
    Q_OBJECT
public:

    /**
     * @brief SchedulerFirstComeFirstServed
     * Konstruktor der Klasse SchedulerFirstComeFirstServed
     */
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

    /**
     * @brief handleSheduling
     * Führt das Scheduling aus.
     */
    virtual void handleSheduling();

    /**
     * @brief pauseTimer
     * Pausiert den Timer und somit das Scheduling.
     */
    virtual void pauseTimer();

    /**
     * @brief reset
     * Setzt das Scheduling auf die Standardwerte zurück
     */
    virtual void reset();

    /**
     * @brief setSimSpeed
     * @param value
     * Setzt die Simulationsgeschwindigkeit (0=langsam, 1=normal, 2=schnell)
     */
    virtual void setSimSpeed(qint64 value);

signals:
    /**
     * @brief signalUpdateProcessTable
     * @param processPointer
     * @param processCounter
     *
     * Signal zum Refreshen der ProzessTabelle in der GUI.
     */
    void signalUpdateProcessTable(qint64 processPointer, qint64 processCounter);

    /**
     * @brief signalShedulingFinished
     * @param sheduler
     *
     * Signal wird zur GUI gesendet wenn das Scheduling beendet wurde.
     */
    void signalShedulingFinished(qint64 sheduler);

    /**
     * @brief signalMessageStatusBar
     * @param message
     * @param timeout
     *
     * Signal für Nachrichten in der StatusBar des Hauptfensters.
     */
    void signalMessageStatusBar(QString message, qint64 timeout);

private slots:
    /**
     * @brief timerEvent
     * Callbackfuntion für den QTimer
     * -# Springt wieder in die handleSheduling Methode
     * -# Sendet das Signal signalUpdateProcessTable an das Hauptfenster, damit die Simulation in der Process Tabelle angezeigt werden kann
     */
    virtual void timerEvent() {
        handleSheduling();
        emit signalUpdateProcessTable(this->m_prozessPointer, this->m_prozessCounter);
    };
};

#endif // SCHEDULERFIRSTCOMEFIRSTSERVED_H
