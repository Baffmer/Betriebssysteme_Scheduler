#include "dialogprozesserstellen.h"
#include "ui_dialogprozesserstellen.h"

DialogProzessErstellen::DialogProzessErstellen(QWidget *parent, qint64 PID) :
    QDialog(parent), m_PID(PID),
    ui(new Ui::DialogProzessErstellen)
{
    ui->setupUi(this);
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

