#include "processtable.h"

ProcessTable::ProcessTable()
{

}

void ProcessTable::addProcess(Process process)
{
    m_processList.push_back(process);

    emitProcessTableUpdate();
}

void ProcessTable::removeProcess(Process process)
{
    //m_processList.removeAt(m_processList.indexOf(process));
    m_processList.removeOne(process);

    emitProcessTableUpdate();
}

void ProcessTable::removeProcessAt(qint64 pos)
{
    m_processList.removeAt(pos);

    emitProcessTableUpdate();
}

void ProcessTable::removeProcessByPID(qint64 PID)
{
    // TODO

    emitProcessTableUpdate();
}

void ProcessTable::addProcessAt(qint64 pos, Process process)
{
    m_processList.insert(pos, process);

    emitProcessTableUpdate();
}

void ProcessTable::updateProcessAt(qint64 pos, Process process)
{
    m_processList.replace(pos, process);

    emitProcessTableUpdate();
}

void ProcessTable::updateProcessByPID(qint64 PID, Process updatedProcess)
{
    for (Process &process : m_processList) {
        if(process.PID() == PID){
            process = updatedProcess;
        }
    }

    emitProcessTableUpdate();
}

void ProcessTable::removeAllProcesses()
{
    m_processList.clear();

    emitProcessTableUpdate();
}

void ProcessTable::sortProcessListByPrio()
{
    std::qsort(m_processList.begin(), m_processList.size(), sizeof(Process), [](const void* x, const void* y)
       {
           const Process process1 = *static_cast<const Process*>(x);
           const Process process2 = *static_cast<const Process*>(y);
           int arg1 = process1.priorisierung();
           int arg2 = process2.priorisierung();

           if (arg1 < arg2)
               return -1;
           if (arg1 > arg2)
               return 1;
           return 0;
       });

    emitProcessTableUpdate();
}

Process ProcessTable::getProcessByPID(qint64 PID)
{
    for (Process &process : m_processList) {
        if(process.PID() == PID){
            return process;
        }
    }
}

qint64 ProcessTable::sizeProcessList()
{
    return m_processList.size();
}

ProcessTable *ProcessTable::instance()
{
    static ProcessTable* model = new ProcessTable();
    return model;
}

void ProcessTable::printAllProcesses()
{
    for(Process &process : m_processList){
        qDebug() << "PID:" << process.PID()
                 << "Zustand:" << process.zustand()
                 << "Priorisierung:" << process.priorisierung()
                 << "ProzessorRegister:" << process.prozessorRegister()
            << "Hauptspeicher:" << process.hauptspeicher()
            << "AnzahlEinAusgabe:" << process.anzahlEinAusgabe()
            << "DauerThreads:" << process.dauerThreads();
    }
}

void ProcessTable::emitProcessTableUpdate()
{
    emit processListChanged();
}

qint64 ProcessTable::dauerProzesswechsel() const
{
    return m_dauerProzesswechsel;
}

void ProcessTable::setDauerProzesswechsel(qint64 newDauerProzesswechsel)
{
    m_dauerProzesswechsel = newDauerProzesswechsel;
}

void ProcessTable::updateTimeLines()
{
    for(Process &process : m_processList){
        process.createTimeLine(this->m_ioDauer);
    }
}

void ProcessTable::setProzessZustand(qint64 pos, Process::Zustand zustand)
{
    Process process = this->m_processList.at(pos);

    process.setZustand(zustand);

    this->m_processList.replace(pos, process);
}

qint64 ProcessTable::simSpeed() const
{
    return m_simSpeed;
}

void ProcessTable::setSimSpeed(qint64 newSimSpeed)
{
    m_simSpeed = newSimSpeed;
}

qint64 ProcessTable::ioDauer() const
{
    return m_ioDauer;
}

void ProcessTable::setIoDauer(qint64 newIoDauer)
{
    m_ioDauer = newIoDauer;
}

qint64 ProcessTable::quantum() const
{
    return m_quantum;
}

void ProcessTable::setQuantum(qint64 newQuantum)
{
    m_quantum = newQuantum;
}

QList<Process> ProcessTable::processList()
{
    return m_processList;
}
