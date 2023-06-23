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

    //emitProcessTableUpdate();
}

void ProcessTable::removeProcessAt(qint64 pos)
{
    m_processList.removeAt(pos);

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

QList<Process> ProcessTable::processList() const
{
    return m_processList;
}
