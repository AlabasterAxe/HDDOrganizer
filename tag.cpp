#include "tag.h"
#include <iostream>

#include <QVariant>

Tag::Tag(const QList<QVariant> &data, Tag* parent)
{
    this->parent_ = parent;
    this->data_ = data;
}

bool Tag::addChild(const QString tagName) {
    if (tagName.length() == 0)
        return false;

    QList<QVariant> data;
    data << tagName << 0;
    Tag* tag = new Tag(data, this);
    this->children_.append(tag);
    return true;
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

    for (int i = 0; i < urlQuantity; ++i) {
        if (files[i].isLocalFile()) {
            QString fileName = files[i].toLocalFile();
                this->files_.insert(fileName);
                this->setData(1,this->data(1).toInt()+1);
        }
    }
    return true;
}

bool Tag::setData(int column, QVariant value) {
    this->data_[column] = value;
    return true;
}

Tag* Tag::parent() {
    return this->parent_;
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
