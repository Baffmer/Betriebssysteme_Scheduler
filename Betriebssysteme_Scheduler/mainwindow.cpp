#include "mainwindow.h"
#include "./ui_mainwindow.h"

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
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // init
    on_comboBoxActiveProzess_activated(0);
    //ui->textBrowserSchedulerInfos->setOpenLinks(false);

    // Native Menu Bar auf dem MAC deaktivieren
    ui->menubar->setNativeMenuBar(false);

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(10000);

    // Progressbar in Statusbar hinzufügen
    /*QProgressBar* progressBar = new QProgressBar(this);
    progressBar->setRange(0, 100);
    progressBar->setValue(25);
    progressBar->setTextVisible(true);
    //progressBar->setFormat("Connecting");
    ui->statusbar->addPermanentWidget(progressBar);*/

    // Scheduler

    schedulerFirstComeFirstServed = new SchedulerFirstComeFirstServed;
    schedulerRoundRobin = new SchedulerRoundRobin;

    // Connections

    connect(ProcessTable::instance(), &ProcessTable::processListChanged, this, &MainWindow::updateProcessTable);
    connect(ui->tableWidgetProzesstabelle, &QTableWidget::itemSelectionChanged, this, &MainWindow::updateProcessInformationTable);
    connect(schedulerFirstComeFirstServed, &SchedulerFirstComeFirstServed::signalUpdateProcessTable, this, &MainWindow::updateShedulerInfos);
    connect(schedulerFirstComeFirstServed, &SchedulerFirstComeFirstServed::signalShedulingFinished, this, &MainWindow::shedulingFinishedHandler);
    connect(schedulerFirstComeFirstServed, &SchedulerFirstComeFirstServed::signalMessageStatusBar, this, &MainWindow::printMessageStatusBar);
    connect(schedulerRoundRobin, &SchedulerRoundRobin::signalUpdateProcessTable, this, &MainWindow::updateShedulerInfos);
    connect(schedulerRoundRobin, &SchedulerRoundRobin::signalShedulingFinished, this, &MainWindow::shedulingFinishedHandler);
    connect(schedulerRoundRobin, &SchedulerRoundRobin::signalMessageStatusBar, this, &MainWindow::printMessageStatusBar);

    ui->statusbar->showMessage("Willkommen im Scheduler Simulator!", 3000);
}

/**
 * @brief MainWindow::~MainWindow
 *
 * Destruktor für die MainWindow Klasse
 */
MainWindow::~MainWindow()
{
    delete ui;
    delete schedulerFirstComeFirstServed;
    delete schedulerRoundRobin;
}

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
void MainWindow::updateProcessTable()
{
    //qDebug() << "updating Process Table";
    QList<QTableWidgetSelectionRange> selectedRange;
    selectedRange = ui->tableWidgetProzesstabelle->selectedRanges();

    int row = 0;

    ui->tableWidgetProzesstabelle->clearContents();
    ui->tableWidgetProzesstabelle->setRowCount(ProcessTable::instance()->sizeProcessList());

    for(Process &process : ProcessTable::instance()->processList()){

        QString zustand;
        QColor processColor;

        switch (process.zustand()) {
        case Process::BLOCKIERT:
            zustand = "BLOCKIERT";
            processColor = Qt::red;
            break;
        case Process::RECHENBEREIT:
            zustand = "RECHENBEREIT";
            processColor = Qt::yellow;
            break;
        case Process::RECHNEND:
            zustand = "RECHNEND";
            processColor = Qt::green;
            break;
        case Process::ABGESCHLOSSEN:
            zustand = "ABGESCHLOSSEN";
            processColor = Qt::gray;
            break;
        case Process::ABGEBROCHEN:
            zustand = "ABGEBROCHEN";
            processColor = Qt::darkGray;
            break;
        default:
            break;
        }

        QTableWidgetItem* processItem = new QTableWidgetItem(QString::number(process.PID()), 0);
        processItem->setTextAlignment(Qt::AlignCenter);
        processItem->setBackground(processColor);
        ui->tableWidgetProzesstabelle->setItem(row, 0, processItem);

        QString taetigkeit = "";
        if(!ProcessTable::instance()->processList().at(this->m_prozessPointer).timeLineList().empty() && row == this->m_prozessPointer){
            taetigkeit = ProcessTable::instance()->processList().at(this->m_prozessPointer).timeLineList().at(this->m_prozessCounter);
        }
        if(zustand.contains("ABGESCHLOSSEN")){
            taetigkeit = "";
        }
        if(zustand.contains("RECHENBEREIT")){
            taetigkeit = "";
        }
        QTableWidgetItem* taetigkeitItem;
        taetigkeitItem = new QTableWidgetItem(taetigkeit, 0);
        taetigkeitItem->setTextAlignment(Qt::AlignCenter);
        taetigkeitItem->setBackground(processColor);
        ui->tableWidgetProzesstabelle->setItem(row, 2, taetigkeitItem);

        QTableWidgetItem* zustandItem = new QTableWidgetItem(zustand, 0);
        zustandItem->setTextAlignment(Qt::AlignCenter);
        zustandItem->setBackground(processColor);
        ui->tableWidgetProzesstabelle->setItem(row, 1, zustandItem);

        QTableWidgetItem* priorisierungItem = new QTableWidgetItem(QString::number(process.priorisierung()), 0);
        priorisierungItem->setTextAlignment(Qt::AlignCenter);
        priorisierungItem->setBackground(processColor);
        ui->tableWidgetProzesstabelle->setItem(row++, 3, priorisierungItem);
    }

    if(selectedRange.size() == 1){
        ui->tableWidgetProzesstabelle->selectRow(selectedRange.at(0).topRow());
    }
}

