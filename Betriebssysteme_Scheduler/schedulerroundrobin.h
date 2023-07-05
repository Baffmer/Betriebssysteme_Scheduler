#ifndef SCHEDULERROUNDROBIN_H
#define SCHEDULERROUNDROBIN_H

#include "scheduler.h"

class SchedulerRoundRobin : public Scheduler
{
    Q_OBJECT
public:

    /**
     * @brief SchedulerRoundRobin
     * Konstruktor der Klasse SchedulerRoundRobin
     */
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
        emit signalUpdateProcessTable(this->m_prozessPointer, this->m_prozessCounterList.at(this->m_prozessPointer));
    };

private:

    // RoundRobin Quantum
    qint64 m_quantumCounter = 0; ///< Zeitquantum für Round Robin
    QList<qint64> m_prozessCounterList; ///< Für Round Robin ist ein Tätigkeitsindex für jeden Prozess erforderlich

    qint64 m_processesFinishedCounter = 0; ///< Speichert die Anzahl der beendeten Prozesse

    void incrementProcessCounterAt(qint64 pos); ///< Erhöht die Anzahl der beendeten Prozesse um 1
};

#endif // SCHEDULERROUNDROBIN_H
