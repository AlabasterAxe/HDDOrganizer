// Matthew Keller
//
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

    bool removeChild(Tag* child);

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
    QSet<QString> allFiles();

    // Returns a pointer to the parent of the current node.
    Tag* parent() const;

    // Return the internal DOM pointer.
    QDomElement* domNodePointer() const;

private:
    QList<Tag*> children_;
    Tag* parent_;
    QDomElement* domNode_;
    QSet<QString> files_;
    QList<QVariant> data_;
};

#endif // TAG_H