/**
 * @brief MainWindow::updateProcessInformationTable
 *
 * -# Buttons für Prozesse aktivieren, wenn mindestens ein Prozess in der Liste ist
 * -# Speichern der aktuellen selektierten Zeile und neues setzen beim Beenden der Methode
 * -# Setzen der Prozesseigenschaften in der Tabelle ProcessInformationTable
 * -# PID, priorisierung, prozessorRegister (Platzhalter), hauptspeicher (Platzhalter), anzahlEinAusgabe, anzahlThreads, dauerThreads
 *
 */
void MainWindow::updateProcessInformationTable()
{
    //qDebug() << "updating Process Information Table";

    //Buttons aktivieren
    if(ProcessTable::instance()->processList().empty()){
        ui->pushButtonProzessAbbrechen->setEnabled(false);
        ui->pushButtonProzessBearbeiten->setEnabled(false);
        ui->pushButtonProzessLoeschen->setEnabled(false);
    } else {
        ui->pushButtonProzessAbbrechen->setEnabled(true);
        ui->pushButtonProzessBearbeiten->setEnabled(true);
        ui->pushButtonProzessLoeschen->setEnabled(true);
    }

    qint64 selectedRow = ui->tableWidgetProzesstabelle->currentRow();
    //qDebug() << "current row: " << selectedRow;

    qint64 PID = ui->tableWidgetProzesstabelle->item(selectedRow, 0)->text().toULongLong();
    //qDebug() << "PID: " << PID;

    int row = 0;

    Process process = ProcessTable::instance()->getProcessByPID(PID);

    ui->tableWidgetProzessinformationen->clearContents();

    QTableWidgetItem* prozessorRegisterItem = new QTableWidgetItem(QString::number(process.prozessorRegister()), 0);
    prozessorRegisterItem->setTextAlignment(Qt::AlignCenter);

    ui->tableWidgetProzessinformationen->setItem(0, 0, prozessorRegisterItem);

    QTableWidgetItem* hauptspeicherItem = new QTableWidgetItem(QString::number(process.hauptspeicher()), 0);
    hauptspeicherItem->setTextAlignment(Qt::AlignCenter);

    ui->tableWidgetProzessinformationen->setItem(0, 1, hauptspeicherItem);

    QTableWidgetItem* anzahlEinAusgabeItem = new QTableWidgetItem(QString::number(process.anzahlEinAusgabe()), 0);
    anzahlEinAusgabeItem->setTextAlignment(Qt::AlignCenter);

    ui->tableWidgetProzessinformationen->setItem(0, 2, anzahlEinAusgabeItem);

    QTableWidgetItem* anzahlThreadsItem = new QTableWidgetItem(QString::number(process.anzahlThreads()), 0);
    anzahlThreadsItem->setTextAlignment(Qt::AlignCenter);

    ui->tableWidgetProzessinformationen->setItem(0, 3, anzahlThreadsItem);

    QTableWidgetItem* dauerThreadsItem = new QTableWidgetItem(QString::number(process.dauerThreads()), 0);
    dauerThreadsItem->setTextAlignment(Qt::AlignCenter);

    ui->tableWidgetProzessinformationen->setItem(0, 4, dauerThreadsItem);
}

