// Matthew Keller
//
// This class subclasses the Generic Tree View in order to redefine the
// drag behavior.

#ifndef TAGTREEVIEW_H
#define TAGTREEVIEW_H

#include <QTreeView>

class TagTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit TagTreeView(QWidget *parent = 0);

    // Accepts data of type "text/uri" rejects all others.
    void dragMoveEvent(QDragMoveEvent* event);

    // Same as above.
    void dragEnterEvent(QDragEnterEvent* event);

signals:
    
public slots:
    
};

#endif // TAGTREEVIEW_H
