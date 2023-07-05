#ifndef DIALOGPROZESSERSTELLEN_H
#define DIALOGPROZESSERSTELLEN_H

#include <QDialog>
#include "process.h"

namespace Ui {
class DialogProzessErstellen;
}

/**
 * @brief The DialogProzessErstellen class
 */
class DialogProzessErstellen : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief DialogProzessErstellen
     * @param parent
     * @param PID
     * Konstruktor für DialogProzessErstellen.
     */
    explicit DialogProzessErstellen(QWidget *parent = nullptr, qint64 PID = 0);

    /**
     * @brief DialogProzessErstellen
     * @param parent
     * @param process
     * Konstruktor für DialogProzessErstellen für das Bearbeiten eines Prozesses.
     */
    DialogProzessErstellen(QWidget *parent = nullptr, Process process = Process());

    /**
     * @brief ~DialogProzessErstellen
     * Destruktor für DialogProzessErstellen.
     */
    ~DialogProzessErstellen();

    /**
     * @brief process
     * @return Process
     * Getter für den neu erstellten Process.
     */
    Process process() const;

private slots:

    /**
     * @brief on_buttonBox_accepted
     * Callback für den Button "OK"
     */
    void on_buttonBox_accepted();

private:
    Ui::DialogProzessErstellen *ui; ///< Pointer für die GUI

    qint64 m_PID; ///< PID wird vom Hauptprogramm übergeben
    Process m_process; ///< Prozess zum bearbeiten oder neu erstellen eines Prozesses
};

#endif // DIALOGPROZESSERSTELLEN_H
