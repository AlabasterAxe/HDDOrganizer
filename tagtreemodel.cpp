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

#include "tagtreemodel.h"
#include "tag.h"
#include "tagmimedata.h"

#include <iostream>
#include <QMimeData>
#include <QStringList>
#include <QUrl>
#include <QDomDocument>
#include <QDebug>

TagTreeModel::TagTreeModel(QString name, QObject *parent) :
    QAbstractItemModel(parent)
{
    this->domTree_ = new QDomDocument(QString("HDDOrganizer"));
    QDomElement domRootElement = this->domTree_->createElement(QString("tags"));
    this->domTree_->appendChild(domRootElement);
    this->domTreeRoot_ = new QDomElement(domRootElement);
    this->domTreeRoot_->setAttribute(QString("version"),QString("0.4"));
    // initialize the root node with the titles of the columns for the tree view
    QList<QVariant> data;
    data << "Tag" << "# of Files";
    this->root_ = new Tag(data, this->domTreeRoot_);

}

TagTreeModel::~TagTreeModel() {
    delete root_;
    delete domTreeRoot_;
    delete domTree_;
}

void TagTreeModel::translate(QDomElement * domNode, Tag * tagNode) {
    QDomNodeList domChildren = domNode->childNodes();

    int numChildren = domChildren.size();
    for (int i = 0; i < numChildren; ++i) {
        QDomElement* child = new QDomElement(domChildren.item(i).toElement());
        Tag* childTag = tagNode->addChild(child->attribute(QString("name")), child);
        this->translate(child, childTag);
    }

    QString fileString = domNode->attribute(QString("files"));
    QStringList filesList = fileString.split(QChar('|'), QString::SkipEmptyParts);
    tagNode->setFiles(filesList);
}

QModelIndex TagTreeModel::index(int row, int column, const QModelIndex & parent) const {

    if (!hasIndex(row, column, parent))
        return QModelIndex();

    Tag* parentTag;

    if (!parent.isValid())
        parentTag = this->root_;
    else
        parentTag = static_cast<Tag*>(parent.internalPointer());

    Tag* childTag = parentTag->child(row);
    if (childTag)
        return createIndex(row, column, childTag);
    else
        return QModelIndex();
}

int TagTreeModel::rowCount(const QModelIndex & parent) const {
    if (parent.column() > 0)
        return 0;

    Tag* parentTag;

    if (!parent.isValid())
        parentTag = this->root_;
    else
        parentTag = static_cast<Tag*>(parent.internalPointer());

    return parentTag->rowCount();
}

int TagTreeModel::columnCount(const QModelIndex & parent) const {
    return 2;
}

QVariant TagTreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant();

    switch (section) {
        case 0:
        return QString("Name");
        case 1:
        return QString("# of Files");
    }
    return QVariant();
}

QVariant TagTreeModel::data(const QModelIndex &index, int role) const {

    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    Tag* tag = static_cast<Tag*>(index.internalPointer());
    return tag->data(index.column());
}

Qt::ItemFlags TagTreeModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled | Qt::ItemIsDragEnabled;
}

QModelIndex TagTreeModel::parent(const QModelIndex & child) const {
    if (!child.isValid())
        return QModelIndex();

    Tag* childTag = static_cast<Tag*>(child.internalPointer());
    Tag* parentTag = childTag->parent();

    if (parentTag == this->root_) {
       return QModelIndex();
    }

    return createIndex(parentTag->row(), 0, parentTag);
}

bool TagTreeModel::setData(const QModelIndex &index, const QVariant &value,
                        int role) {
    if (role != Qt::EditRole)
        return false;

    Tag* item = static_cast<Tag*>(index.internalPointer());
    bool result = item->setData(index.column(), value);

    if (result)
        emit dataChanged(index, index);

    return result;
}

bool TagTreeModel::insertTag(const QString tagName, const QModelIndex& parent) {
    Tag* parentTag = getIndexTag(parent);
    QDomNode* parentDomNode;

    if (parentTag != this->root_ && parentTag != 0) {
        parentDomNode = parentTag->domNodePointer();
    } else {
        parentDomNode = this->domTreeRoot_;
    }

    int position = parentTag->rowCount();
    bool result;

    QDomElement tagDomNode = this->domTree_->createElement(QString("tag"));
    QDomElement* tagDomNodePointer = new QDomElement(tagDomNode);
    tagDomNodePointer->setAttribute(QString("name"),tagName);

    beginInsertRows(parent, position, position);
    QDomNode newDomNode = parentDomNode->appendChild(tagDomNode);

    if (newDomNode == QDomElement())
        return false;

    result = parentTag->addChild(tagName, tagDomNodePointer);
    endInsertRows();
    return result;
}

