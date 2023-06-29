#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopServices>
#include <QString>

#include "dialogprozesserstellen.h"
#include "processtable.h"
#include "dialogueberinformationen.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
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

private:
    Ui::MainWindow *ui;

    qint64 m_scheduler;
};
#endif // MAINWINDOW_H