/**
 * @brief MainWindow::on_pushButtonProzessErstellen_clicked
 *
 * Callback für den Button "Prozess erstellen"
 * Ruft das Eingabefenster "Prozess erstellen" auf und setzt anschließend die Werte:\n
 * PID, priorisierung, prozessorRegister, hauptspeicher, anzahlEinAusgabe, anzahlThreads, dauerThreads
 *
 */
void MainWindow::on_pushButtonProzessErstellen_clicked()
{
    DialogProzessErstellen prozessErstellen(this, ProcessTable::instance()->sizeProcessList());
    //qint64 PID, qint64 priorisierung, qint64 prozessorRegister, qint64 hauptspeicher, qint64 anzahlEinAusgabe, qint64 anzahlThreads, qint64 dauerThreads
    if(prozessErstellen.exec() == QDialog::Accepted){
        ProcessTable::instance()->addProcess(Process(prozessErstellen.process()));
    }

    ProcessTable::instance()->printAllProcesses();

    ui->statusbar->showMessage("neuer Prozess wurde erstellt", 3000);
}

/**
 * @brief MainWindow::on_pushButtonProzessBearbeiten_clicked
 *
 * Callback für den Prozess Bearbeiten Button.
 * -# Öffnet das Fenster "Prozess bearbeiten" mit den Werten des markierten Prozesses.
 * -# Anschließend können diese bearbeitet und dann gespeichert werden.
 * -# Deaktiviert Prozess-Manipulation-Buttons, da die Auswahl verschwindet.
 */
void MainWindow::on_pushButtonProzessBearbeiten_clicked()
{
    qDebug() << "Prozess bearbeiten Button geklickt";

    qint64 selectedRow = ui->tableWidgetProzesstabelle->currentRow();
    qint64 PID = ui->tableWidgetProzesstabelle->item(selectedRow, 0)->text().toULongLong();

    Process process = ProcessTable::instance()->getProcessByPID(PID);

    DialogProzessErstellen prozessErstellen(this, process);
    //qint64 PID, qint64 priorisierung, qint64 prozessorRegister, qint64 hauptspeicher, qint64 anzahlEinAusgabe, qint64 anzahlThreads, qint64 dauerThreads
    if(prozessErstellen.exec() == QDialog::Accepted){
        ProcessTable::instance()->updateProcessByPID(PID, Process(prozessErstellen.process()));
    }

    ProcessTable::instance()->printAllProcesses();

    //Buttons deaktivieren
    ui->pushButtonProzessAbbrechen->setEnabled(false);
    ui->pushButtonProzessBearbeiten->setEnabled(false);
    ui->pushButtonProzessLoeschen->setEnabled(false);
}

/**
 * @brief MainWindow::on_pushButtonProzessLoeschen_clicked
 *
 * -# Entfernt den markierten Prozess aus der processList und der Prozess Tabelle.
 * -# Deaktiviert Prozess-Manipulation-Buttons, da die Auswahl verschwindet.
 *
 */
