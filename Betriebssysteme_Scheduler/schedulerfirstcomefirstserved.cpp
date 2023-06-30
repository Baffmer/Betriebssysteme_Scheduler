#include "schedulerfirstcomefirstserved.h"

SchedulerFirstComeFirstServed::SchedulerFirstComeFirstServed()
{
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(timerEvent()));

    if(ProcessTable::instance()->simSpeed() == 0){
        this->m_tick = 1000;
    } else if (ProcessTable::instance()->simSpeed() == 1) {
        this->m_tick = 500;
    } else {
        this->m_tick = 250;
    }

    m_timer.setSingleShot(true);
}

void SchedulerFirstComeFirstServed::handleFirstComeFirstServedSheduling()
{
    // State Machine

    switch(this->m_schedulingStatus){
    case INIT:
        if(ProcessTable::instance()->processList().at(this->m_prozessPointer).zustand() == Process::RECHENBEREIT){
            ProcessTable::instance()->setProzessZustand(0, Process::RECHNEND);
            this->m_schedulingStatus = PROZESS;
        } else if(ProcessTable::instance()->processList().size() < this->m_prozessPointer){
            this->m_prozessPointer++;
        } else {
            this->m_schedulingStatus = ENDE;
        }
        m_timer.start(0);
        break;

    case PROZESS:
        if(ProcessTable::instance()->processList().size() > this->m_prozessPointer){
            if(ProcessTable::instance()->processList().at(this->m_prozessPointer).timeLineList().size()-1 > this->m_prozessCounter){
                qDebug() << "nächster Schritt";
                this->m_prozessCounter++;
                m_timer.start(this->m_tick);
            } else {
                ProcessTable::instance()->setProzessZustand(m_prozessPointer, Process::ABGESCHLOSSEN);

                qDebug() << "nächster Prozess";
                this->m_prozessCounter = 0;
                this->m_prozessPointer++;
                this->m_schedulingStatus = PROZESSWECHSEL;
                m_timer.start(0);
            }
        } else {
            this->m_schedulingStatus = ENDE;
            m_timer.start(0);
        }

        break;

    case PROZESSWECHSEL:
        if(ProcessTable::instance()->processList().at(this->m_prozessPointer).zustand() == Process::RECHENBEREIT){
            ProcessTable::instance()->setProzessZustand(m_prozessPointer, Process::RECHNEND);
            qDebug() << "Prozesswechsel";
            this->m_schedulingStatus = PROZESS;
            m_timer.start(ProcessTable::instance()->dauerProzesswechsel()*this->m_tick);
        }
        break;

    case ENDE:
        qDebug() << "First Come First Served Sheduling Ende";
        this->m_schedulingStatus = INIT;
        this->m_prozessPointer = 0;
        this->m_prozessCounter = 0;
        break;

    default:
        break;
    }
}
