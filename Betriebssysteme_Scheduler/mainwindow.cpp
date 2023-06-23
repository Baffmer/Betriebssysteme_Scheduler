#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

        QTableWidgetItem* processItem = new QTableWidgetItem(QString::number(process.PID()), 0);
        processItem->setTextAlignment(Qt::AlignCenter);

        ui->tableWidgetProzesstabelle->setItem(row, 0, processItem);

        QString zustand;

        switch (process.zustand()) {
        case 0:
            zustand = "BLOCKIERT";
            break;
        case 1:
            zustand = "RECHENBEREIT";
            break;
        case 2:
            zustand = "RECHNEND";
            break;
        default:
            break;
        }

        QTableWidgetItem* zustandItem = new QTableWidgetItem(QString::number(process.zustand()), 0);
        zustandItem->setTextAlignment(Qt::AlignCenter);

        ui->tableWidgetProzesstabelle->setItem(row, 1, zustandItem);

        QTableWidgetItem* priorisierungItem = new QTableWidgetItem(QString::number(process.priorisierung()), 0);
        priorisierungItem->setTextAlignment(Qt::AlignCenter);

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
}


void MainWindow::on_pushButtonBeispieleLaden_clicked()
{
    ProcessTable::instance()->removeAllProcesses();
    ui->tableWidgetProzessinformationen->clearContents();

    qDebug() << "Beispiele laden Button geklickt";
    //qint64 PID, qint64 priorisierung, qint64 prozessorRegister, qint64 hauptspeicher, qint64 anzahlEinAusgabe, qint64 anzahlThreads, qint64 dauerThreads
    ProcessTable::instance()->addProcess(Process(0, 0, 16, 64, 10, 5, 10));
    ProcessTable::instance()->addProcess(Process(1, 1, 32, 32, 20, 10, 20));
    ProcessTable::instance()->addProcess(Process(2, 3, 16, 16, 50, 20, 30));
    ProcessTable::instance()->addProcess(Process(3, 2, 8, 128, 20, 15, 10));
    ProcessTable::instance()->addProcess(Process(4, 3, 16, 64, 10, 5, 50));
    ProcessTable::instance()->addProcess(Process(5, 0, 64, 32, 10, 50, 10));
    ProcessTable::instance()->addProcess(Process(6, 1, 128, 64, 5, 1, 100));
}

