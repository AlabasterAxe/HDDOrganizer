#ifndef FILETABLEVIEW_H
#define FILETABLEVIEW_H

#include <QTableView>

class FileTableView : public QTableView
{
    Q_OBJECT
public:
    explicit FileTableView(QWidget *parent = 0);

public slots:
    void openAs();
protected:
    void contextMenuEvent(QContextMenuEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    QAction* openAsAct_;

signals:
    void newSelection();
};

#endif // FILETABLEVIEW_H
