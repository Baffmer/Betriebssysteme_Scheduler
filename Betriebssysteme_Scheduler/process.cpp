#include "process.h"

/*!
 * \brief Process::Process
 */

Process::Process()
{

}

Process::Process(qint64 PID, qint64 priorisierung, qint64 prozessorRegister, qint64 hauptspeicher, qint64 anzahlEinAusgabe, qint64 anzahlThreads, qint64 dauerThreads)
    : m_PID(PID), m_zustand(RECHENBEREIT), m_priorisierung(priorisierung), m_prozessorRegister(prozessorRegister), m_hauptspeicher(hauptspeicher), m_anzahlEinAusgabe(anzahlEinAusgabe), m_anzahlThreads(anzahlThreads), m_dauerThreads(dauerThreads)
{

}

qint64 m_PID;
Zustand m_zustand;
qint64 m_priorisierung;

qint64 Process::prozessorRegister() const
{
    return m_prozessorRegister;
}

void Process::setProzessorRegister(qint64 newProzessorRegister)
{
    m_prozessorRegister = newProzessorRegister;
}

qint64 Process::hauptspeicher() const
{
    return m_hauptspeicher;
}

void Process::setHauptspeicher(qint64 newHauptspeicher)
{
    m_hauptspeicher = newHauptspeicher;
}

qint64 Process::anzahlEinAusgabe() const
{
    return m_anzahlEinAusgabe;
}

void Process::setAnzahlEinAusgabe(qint64 newAnzahlEinAusgabe)
{
    m_anzahlEinAusgabe = newAnzahlEinAusgabe;
}

qint64 Process::anzahlThreads() const
{
    return m_anzahlThreads;
}

void Process::setAnzahlThreads(qint64 newAnzahlThreads)
{
    m_anzahlThreads = newAnzahlThreads;
}

qint64 Process::dauerThreads() const
{
    return m_dauerThreads;
}

void Process::setDauerThreads(qint64 newDauerThreads)
{
    m_dauerThreads = newDauerThreads;
}

qint64 Process::PID() const
{
    return m_PID;
}

void Process::setPID(qint64 newPID)
{
    m_PID = newPID;
}

Zustand Process::zustand() const
{
    return m_zustand;
}

void Process::setZustand(Zustand newZustand)
{
    m_zustand = newZustand;
}

qint64 Process::priorisierung() const
{
    return m_priorisierung;
}

void Process::setPriorisierung(qint64 newPriorisierung)
{
    m_priorisierung = newPriorisierung;
}
