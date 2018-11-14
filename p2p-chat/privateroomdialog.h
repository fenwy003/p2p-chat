#ifndef PRIVATEROOMDIALOG_H
#define PRIVATEROOMDIALOG_H

#include <QDialog>

namespace Ui {
class PrivateRoomDialog;
}

/**
 * @brief The PrivateRoomDialog class represents a popup dialog that is used to obtain
 * the name of a new private chatroom.
 */
class PrivateRoomDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PrivateRoomDialog(QWidget *parent = 0);
    ~PrivateRoomDialog();

    /**
     * @brief roomName get the  private room's name
     * @return the private room name
     */
    QString roomName() const;

private slots:
    /**
     * @brief on_submitButton_clicked validate user input for room name. If valid, then
     * closes the dialog. Otherwise, it generates an message box to notify user about the
     * existence of an input error.
     */
    void on_submitButton_clicked();

private:
    Ui::PrivateRoomDialog *ui;
    QString _roomName; /**< user's input name for a new private chatroom. */
};

#endif // PRIVATEROOMDIALOG_H
