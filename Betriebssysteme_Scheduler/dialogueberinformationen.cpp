#include "dialogueberinformationen.h"
#include "ui_dialogueberinformationen.h"

DialogUeberInformationen::DialogUeberInformationen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogUeberInformationen)
{
    ui->setupUi(this);
}

DialogUeberInformationen::~DialogUeberInformationen()
{
    delete ui;
}

void DialogUeberInformationen::on_pushButtonOk_clicked()
{

}

