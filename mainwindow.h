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

// Defines the main UI.
// Connects the various elements of the UI using QT's signals and slots
// system.

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include <QPair>
#include <QSet>

class QFileSystemModel;
class Drive;
class Tag;
class NotificationDialog;
class TagNameDialog;
class Operation;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    Ui::MainWindow *ui;
    ~MainWindow();
    void setExpressionLabel(QString expression);

    Tag * parse(QString &str, Tag *parent, Tag *tag1 = 0);
    Tag *extractTag(QString &str, Tag *parent);
    Operation *extractOperation(QString &str);
private:
    Drive* drive_;
    NotificationDialog* notificationDialog_;
    TagNameDialog* tagNameDialog_;

private slots:
    void selectDirectory();
    void addTag();
    void removeTag();
    void recalculate();
    void filter();
    void sortTableByColumn(int column, Qt::SortOrder order);
};

#endif // MAINWINDOW_H
