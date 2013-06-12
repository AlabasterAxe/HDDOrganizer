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

// The tag class represents a single node in the tag tree.
// This corresponds with the ui notion of a "Tag" which can be added
// and removed and nested under one another.
//
// Each tag maintains its own hash set of files and and list of child
// tags.
//
// The set of files that a tag "has" is the unique set of files that
// it and all of its recursive children.

#ifndef TAG_H
#define TAG_H

#include <QVariant>
#include <QUrl>
#include <QSet>

class QDomElement;

class Tag
{
public:

    // There is currently no default constructor for a tag
    // but certain functions in QT return an uninitialized object
    // as an effective false.
    Tag();
    Tag(const QList<QVariant> &data, QDomElement* domNode = 0, Tag *parent = 0);

    ~Tag();

    // This function returns the child at the rowth location
    // in the underlying QList.
    Tag* child(int row);

    // Returns the location in the parent tag that this tag
    // is located.
    int row();

    // Returns the number of child tags this tag has.
    int rowCount() const;

    // Returns 2.
    int columnCount() const;

    // Creates a tag with the given tag name and associated
    // dom node. It returns a pointer to the newly created
    // tag on success and 0 on failure.
    Tag* addChild(QString tagName, QDomElement* tagDomNode);

    //
    void addChild(Tag* child);

    void addChildren(QList<Tag*> tags);

    bool removeChild(Tag* child);

    bool removeChildren(QList<Tag*> tags);

    // Appends files to the internal list of files as well as the
    // matching DOM node. This function is used for drag and drop
    // additions of files to a tag.
    //
    // TODO(mattkeller): Dynamic upwards updating of file counts
    bool addFiles(const QList<QUrl> files);

    // Replaces any existing files in the internal list
    // with the QList of QStrings. It does not alter the matching
    // DOM node.
    bool setFiles(const QList<QString> files);

    // Returns the name of the node for column 1 and the number of
    // files in the files set for column 2.
    QVariant data(int column) const;

    // Assigns the specified column the value of value.
    bool setData(int column, QVariant value);

    // Recursively compiles all of the unique files from the
    // children nodes and returns them in a set.
    virtual QSet<QString> allFiles();

    // Returns a pointer to the parent of the current node.
    Tag* parent() const;

    void setParent(Tag* parent);

    virtual bool isExpression();

    virtual bool isImplicit();

    virtual void setImplicity(bool implicity);

    // Return the internal DOM pointer.
    QDomElement* domNodePointer() const;

    void setDomNodePointer(QDomElement *domNode);
protected:
    Tag* parent_;
    QDomElement* domNode_;
    QSet<QString> files_;
    QList<QVariant> data_;
    QList<Tag*> children_;
};

#endif // TAG_H
