#include "processtable.h"

ProcessTable::ProcessTable()
{

}

void ProcessTable::addProcess(Process process)
{
    m_processList.push_back(process);
}

void ProcessTable::removeProcess(Process process)
{
    //m_lagerList.removeAt(m_lagerList.indexOf(item));
}

void ProcessTable::removeProcessAt(qint64 pos)
{
    m_processList.removeAt(pos);
}

void ProcessTable::removeAllProcesses()
{
    m_processList.clear();
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
    for(Process &item : m_processList){
        qDebug() << "PID:" << item.PID()
                 << "Zustand:" << item.zustand()
                 << "Priorisierung:" << item.priorisierung()
                 << "ProzessorRegister:" << item.prozessorRegister()
            << "Hauptspeicher:" << item.hauptspeicher()
            << "AnzahlEinAusgabe:" << item.anzahlEinAusgabe()
            << "DauerThreads:" << item.dauerThreads();
    }
}
