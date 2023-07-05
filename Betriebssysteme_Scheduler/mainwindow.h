#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopServices>
#include <QString>
#include <QMetaObject>
#include <QElapsedTimer>

#include "dialogprozesserstellen.h"
#include "processtable.h"
#include "dialogueberinformationen.h"
#include "dialogsimparameter.h"

#include "scheduler.h"
#include "schedulerfirstcomefirstserved.h"
#include "schedulerroundrobin.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    /**
     * @brief The Scheduler enum
     */
    enum Scheduler{
        FIRST_COME_FIRST_SERVED,
        SHORTEST_JOB_FIRST,
        ROUND_ROBIN_SCHEDULING,
        PRIORITAETSSCHEDULING,
        ROUND_ROBIN_SCHEDULING_PRIO
    };

    /**
     * @brief MainWindow::MainWindow
     * @param parent
     *
     * Konstruktor für die MainWindow Klasse
     * -# Setzt die ComboBox für den Scheduler initial
     * -# Deaktiviert die NativeMenuBar auf dem MAC
     * -# Setzt das Maximum (10000) und das Minimum (0) der ProgressBar
     * -# Instanzen für die Scheduler werden angelegt
     * -# Connections werden definiert
     *
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief MainWindow::~MainWindow
     *
     * Destruktor für die MainWindow Klasse
     */
    ~MainWindow();

    /**
     * @brief MainWindow::scheduler
     * @return m_scheduler
     *
     * Getter für die Variable m_scheduler
     *
     */
    qint64 scheduler() const;

    /**
     * @brief MainWindow::setScheduler
     * @param newScheduler
     *
     * Setter für die Variable m_scheduler
     * -# Setzt den ausgewählten Scheduler vom Dropdown Menü in der Varaible m_scheduler.
     * -# Falls für den Scheduler die processList sortiert werden muss, wird dieses hier schon gemacht, um den Nutzer visuell dies zeigen zu können.
     *
     */
    void setScheduler(qint64 newScheduler);

