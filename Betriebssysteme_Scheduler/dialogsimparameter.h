#ifndef DIALOGSIMPARAMETER_H
#define DIALOGSIMPARAMETER_H

#include <QDialog>

namespace Ui {
class DialogSimParameter;
}

/**
 * @brief The DialogSimParameter class
 */
class DialogSimParameter : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief DialogSimParameter
     * @param parent
     * Konstruktor für die DialogSimParameter Klasse
     */
    explicit DialogSimParameter(QWidget *parent = nullptr);

    /**
     * @brief ~DialogSimParameter
     *
     * Destruktor für die DialogSimParameter Klasse
     */
    ~DialogSimParameter();

    /**
     * @brief quantum
     * @return m_quantum
     * Getter für m_quantum
     */
    qint64 quantum() const;

    /**
     * @brief ioDauer
     * @return m_ioDauer
     * Getter für m_ioDauer
     */
    qint64 ioDauer() const;

    /**
     * @brief simSpeed
     * @return m_simSpeed
     * Getter für m_simSpeed
     */
    qint64 simSpeed() const;

    /**
     * @brief dauerProzesswechsel
     * @return m_dauerProzesswechsel
     * Getter für m_dauerProzesswechsel
     */
    qint64 dauerProzesswechsel() const;

private slots:
    /**
     * @brief on_buttonBox_accepted
     * Callback für den Button "OK"
     */
    void on_buttonBox_accepted();

private:
    Ui::DialogSimParameter *ui; ///< Pointer für die GUI

    qint64 m_quantum; ///< Zeit-Quantum für Round Robin
    qint64 m_ioDauer; ///< Zeitdauer einer Ein- bzw. Ausgabeoperation
    qint64 m_simSpeed; ///< Simulationsgeschwindigkeit 0=langsam, 1=normal, 2=schnell
    qint64 m_dauerProzesswechsel; ///< Zeitdauer eines Prozesswechsels
};

#endif // DIALOGSIMPARAMETER_H
