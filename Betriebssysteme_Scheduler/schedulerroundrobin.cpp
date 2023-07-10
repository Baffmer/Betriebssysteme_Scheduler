#include "schedulerroundrobin.h"

void SchedulerRoundRobin::handleSheduling()
{
    // init prozess counter list
    if(this->m_prozessCounterList.empty()){
        for(qint64 i=0; i<ProcessTable::instance()->processList().size(); i++){
            this->m_prozessCounterList.append(0);
        }
    }
    // State Machine
    switch(this->m_schedulingStatus){

    case INIT:

        qDebug() << "Anzahl Prozesse" << ProcessTable::instance()->processList().size() << "/ Zeit-Quantum:" << ProcessTable::instance()->quantum();
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

        if(ProcessTable::instance()->processList().at(this->m_prozessPointer).zustand() != Process::ABGEBROCHEN && ProcessTable::instance()->processList().at(this->m_prozessPointer).zustand() != Process::ABGESCHLOSSEN){
            if(ProcessTable::instance()->processList().at(this->m_prozessPointer).timeLineList().size() - 1 > this->m_prozessCounterList.at(this->m_prozessPointer)){
                if(this->m_quantumCounter < ProcessTable::instance()->quantum()){
                    if(ProcessTable::instance()->processList().at(this->m_prozessPointer).timeLineList().at(this->m_prozessCounterList.at(this->m_prozessPointer) + 1).contains("I/O")){
                        ProcessTable::instance()->setProzessZustand(this->m_prozessPointer, Process::BLOCKIERT);
                    } else {
                        ProcessTable::instance()->setProzessZustand(this->m_prozessPointer, Process::RECHNEND);
                    }
                    this->incrementProcessCounterAt(this->m_prozessPointer);
                    //qDebug() << this->m_prozessCounterList;
                    if(this->m_processesFinishedCounter != ProcessTable::instance()->sizeProcessList() - 1){
                        this->m_quantumCounter++;
                        emit signalMessageStatusBar("Quantum: " + QString::number(this->m_quantumCounter), 1000);
                    } else {
                        emit signalMessageStatusBar("letzter Prozess", 2000);
                    }
                    m_timer.start(this->m_tick);
                } else {
                    if(ProcessTable::instance()->processList().at(this->m_prozessPointer).timeLineList().size() - 1 > this->m_prozessCounterList.at(this->m_prozessPointer)){
                        ProcessTable::instance()->setProzessZustand(this->m_prozessPointer, Process::RECHENBEREIT);
                    } else {
                        ProcessTable::instance()->setProzessZustand(this->m_prozessPointer, Process::ABGESCHLOSSEN);
                        this->m_processesFinishedCounter++;
                    }
                    this->m_schedulingStatus = PROZESSWECHSEL;
                    m_timer.start(0);
                }
            } else {

                ProcessTable::instance()->setProzessZustand(this->m_prozessPointer, Process::ABGESCHLOSSEN);
                this->m_processesFinishedCounter++;
                //qDebug() << "nächster Prozess";
                this->m_schedulingStatus = PROZESSWECHSEL;
                m_timer.start(0);
            }
        } else {
            //qDebug() << "nächster Prozess";
            this->m_schedulingStatus = PROZESSWECHSEL;
            m_timer.start(0);
        }
        break;

    case PROZESSWECHSEL:

        if(this->m_processesFinishedCounter < ProcessTable::instance()->sizeProcessList()){
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
                this->m_prozessPointer++;
                this->m_quantumCounter = 0;
            } else { // hier wieder von vorne
                this->m_prozessPointer = 0;
                this->m_quantumCounter = 0;
                this->m_schedulingStatus = PROZESS;
                m_timer.start(ProcessTable::instance()->dauerProzesswechsel()*this->m_tick);
            }
        } else {
            this->m_schedulingStatus = ENDE;
            m_timer.start(0);
        }
        break;

    case ENDE:

        qDebug() << "Round Robin Sheduling Ende";
        this->m_schedulingStatus = INIT;
        this->m_prozessPointer = 0;

        for(qint64 i=0; i<ProcessTable::instance()->processList().size(); i++){
            this->m_prozessCounterList.replace(i, 0);
        }

        emit signalShedulingFinished(0);
        break;

    default:
        break;
    }
}

void SchedulerRoundRobin::pauseTimer()
{
    m_timer.stop();
}

void SchedulerRoundRobin::reset()
{
    if(!ProcessTable::instance()->processList().empty()){
        for(qint64 i=0; i<ProcessTable::instance()->processList().size(); i++){
            this->m_prozessCounterList.replace(i, 0);
        }
    }

    this->m_prozessPointer = 0;
    this->m_schedulingStatus = this->INIT;
    this->m_quantumCounter = 0;
    this->m_processesFinishedCounter = 0;
}

void SchedulerRoundRobin::setSimSpeed(qint64 value)
{
    if(value == 0){
        this->m_tick = 500;
    } else if (value == 1) {
        this->m_tick = 250;
    } else {
        this->m_tick = 100;
    }
}

void SchedulerRoundRobin::incrementProcessCounterAt(qint64 pos){
    this->m_prozessCounterList.replace(pos, this->m_prozessCounterList.at(pos) + 1);
}
