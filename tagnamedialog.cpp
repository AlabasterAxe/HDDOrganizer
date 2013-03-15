/*  Copyright 2013 Matthew Keller

    This file is part of HDDOrganizer.

    HDDOrganizer is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    HDDOrganizer is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with HDDOrganizer.  If not, see <http://www.gnu.org/licenses/>.
*/

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
