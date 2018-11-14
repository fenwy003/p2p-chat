#include "privatechatdialog.h"
#include "ui_privatechatdialog.h"
#include <QMessageBox>

PrivateChatDialog::PrivateChatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrivateChatDialog)
{
    ui->setupUi(this);
}

PrivateChatDialog::~PrivateChatDialog()
{
    delete ui;
}

QString PrivateChatDialog::receiver() const {
    return _receiver;
}

QString PrivateChatDialog::text() const {
    return _text;
}

void PrivateChatDialog::on_submitButton_clicked()
{
    if (ui->nameEdit->toPlainText() == "") {
        QMessageBox::warning(this, "Invalid Receiver Name", "Receiver name cannot be empty",
                             QMessageBox::Ok);
    }
    else if (ui->nameEdit->toPlainText().contains('|') ||
             ui->textEdit->toPlainText().contains('|')){
        QMessageBox::warning(this, "Input With Reserved Charater(s)",
                             "| is a reserved keyword, please ensure your input do not contains the symbol.",
                             QMessageBox::Ok);
    }
    else {
        _receiver = ui->nameEdit->toPlainText();
        _text = ui->textEdit->toPlainText();
        this->close();
    }
}