void MainWindow::on_pushButtonProzessLoeschen_clicked()
{
    qDebug() << "Prozess löschen Button geklickt";

    qint64 selectedRow = ui->tableWidgetProzesstabelle->currentRow();
    qint64 PID = ui->tableWidgetProzesstabelle->item(selectedRow, 0)->text().toULongLong();

    Process process = ProcessTable::instance()->getProcessByPID(PID);

    ProcessTable::instance()->removeProcess(process);

    ui->tableWidgetProzessinformationen->clearContents();

    //Buttons deaktivieren
    ui->pushButtonProzessAbbrechen->setEnabled(false);
    ui->pushButtonProzessBearbeiten->setEnabled(false);
    ui->pushButtonProzessLoeschen->setEnabled(false);
}

/**
 * @brief MainWindow::on_pushButtonProzessAbbrechen_clicked
 *
 * -# Setzt den Zustand des markierten Prozesses auf ABGEBROCHEN.
 * -# Deaktiviert Prozess-Manipulation-Buttons, da die Auswahl verschwindet.
 *
 */
void MainWindow::on_pushButtonProzessAbbrechen_clicked()
{
    qDebug() << "Prozess abbrechen Button geklickt";

    qint64 selectedRow = ui->tableWidgetProzesstabelle->currentRow();
    qint64 PID = ui->tableWidgetProzesstabelle->item(selectedRow, 0)->text().toULongLong();

    Process process = ProcessTable::instance()->getProcessByPID(PID);
    process.setZustand(Process::ABGEBROCHEN);

    ProcessTable::instance()->updateProcessByPID(PID, process);

    ProcessTable::instance()->printAllProcesses();
}

/**
 * @brief MainWindow::scheduler
 * @return m_scheduler
 *
 * Getter für die Variable m_scheduler
 *
 */
qint64 MainWindow::scheduler() const
{
    return this->m_scheduler;
}

/**
 * @brief MainWindow::setScheduler
 * @param newScheduler
 *
 * Setter für die Variable m_scheduler
 * -# Setzt den ausgewählten Scheduler vom Dropdown Menü in der Varaible m_scheduler.
 * -# Falls für den Scheduler die processList sortiert werden muss, wird dieses hier schon gemacht, um den Nutzer visuell dies zeigen zu können.
 *
 */
void MainWindow::setScheduler(qint64 newScheduler)
{
    this->m_scheduler = newScheduler;

    switch(this->m_scheduler){
    case FIRST_COME_FIRST_SERVED:
        ProcessTable::instance()->sortProcessListByPID();
        ui->statusbar->showMessage("Fist Come First Served Scheduling gewählt.", 3000);
        break;

    case SHORTEST_JOB_FIRST:
        ProcessTable::instance()->sortProcessListByTime();
        ui->statusbar->showMessage("Shortest Job First Scheduling gewählt.", 3000);
        break;

    case ROUND_ROBIN_SCHEDULING:
        ui->statusbar->showMessage("Round Robin Scheduling gewählt.", 3000);
        break;

    case PRIORITAETSSCHEDULING:
        ProcessTable::instance()->sortProcessListByPrio();
        ui->statusbar->showMessage("Prioritätsscheduling gewählt.", 3000);
        break;

    case ROUND_ROBIN_SCHEDULING_PRIO:
        ProcessTable::instance()->sortProcessListByPrio();
        ui->statusbar->showMessage("Round Robin mit Prio Scheduling gewählt.", 3000);
        break;

    default:
        break;
    }
}

/**
 * @brief MainWindow::on_pushButtonBeispieleLaden_clicked
 *
 * Diese Methode lädt diverse Beispiele, damit zu Demonstrationszwecken schnell Prozesse geladen werden können.
 *
 */
void MainWindow::on_pushButtonBeispieleLaden_clicked()
{
    qDebug() << "Beispiele laden Button geklickt";
    on_pushButtonSimAbbrechen_clicked();

    ProcessTable::instance()->removeAllProcesses();
    ui->tableWidgetProzessinformationen->clearContents();

    //qint64 PID, qint64 priorisierung, qint64 prozessorRegister, qint64 hauptspeicher, qint64 anzahlEinAusgabe, qint64 anzahlThreads, qint64 dauerThreads
    ProcessTable::instance()->addProcess(Process(0, 0, 0, 0, 3, 3, 3));
    ProcessTable::instance()->addProcess(Process(1, 1, 0, 0, 2, 2, 4));
    ProcessTable::instance()->addProcess(Process(2, 4, 0, 0, 1, 3, 3));
    ProcessTable::instance()->addProcess(Process(3, 2, 0, 0, 5, 2, 10));
    ProcessTable::instance()->addProcess(Process(4, 3, 0, 0, 5, 5, 7));
    //ProcessTable::instance()->addProcess(Process(5, 0, 64, 32, 4, 3, 5));
    //ProcessTable::instance()->addProcess(Process(6, 1, 128, 64, 1, 2, 3));

    ProcessTable::instance()->updateTimeLines();

    ui->statusbar->showMessage("Beispiele geladen.", 3000);
}