private slots:

    /**
     * @brief MainWindow::on_pushButtonProzessErstellen_clicked
     *
     * Callback für den Button "Prozess erstellen"
     * Ruft das Eingabefenster "Prozess erstellen" auf und setzt anschließend die Werte:\n
     * PID, priorisierung, prozessorRegister, hauptspeicher, anzahlEinAusgabe, anzahlThreads, dauerThreads
     *
     */
    void on_pushButtonProzessErstellen_clicked();

    /**
     * @brief MainWindow::updateProcessTable
     *
     * Diese Methode aktualisiert die Tabelle mit den Prozessen
     * -# Dafür werden die Prozesse aus der ProzessListe des Sigletons ProcessTable verwendet
     * -# Darüber hinaus werden die Variablen m_processCounter (für den aktuellen Thread) und m_processPointer (für den aktuellen Prozess) verwendet
     * -# Hier werden dann die Hintergrundfarben rot (I/O), grün (rechnend), geld (rechenbereit), grau (abgeschlossen), dunkelgrau (abgebrochen) gesetzt
     * -# die Tätigkeit des aktuellen Prozesses wird gesetzt
     * -# die Zustände aller Prozesse sind immer sichtbar
     * -# zusätzlich bleibt die ausgewählte Zeile immer angewählt
     */
    void updateProcessTable();

    /**
     * @brief MainWindow::updateProcessInformationTable
     *
     * -# Buttons für Prozesse aktivieren, wenn mindestens ein Prozess in der Liste ist
     * -# Speichern der aktuellen selektierten Zeile und neues setzen beim Beenden der Methode
     * -# Setzen der Prozesseigenschaften in der Tabelle ProcessInformationTable
     * -# PID, priorisierung, prozessorRegister (Platzhalter), hauptspeicher (Platzhalter), anzahlEinAusgabe, anzahlThreads, dauerThreads
     *
     */
    void updateProcessInformationTable();

    /**
     * @brief MainWindow::on_pushButtonProzessBearbeiten_clicked
     *
     * Callback für den Prozess Bearbeiten Button.
     * -# Öffnet das Fenster "Prozess bearbeiten" mit den Werten des markierten Prozesses.
     * -# Anschließend können diese bearbeitet und dann gespeichert werden.
     * -# Deaktiviert Prozess-Manipulation-Buttons, da die Auswahl verschwindet.
     */
    void on_pushButtonProzessBearbeiten_clicked();

    /**
     * @brief MainWindow::on_pushButtonProzessLoeschen_clicked
     *
     * -# Entfernt den markierten Prozess aus der processList und der Prozess Tabelle.
     * -# Deaktiviert Prozess-Manipulation-Buttons, da die Auswahl verschwindet.
     *
     */
    void on_pushButtonProzessLoeschen_clicked();

    /**
     * @brief MainWindow::on_pushButtonProzessAbbrechen_clicked
     *
     * -# Setzt den Zustand des markierten Prozesses auf ABGEBROCHEN.
     * -# Deaktiviert Prozess-Manipulation-Buttons, da die Auswahl verschwindet.
     *
     */
    void on_pushButtonProzessAbbrechen_clicked();

    /**
     * @brief MainWindow::on_pushButtonSimStarten_clicked
     *
     * Callback für den Button "Simulation starten".
     * -# Startet die entsprechende Simulation, welche im Scheduler Dropdown Menü ausgewählt wurde
     * -# Blockiert die Buttons: Simualtion starten, Beispiele laden
     * -# Startet den QTimer zum Messen der Simulationsdauer
     * -# Setzt die ProgressBar zurück, wenn die vorherige Simulatino beendet wwurde
     *
     */
    void on_pushButtonSimStarten_clicked();

    /**
     * @brief MainWindow::on_comboBoxActiveProzess_activated
     * @param index
     *
     * Slot für die Combo Box, wenn ein Scheduler gewählt wurde.
     * -# Gibt Informationen (Vor- u. Nachteile) zu den gewählten Schedulern aus
     * -# Speichert den ausgewählten Scheduler in der Variable m_scheduler
     */
    void on_comboBoxActiveProzess_activated(int index);

    /**
     * @brief MainWindow::on_pushButtonBeispieleLaden_clicked
     *
     * Diese Methode lädt diverse Beispiele, damit zu Demonstrationszwecken schnell Prozesse geladen werden können.
     *
     */
    void on_pushButtonBeispieleLaden_clicked();

    /**
     * @brief MainWindow::on_action_ber_triggered
     *
     * Öffnet ein Fenster mit Informationen über das Projekt, einen Git-Link und den Entwicklern.
     *
     */
    void on_action_ber_triggered();

    /**
     * @brief MainWindow::on_actionDoku_triggered
     *
     * Öffnet den Standardbrowser zum Git-Repository auf GitHub
     *
     */
    void on_actionDoku_triggered();

    /**
     * @brief MainWindow::on_pushButtonSimEinstellungen_clicked
     *
     * Öffnet ein Fenster zum Einstellen folgender Simulationsparameter:
     * -# I/O Dauer
     * -# Zeit Quantum für Round Robin
     * -# Simulationsgeschwindigkeit (langsam, normal, schnell)
     * -# Dauer für einen Prozesswechsel
     *
     * Startet den Prozess zum Erstellen der TimeLines der Prozesse
     *
     */
    void on_pushButtonSimEinstellungen_clicked();

    /**
     * @brief MainWindow::updateShedulerInfos
     * @param prozessPointer
     * @param prozessCounter
     *
     * Slot für die Variablen m_prozessCounter und m_prozessPointer. Der Slot wird von den Schedulern aufgerufen, um die Prozesstabelle aktualisieren zu können.
     *
     */
    void updateShedulerInfos(qint64 prozessPointer, qint64 prozessCounter);

    /**
     * @brief MainWindow::on_pushButtonSimPausieren_clicked
     *
     * Callback für den Button "Simulation pausieren"
     * -# Buttons: Simulation pausieren und Beispiele laden werden deaktiviert
     * -# Zwischenergebnis der Zeitmessung wird in der Variablen m_elapsedTime gespeichert
     *
     */
    void on_pushButtonSimPausieren_clicked();

    /**
     * @brief MainWindow::shedulingFinishedHandler
     * @param sheduler
     *
     * Slot für die Scheduler, welcher aufgerufen wird wenn die Simualtion beendet wurde.
     * -# gibt die Buttons Simulation starten und Beispiele laden wieder frei und sperrt die Buttons Simualtion Abbrechen/Pausieren
     * -# Setzt die Simulationszeit zusammen und gibt diese aus, setzt diese anschließend zurück
     * -# Gibt die Anzahl der Prozesswechsel aus
     * -# Setzt die ProgressBar auf 100%
     *
     */
    void shedulingFinishedHandler(qint64 sheduler);

    /**
     * @brief MainWindow::on_pushButtonSimAbbrechen_clicked
     *
     * Button zum Abbrechen der laufenden Simulation, setzt alle Werte die wichtig für den Start einer Simulation sind auf Startparameter zurück.
     *
     */
    void on_pushButtonSimAbbrechen_clicked();

    /**
     * @brief MainWindow::printMessageStatusBar
     * @param message
     * @param timeout
     *
     * Slot für Meldungen in der Statusbar. Dieser Slot wird von den Schedulern verwendet, um Meldungen in der Statsubar anzeigen zu können.
     *
     */
    void printMessageStatusBar(QString message, qint64 timeout);

private:
    Ui::MainWindow *ui; ///< Pointer für das User Interface

    qint64 m_scheduler; ///< Speichert den Scheduler, welcher im DropDown Menü ausgewählt wird

    qint64 m_prozessPointer = 0; ///< Enthält den Index, welcher Prozess in der Prozess Tabelle bearbeitet wird
    qint64 m_prozessCounter = 0; ///< Enthält den Index, welche Tätigkeit im Zeitstrahl bearbeitet wird

    // Scheduler
    SchedulerFirstComeFirstServed* m_schedulerFirstComeFirstServed; ///< Instanz für die Scheduler: FCFS, Shortest First, Prio First
    SchedulerRoundRobin* m_schedulerRoundRobin; ///< Instanz für die Scheduler Round Roben mit und ohne Priorität

    // Timer
    QElapsedTimer m_timer; ///< Objekt zum Messen der Simulationsdauer
    qint64 m_elapsedTime = 0; ///< Speichert die Simulationsdauer in ms
};
#endif // MAINWINDOW_H
