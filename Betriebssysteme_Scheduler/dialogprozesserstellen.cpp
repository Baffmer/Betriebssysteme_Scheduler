#include "dialogprozesserstellen.h"
#include "ui_dialogprozesserstellen.h"

DialogProzessErstellen::DialogProzessErstellen(QWidget *parent, qint64 PID) :
    QDialog(parent), m_PID(PID),
    ui(new Ui::DialogProzessErstellen)
{
    ui->setupUi(this);
}

DialogProzessErstellen::DialogProzessErstellen(QWidget *parent, Process process):
    QDialog(parent), m_process(process),
    ui(new Ui::DialogProzessErstellen)
{
    ui->setupUi(this);

    ui->spinBoxPriorisierung->setValue(process.priorisierung());
    ui->lineEditProzessregister->setText(QString::number(process.prozessorRegister()));
    ui->lineEditJHauptspeicher->setText(QString::number(process.hauptspeicher()));
    ui->spinBoxAnzahlEinAusgaben->setValue(process.anzahlEinAusgabe());
    ui->spinBoxAnzahlThreads->setValue(process.anzahlThreads());
    ui->spinBoxDauerThreads->setValue(process.dauerThreads());
    m_PID = process.PID();
}

DialogProzessErstellen::~DialogProzessErstellen()
{
    delete ui;
}

void DialogProzessErstellen::on_buttonBox_accepted()
{
    m_process = Process(m_PID, ui->spinBoxPriorisierung->text().toLongLong(), ui->lineEditProzessregister->text().toUInt(), ui->lineEditJHauptspeicher->text().toLongLong(), ui->spinBoxAnzahlEinAusgaben->text().toLongLong(), ui->spinBoxAnzahlThreads->text().toLongLong(), ui->spinBoxDauerThreads->text().toLongLong());
}

Process DialogProzessErstellen::process() const
{
    return m_process;
}

