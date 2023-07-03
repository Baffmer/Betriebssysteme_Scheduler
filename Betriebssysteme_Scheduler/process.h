#ifndef PROCESS_H
#define PROCESS_H

#include <QDialog>
#include <QDebug>

class Process
{

public:
    Process();
    Process(qint64 PID, qint64 priorisierung, qint64 prozessorRegister, qint64 hauptspeicher, qint64 anzahlEinAusgabe, qint64 anzahlThreads, qint64 dauerThreads);

    enum Zustand{
        BLOCKIERT,
        RECHENBEREIT,
        RECHNEND,
        ABGESCHLOSSEN,
        ABGEBROCHEN
    };

    qint64 prozessorRegister() const;
    void setProzessorRegister(qint64 newProzessorRegister);

    qint64 hauptspeicher() const;
    void setHauptspeicher(qint64 newHauptspeicher);

    qint64 anzahlEinAusgabe() const;
    void setAnzahlEinAusgabe(qint64 newAnzahlEinAusgabe);

    qint64 anzahlThreads() const;
    void setAnzahlThreads(qint64 newAnzahlThreads);

    qint64 dauerThreads() const;
    void setDauerThreads(qint64 newDauerThreads);

    qint64 PID() const;
    void setPID(qint64 newPID);

    Zustand zustand() const;
    void setZustand(Zustand newZustand);

    qint64 priorisierung() const;
    void setPriorisierung(qint64 newPriorisierung);

    bool operator == (const Process& process) const {
        return m_PID == process.PID();
    }

    bool operator < (const Process& process) const {
        if (m_priorisierung < process.priorisierung()){
            return -1;
        } else if(m_priorisierung > process.priorisierung()){
            return 1;
        } else {
            return 0;
        }
    }

    qint64 ioDauer() const;
    void setIoDauer(qint64 newIoDauer);
    void createTimeLine(qint64 ioDauer);

    QList<QString> timeLineList() const;

private:

    void printTimeLine();

    // Prozesstabelle
    qint64 m_PID;
    Zustand m_zustand;
    qint64 m_priorisierung;

    // Prozessinformationstabelle
    qint64 m_prozessorRegister;
    qint64 m_hauptspeicher;
    qint64 m_anzahlEinAusgabe;
    qint64 m_anzahlThreads;
    qint64 m_dauerThreads;
    qint64 m_ioDauer;

    //Zeitstrahl
    QList<QString> m_timeLineList;
};
#endif // PROCESS_H
