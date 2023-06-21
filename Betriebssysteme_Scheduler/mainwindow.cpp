#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

