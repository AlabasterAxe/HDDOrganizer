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

// Generic Notification Dialog Box.

#ifndef INDEXDIALOG_H
#define INDEXDIALOG_H

#include <QDialog>

namespace Ui {
class NotificationDialog;
}

class NotificationDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NotificationDialog(QWidget *parent = 0);
    ~NotificationDialog();

    // Accepts the message and the window title and returns the
    // user's selection.
    bool notify(QString message, QString windowName);
    
private:
    Ui::NotificationDialog *ui;
};

#endif // INDEXDIALOG_H
