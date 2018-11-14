#ifndef ROOMACTIONSDIALOG_H
#define ROOMACTIONSDIALOG_H

#include <QDialog>

namespace Ui {
class RoomActionsDialog;
}

/**
 * @brief The RoomActionsDialog class represents a popup dialog that is used
 * to obtain user's desired room action and the action's targeting room and user
 */
class RoomActionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RoomActionsDialog(QWidget *parent = 0);
    ~RoomActionsDialog();

    /**
     * @brief action get the room action from the dialog
     * @return the room action
     */
    QString roomAction() const;

    /**
     * @brief targetRoom get the targeting room name from the dialog
     * @return the targeting room name
     */
    QString targetRoom() const;

    /**
     * @brief targetUser get the targeting client name from the dialog
     * @return the targeting client name
     */
    QString targetUser() const;

private slots:
    /**
     * @brief on_submitButton_clicked validate user inputs for the three private variables below.
     * If all inputs are valid, then closes the dialog. Otherwise, generates an message box to notify user
     * about the existence of an input error.
     */
    void on_submitButton_clicked();

    /**
     * @brief on_comboBox_currentTextChanged enable/disables clientNameEdit depending on selected option
     * from the combo box.
     * @param arg1 user's selected option
     */
    void on_comboBox_currentTextChanged(const QString &arg1);

private:
    Ui::RoomActionsDialog *ui;
    QString _roomAction; /**< the room action to be performed. */
    QString _targetRoom; /**< the targeting room of the room action. */
    QString _targetUser; /**< the targeting user of the room action. */
};

#endif // ROOMACTIONSDIALOG_H
