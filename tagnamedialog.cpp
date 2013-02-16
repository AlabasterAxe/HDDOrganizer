#include "tagnamedialog.h"
#include "ui_tagnamedialog.h"

TagNameDialog::TagNameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TagNameDialog)
{
    ui->setupUi(this);

}

TagNameDialog::~TagNameDialog()
{
    delete ui;
}

QString TagNameDialog::tagName() {
    QString result = this->ui->lineEdit->text();
    this->ui->lineEdit->clear();
    return result;
}
