#include "privatesenddialog.h"
#include "ui_privatesenddialog.h"
#include <QMessageBox>

PrivateSendDialog::PrivateSendDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrivateSendDialog)
{
    ui->setupUi(this);
}

PrivateSendDialog::~PrivateSendDialog()
{
    delete ui;
}

QString PrivateSendDialog::receiver() const {
    return _receiver;
}

void PrivateSendDialog::on_submitButton_clicked()
{
    if (ui->nameEdit->toPlainText() == "") {
        QMessageBox::warning(this, "Invalid Receiver Name", "Receiver name cannot be empty",
                             QMessageBox::Ok);
    }
    else if (ui->nameEdit->toPlainText().contains('|')) {
        QMessageBox::warning(this, "Input With Reserved Charater(s)",
                             "| is a reserved keyword, please ensure your input do not contains the symbol.",
                             QMessageBox::Ok);
        ui->nameEdit->clear();
    }
    else {
        _receiver = ui->nameEdit->toPlainText();
        this->close();
    }
}
