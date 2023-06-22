#ifndef PROCESSTABLE_H
#define PROCESSTABLE_H

#include <QDebug>
#include <QList>

#include "process.h"

class ProcessTable : public QObject
{
    Q_OBJECT

public:
    static ProcessTable* instance();

    void addProcess(Process process);
    void removeProcess(Process process);
    void removeProcessAt(qint64 pos);
    void removeAllProcesses();
    Process getProcessByPID(qint64 PID);
    qint64 sizeProcessList();

    void printAllProcesses();

    QList<Process> processList() const;

private:
    ProcessTable();
    void emitProcessTableUpdate();

    QList<Process> m_processList;

signals:
    void processListChanged();
};

#endif // PROCESSTABLE_H
