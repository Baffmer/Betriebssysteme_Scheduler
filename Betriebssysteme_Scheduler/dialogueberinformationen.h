#ifndef DIALOGUEBERINFORMATIONEN_H
#define DIALOGUEBERINFORMATIONEN_H

#include <QDialog>

namespace Ui {
class DialogUeberInformationen;
}

/**
 * @brief The DialogUeberInformationen class
 */
class DialogUeberInformationen : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief DialogUeberInformationen
     * @param parent
     * Konstruktor für die Klasse DialogUeberInformationen
     */
    explicit DialogUeberInformationen(QWidget *parent = nullptr);

    /**
     * @brief ~DialogUeberInformationen
     * Destruktor für die Klasse DialogUeberInformationen
     */
    ~DialogUeberInformationen();

private slots:
    /**
     * @brief on_pushButtonOk_clicked
     * Callback für den Button "OK"
     */
    void on_pushButtonOk_clicked();

private:
    Ui::DialogUeberInformationen *ui; ///< Pointer für die GUI
};

#endif // DIALOGUEBERINFORMATIONEN_H
