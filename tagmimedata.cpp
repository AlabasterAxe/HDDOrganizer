#include "tagmimedata.h"

class Tag;

TagMimeData::TagMimeData() :
    QMimeData()
{
}

QList<Tag*> TagMimeData::tags() const {
    return this->tags_;
}

void TagMimeData::setTags(QList<Tag*> tags) {
    this->tags_ = tags;
}

void TagMimeData::addRegion(int start, int end, QList<Tag *> tags)
{
    QPair<int,int> bounds(start,end);
    QPair<QPair<int,int>, QList<Tag *>> region(bounds, tags);
    this->regions_.append(region);
}

QModelIndex TagMimeData::parent() const {
    return this->parent_;
}

 QList<QPair<QPair<int, int>, QList<Tag*> > > TagMimeData::regions() const
{
    return this->regions_;
}

void TagMimeData::setParent(QModelIndex parent) {
    this->parent_ = parent;
}
