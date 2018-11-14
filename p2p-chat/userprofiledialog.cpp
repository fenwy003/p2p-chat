#include "userprofiledialog.h"
#include "ui_userprofiledialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
UserProfileDialog::UserProfileDialog(QWidget *parent) : QDialog(parent), ui(new Ui::UserProfileDialog)
{
    ui->setupUi(this);
}

UserProfileDialog::~UserProfileDialog()
{
    delete ui;
}

UserProfile UserProfileDialog::profile() {
    return _profile;
}

void UserProfileDialog::setProfile(UserProfile profile) {
    // Set user profile to the dialog
    _profile = profile;

    // Then display user's details
    if (!_profile.profilePhoto().isNull()) {
        QGraphicsScene *scenePtr = new QGraphicsScene(this);
        scenePtr->addPixmap(_profile.profilePhoto().scaled(48,48));
        ui->photoDisplay->setScene(scenePtr);
    }
    ui->nicknameEdit->setText(_profile.nickname());
    ui->locationEdit->setText(_profile.location());
    ui->timezoneEdit->setText(_profile.timezone());
}

void UserProfileDialog::on_uploadPhotoButton_clicked() {
    QString imagePath = QFileDialog::getOpenFileName(this,
                                                     tr("Selecting Profile Photo"), "",
                                                     tr("PNG (*png)"));
    if (!imagePath.isNull()) {
        _uploadedPhoto.load(imagePath);
        // Display the uploaded photo
        QGraphicsScene *scenePtr = new QGraphicsScene(this);
        scenePtr->addPixmap(_uploadedPhoto.scaled(48,48));
        ui->photoDisplay->setScene(scenePtr);
    }
}

void UserProfileDialog::toggleWindowMode(bool editable) {
    if (editable == false) {
        ui->nicknameEdit->setReadOnly(true);
        ui->locationEdit->setReadOnly(true);
        ui->timezoneEdit->setReadOnly(true);
        ui->saveButton->setVisible(false);
        ui->uploadPhotoButton->setVisible(false);
    }
    else {
        ui->nicknameEdit->setReadOnly(false);
        ui->locationEdit->setReadOnly(false);
        ui->timezoneEdit->setReadOnly(false);
        ui->saveButton->setVisible(true);
        ui->uploadPhotoButton->setVisible(true);
    }
}

void UserProfileDialog::toggleNicknameEdit(bool updateable) {
    if (updateable == true) {
        ui->nicknameEdit->setReadOnly(false);
    }
    else {
        ui->nicknameEdit->setReadOnly(true);
    }
}

void UserProfileDialog::on_saveButton_clicked() {
    // Ensure input do not contains reserved keyword |
    if (!ui->nicknameEdit->text().contains('|') && !ui->locationEdit->text().contains('|') &&
            !ui->timezoneEdit->text().contains('|')) {
        QMessageBox::StandardButton confirmation;
        confirmation = QMessageBox::question(this, tr("Save User Profile Change"), tr("Save profile changes?")
                                             , QMessageBox::Yes|QMessageBox::No);
        if (confirmation == QMessageBox::Yes) {
            QMessageBox::information(this, tr("User Profile Updated"), tr("Profile have been updated!"));
            // Only update profile photo when user actually did upload a new one
            if (!_uploadedPhoto.isNull()) {
                _profile.setProfilePhoto(_uploadedPhoto);
            }
            _profile.setNickname(ui->nicknameEdit->text());
            _profile.setLocation(ui->locationEdit->text());
            _profile.setTimezone(ui->timezoneEdit->text());
            this->close();
        }
    }
    else {
        QMessageBox::warning(this, "Input With Reserved Charater(s)",
                             "| is a reserved keyword, please ensure your input do not contains the symbol.",
                             QMessageBox::Ok);
    }
}
