#include "drive.h"
#include "tagtreemodel.h"
#include <iostream>
#include <QDir>
#include <QImage>
#include <QDateTime>

Drive::Drive(QString dir): columns_(4)
{
    // Initializes the drive object's private member,
    // the directory name
    this->directory_ = new QDir(dir);
    this->results_ = this->directory_->entryInfoList(QDir::Files);

    this->tagTree_ = new TagTreeModel;

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

bool Drive::load(QString file) {
    // This function looks for the HDDOrganizer index file
    // in the root of the directory "Drive"
    //
    // It returns false if the file doesn't exist, is corrupt,
    // is not the correct format, etc...
    //
    // This is where the XML parsing code will go
    return false;
}

bool Drive::save(QString file) {
    // This function writes the HDDOrganizer index file to disk
    //
    // It returns false if there are not the right permissions,
    // or if the file already exists.
    //
    // This function translates the underlying object in to XML
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
    this->results_ = this->tagTree_->computeResult(tags);
    emit(doneCalculating());
    return true;
}
//bool compute()
