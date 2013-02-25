#include "filetableview.h"
#include "drive.h"
#include <QMenu>
#include <QContextMenuEvent>
#include <QFileDialog>
#include <QDebug>

FileTableView::FileTableView(QWidget *parent) :
    QTableView(parent)
{
    this->openAsAct_ = new QAction(tr("&Open with..."), this);
    this->openAsAct_->setStatusTip(tr("Pick the application you would like to open the file with"));
    connect(this->openAsAct_, SIGNAL(triggered()), this, SLOT(openAs()));

}

void FileTableView::keyPressEvent(QKeyEvent * event)
{
    QTableView::keyPressEvent(event);
}

void FileTableView::contextMenuEvent(QContextMenuEvent *event) {
    QMenu menu(this);
    menu.addAction(this->openAsAct_);
    QAction* result = menu.exec(event->globalPos());
    QModelIndex row = this->indexAt(event->pos());
    Drive* driveModel = static_cast<Drive*>(this->model());
    if (result)
        driveModel->open(row, result->data().toString());
}

void FileTableView::openAs()
{
    QString program = QFileDialog::getOpenFileName(this, tr("Select Program"), QString("C:/"), tr("Executables (*.exe)"));
    this->openAsAct_->setData(program);
}
