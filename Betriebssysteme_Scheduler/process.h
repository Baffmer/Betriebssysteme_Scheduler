#ifndef PROCESS_H
#define PROCESS_H

#include <QDialog>
#include <QDebug>

/**
 * @brief The Process class
 */
class Process
{

public:

    /**
     * @brief Process
     *
     * Standardkonstruktor für einen Prozess
     *
     */
    Process();

    /**
     * @brief Process
     * @param PID
     * @param priorisierung
     * @param prozessorRegister
     * @param hauptspeicher
     * @param anzahlEinAusgabe
     * @param anzahlThreads
     * @param dauerThreads
     *
     * Konstruktor für einen Prozess
     *
     */
    Process(qint64 PID, qint64 priorisierung, qint64 prozessorRegister, qint64 hauptspeicher, qint64 anzahlEinAusgabe, qint64 anzahlThreads, qint64 dauerThreads);

    /**
     * @brief The Zustand enum
     */
    enum Zustand{
        BLOCKIERT,
        RECHENBEREIT,
        RECHNEND,
        ABGESCHLOSSEN,
        ABGEBROCHEN
    };

    /**
     * @brief prozessorRegister
     * @return m_prozessorRegister
     * Getter für m_prozessorRegister
     */
    qint64 prozessorRegister() const;

    /**
     * @brief setProzessorRegister
     * @param newProzessorRegister
     * Setter für m_prozessorRegister
     */
    void setProzessorRegister(qint64 newProzessorRegister);

    /**
     * @brief hauptspeicher
     * @return m_hauptspeicher
     * Getter für m_hauptspeicher
     */
    qint64 hauptspeicher() const;

    /**
     * @brief setHauptspeicher
     * @param newHauptspeicher
     * Setter für m_hautspeicher
     */
    void setHauptspeicher(qint64 newHauptspeicher);

    /**
     * @brief anzahlEinAusgabe
     * @return m_anzahlEinAusgabe
     * Getter für m_anzahlEinAusgabe
     */
    qint64 anzahlEinAusgabe() const;

    /**
     * @brief setAnzahlEinAusgabe
     * @param newAnzahlEinAusgabe
     * Setter für m_anzahlEinAusgabe
     */
    void setAnzahlEinAusgabe(qint64 newAnzahlEinAusgabe);

    /**
     * @brief anzahlThreads
     * @return m_anzahlThreads
     * Getter für m_anzahlThreads
     */
    qint64 anzahlThreads() const;

    /**
     * @brief setAnzahlThreads
     * @param newAnzahlThreads
     * Setter für m_anzahlThreads
     */
    void setAnzahlThreads(qint64 newAnzahlThreads);

    /**
     * @brief dauerThreads
     * @return m_dauerThreads
     * Getter für m_dauerThreads
     */
    qint64 dauerThreads() const;

    /**
     * @brief setDauerThreads
     * @param newDauerThreads
     * Setter für m_dauerThreads
     */
    void setDauerThreads(qint64 newDauerThreads);

    /**
     * @brief PID
     * @return m_PID
     * Getter für m_PID
     */
    qint64 PID() const;

    /**
     * @brief setPID
     * @param newPID
     * Setter m_PID
     */
    void setPID(qint64 newPID);

    /**
     * @brief zustand
     * @return m_zustand
     * Getter für m_zustand
     */
    Zustand zustand() const;

    /**
     * @brief setZustand
     * @param newZustand
     * Setter für m_zustand
     */
    void setZustand(Zustand newZustand);

    /**
     * @brief priorisierung
     * @return m_priorisierung
     * Getter für m_priorisierung
     */
    qint64 priorisierung() const;

    /**
     * @brief setPriorisierung
     * @param newPriorisierung
     * Setter für m_priorisierung
     */
    void setPriorisierung(qint64 newPriorisierung);

    /**
     * @brief operator ==
     * @param process
     * @return true wenn PIDs identisch false stattdessen
     */
    bool operator == (const Process& process) const {
        return m_PID == process.PID();
    }

    /**
     * @brief operator <
     * @param process
     * @return -1 wenn Prio kleiner als Vergleichswert, 1 wenn größer, 0 wenn identisch
     */
    bool operator < (const Process& process) const {
        if (m_priorisierung < process.priorisierung()){
            return -1;
        } else if(m_priorisierung > process.priorisierung()){
            return 1;
        } else {
            return 0;
        }
    }

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
     * @brief createTimeLine
     * @param ioDauer
     *
     * Erstellt einen Zeitstrahl für disen Process als QList.
     * Teilt die Threads in Zeitslots ein und verteilt m_anzahlEinAusgabe I/O Slots mit der Länge m_ioDauer.
     *
     */
    void createTimeLine(qint64 ioDauer);

    /**
     * @brief timeLineList
     * @return m_timeLineList
     * Getter für m_timeLineList
     */
    QList<QString> timeLineList() const;

private:

    /**
     * @brief printTimeLine
     * Gibt den Zeitstrahl auf der Konsole aus.
     */
    void printTimeLine();

    // Prozesstabelle
    qint64 m_PID; ///< Prozess ID Nummer
    Zustand m_zustand; ///< Zustand dees Prozesses (RECHNEND, RECHENBEREIT, BLOCKIERT, ABGEBROCHEN, ABGESCHLOSSEN)
    qint64 m_priorisierung; ///< Priorisierung des Prozesses, kleine Nummer -> hohe Priorisierung

    // Prozessinformationstabelle
    qint64 m_prozessorRegister; ///< Platzhalter für ProzessRegister
    qint64 m_hauptspeicher; ///< Platzhalter für Hauptspeicher
    qint64 m_anzahlEinAusgabe; ///< Anzahl der Ein- u. Ausgabeoperationen
    qint64 m_anzahlThreads; ///< Anzahl der Threads in dem Prozess
    qint64 m_dauerThreads; ///< Dauer eines Threads
    qint64 m_ioDauer; ///< Dauer einer Ein- u. Ausgabeoperation

    //Zeitstrahl
    QList<QString> m_timeLineList; ///< QList für den Zeitstrahl
};
#endif // PROCESS_H
