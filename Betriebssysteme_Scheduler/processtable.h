#ifndef PROCESSTABLE_H
#define PROCESSTABLE_H

#include <QDebug>
#include <QList>

#include "process.h"

class ProcessTable
{
public:
    static ProcessTable* instance();

    void addProcess(Process process);
    void removeProcess(Process process);
    void removeProcessAt(qint64 pos);
    void removeAllProcesses();
    qint64 sizeProcessList();

    void printAllProcesses();

private:
    ProcessTable();

    QList<Process> m_processList;
};

#endif // PROCESSTABLE_H
