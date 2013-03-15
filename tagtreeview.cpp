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
