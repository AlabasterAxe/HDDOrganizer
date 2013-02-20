// Matthew Keller
//
// Asks the user for a tag name.

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

    // Returns the value of the line edit.
    QString tagName();
    
private:
    Ui::TagNameDialog *ui;
};

#endif // TAGNAMEDIALOG_H
