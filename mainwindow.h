// Matthew Keller
//
// Defines the main UI.
// Connects the various elements of the UI using QT's signals and slots
// system.

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QFileSystemModel;
class Drive;
class NotificationDialog;
class TagNameDialog;
class QModelIndex;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    QFileSystemModel* fileSys_;
    Drive* drive_;
    NotificationDialog* notificationDialog_;
    TagNameDialog* tagNameDialog_;

private slots:
    void selectDirectory();
    void addTag();
    void recalculate();
    void sortTableByColumn(int column, Qt::SortOrder order);
};

#endif // MAINWINDOW_H
