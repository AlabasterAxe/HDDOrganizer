#include "drive.h"
#include "tagtreemodel.h"
#include "tag.h"
#include <iostream>
#include <QDir>
#include <QUrl>
#include <QMimeData>
#include <QImage>
#include <QDateTime>
#include <QTextStream>
#include <QDebug>
#include <QDomNode>
#include <QPicture>
#include <QDesktopServices>
#include <QProcess>
#include "MainWindow.h"
#include "ui_mainwindow.h"

Drive::Drive(QString dir, MainWindow *parent): columns_(4),
                                            sortColumn_(0),
                                            sortOrder_(Qt::AscendingOrder),
                                            parent_(parent)
{
    // Initializes the drive object's private member,
    // the directory name
    this->directory_ = new QDir(dir);
    this->results_ = this->directory_->entryInfoList(QDir::Files);

    this->tagTree_ = new TagTreeModel(dir);

    // Insert the 4 columns that represent:
    //   - Name
    //   - Size
    //   - Type
    //   - Date
    this->insertColumns(0,4);
}

Drive::~Drive(){
   delete directory_;
}

bool Drive::load() {
    // This function looks for the HDDOrganizer index file
    // in the root of the directory "Drive"
    //
    // It returns false if the file doesn't exist, is corrupt,
    // is not the correct format, etc...

    QString file(this->directory_->absolutePath() + QString("/HDDO.xml"));
    QFile loadFile(file);

    bool result = false;
    if (loadFile.exists())
        result = this->tagTree_->setContent(&loadFile);

    return result;
}

void Drive::reset()
{
    this->results_ = this->directory_->entryInfoList(QDir::Files);
    this->sort(sortColumn_,sortOrder_);
}

void Drive::preview(const QModelIndex & index)
{
    QFileInfo file = results_[index.row()];
    QString filetype = file.suffix().toLower();
    QLabel* label = this->parent_->ui->label;

    if (filetype == "jpg") {
        QPixmap pic(file.absoluteFilePath());
        if (pic.width() > pic.height())
            label->setPixmap(pic.scaledToWidth(label->width()));
        else
            label->setPixmap(pic.scaledToHeight(label->height()));
        //this->parent_->ui->label->setPixmap(*pic);
    }
}

void Drive::open(const QModelIndex &index, const QString &program)
{
    QString fileName = this->results_[index.row()].absoluteFilePath();
    if (program.isEmpty()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
    } else {
        QStringList args(fileName);
        QProcess::startDetached(program, args);
    }
}

bool Drive::save() {

    QString file(this->directory_->absolutePath() + QString("/HDDO.xml"));
    // This function writes the HDDOrganizer index file to disk
    //
    // It returns false if there are not the right permissions,
    // or if the file already exists.
    //
    // This function translates the underlying object in to XML

    QString data = this->tagTree_->stringify();
    QFile saveFile(file);

    if (saveFile.open(QIODevice::WriteOnly)) {
        QTextStream out(&saveFile);
        out << data;
        return true;
    }

    return false;
}

bool Drive::overwrite(QString file) {
    // This function is the same as save but will not return false
    // if the file already exists. It will just overwrite the
    // file.
    return false;
}

bool Drive::index() {
    // This function will check for new files and if files have
    // been deleted.
    //
    // The function retuns false if the directory cannot be opened.
    //
    // TODO: Figure out what happens if the name of the file changes
    // outside of the program
    //
    // TODO: Rename this function. Calling this function index is
    // confusing because there is a function inherited from
    // QAbstractTableModel called index. It has a different signature
    // so it compiles but it would be best if there weren't overloaded
    // functions that do two completely different things.
    QFileInfoList files = this->directory_->entryInfoList(QDir::Files);


    std::cerr << "Indexed" << std::endl;

    return false;
}

int Drive::columnCount(const QModelIndex &parent) const {
    // eventually this will return the number of columns each row has
    // likely 3 or 4

    // Name:
    // Date Modified:
    // Size:
    // Type: (maybe)

    return this->columns_;
}

int Drive::rowCount(const QModelIndex &parent) const {
    // similar to column count, this functions indicates the
    // number of results for a given expression

    // this number could be as many as 1000 or even higher
    //qDebug() << results_.size();
    return results_.size();
}

QVariant Drive::data(const QModelIndex &index, int role) const {
    QFileInfo file = results_.at(index.row());

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case 0: return file.fileName();
                    break;
            case 1: return file.size();
                    break;
            case 2: return file.suffix();
                    break;
            case 3: return file.lastModified();
                    break;
        }
    } else {
        return QVariant();
    }
    return QVariant();
}

