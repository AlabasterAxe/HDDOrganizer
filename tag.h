#ifndef TAG_H
#define TAG_H

#include <QVariant>
#include <QUrl>
#include <QSet>

class QDomElement;

class Tag
{
public:
    Tag(const QList<QVariant> &data, QDomElement* domNode = 0, Tag *parent = 0);

    Tag* child(int row);
    int row();
    int rowCount() const;
    int columnCount() const;
    Tag* addChild(QString tagName, QDomElement* tagDomNode);
    bool addFiles(const QList<QUrl> files);
    bool setFiles(const QList<QString> files);
    QVariant data(int column) const;
    bool setData(int column, QVariant value);
    QSet<QString> allFiles();
    Tag* parent() const;
    QDomElement* domNodePointer() const;

private:
    QList<Tag*> children_;
    Tag* parent_;
    QDomElement* domNode_;
    QSet<QString> files_;
    QList<QVariant> data_;
};

#endif // TAG_H
