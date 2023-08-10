#ifndef PROCESSTABLE_H
#define PROCESSTABLE_H

#include <QDebug>
#include <QList>

#include "process.h"

/**
 * @brief The ProcessTable class
 */
class ProcessTable : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief instance
     * @return Pointer auf das Singleton Objekt
     */
    static ProcessTable* instance();

    /**
     * @brief addProcess
     * @param process
     * Fügt der m_processList einen neuen Prozess hinzu.
     */
    void addProcess(Process process);

    /**
     * @brief removeProcess
     * @param process
     * Entfernt den angegeben Prozess aus der m_processList.
     */
    void removeProcess(Process process);

    /**
     * @brief removeProcessAt
     * @param pos
     * Entfernt einen Prozess aus der angegebenen Position in der m_processList.
     */
    void removeProcessAt(qint64 pos);

    /**
     * @brief removeProcessByPID
     * @param PID
     * Entfernt einen Prozess mit der angegebenen PID aus der m_processList.
     */
    void removeProcessByPID(qint64 PID);

    /**
     * @brief addProcessAt
     * @param pos
     * @param process
     * Fügt einen übergebenen Prozess an der übergebenen Position in der m_processList hinzu.
     */
    void addProcessAt(qint64 pos, Process process);

    /**
     * @brief updateProcessAt
     * @param pos
     * @param process
     * Überschreibt den übergebenen Prozess an der übergebenen Position in der m_processList.
     */
    void updateProcessAt(qint64 pos, Process process);

    /**
     * @brief updateProcessByPID
     * @param PID
     * @param process
     * Überschreibt den übergebenen Prozess mit der übergebenen PID in der m_processList.
     */
    void updateProcessByPID(qint64 PID, Process process);

    /**
     * @brief removeAllProcesses
     * Entfernt alle Prozesse aus der m_processList
     */
    void removeAllProcesses();

    /**
     * @brief sortProcessListByPrio
     * Sortiert alle Prozesse in der m_processList absteigend nach der Priorität.
     */
    void sortProcessListByPrio();

    /**
     * @brief sortProcessListByPID
     * Sortiert alle Prozesse in der m_processList absteigend nach der PID.
     */
    void sortProcessListByPID();

    /**
     * @brief sortProcessListByTime
     * Sortiert alle Prozesse in der m_processList absteigend nach der zeitlichen Länge der Prozesse.
     */
    void sortProcessListByTime();

    /**
     * @brief getProcessByPID
     * @param PID
     * @return Process
     * Getter für einen Prozess aus der m_processList abhängig von der PID.
     */
    Process getProcessByPID(qint64 PID);

    /**
     * @brief sizeProcessList
     * @return Länge der m_processList (Anzahl der Prozesse)
     */
    qint64 sizeProcessList();

    /**
     * @brief printAllProcesses
     * Gibt alle Prozesse auf der Konsole aus
     */
    void printAllProcesses();

    /**
     * @brief processList
     * @return m_processList
     * Getter für m_processList
     */
    QList<Process> processList();

    /**
     * @brief quantum
     * @return m_quantum
     * Getter für m_quantum
     */
    qint64 quantum() const;

    /**
     * @brief setQuantum
     * @param newQuantum
     * Setter für m_quantum
     */
    void setQuantum(qint64 newQuantum);

    /**
     * @brief ioDauer
     * @return m_ioDauer
     * Getter für m_ioDauer
     */
    qint64 ioDauer() const;

    /**
     * @brief setIoDauer
     * @param newIoDauer
     * Setter für m_ioDauer
     */
    void setIoDauer(qint64 newIoDauer);

    /**
     * @brief simSpeed
     * @return m_simSpeed
     * Getter für m_simSpeed
     */
    qint64 simSpeed() const;

    /**
     * @brief setSimSpeed
     * @param newSimSpeed
     * Setter für m_simSpeed
     */
    void setSimSpeed(qint64 newSimSpeed);

    /**
     * @brief dauerProzesswechsel
     * @return m_dauerProzesswechsel
     * Getter für m_dauerProzesswechsel
     */
    qint64 dauerProzesswechsel() const;

    /**
     * @brief setDauerProzesswechsel
     * @param newDauerProzesswechsel
     * Setter für m_dauerProzesswechsel
     */
    void setDauerProzesswechsel(qint64 newDauerProzesswechsel);

    /**
     * @brief updateTimeLines
     * Erstellt alle Zeitstrahlen der Prozesse in m_processList neu
     */
    void updateTimeLines();

    /**
     * @brief setProzessZustand
     * @param pos
     * @param zustand
     *
     * Setzt den übergebenen Zustand an der übergebenen Position in der m_processList
     */
    void setProzessZustand(qint64 pos, Process::Zustand zustand);

    /**
     * @brief getSimTimeSlotsSum
     * @return Anzahl der Slots im Zeitstrahl (Länge der m_timeLineList)
     */
    qint64 getSimTimeSlotsSum();

    /**
     * @brief resetAnzahlProzesswechsel
     * Setzt die Anzahl der Prozesswechsel einer Simulation wieder auf 0 zurück, damit die nächste Simulation evaluiert werden kann.
     */
    void resetAnzahlProzesswechsel();

    /**
     * @brief incrementAnzahlProzesswechsel
     * Erhöht die Anzahl der Prozesswechsel um 1 (für die Evaluation)
     */
    void incrementAnzahlProzesswechsel();

    /**
     * @brief getAnzahlProzesswechsel
     * @return m_anzahlProzesswechsel
     * Getter für m_anzahlProzesswechsel (für die Evaluation)
     */
    qint64 getAnzahlProzesswechsel();

    /**
     * @brief resetSimulation
     * Setzt alle Prozesse der m_processList auf deren Standardwerte zurück.
     */
    void resetSimulation();

    /**
     * @brief currentPID
     * @return m_currentPID
     *
     * Getter für m_currentPID
     *
     */
    qint64 currentPID() const;

    /**
     * @brief setCurrentPID
     * @param newCurrentPID
     *
     * Setter für m_currentPID
     *
     */
    void setCurrentPID(qint64 newCurrentPID);