/**
 * @brief MainWindow::on_action_ber_triggered
 *
 * Öffnet ein Fenster mit Informationen über das Projekt, einen Git-Link und den Entwicklern.
 *
 */
void MainWindow::on_action_ber_triggered()
{
    qDebug() << "Über Button geklickt";

    DialogUeberInformationen infoFenster;
    infoFenster.exec();
}

/**
 * @brief MainWindow::on_actionDoku_triggered
 *
 * Öffnet den Standardbrowser zum Git-Repository auf GitHub
 *
 */
void MainWindow::on_actionDoku_triggered()
{
    qDebug() << "Hilfe Button geklickt";

    QString link = "https://github.com/Baffmer/Betriebssysteme_Scheduler";
    QDesktopServices::openUrl(QUrl(link));
}

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
void MainWindow::on_pushButtonSimEinstellungen_clicked()
{
    qDebug() << "Sim-Einstellungen Button geklickt";

    DialogSimParameter prozessParameter(this);
    if(prozessParameter.exec() == QDialog::Accepted){
        ProcessTable::instance()->setIoDauer(prozessParameter.ioDauer());
        ProcessTable::instance()->setQuantum(prozessParameter.quantum());
        ProcessTable::instance()->setSimSpeed(prozessParameter.simSpeed());
        ProcessTable::instance()->setDauerProzesswechsel(prozessParameter.dauerProzesswechsel());

        this->schedulerFirstComeFirstServed->setSimSpeed(ProcessTable::instance()->simSpeed());

        qDebug() << "Sim Geschwindigkeit:" << ProcessTable::instance()->simSpeed() << "I/O Dauer:" << ProcessTable::instance()->ioDauer() << "Zeit Quantum:" << ProcessTable::instance()->quantum() << "Dauer Prozesswechsel:" << ProcessTable::instance()->dauerProzesswechsel();

        ProcessTable::instance()->updateTimeLines();

        ui->statusbar->showMessage("Simulationseinstellungen übernommen.", 3000);
    } else {
        ui->statusbar->showMessage(" keine neuen Simulationseinstellungen übernommen.", 3000);
    }
}

/**
 * @brief MainWindow::updateShedulerInfos
 * @param prozessPointer
 * @param prozessCounter
 *
 * Slot für die Variablen m_prozessCounter und m_prozessPointer. Der Slot wird von den Schedulern aufgerufen, um die Prozesstabelle aktualisieren zu können.
 *
 */
void MainWindow::updateShedulerInfos(qint64 prozessPointer, qint64 prozessCounter)
{
    this->m_prozessCounter = prozessCounter;
    this->m_prozessPointer = prozessPointer;

    // update progressBar
    qint64 newProgressBarValue = ui->progressBar->value() + ceil(10000.0 / ProcessTable::instance()->getSimTimeSlotsSum());
    ui->progressBar->setValue(newProgressBarValue);

    updateProcessTable();
}


/**
 * @brief MainWindow::on_pushButtonSimAbbrechen_clicked
 *
 * Button zum Abbrechen der laufenden Simulation, setzt alle Werte die wichtig für den Start einer Simulation sind auf Startparameter zurück.
 *
 */
