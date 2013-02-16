#ifndef TAGNAMEDIALOG_H
#define TAGNAMEDIALOG_H

#include <QDialog>

namespace Ui {
class TagNameDialog;
}

class TagNameDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit TagNameDialog(QWidget *parent = 0);
    ~TagNameDialog();
    QString tagName();
    
private:
    Ui::TagNameDialog *ui;
};

#endif // TAGNAMEDIALOG_H
