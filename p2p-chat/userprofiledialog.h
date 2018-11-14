#ifndef USERPROFILEDIALOG_H
#define USERPROFILEDIALOG_H

#include <QDialog>
#include "userprofile.h"

namespace Ui {
class UserProfileDialog;
}

/**
 * @brief The UserProfileDialog class represents a dialog that is used to present
 * an user's profile. As well as for changing the detail of an user's profile.
 */
class UserProfileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserProfileDialog(QWidget *parent = 0);
    ~UserProfileDialog();

    /**
     * @brief profile returns the client's user profile
     * @return client's user profile
     */
    UserProfile profile();

    /**
     * @brief setProfile sets the user's profile to their user profile window
     * @param profile the profile to set the window to
     */
    void setProfile(UserProfile profile);

    /**
     * @brief toggleWindowMode enable/disable input fields in the ui.
     * @param editable true to enable, false to disable.
     */
    void toggleWindowMode(bool editable);

    /**
     * @brief toggleNicknameEdit enable/disable the input field for nickname in the ui.
     * @param updateable true to allow update, false to prevent update.
     */
    void toggleNicknameEdit(bool updateable);

private:
    Ui::UserProfileDialog *ui;
    UserProfile _profile; /**< the user profile of the client. */
    QPixmap _uploadedPhoto; /**< the profile photo uploaded by the client. */

private slots:
    /**
     * @brief on_uploadPhotoButton_clicked generates a filedialog which allows users to
     * choose and upload their desired 48x48 png profile photo.
     */
    void on_uploadPhotoButton_clicked();

    /**
     * @brief on_saveButton_clicked updates currently displayed profile's details and close
     * the dialog.
     */
    void on_saveButton_clicked();
};

#endif // USERPROFILEDIALOG_H
