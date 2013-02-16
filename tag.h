#ifndef TAG_H
#define TAG_H

#include <vector>
#include <QFileInfo>
#include <QUrl>
#include <QSet>

class Tag
{
public:
    Tag(const QList<QVariant> &data, Tag *parent = 0);

    Tag* child(int row);
    int row();
    int rowCount() const;
    int columnCount() const;
    bool addChild(QString tagName);
    bool addFiles(const QList<QUrl> files);
    QVariant data(int column) const;
    bool setData(int column, QVariant value);
    QSet<QString> allFiles();
    Tag* parent();

private:
    QList<Tag*> children_;
    Tag* parent_;
    QSet<QString> files_;
    QList<QVariant> data_;
};

#endif // TAG_H
