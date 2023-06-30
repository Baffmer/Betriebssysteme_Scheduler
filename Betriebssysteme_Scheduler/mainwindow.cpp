#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // init
    on_comboBoxActiveProzess_activated(0);
    ui->textBrowserSchedulerInfos->setOpenLinks(false);

    // Native Menu Bar auf dem MAC deaktivieren
    ui->menubar->setNativeMenuBar(false);

    // Progressbar in Statusbar hinzufügen
    /*QProgressBar* progressBar = new QProgressBar(this);
    progressBar->setRange(0, 100);
    progressBar->setValue(25);
    progressBar->setTextVisible(true);
    //progressBar->setFormat("Connecting");
    ui->statusbar->addPermanentWidget(progressBar);*/

    ui->statusbar->showMessage("Willkommen im Scheduler Simulator!", 3000);

    connect(ProcessTable::instance(), &ProcessTable::processListChanged, this, &MainWindow::updateProcessTable);
    connect(ui->tableWidgetProzesstabelle, &QTableWidget::itemSelectionChanged, this, &MainWindow::updateProcessInformationTable);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::updateProcessTable()
{
    qDebug() << "updating Process Table";

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
        default:
            break;
        }

        QTableWidgetItem* processItem = new QTableWidgetItem(QString::number(process.PID()), 0);
        processItem->setTextAlignment(Qt::AlignCenter);
        processItem->setBackground(processColor);

        ui->tableWidgetProzesstabelle->setItem(row, 0, processItem);

        QTableWidgetItem* zustandItem = new QTableWidgetItem(zustand, 0);
        zustandItem->setTextAlignment(Qt::AlignCenter);
        zustandItem->setBackground(processColor);

        ui->tableWidgetProzesstabelle->setItem(row, 1, zustandItem);

        QTableWidgetItem* priorisierungItem = new QTableWidgetItem(QString::number(process.priorisierung()), 0);
        priorisierungItem->setTextAlignment(Qt::AlignCenter);
        priorisierungItem->setBackground(processColor);

        ui->tableWidgetProzesstabelle->setItem(row++, 2, priorisierungItem);
    }
}

void MainWindow::updateProcessInformationTable()
{
    qDebug() << "updating Process Information Table";

    //Buttons aktivieren
    ui->pushButtonProzessAbbrechen->setEnabled(true);
    ui->pushButtonProzessBearbeiten->setEnabled(true);
    ui->pushButtonProzessLoeschen->setEnabled(true);

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
}


void MainWindow::on_pushButtonSimStarten_clicked()
{
    qDebug() << "Simulation starten Button geklickt";

    ProcessTable::instance()->sortProcessListByPrio();
}

qint64 MainWindow::scheduler() const
{
    return m_scheduler;
}

void MainWindow::setScheduler(qint64 newScheduler)
{
    m_scheduler = newScheduler;
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

    default:
        break;
    }
}


void MainWindow::on_pushButtonBeispieleLaden_clicked()
{
    qDebug() << "Beispiele laden Button geklickt";

    ProcessTable::instance()->removeAllProcesses();
    ui->tableWidgetProzessinformationen->clearContents();

    //qint64 PID, qint64 priorisierung, qint64 prozessorRegister, qint64 hauptspeicher, qint64 anzahlEinAusgabe, qint64 anzahlThreads, qint64 dauerThreads
    ProcessTable::instance()->addProcess(Process(0, 0, 16, 64, 3, 5, 3));
    ProcessTable::instance()->addProcess(Process(1, 1, 32, 32, 2, 2, 4));
    ProcessTable::instance()->addProcess(Process(2, 3, 16, 16, 1, 3, 5));
    ProcessTable::instance()->addProcess(Process(3, 2, 8, 128, 2, 4, 3));
    ProcessTable::instance()->addProcess(Process(4, 3, 16, 64, 1, 1, 4));
    ProcessTable::instance()->addProcess(Process(5, 0, 64, 32, 4, 3, 5));
    ProcessTable::instance()->addProcess(Process(6, 1, 128, 64, 1, 2, 3));

    ProcessTable::instance()->updateTimeLines();
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
    }

    qDebug() << "Sim Geschwindigkeit:" << ProcessTable::instance()->simSpeed() << "I/O Dauer:" << ProcessTable::instance()->ioDauer() << "Zeit Quantum:" << ProcessTable::instance()->quantum() << "Dauer Prozesswechsel:" << ProcessTable::instance()->dauerProzesswechsel();

    ProcessTable::instance()->updateTimeLines();
}

