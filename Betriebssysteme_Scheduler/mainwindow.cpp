#include "mainwindow.h"
#include "./ui_mainwindow.h"


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

    m_schedulerFirstComeFirstServed = new SchedulerFirstComeFirstServed;
    m_schedulerRoundRobin = new SchedulerRoundRobin;

    // Connections

    connect(ProcessTable::instance(), &ProcessTable::signalProcessListChanged, this, &MainWindow::updateProcessTable);
    connect(ui->tableWidgetProzesstabelle, &QTableWidget::itemSelectionChanged, this, &MainWindow::updateProcessInformationTable);
    connect(m_schedulerFirstComeFirstServed, &SchedulerFirstComeFirstServed::signalUpdateProcessTable, this, &MainWindow::updateShedulerInfos);
    connect(m_schedulerFirstComeFirstServed, &SchedulerFirstComeFirstServed::signalShedulingFinished, this, &MainWindow::shedulingFinishedHandler);
    connect(m_schedulerFirstComeFirstServed, &SchedulerFirstComeFirstServed::signalMessageStatusBar, this, &MainWindow::printMessageStatusBar);
    connect(m_schedulerRoundRobin, &SchedulerRoundRobin::signalUpdateProcessTable, this, &MainWindow::updateShedulerInfos);
    connect(m_schedulerRoundRobin, &SchedulerRoundRobin::signalShedulingFinished, this, &MainWindow::shedulingFinishedHandler);
    connect(m_schedulerRoundRobin, &SchedulerRoundRobin::signalMessageStatusBar, this, &MainWindow::printMessageStatusBar);

    ui->statusbar->showMessage("Willkommen im Scheduler Simulator!", 3000);
}


MainWindow::~MainWindow()
{
    delete ui;
    delete m_schedulerFirstComeFirstServed;
    delete m_schedulerRoundRobin;
}


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


qint64 MainWindow::scheduler() const
{
    return this->m_scheduler;
}


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


void MainWindow::on_action_ber_triggered()
{
    qDebug() << "Über Button geklickt";

    DialogUeberInformationen infoFenster;
    infoFenster.exec();
}


void MainWindow::on_actionDoku_triggered()
{
    qDebug() << "Hilfe Button geklickt";

    QString link = "https://github.com/Baffmer/Betriebssysteme_Scheduler";
    QDesktopServices::openUrl(QUrl(link));
}


void MainWindow::on_pushButtonSimEinstellungen_clicked()
{
    qDebug() << "Sim-Einstellungen Button geklickt";

    DialogSimParameter prozessParameter(this);
    if(prozessParameter.exec() == QDialog::Accepted){
        ProcessTable::instance()->setIoDauer(prozessParameter.ioDauer());
        ProcessTable::instance()->setQuantum(prozessParameter.quantum());
        ProcessTable::instance()->setSimSpeed(prozessParameter.simSpeed());
        ProcessTable::instance()->setDauerProzesswechsel(prozessParameter.dauerProzesswechsel());

        this->m_schedulerFirstComeFirstServed->setSimSpeed(ProcessTable::instance()->simSpeed());
        this->m_schedulerRoundRobin->setSimSpeed(ProcessTable::instance()->simSpeed());

        qDebug() << "Sim Geschwindigkeit:" << ProcessTable::instance()->simSpeed() << "I/O Dauer:" << ProcessTable::instance()->ioDauer() << "Zeit Quantum:" << ProcessTable::instance()->quantum() << "Dauer Prozesswechsel:" << ProcessTable::instance()->dauerProzesswechsel();

        ProcessTable::instance()->updateTimeLines();

        ui->statusbar->showMessage("Simulationseinstellungen übernommen.", 3000);
    } else {
        ui->statusbar->showMessage(" keine neuen Simulationseinstellungen übernommen.", 3000);
    }
}


void MainWindow::updateShedulerInfos(qint64 prozessPointer, qint64 prozessCounter)
{
    this->m_prozessCounter = prozessCounter;
    this->m_prozessPointer = prozessPointer;

    // update progressBar
    qint64 newProgressBarValue = ui->progressBar->value() + ceil(10000.0 / ProcessTable::instance()->getSimTimeSlotsSum());
    ui->progressBar->setValue(newProgressBarValue);

    updateProcessTable();
}



void MainWindow::on_pushButtonSimAbbrechen_clicked()
{
    qDebug() << "Sim-Abbrechen Button geklickt";

    this->m_elapsedTime = 0;

    ui->pushButtonSimStarten->setEnabled(true);
    ui->pushButtonSimAbbrechen->setEnabled(false);
    ui->pushButtonSimPausieren->setEnabled(false);
    ui->pushButtonBeispieleLaden->setEnabled(true);

    this->m_schedulerFirstComeFirstServed->pauseTimer();
    this->m_schedulerRoundRobin->pauseTimer();

    ProcessTable::instance()->resetSimulation();

    ui->progressBar->setValue(0);

    this->m_schedulerFirstComeFirstServed->reset();
    this->m_schedulerRoundRobin->reset();

    ui->statusbar->showMessage("Simulation abgebrochen.", 3000);
}


void MainWindow::printMessageStatusBar(QString message, qint64 timeout)
{
    ui->statusbar->showMessage(message, 3000);
}


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
        this->m_schedulerFirstComeFirstServed->handleSheduling();
        ui->statusbar->showMessage("Fist Come First Served Scheduling gestartet...", 3000);
        break;

    case SHORTEST_JOB_FIRST:

        this->m_schedulerFirstComeFirstServed->handleSheduling();
        ui->statusbar->showMessage("Shortest Job First Scheduling gestartet...", 3000);
        break;

    case ROUND_ROBIN_SCHEDULING:
        this->m_schedulerRoundRobin->handleSheduling();
        ui->statusbar->showMessage("Round Robin Scheduling gestartet...", 3000);
        break;

    case PRIORITAETSSCHEDULING:
        this->m_schedulerFirstComeFirstServed->handleSheduling();
        ui->statusbar->showMessage("Prioritätsscheduling gestartet...", 3000);
        break;

    case ROUND_ROBIN_SCHEDULING_PRIO:
        this->m_schedulerRoundRobin->handleSheduling();
        ui->statusbar->showMessage("Round Robin mit Prio Scheduling gestartet...", 3000);
        break;

    default:
        break;
    }
}



void MainWindow::on_pushButtonSimPausieren_clicked()
{
    qDebug() << "Sim-Pausieren Button geklickt";
    ui->pushButtonSimStarten->setEnabled(true);
    ui->pushButtonSimAbbrechen->setEnabled(true);
    ui->pushButtonSimPausieren->setEnabled(false);
    ui->pushButtonBeispieleLaden->setEnabled(false);

    this->m_elapsedTime += this->m_timer.elapsed();
    this->m_schedulerFirstComeFirstServed->pauseTimer();
    this->m_schedulerRoundRobin->pauseTimer();

    ui->statusbar->showMessage("Simulation pausiert", 3000);
}


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
