// Matthew Keller
// This is the main storage item for the tag tree model

#include "tag.h"
#include <iostream>

#include <QVariant>
#include <QDomElement>

Tag* Tag::addChild(const QString tagName, QDomElement* tagDomNode) {
    if (tagName.length() == 0)
        return 0;

    QList<QVariant> data;
    data << tagName << 0;
    Tag* tag = new Tag(data, tagDomNode, this);
    this->children_.append(tag);
    return tag;
}

bool Tag::removeChild(Tag *child)
{
    this->domNode_->removeChild(*child->domNodePointer());
    return this->children_.removeOne(child);
}

Tag::Tag(const QList<QVariant> &data, QDomElement* domNode, Tag *parent)
{
    this->parent_ = parent;
    this->data_ = data;
    this->domNode_ = domNode;
}

Tag::~Tag()
{
    delete domNode_;
    for(int i = 0; i < this->children_.size();++i) {
        delete children_[i];
    }

    children_.clear();
    // TODO Recursively delete children nodes.
}


Tag* Tag::child(int row) {
    return this->children_.value(row);
}

int Tag::columnCount() const {
    return this->data_.count();
}

int Tag::rowCount() const {
    return this->children_.count();
}

QVariant Tag::data(int column) const {
    return this->data_.value(column);
}

bool Tag::addFiles(const QList<QUrl> files) {
    int urlQuantity = files.size();
    QString attrName("files");
    QString fileString;

    if(domNode_)
        fileString = domNode_->attribute(attrName);
    else
        return false;

    for (int i = 0; i < urlQuantity; ++i) {
        if (files[i].isLocalFile()) {
            QString fileName = files[i].toLocalFile();
            if (!files_.contains(fileName)) {
                this->files_.insert(fileName);
                this->setData(1,this->data(1).toInt()+1);
                fileString.append(fileName);
                fileString.append(QString("|"));
            }
        }
    }

    this->domNode_->setAttribute(attrName,fileString);
    return true;
}

bool Tag::setFiles(const QList<QString> files) {
    int fileQuantity = files.size();
    QString attrName("files");
    QString fileString;

    this->files_.clear();

    for (int i = 0; i < fileQuantity; ++i) {
        QString fileName = files[i];
        this->files_.insert(fileName);
        this->setData(1,this->data(1).toInt()+1);
    }

    return true;
}

bool Tag::setData(int column, QVariant value) {
    this->data_[column] = value;
    return true;
}

Tag* Tag::parent() const {
    return this->parent_;
}

QDomElement* Tag::domNodePointer() const
{
    return this->domNode_;
}

int Tag::row() {
    if (parent_)
        return parent_->children_.indexOf(const_cast<Tag*>(this));

     return 0;
}

QSet<QString> Tag::allFiles() {
    int childQuantity = this->children_.size();
    QSet<QString> uniqueFiles = this->files_;
    for(int i = 0; i < childQuantity; ++i) {
        uniqueFiles = uniqueFiles.unite(this->children_[i]->allFiles());
    }

    return uniqueFiles;
}