QVariant Drive::headerData ( int section,
                             Qt::Orientation orientation,
                             int role) const {
    switch (role) {
        case Qt::TextAlignmentRole:
            return Qt::AlignLeft;
    }

    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QAbstractItemModel::headerData(section, orientation, role);

    QString returnValue;
    switch (section) {
    case 0: returnValue = tr("Name");
            break;
    case 1: returnValue = tr("Size");
            break;
    case 2: returnValue =
#ifdef Q_OS_MAC
                   tr("Kind", "Match OS X Finder");
#else
                   tr("Type", "All other platforms");
#endif
           break;
    // Windows   - Type
    // OS X      - Kind
    // Konqueror - File Type
    // Nautilus  - Type
    case 3: returnValue = tr("Date Modified");
            break;
    default: return QVariant();
    }
    return returnValue;
}

bool Drive::recalculate(QModelIndexList tags) {
    if(this->results_.size() > 0) {
        beginRemoveRows(QModelIndex(), 0, this->results_.size()-1);
        this->results_ = QFileInfoList();
        endRemoveRows();
    }

    QFileInfoList newResults;
    if(tags.size() == 0) {
        this->parent_->setExpressionLabel(tr("All Files"));
        newResults = this->directory_->entryInfoList(QDir::Files);
    } else {
        QString expressionLabel = "";
        for (int i = 0; i < tags.size(); i += TAG_TREE_COLUMNS) {
             Tag* tag = tagTree_->getIndexTag(tags[i]);
             expressionLabel.append(tag->data(0).toString());

             if  (i < tags.size() - TAG_TREE_COLUMNS)
                 expressionLabel.append(tr(" &#x2229; "));
        }
        this->parent_->setExpressionLabel(expressionLabel);
        newResults = this->tagTree_->computeResult(tags);
    }

    if(newResults.size() > 0) {
        beginInsertRows(QModelIndex(), 0, newResults.size()-1);
        this->results_ = newResults;
        endInsertRows();
    }

    this->sort(this->sortColumn_,this->sortOrder_);
    emit(doneCalculating());
    return true;
}

bool filenameLessthan(const QFileInfo& file1, const QFileInfo& file2){
    return file1.fileName() < file2.fileName();
}

bool sizeLessthan(const QFileInfo& file1, const QFileInfo& file2){
    return file1.size() < file2.size();
}

bool typeLessthan(const QFileInfo& file1, const QFileInfo& file2){
    return file1.completeSuffix() < file2.completeSuffix();
}

bool dateLessthan(const QFileInfo& file1, const QFileInfo& file2){
    return file1.lastModified() < file2.lastModified();
}

bool filenameGreaterthan(const QFileInfo& file1, const QFileInfo& file2){
    return file1.fileName() > file2.fileName();
}

bool sizeGreaterthan(const QFileInfo& file1, const QFileInfo& file2){
    return file1.size() > file2.size();
}

bool typeGreaterthan(const QFileInfo& file1, const QFileInfo& file2){
    return file1.completeSuffix() > file2.completeSuffix();
}

bool dateGreaterthan(const QFileInfo& file1, const QFileInfo& file2){
    return file1.lastModified() > file2.lastModified();
}

void Drive::sort(int column, Qt::SortOrder order)
{
    this->sortColumn_ = column;
    this->sortOrder_ = order;
    if (order == Qt::AscendingOrder) {
        switch (column) {
        case 0:
            qSort(results_.begin(), results_.end(), filenameLessthan);
            return;
        case 1:
            qSort(results_.begin(), results_.end(), sizeLessthan);
            return;
        case 2:
            qSort(results_.begin(), results_.end(), typeLessthan);
            return;
        case 3:
            qSort(results_.begin(), results_.end(), dateLessthan);
            return;
        }
    } else {
        switch (column) {
        case 0:
            qSort(results_.begin(), results_.end(), filenameGreaterthan);
            return;
        case 1:
            qSort(results_.begin(), results_.end(), sizeGreaterthan);
            return;
        case 2:
            qSort(results_.begin(), results_.end(), typeGreaterthan);
            return;
        case 3:
            qSort(results_.begin(), results_.end(), dateGreaterthan);
            return;
        }
    }
}

Qt::ItemFlags Drive::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
}

QMimeData* Drive::mimeData(const QModelIndexList &indexes) const
{
    QList<QUrl> urls;
    QList<QModelIndex>::const_iterator it = indexes.begin();
    for (; it != indexes.end(); ++it)
        if ((*it).column() == 0)
            urls << QUrl::fromLocalFile(this->results_[it->row()].absoluteFilePath());
    QMimeData *data = new QMimeData();
    data->setUrls(urls);
    return data;
}
//bool compute()
