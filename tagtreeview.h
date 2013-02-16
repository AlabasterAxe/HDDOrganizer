#ifndef TAGTREEVIEW_H
#define TAGTREEVIEW_H

#include <QTreeView>

class TagTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit TagTreeView(QWidget *parent = 0);
    void dragMoveEvent(QDragMoveEvent* event);
    void dragEnterEvent(QDragEnterEvent* event);

signals:
    
public slots:
    
};

#endif // TAGTREEVIEW_H
