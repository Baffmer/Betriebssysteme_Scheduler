#include "schedulerfirstcomefirstserved.h"

SchedulerFirstComeFirstServed::SchedulerFirstComeFirstServed()
{
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

void SchedulerFirstComeFirstServed::handleFirstComeFirstServedSheduling()
{
    // State Machine

    switch(this->m_schedulingStatus){
    case INIT:
        qDebug() << "Anzahl Prozesse" << ProcessTable::instance()->processList().size();
        if(ProcessTable::instance()->processList().size() > this->m_prozessPointer){ // Existieren Prozesse?
            if(ProcessTable::instance()->processList().at(this->m_prozessPointer).zustand() == Process::RECHENBEREIT){ // Ist der erste vorhandene Process rechenbereit?
                //ProcessTable::instance()->setProzessZustand(0, Process::RECHNEND);
                this->m_schedulingStatus = PROZESS;
            } else {
                this->m_prozessPointer++;
            }
        } else {
            this->m_schedulingStatus = ENDE;
        }
        m_timer.start(0);
        break;

    case PROZESS:

        if(ProcessTable::instance()->processList().at(this->m_prozessPointer).zustand() != Process::ABGEBROCHEN){
            if(ProcessTable::instance()->processList().at(this->m_prozessPointer).timeLineList().size() - 1 > this->m_prozessCounter){
                //qDebug() << "nächster Schritt";
                    if(ProcessTable::instance()->processList().at(this->m_prozessPointer).timeLineList().at(this->m_prozessCounter + 1).contains("I/O")){
                        ProcessTable::instance()->setProzessZustand(m_prozessPointer, Process::BLOCKIERT);
                } else {
                        ProcessTable::instance()->setProzessZustand(m_prozessPointer, Process::RECHNEND);
                    }
                this->m_prozessCounter++;
                m_timer.start(this->m_tick);
            } else {
                ProcessTable::instance()->setProzessZustand(m_prozessPointer, Process::ABGESCHLOSSEN);
                //qDebug() << "nächster Prozess";
                this->m_prozessCounter = 0;
                this->m_schedulingStatus = PROZESSWECHSEL;
                m_timer.start(0);
            }
        } else {
            //qDebug() << "nächster Prozess";
            //this->m_prozessCounter = 0;
            this->m_schedulingStatus = PROZESSWECHSEL;
            m_timer.start(0);
        }
        break;

    case PROZESSWECHSEL:
        if(ProcessTable::instance()->processList().size() - 1 > this->m_prozessPointer){
            if(ProcessTable::instance()->processList().at(this->m_prozessPointer + 1).zustand() == Process::RECHENBEREIT){
                //;
                //qDebug() << "Prozesswechsel";
                emit signalMessageStatusBar("Prozesswechsel", 2000);
                ProcessTable::instance()->incrementAnzahlProzesswechsel();
                this->m_schedulingStatus = PROZESS;
                m_timer.start(ProcessTable::instance()->dauerProzesswechsel()*this->m_tick);
            } else {
                m_timer.start(0);
            }
            this->m_prozessCounter = 0;
            this->m_prozessPointer++;
        } else {
            this->m_schedulingStatus = ENDE;
            m_timer.start(0);
        }
        break;

    case ENDE:
        qDebug() << "First Come First Served Sheduling Ende";
        this->m_schedulingStatus = INIT;
        this->m_prozessPointer = 0;
        this->m_prozessCounter = 0;
        emit signalShedulingFinished(0);
        break;

    default:
        break;
    }
}

void SchedulerFirstComeFirstServed::pauseTimer()
{
    m_timer.stop();
}

void SchedulerFirstComeFirstServed::reset()
{
    this->m_prozessCounter = 0;
    this->m_prozessPointer = 0;
    this->m_schedulingStatus = this->INIT;
}

void SchedulerFirstComeFirstServed::setSimSpeed(qint64 value)
{
    if(value == 0){
        this->m_tick = 500;
    } else if (value == 1) {
        this->m_tick = 250;
    } else {
        this->m_tick = 100;
    }
}
