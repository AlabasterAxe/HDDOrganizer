// Matthew Keller
//
// The drive class represensts a particular folder in your computer.
// Although there is no technical reason why it couldn't represent the
// contents of any folder, the software was imagined as an organization
// tool for external drives. Hence the name drive.
//
// It is extends the QT AbstractTableModel in order to leverage the
// model view framework that QT has in place.
//
// When attached to a view, it displays all of the files in the specified
// directory, or, if the member TagTreeModel is attached to a TagTreeView,
// the intersection of files in the currently selected tags.

#ifndef DRIVE_H
#define DRIVE_H

#define TAG_TREE_COLUMNS 2

#include <QStringList>
#include <QAbstractTableModel>
#include <QFileInfoList>
#include "QFileInfo.h"

class QDir;
class QAction;
class TagTreeModel;
class MainWindow;

class Drive : public QAbstractTableModel
{
    Q_OBJECT

public:

    // constructs a Drive with the specified directory.
    Drive(QString dir, MainWindow* parent);
    ~Drive();

    // not sure if this is necessary
    bool overwrite(QString file);

    // This is an inherited pure virtual function from AbstractTable Model.
    // returns 4.
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    // Inherited.
    // Returns the number of files.
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    // Returns the data for the given row/column combination.
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    // Returns the names of the header for the view.
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

    // Computes the result list based on the tags that are selected in the
    // TagTreeView.
    bool recalculate(QModelIndexList tags);

    // Redefine Q
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

    // Defines the interaction for the particular QModelIndex.
    Qt::ItemFlags flags(const QModelIndex &index) const;

    TagTreeModel* tagTree_;

    QMimeData *mimeData(const QModelIndexList &indexes) const;

public slots:
    bool index();
    bool save();
    bool load();
    void reset();
    void preview(const QModelIndex &index);
    void open(const QModelIndex &index, const QString & program = 0);

private:
    QDir *directory_;
    QList<QFileInfo> results_;
    int columns_;
    int sortColumn_;
    MainWindow* parent_;
    Qt::SortOrder sortOrder_;

signals:
    void doneCalculating();
    void rowsAboutToBeRemoved(int start, int end);
};

#endif // DRIVE_H
