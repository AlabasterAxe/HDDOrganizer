#ifndef TAGMIMEDATA_H
#define TAGMIMEDATA_H

#include <QMimeData>
#include <QModelIndex>
#include <QSet>

class Tag;

class TagMimeData : public QMimeData
{
    Q_OBJECT
public:
    explicit TagMimeData();
    QList<Tag*> tags() const;
    QModelIndex parent() const;
    QList<QPair<QPair<int, int>, QList<Tag*> > > regions() const;

    void setParent(QModelIndex parent);
    void setTags(QList<Tag *> tags);
    void addRegion(int start, int end, QList<Tag*> tags);
signals:
    
public slots:

private:
    QList<Tag*> tags_;
    QModelIndex parent_;
    QList<QPair<QPair<int, int>, QList<Tag*> > > regions_;

};

#endif // TAGMIMEDATA_H