void MainWindow::on_pushButtonSimAbbrechen_clicked()
{
    qDebug() << "Sim-Abbrechen Button geklickt";

    this->m_elapsedTime = 0;

    ui->pushButtonSimStarten->setEnabled(true);
    ui->pushButtonSimAbbrechen->setEnabled(false);
    ui->pushButtonSimPausieren->setEnabled(false);
    ui->pushButtonBeispieleLaden->setEnabled(true);

    schedulerFirstComeFirstServed->pauseTimer();
    schedulerRoundRobin->pauseTimer();

    ProcessTable::instance()->resetSimulation();

    ui->progressBar->setValue(0);

    schedulerFirstComeFirstServed->reset();
    schedulerRoundRobin->reset();

    ui->statusbar->showMessage("Simulation abgebrochen.", 3000);
}

/**
 * @brief MainWindow::printMessageStatusBar
 * @param message
 * @param timeout
 *
 * Slot für Meldungen in der Statusbar. Dieser Slot wird von den Schedulern verwendet, um Meldungen in der Statsubar anzeigen zu können.
 *
 */
void MainWindow::printMessageStatusBar(QString message, qint64 timeout)
{
    ui->statusbar->showMessage(message, 3000);
}

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
void MainWindow::on_pushButtonSimStarten_clicked()
{
    qDebug() << "Simulation starten Button geklickt";
    ui->pushButtonSimStarten->setEnabled(false);
    ui->pushButtonSimAbbrechen->setEnabled(true);
    ui->pushButtonSimPausieren->setEnabled(true);
    ui->pushButtonBeispieleLaden->setEnabled(false);

    // Anzahl Prozesswechsel zurücksetzen
    ProcessTable::instance()->resetAnzahlProzesswechsel();

    // Timer für die Evaluation Simulationssdauer
    this->m_timer.start();

    if(ui->progressBar->value() == 10000){
        ui->progressBar->setValue(0);
    }

    switch(this->m_scheduler){
    case FIRST_COME_FIRST_SERVED:
        //ProcessTable::instance()->sortProcessListByPID();
        schedulerFirstComeFirstServed->handleFirstComeFirstServedSheduling();
        ui->statusbar->showMessage("Fist Come First Served Scheduling gestartet...", 3000);
        break;

    case SHORTEST_JOB_FIRST:
        //ProcessTable::instance()->sortProcessListByTime();
        schedulerFirstComeFirstServed->handleFirstComeFirstServedSheduling();
        ui->statusbar->showMessage("Shortest Job First Scheduling gestartet...", 3000);
        break;

    case ROUND_ROBIN_SCHEDULING:
        schedulerRoundRobin->handleRoundRobinSheduling();
        ui->statusbar->showMessage("Round Robin Scheduling gestartet...", 3000);
        break;

    case PRIORITAETSSCHEDULING:
        //ProcessTable::instance()->sortProcessListByPrio();
        schedulerFirstComeFirstServed->handleFirstComeFirstServedSheduling();
        ui->statusbar->showMessage("Prioritätsscheduling gestartet...", 3000);
        break;

    case ROUND_ROBIN_SCHEDULING_PRIO:
        //ProcessTable::instance()->sortProcessListByPrio();
        schedulerRoundRobin->handleRoundRobinSheduling();
        ui->statusbar->showMessage("Round Robin mit Prio Scheduling gestartet...", 3000);
        break;

    default:
        break;
    }
}


/**
 * @brief MainWindow::on_pushButtonSimPausieren_clicked
 *
 * Callback für den Button "Simulation pausieren"
 * -# Buttons: Simulation pausieren und Beispiele laden werden deaktiviert
 * -# Zwischenergebnis der Zeitmessung wird in der Variablen m_elapsedTime gespeichert
 *
 */
