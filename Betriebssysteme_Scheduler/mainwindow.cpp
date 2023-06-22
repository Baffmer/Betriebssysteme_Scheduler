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


void MainWindow::on_pushButtonProzessErstellen_clicked()
{
    DialogProzessErstellen prozessErstellen(this, ProcessTable::instance()->sizeProcessList());
    //qint64 PID, qint64 priorisierung, qint64 prozessorRegister, qint64 hauptspeicher, qint64 anzahlEinAusgabe, qint64 anzahlThreads, qint64 dauerThreads
    if(prozessErstellen.exec() == QDialog::Accepted){
        ProcessTable::instance()->addProcess(Process(prozessErstellen.process()));
    }

    ProcessTable::instance()->printAllProcesses();
}

void MainWindow::updateProcessTable()
{
    qDebug() << "updating Process Table";

    int row = 0;

    ui->tableWidgetProzesstabelle->clearContents();
    ui->tableWidgetProzesstabelle->setRowCount(ProcessTable::instance()->sizeProcessList());

    for(Process &process : ProcessTable::instance()->processList()){
        ui->tableWidgetProzesstabelle->setItem(row, 0, new QTableWidgetItem(QString::number(process.PID()), 0));

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

        ui->tableWidgetProzesstabelle->setItem(row, 1, new QTableWidgetItem(zustand, 0));
        ui->tableWidgetProzesstabelle->setItem(row++, 2, new QTableWidgetItem(QString::number(process.priorisierung()), 0));
    }
}

void MainWindow::updateProcessInformationTable()
{
    qDebug() << "updating Process Information Table";

    qint64 selectedRow = ui->tableWidgetProzesstabelle->currentRow();
    //qDebug() << "current row: " << selectedRow;

    qint64 PID = ui->tableWidgetProzesstabelle->item(selectedRow, 0)->text().toULongLong();
    //qDebug() << "PID: " << PID;

    int row = 0;

    Process process = ProcessTable::instance()->getProcessByPID(PID);

    ui->tableWidgetProzessinformationen->clearContents();

    ui->tableWidgetProzessinformationen->setItem(0, 0, new QTableWidgetItem(QString::number(process.prozessorRegister()), 0));
    ui->tableWidgetProzessinformationen->setItem(0, 1, new QTableWidgetItem(QString::number(process.hauptspeicher()), 0));
    ui->tableWidgetProzessinformationen->setItem(0, 2, new QTableWidgetItem(QString::number(process.anzahlEinAusgabe()), 0));
    ui->tableWidgetProzessinformationen->setItem(0, 3, new QTableWidgetItem(QString::number(process.anzahlThreads()), 0));
    ui->tableWidgetProzessinformationen->setItem(0, 4, new QTableWidgetItem(QString::number(process.dauerThreads()), 0));
}

