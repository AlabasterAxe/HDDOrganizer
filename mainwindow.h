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
    //QDir drive;
    QFileSystemModel* fileSys_;
    Drive* drive_;
    NotificationDialog* notificationDialog_;
    TagNameDialog* tagNameDialog_;

private slots:
    void selectDirectory();
    void addTag();
    void recalculate();
};

#endif // MAINWINDOW_H
