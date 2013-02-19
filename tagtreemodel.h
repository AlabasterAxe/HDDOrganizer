#ifndef TAGTREEMODEL_H
#define TAGTREEMODEL_H

#include <QAbstractItemModel>
#include <QFileInfoList>

class Tag;
class QDomDocument;
class QDomElement;

class TagTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit TagTreeModel(QString name, QObject *parent = 0);
    ~TagTreeModel();
    QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex & parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex & child) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    Qt::ItemFlags TagTreeModel::flags(const QModelIndex &index) const;
    bool insertTag(const QString tagName, const QModelIndex& parent = QModelIndex());
    Tag* getIndexTag(const QModelIndex& index) const;
    bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent);
    QFileInfoList computeResult(QModelIndexList tags) const;
    QString stringify() const;
    bool setContent(QIODevice * dev);

    void translate(QDomElement *domNode, Tag *tagNode);
private:
    Tag* root_;
    QDomDocument* domTree_;
    QDomElement* domTreeRoot_;

signals:
    
public slots:

    
};

#endif // TAGTREEMODEL_H