#ifndef DIALOGSIMPARAMETER_H
#define DIALOGSIMPARAMETER_H

#include <QDialog>

namespace Ui {
class DialogSimParameter;
}

class DialogSimParameter : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSimParameter(QWidget *parent = nullptr);
    ~DialogSimParameter();

    qint64 quantum() const;

    qint64 ioDauer() const;

    qint64 simSpeed() const;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogSimParameter *ui;

    qint64 m_quantum;
    qint64 m_ioDauer;
    qint64 m_simSpeed;
};

#endif // DIALOGSIMPARAMETER_H
