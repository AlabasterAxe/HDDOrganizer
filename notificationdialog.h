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
    bool notify(QString message, QString windowName);
    
private:
    Ui::NotificationDialog *ui;
};

#endif // INDEXDIALOG_H