private:
    /**
     * @brief ProcessTable
     * private Konstruktor für das Singleton ProcessTable
     */
    ProcessTable();

    /**
     * @brief emitProcessTableUpdate
     * Signal zum Refreshen der Prozess Tabelle in der Benutzeroberfläche
     */
    void emitProcessTableUpdate();

    qint64 m_currentPID = 0; ///< die aktuell größte PID

    QList<Process> m_processList; ///< QList für die Prozesse

    // Sim Parameter
    qint64 m_quantum = 10; ///< Zeit Quantum für den Round Robin Scheduler
    qint64 m_ioDauer = 2; ///< Zeitdauer einer Ein- bzw. Ausgabeoperation
    qint64 m_simSpeed = 1; ///< Simulationsgeschwindigkeit (0=langsam, 1=normal, 2=schnell)
    qint64 m_dauerProzesswechsel = 5; ///< Zeitdauer eines Prozesswechsels

    qint64 m_simTimeSlotsSum = 0; ///< Summe aller Zeitslots der Zeitstrahlen in der m_processList (benötigt für die ProgressBar)

    // Evaluation
    qint64 m_anzahlProzesswechsel = 0; ///< Anzahl der Prozesswechsel (für die Evaluation)
    qint64 m_simulationsdauer = 0; ///< Zeitmessung der Simulation (für die Evaluation)

signals:
    void signalProcessListChanged(); ///< Signal für die GUI Prozess Tabelle, wenn sich etwas in der m_processList geändert hat, z.B. beim Erstellen/Bearbeiten/Löschen eines Prozesses
};

#endif // PROCESSTABLE_H
