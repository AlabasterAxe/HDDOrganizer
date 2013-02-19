#ifndef DRIVE_H
#define DRIVE_H

#include <QStringList>
#include <QAbstractTableModel>
#include <QFileInfoList>

class QDir;
class TagTreeModel;

class Drive : public QAbstractTableModel
{
    Q_OBJECT

public:
    Drive(QString dir);
    ~Drive();
    bool overwrite(QString file);
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    bool recalculate(QModelIndexList tags);

    TagTreeModel* tagTree_;

public slots:
    bool index();
    bool save();
    bool load();

private:
    QDir *directory_;
    QFileInfoList results_;
    int columns_;

signals:
    void doneCalculating();
};

#endif // DRIVE_H
