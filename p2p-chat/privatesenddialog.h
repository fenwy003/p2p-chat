#ifndef PRIVATESENDDIALOG_H
#define PRIVATESENDDIALOG_H

#include <QDialog>

namespace Ui {
class PrivateSendDialog;
}

/**
 * @brief The PrivateSendDialog class represents a dialog that is used to obtain the reciver's
 * nickname or private room name when an user wants to send a file or an image to a single user
 * or users of a private room only.
 */
class PrivateSendDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PrivateSendDialog(QWidget *parent = 0);
    ~PrivateSendDialog();

    /**
     * @brief receiver get the receiver's name user inputted
     * @return user's inputted receiver name
     */
    QString receiver() const;

    Ui::PrivateSendDialog *ui;

private slots:
    /**
     * @brief on_submitButton_clicked validate user input for receiver name. If valid,
     * then closes the dialog. Otherwise, it generates an message box to notify user
     * about the existence of an input error.
     */
    void on_submitButton_clicked();

private:
    QString _receiver; /**< the receiver's name. */
};

#endif // PRIVATESENDDIALOG_H
