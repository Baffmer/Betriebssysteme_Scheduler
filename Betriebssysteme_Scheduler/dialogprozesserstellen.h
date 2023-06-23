#ifndef DIALOGPROZESSERSTELLEN_H
#define DIALOGPROZESSERSTELLEN_H

#include <QDialog>
#include "process.h"

namespace Ui {
class DialogProzessErstellen;
}

class DialogProzessErstellen : public QDialog
{
    Q_OBJECT

public:
    explicit DialogProzessErstellen(QWidget *parent = nullptr, qint64 PID = 0);
    DialogProzessErstellen(QWidget *parent = nullptr, Process process = Process());

    ~DialogProzessErstellen();

    Process process() const;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogProzessErstellen *ui;

    qint64 m_PID;
    Process m_process;
};

#endif // DIALOGPROZESSERSTELLEN_H
