#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopServices>
#include <QString>
#include <QMetaObject>

#include "dialogprozesserstellen.h"
#include "processtable.h"
#include "dialogueberinformationen.h"
#include "dialogsimparameter.h"

#include "schedulerfirstcomefirstserved.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    enum Scheduler{
        FIRST_COME_FIRST_SERVED,
        SHORTEST_JOB_FIRST,
        ROUND_ROBIN_SCHEDULING,
        PRIORITAETSSCHEDULING
    };

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    qint64 scheduler() const;
    void setScheduler(qint64 newScheduler);

private slots:
    void on_pushButtonProzessErstellen_clicked();
    void updateProcessTable();
    void updateProcessInformationTable();

    void on_pushButtonProzessBearbeiten_clicked();

    void on_pushButtonProzessLoeschen_clicked();

    void on_pushButtonProzessAbbrechen_clicked();

    void on_pushButtonSimStarten_clicked();

    void on_comboBoxActiveProzess_activated(int index);

    void on_pushButtonBeispieleLaden_clicked();

    void on_action_ber_triggered();

    void on_actionDoku_triggered();

    void on_pushButtonSimEinstellungen_clicked();

    void updateShedulerInfos(qint64 prozessPointer, qint64 prozessCounter);

private:
    Ui::MainWindow *ui;

    qint64 m_scheduler;

    qint64 m_prozessPointer = 0;
    qint64 m_prozessCounter = 0;

    // Scheduler
    SchedulerFirstComeFirstServed* schedulerFirstComeFirstServed;
};
#endif // MAINWINDOW_H
