#ifndef PRIVATECHATDIALOG_H
#define PRIVATECHATDIALOG_H

#include <QDialog>

namespace Ui {
class PrivateChatDialog;
}

/**
 * @brief The PrivateChatDialog class represents a popup dialog that is used prompt user for either a
 * receiver name or a room name and message text when an user wishes to sends a private message to a
 * specific user or private chat room.
 */
class PrivateChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PrivateChatDialog(QWidget *parent = 0);
    ~PrivateChatDialog();

    /**
     * @brief receiver get the private message's receiver name
     * @return messgae's receiver name
     */
    QString receiver() const;

    /**
     * @brief text get the private message's text content
     * @return message's text content
     */
    QString text() const;

    Ui::PrivateChatDialog *ui;

private slots:
    /**
     * @brief on_submitButton_clicked validate user input for receiver name and message text.
     * If valid, then closes the dialog. Otherwise, it generates an message box to notify user
     * about the existence of an input error.
     */
    void on_submitButton_clicked();

private:
    QString _receiver; /**< the message receiver name. */
    QString _text; /**< the message text content. */
};

#endif // PRIVATECHATDIALOG_H
