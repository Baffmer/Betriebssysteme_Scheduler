#include "schedulerfirstcomefirstserved.h"

SchedulerFirstComeFirstServed::SchedulerFirstComeFirstServed()
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerEvent()));
}

void SchedulerFirstComeFirstServed::startFirstComeFirstServedSheduling()
{


    qDebug() << "Starte First Come First Served Sheduling";

    // Simulationsgeschwindigkeit
    qint64 tick;

    if(ProcessTable::instance()->simSpeed() == 0){
        tick = 1000;
    } else if (ProcessTable::instance()->simSpeed() == 1) {
        tick = 500;
    } else {
        tick = 250;
    }

    qint64 maxLength = 0;

    timer.setSingleShot(true);
    timer.start(tick);

    /*for(Process &process : ProcessTable::instance()->processList()){
        for (qint64 i = 0; i < process.timeLineList().size(); i++) {
            timer->start(tick);
        }
        timer->start(ProcessTable::instance()->dauerProzesswechsel());
    }*/
}
