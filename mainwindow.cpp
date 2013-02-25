#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "drive.h"
#include "tag.h"
#include "tagtreemodel.h"
#include "notificationdialog.h"
#include "tagnamedialog.h"

#include <QFileDialog>
#include <QFileSystemModel>

#define TAG_TREE_COLUMNS 2

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->drive_ = 0;
    this->notificationDialog_ = new NotificationDialog;
    this->tagNameDialog_ = new TagNameDialog;
    this->selectDirectory();

    QObject::connect(this->ui->actionSelectDirectory,SIGNAL(triggered()),
                     this,SLOT(selectDirectory()));
    QObject::connect(this->ui->pushButton,SIGNAL(pressed()),
                     this,SLOT(addTag()));
    QObject::connect(this->ui->treeView,SIGNAL(viewClicked()),
                     this,SLOT(recalculate()));
    QObject::connect(this->ui->tableView->horizontalHeader(),
                     SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)),
                     this,
                     SLOT(sortTableByColumn(int, Qt::SortOrder)));
    QObject::connect(this->ui->treeView,SIGNAL(viewReleased()),
                     this->ui->tableView,SLOT(reset()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete drive_;
    delete notificationDialog_;
}

void MainWindow::selectDirectory() {
    // Prompt the user for which drive they would like to work with.
    QString driveName = QFileDialog::getExistingDirectory(this,
                                                          tr("Select Directory"));
    if (driveName.isEmpty() && !this->drive_) {
        QString dialogMessage("Please select a valid directory. Select \"Ok\" to select a new directory or \"Cancel\" to exit the program. ");
        QString dialogTitle("Select a directory");
        if (this->notificationDialog_->notify(dialogMessage,dialogTitle)) {
             selectDirectory();
             return;
        } else {
             exit(0);
        }
    }

    if (this->drive_)
        delete this->drive_;
    this->drive_ = new Drive(driveName, this);
    this->ui->tableView->setModel(this->drive_);
    this->ui->treeView->setModel(this->drive_->tagTree_);

    QObject::connect(this->drive_,SIGNAL(doneCalculating()),this->ui->tableView,SLOT(reset()));
    QObject::connect(this->ui->actionSave,SIGNAL(triggered()),this->drive_,SLOT(save()));
    QObject::connect(this->ui->tableView,SIGNAL(clicked(const QModelIndex)),
                     this->drive_,SLOT(preview(QModelIndex)));
    QObject::connect(this->ui->tableView,SIGNAL(doubleClicked(const QModelIndex)),
                     this->drive_,SLOT(open(QModelIndex)));
    QObject::connect(this->ui->tableView,SIGNAL(activated(const QModelIndex)),
                     this->drive_,SLOT(preview(QModelIndex)));

    if(!this->drive_->load()) {
        QString dialogMessage("No index file was found for this \"Drive\". Would you like to index it now?");
        QString dialogTitle("Index Drive?");
        if(this->notificationDialog_->notify(dialogMessage, dialogTitle)) {
            this->drive_->index();
        }
    }

    this->setWindowTitle(driveName);
}

void MainWindow::addTag() {
    if(this->tagNameDialog_->exec()) {
        QModelIndexList selection = this->ui->treeView->selectionModel()->selectedIndexes();
        if (selection.count() > 1*TAG_TREE_COLUMNS) {
            QString message = "Please select a single tag to create a child tag, and no tags to create a top level tag";
            this->ui->statusBar->showMessage(message, 1000);
        }

        QString tagName = this->tagNameDialog_->tagName();
        if (selection.count() == 0) {
            this->drive_->tagTree_->insertTag(tagName);
        } else if (selection.count() == 1*TAG_TREE_COLUMNS) {
            this->drive_->tagTree_->insertTag(tagName,selection[0]);
        }
    }
}

void MainWindow::recalculate() {
    QModelIndexList selection = this->ui->treeView->selectionModel()->selectedIndexes();
    this->drive_->recalculate(selection);
}


void MainWindow::sortTableByColumn(int column, Qt::SortOrder order)
{
    this->ui->tableView->sortByColumn(column, order);
    this->ui->tableView->reset();
}
