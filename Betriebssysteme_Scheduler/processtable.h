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
    void removeProcessByPID(qint64 PID);
    void addProcessAt(qint64 pos, Process process);
    void updateProcessAt(qint64 pos, Process process);
    void updateProcessByPID(qint64 PID, Process process);
    void removeAllProcesses();
    void sortProcessListByPrio();
    Process getProcessByPID(qint64 PID);
    qint64 sizeProcessList();

    void printAllProcesses();

    QList<Process> processList() const;

    qint64 quantum() const;
    void setQuantum(qint64 newQuantum);

    qint64 ioDauer() const;
    void setIoDauer(qint64 newIoDauer);

    qint64 simSpeed() const;
    void setSimSpeed(qint64 newSimSpeed);

    qint64 dauerProzesswechsel() const;
    void setDauerProzesswechsel(qint64 newDauerProzesswechsel);

    void updateTimeLines();

private:
    ProcessTable();
    void emitProcessTableUpdate();

    QList<Process> m_processList;

    // Sim Parameter
    qint64 m_quantum = 30;
    qint64 m_ioDauer = 2;
    qint64 m_simSpeed = 1; // normal
    qint64 m_dauerProzesswechsel = 1;

signals:
    void processListChanged();
};

#endif // PROCESSTABLE_H