void MainWindow::on_pushButtonSimPausieren_clicked()
{
    qDebug() << "Sim-Pausieren Button geklickt";
    ui->pushButtonSimStarten->setEnabled(true);
    ui->pushButtonSimAbbrechen->setEnabled(true);
    ui->pushButtonSimPausieren->setEnabled(false);
    ui->pushButtonBeispieleLaden->setEnabled(false);

    this->m_elapsedTime += this->m_timer.elapsed();
    schedulerFirstComeFirstServed->pauseTimer();

    ui->statusbar->showMessage("Simulation pausiert", 3000);
}

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
void MainWindow::shedulingFinishedHandler(qint64 sheduler)
{
    ui->pushButtonSimStarten->setEnabled(true);
    ui->pushButtonSimAbbrechen->setEnabled(false);
    ui->pushButtonSimPausieren->setEnabled(false);
    ui->pushButtonBeispieleLaden->setEnabled(true);

    // Evaluation
    ui->labelSimZeitOut->setText(QString::number((this->m_elapsedTime + this->m_timer.elapsed())/1000.0) + "s");
    ui->labelProzesswechselOut->setText(QString::number(ProcessTable::instance()->getAnzahlProzesswechsel()));

    this->m_elapsedTime = 0;
    ui->progressBar->setValue(10000);
    ui->statusbar->showMessage("Simulation beendet", 3000);
}

/**
 * @brief MainWindow::on_comboBoxActiveProzess_activated
 * @param index
 *
 * Slot für die Combo Box, wenn ein Scheduler gewählt wurde.
 * -# Gibt Informationen (Vor- u. Nachteile) zu den gewählten Schedulern aus
 * -# Speichert den ausgewählten Scheduler in der Variable m_scheduler
 */
