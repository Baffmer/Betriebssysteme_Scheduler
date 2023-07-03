#include "process.h"

/*!
 * \brief Process::Process
 */

Process::Process()
{

}

Process::Process(qint64 PID, qint64 priorisierung, qint64 prozessorRegister, qint64 hauptspeicher, qint64 anzahlEinAusgabe, qint64 anzahlThreads, qint64 dauerThreads)
    : m_PID(PID), m_zustand(Zustand::RECHENBEREIT), m_priorisierung(priorisierung), m_prozessorRegister(prozessorRegister), m_hauptspeicher(hauptspeicher), m_anzahlEinAusgabe(anzahlEinAusgabe), m_anzahlThreads(anzahlThreads), m_dauerThreads(dauerThreads)
{

}

qint64 m_PID;
Process::Zustand m_zustand;
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

Process::Zustand Process::zustand() const
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

void Process::createTimeLine(qint64 ioDauer)
{
    // reset Timeline / remove all I/O Slots
    for(qint64 i=0; i<this->m_timeLineList.size(); i++){
        if(this->m_timeLineList.at(i).contains("I/O")){
            this->m_timeLineList.remove(i);
        }
    }

    this->m_ioDauer = ioDauer;

    // Länge des Zeitstrahls berechnen
    qint64 length = this->m_anzahlEinAusgabe * this->ioDauer() + this->m_anzahlThreads * this->m_dauerThreads;

    qint64 ioSlot = (this->m_anzahlThreads * this->m_dauerThreads)/this->m_anzahlEinAusgabe;

    qDebug() << "Dauer Zeitstrahl" << length << "IOslot" << ioSlot;

    for(qint64 i=0; i<this->anzahlThreads(); i++){
        for(qint64 j=0; j<this->dauerThreads(); j++){
            m_timeLineList.append("Thread " + QString::number(i+1));
        }
    }

    qint64 counter = 1;
    // Zeitstrahl mit I/O füllen
    for(qint64 i=1; i<length; i++){
        if(counter % ioSlot == 0){
            for(qint64 j=0; j<this->ioDauer(); j++){
                if(i>m_timeLineList.size()){
                    m_timeLineList.append("I/O");
                    //length++;
                    i++;
                    counter = 0;
                } else {
                    m_timeLineList.insert(i, "I/O");
                    //length++;
                    i++;
                    counter = 0;
                }
            }
        }
        counter++;
    }

    printTimeLine();
}

void Process::printTimeLine()
{
    QDebug deb = qDebug();

    for(QString &myString : m_timeLineList){
        deb << myString;
    }
}

QList<QString> Process::timeLineList() const
{
    return m_timeLineList;
}

qint64 Process::ioDauer() const
{
    return m_ioDauer;
}

void Process::setIoDauer(qint64 newIoDauer)
{
    m_ioDauer = newIoDauer;
}

