#include "tagtreemodel.h"
#include "tag.h"

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
    // initialize the root node with the titles of the columns for the tree view
    QList<QVariant> data;
    data << "Tag" << "# of Files";
    this->root_ = new Tag(data);

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
        Tag* childTag = tagNode->addChild(child->nodeName(), child);
        this->translate(child, childTag);
    }

    QString fileString = domNode->attribute(QString("files"));
    QStringList filesList = fileString.split(QChar('|'), QString::SkipEmptyParts);
    tagNode->setFiles(filesList);

}

QModelIndex TagTreeModel::index(int row, int column, const QModelIndex & parent) const {

    if (!hasIndex(row, column, parent))
        return QModelIndex();

    if (parent.isValid() && parent.column() != 0)
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

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled;
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

    QDomElement tagDomNode = this->domTree_->createElement(tagName);
    QDomElement* tagDomNodePointer = new QDomElement(tagDomNode);

    beginInsertRows(parent, position, position);
    QDomNode newDomNode = parentDomNode->appendChild(tagDomNode);

    if (newDomNode == QDomElement())
        return false;

    result = parentTag->addChild(tagName, tagDomNodePointer);
    endInsertRows();
    return result;
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
    Tag* parentTag = getIndexTag(parent);

    if (parentTag != this->root_)
        parentTag->addFiles(data->urls());
    else
        return false;

    return true;
}

QFileInfoList TagTreeModel::computeResult(QModelIndexList tags) const {
    if (tags.size() < 1)
        return QFileInfoList();

    QSet<QString> first = getIndexTag(tags[0])->allFiles();

    int tagQuantity = tags.size();
    for (int i = 1; i < tagQuantity; ++i)
        first = first.intersect(getIndexTag(tags[i])->allFiles());

    QFileInfoList files;

    for (auto i = first.begin(); i != first.end(); ++i)
        files.append(QFileInfo(*i));

    return files;
}

QString TagTreeModel::stringify() const {
    return this->domTree_->toString();
}

bool TagTreeModel::setContent(QIODevice *dev)
{
    QString error;
    bool result = this->domTree_->setContent(dev, &error);
    QDomElement* domRootElement = new QDomElement(this->domTree_->firstChild().toElement());
    this->domTreeRoot_ = domRootElement;

    this->translate(this->domTreeRoot_, this->root_);

    return result;
}