bool TagTreeModel::deleteTag(const QModelIndex &tag)
{
    if (tag.isValid()) {
        Tag* tagPointer = this->getIndexTag(tag);
        Tag* tagParent = tagPointer->parent();
        if (tagParent == this->root_)
        beginRemoveRows(tag.parent(),tag.row(),tag.row());
        tagParent->removeChild(tagPointer);
        endRemoveRows();
        delete tagPointer;
        return true;
    }

    return false;
}


Tag* TagTreeModel::getIndexTag(const QModelIndex& index) const {
    if (index.isValid()) {
        Tag* tag = static_cast<Tag*>(index.internalPointer());
        if (tag)
            return tag;
    }

    return this->root_;
}

bool TagTreeModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) {

    Tag* newParent = getIndexTag(parent);

    if (data->hasUrls() && newParent != this->root_) {
        newParent->addFiles(data->urls());
    } else if (data->hasFormat(QString("text/pointer"))) {
        const TagMimeData * tagData = qobject_cast<const TagMimeData* >(data);
        QModelIndex oldParent = tagData->parent();
        Tag* oldParentTag = getIndexTag(oldParent);
        QList<QPair<QPair<int,int>, QList<Tag*> > > regions = tagData->regions();
        QList<Tag*> children = tagData->tags();

        if (oldParentTag != newParent &&
            !children.contains(newParent)) {
            for (auto it = regions.begin(); it != regions.end(); ++it) {
                beginMoveRows(oldParent,
                              it->first.first,
                              it->first.second,
                              parent,
                              newParent->rowCount());
                getIndexTag(oldParent)->removeChildren(it->second);
                newParent->addChildren(it->second);
                endMoveRows();
            }
        }
    } else {
        return false;
    }

    return true;
}

QList<Tag *> TagTreeModel::getTagList(QModelIndexList indicies) {
    QList<Tag *> res;

    int indexQuantity = indicies.size();
    for(int i = 0; i < indexQuantity; ++i ) {
        res.append(getIndexTag(indicies[i]));
    }

    return res;
}

QString TagTreeModel::stringify() const {
    return this->domTree_->toString();
}

bool TagTreeModel::setContent(QIODevice *dev)
{
    QString error;
    QDomDocument *fileContents = new QDomDocument("HDDOrganizer");
    bool result = fileContents->setContent(dev, &error);
    QDomElement *fileRoot = new QDomElement(fileContents->firstChild().toElement());

    if(fileRoot->attribute(QString("version")) == "") {
        return false;
    }

    delete this->domTree_;
    delete this->domTreeRoot_;

    this->domTree_ = fileContents;
    this->domTreeRoot_ = fileRoot;

    this->root_->setDomNodePointer(this->domTreeRoot_);
    this->translate(this->domTreeRoot_, this->root_);

    return result;
}

QMimeData* TagTreeModel::mimeData(const QModelIndexList &indexes) const
{
    QList<Tag*> tags;
    QList<Tag*> allTags;
    QModelIndex* parent = 0;
    TagMimeData* data = new TagMimeData();
    int start = -2;
    int end = -2;
    for (auto it = indexes.begin(); it != indexes.end(); ++it) {
        if (it->column() == 0) {
            Tag* tag = this->getIndexTag(*it);
            if (!parent) {
                parent = &it->parent();
            } else if (*parent != it->parent()) {
                return new QMimeData();
            }

            if(it->row() == end+1)
                ++end;
            else {
                if(it != indexes.begin()) {
                    data->addRegion(start, end, tags);
                    tags.clear();
                }
                start = it->row();
                end = start;
            }
            tags << tag;
            allTags << tag;
        }

        auto test = it;
        ++test;
        if (test == indexes.end())
            data->addRegion(start, end, tags);
    }

    data->setData(tr("text/pointer"),QByteArray());
    data->setTags(allTags);
    data->setParent(*parent);
    return data;
}
