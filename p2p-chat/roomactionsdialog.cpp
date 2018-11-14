#include "roomactionsdialog.h"
#include "ui_roomactionsdialog.h"
#include <QMessageBox>

RoomActionsDialog::RoomActionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RoomActionsDialog)
{
    ui->setupUi(this);
}

RoomActionsDialog::~RoomActionsDialog()
{
    delete ui;
}

QString RoomActionsDialog::roomAction() const {
    return _roomAction;
}

QString RoomActionsDialog::targetRoom() const {
    return _targetRoom;
}

QString RoomActionsDialog::targetUser() const {
    return _targetUser;
}

void RoomActionsDialog::on_submitButton_clicked()
{
    if (ui->comboBox->currentText() != "LEAVE") {
        if (ui->roomNameEdit->toPlainText() == "" || ui->clientNameEdit->toPlainText() == "") {
            QMessageBox::warning(this, "Empty Input(s) Found!", "Please ensure all input fields have been filled out.",
                                 QMessageBox::Ok);
        }
        else if (ui->roomNameEdit->toPlainText().contains('|') ||
                 ui->clientNameEdit->toPlainText().contains('|')) {
            QMessageBox::warning(this, "Input With Reserved Charater(s)",
                                 "| is a reserved keyword, please ensure your input do not contains the symbol.",
                                 QMessageBox::Ok);
        }
        else {
            _roomAction = ui->comboBox->currentText();
            _targetRoom = ui->roomNameEdit->toPlainText();
            _targetUser = ui->clientNameEdit->toPlainText();
            this->close();
        }
    }
    else {
        if (ui->roomNameEdit->toPlainText() == "") {
            QMessageBox::warning(this, "Empty Input(s) Found!", "Please ensure all input fields have been filled out.",
                                 QMessageBox::Ok);
        }
        else if (ui->roomNameEdit->toPlainText().contains('|')) {
            QMessageBox::warning(this, "Input With Reserved Charater(s)",
                                 "| is a reserved keyword, please ensure your input do not contains the symbol.",
                                 QMessageBox::Ok);
        }
        else {
            _roomAction = ui->comboBox->currentText();
            _targetRoom = ui->roomNameEdit->toPlainText();
            _targetUser = ui->clientNameEdit->toPlainText();
            this->close();
        }
    }
}

void RoomActionsDialog::on_comboBox_currentTextChanged(const QString &arg1)
{
    // Disable clientNameEdit as the only targeting user of LEAVE action message is the client initiating the LEAVE action
    if (arg1 == "LEAVE") {
        ui->clientNameEdit->setEnabled(false);
    }
    else {
        ui->clientNameEdit->setEnabled(true);
    }
}
