#include "tagtreeview.h"
#include <QDragMoveEvent>
#include <QMimeData>
#include <QDebug>

TagTreeView::TagTreeView(QWidget *parent) :
    QTreeView(parent)
{
}

void TagTreeView::dragMoveEvent( QDragMoveEvent* event) {
    if ( event->mimeData()->hasFormat(QString("text/uri-list")) )
            event->acceptProposedAction();
        else
            event->setDropAction( Qt::IgnoreAction );
}

void TagTreeView::dragEnterEvent( QDragEnterEvent* event ) {
    if ( event->mimeData()->hasFormat(QString("text/uri-list")) )
            event->acceptProposedAction();
        else
        event->setDropAction( Qt::IgnoreAction );
}

void TagTreeView::mousePressEvent(QMouseEvent *event)
{
    QTreeView::mousePressEvent(event);
    emit(viewClicked());
}

void TagTreeView::mouseReleaseEvent(QMouseEvent *event)
{
    QTreeView::mouseReleaseEvent(event);
    emit(viewReleased());
}
