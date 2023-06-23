#ifndef DIALOGUEBERINFORMATIONEN_H
#define DIALOGUEBERINFORMATIONEN_H

#include <QDialog>

namespace Ui {
class DialogUeberInformationen;
}

class DialogUeberInformationen : public QDialog
{
    Q_OBJECT

public:
    explicit DialogUeberInformationen(QWidget *parent = nullptr);
    ~DialogUeberInformationen();

private slots:
    void on_pushButtonOk_clicked();

private:
    Ui::DialogUeberInformationen *ui;
};

#endif // DIALOGUEBERINFORMATIONEN_H