void MainWindow::on_comboBoxActiveProzess_activated(int index)
{
    qDebug() << "Scheduler:" << index << "ausgewählt";

    this->setScheduler(index);

    switch(index){
    case 0:

        ui->textBrowserSchedulerInfos->setHtml(QString (tr(""
                                                        "<h3>First Come First Served (FCFS) Scheduler (FIFO):</h3>"
                                                        "<p>"
                                                        "<i>Vorteile:</i>"
                                                        "<ul>"
                                                        "<li>Einfach zu implementieren und zu verstehen.</li>"
                                                        "<li>Gerechte Behandlung von Aufgaben in der Reihenfolge ihres Eingangs.</li>"
                                                        "<li>Keine Notwendigkeit zur Schätzung oder Berechnung von Aufgabendauern.</li>"
                                                        "</ul>"
                                                        "<p>"
                                                        "<i>Nachteile:</i>"
                                                        "<ul>"
                                                        "<li>Anfällig für das \"Warteschlangen-Effekt\" oder das \"Convoy-Problem\", wenn eine langwierige Aufgabe alle nachfolgenden Aufgaben blockiert.</li>"
                                                        "<li>Kann zu einer hohen durchschnittlichen Wartezeit führen, wenn lange Aufgaben zuerst eintreffen.</li>"
                                                        "<li>Keine Optimierung der Ausführungszeit oder Priorisierung von Aufgaben.</li>"
                                                        "</ul>")));

        break;

    case 1:

        ui->textBrowserSchedulerInfos->setHtml(QString (tr(""
                                                          "<h3>Shortest Job First (SJF) Scheduler:</h3>"
                                                          "<p>"
                                                          "<i>Vorteile:</i>"
                                                          "<ul>"
                                                          "<li>Minimiert die durchschnittliche Wartezeit, indem Aufgaben mit der kürzesten geschätzten Ausführungszeit zuerst ausgeführt werden.</li>"
                                                          "<li>Effiziente Nutzung der Ressourcen, da kurze Aufgaben schnell abgeschlossen werden.</li>"
                                                          "<li>Kann eine hohe Durchsatzrate und schnelle Antwortzeiten ermöglichen.</li>"
                                                          "</ul>"
                                                          "<p>"
                                                          "<i>Nachteile:</i>"
                                                          "<ul>"
                                                          "<li>Schwierigkeiten bei der genauen Schätzung der Ausführungszeit von Aufgaben.</li>"
                                                          "<li>Anfällig für das \"Starvation-Problem\", wenn lange Aufgaben ständig von kürzeren Aufgaben verdrängt werden.</li>"
                                                          "<li>Hohe Komplexität bei der Implementierung, insbesondere bei der Verwaltung von Aufgabenprioritäten basierend auf ihren geschätzten Ausführungszeiten.</li>"
                                                          "</ul>")));
        break;

    case 2:

        ui->textBrowserSchedulerInfos->setHtml(QString (tr(""
                                                          "<h3>Round-Robin-Scheduler:</h3>"
                                                          "<p>"
                                                          "<i>Vorteile:</i>"
                                                          "<ul>"
                                                          "<li>Fairness bei der Verteilung von Ressourcen, da jede Aufgabe in regelmäßigen Zeitintervallen bedient wird.</li>"
                                                          "<li>Einfache Implementierung und geringe Komplexität.</li>"
                                                          "<li>Gut geeignet für Szenarien, in denen schnelle Reaktionen auf eingehende Anfragen erforderlich sind.</li>"
                                                          "</ul>"
                                                          "<p>"
                                                          "<i>Nachteile:</i>"
                                                          "<ul>"
                                                          "<li>Nicht geeignet für Aufgaben mit unterschiedlichen Prioritäten oder zeitkritischen Anforderungen.</li>"
                                                          "<li>Kann ineffizient sein, wenn einige Aufgaben mehr Ressourcen benötigen als andere.</li>"
                                                          "<li>Das Zeitquantum sollte sorgfältig gewählt werden, da zu kleine Werte zu hoher Kontextwechseldauer führen können.</li>"
                                                          "</ul>")));
        break;

    case 3:

        ui->textBrowserSchedulerInfos->setHtml(QString (tr(""
                                                          "<h3>Prioritäts-Scheduler:</h3>"
                                                          "<p>"
                                                          "<i>Vorteile:</i>"
                                                          "<ul>"
                                                          "<li>Flexibel, da die Prioritäten der Aufgaben angepasst werden können.</li>"
                                                          "<li>Ermöglicht die Priorisierung kritischer Aufgaben, um Engpässe zu vermeiden.</li>"
                                                          "<li>Bietet eine bessere Auslastung der Ressourcen, da wichtige Aufgaben bevorzugt behandelt werden.</li>"
                                                          "</ul>"
                                                          "<p>"
                                                          "<i>Nachteile:</i>"
                                                          "<ul>"
                                                          "<li>Das Risiko von Starvation (Verhungern) niedrig priorisierter Aufgaben besteht, wenn sie dauerhaft von höher priorisierten Aufgaben verdrängt werden.</li>"
                                                          "<li>Wenn die Prioritäten nicht gut verwaltet werden, kann es zu einer Beeinträchtigung der Performance und Ressourcenausnutzung kommen.</li>"
                                                          "<li>Hohe Komplexität bei der Implementierung und Verwaltung der Prioritätsregeln.</li>"
                                                          "</ul>")));
        break;

    case 4:

        ui->textBrowserSchedulerInfos->setHtml(QString (tr(""
                                                          "<h3>Round-Robin-Scheduler mit Prioritäten:</h3>"
                                                          "<p>"
                                                          "<i>Vorteile:</i>"
                                                          "<ul>"
                                                          "<li>Fairness bei der Verteilung von Ressourcen und Rechenzeit, da jede Aufgabe in regelmäßigen Zeitintervallen bedient wird.</li>"
                                                          "<li>Ermöglicht die Priorisierung von Aufgaben basierend auf ihrer Wichtigkeit oder Dringlichkeit.</li>"
                                                          "<li>Gut geeignet für Szenarien, in denen eine gewisse Ausgewogenheit zwischen Aufgabenprioritäten und Reaktionszeit erforderlich ist.</li>"
                                                          "</ul>"
                                                          "<p>"
                                                          "<i>Nachteile:</i>"
                                                          "<ul>"
                                                          "<li>Möglicherweise ineffizient bei langen Aufgaben mit hoher Priorität, da sie in kürzeren Zeitquanten wiederholt unterbrochen werden können.</li>"
                                                          "<li>Die Wahl des Zeitquantums und der Prioritätsregeln erfordert sorgfältige Abwägungen, um eine optimale Leistung zu erzielen.</li>"
                                                          "<li>Bei einer schlechten Verwaltung der Prioritäten besteht die Gefahr, dass wichtige Aufgaben durch weniger wichtige Aufgaben verdrängt werden, was zu Starvation führen kann.</li>"
                                                          "</ul>")));
        break;

    default:
        break;
    }
}
