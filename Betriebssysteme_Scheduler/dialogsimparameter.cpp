#include "dialogsimparameter.h"
#include "ui_dialogsimparameter.h"

DialogSimParameter::DialogSimParameter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSimParameter)
{
    ui->setupUi(this);
}

DialogSimParameter::~DialogSimParameter()
{
    delete ui;
}

void DialogSimParameter::on_buttonBox_accepted()
{
    this->m_ioDauer = ui->lineEditIODauer->text().toLongLong();
    this->m_quantum = ui->lineEditQuantum->text().toLongLong();
    this->m_simSpeed = ui->comboBoxSimGeschwindigkeit->currentIndex();
    this->m_dauerProzesswechsel = ui->lineEditDauerProzesswechsel->text().toLongLong();
}

qint64 DialogSimParameter::dauerProzesswechsel() const
{
    return m_dauerProzesswechsel;
}

qint64 DialogSimParameter::simSpeed() const
{
    return m_simSpeed;
}

qint64 DialogSimParameter::ioDauer() const
{
    return m_ioDauer;
}

qint64 DialogSimParameter::quantum() const
{
    return m_quantum;
}

