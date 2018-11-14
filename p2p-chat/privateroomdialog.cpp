#include "privateroomdialog.h"
#include "ui_privateroomdialog.h"
#include <QMessageBox>

PrivateRoomDialog::PrivateRoomDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrivateRoomDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("New Private Chat Room");
}

PrivateRoomDialog::~PrivateRoomDialog()
{
    delete ui;
}

QString PrivateRoomDialog::roomName() const {
    return _roomName;
}

void PrivateRoomDialog::on_submitButton_clicked()
{
    if (ui->roomNameEdit->toPlainText() == "") {
        QMessageBox::warning(this, "Invalid Room Name", "Room name cannot be empty",
                             QMessageBox::Ok);
    }
    else if (ui->roomNameEdit->toPlainText().contains('@')) {
        QMessageBox::warning(this, "Invalid Room Name", "Room name cannot contain @",
                             QMessageBox::Ok);
        ui->roomNameEdit->clear();
    }
    else if (ui->roomNameEdit->toPlainText().contains('|')) {
        QMessageBox::warning(this, "Input With Reserved Charater(s)",
                             "| is a reserved keyword, please ensure your input do not contains the symbol.",
                             QMessageBox::Ok);
        ui->roomNameEdit->clear();
    }
    else {
        _roomName = ui->roomNameEdit->toPlainText();
        this->close();
    }
}
