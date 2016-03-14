#include "listmodel.h"

ListModel::ListModel(QObject *parent) : QAbstractListModel(parent)
{
   // Sample data
   mData.push_back(ModelEntity("My Pictures", QDateTime::currentDateTime(), ModelEntity::Type::Folder, 1037));
   mData.push_back(ModelEntity("image.jpg", QDateTime::currentDateTime(), ModelEntity::Type::File, 876));
   mData.push_back(ModelEntity("text.txt", QDateTime::currentDateTime(), ModelEntity::Type::File, 103));
   mData.push_back(ModelEntity("image2.png", QDateTime::currentDateTime(), ModelEntity::Type::File, 246));
}

int ListModel::rowCount(const QModelIndex &parent) const
{
    return mData.size();
}

int ListModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

QVariant ListModel::data(const QModelIndex &index, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();

    size_t rowIndex = static_cast<size_t>(index.row());
    Column columnIndex = static_cast<Column>(index.column());

    switch(columnIndex)
    {
    case Name:
        return mData[rowIndex].mName;
    case DateModified:
        return mData[rowIndex].mDateModified;
    case Type:
        return mData[rowIndex].mType;
    case Size:
        return mData[rowIndex].mSize;
    default:
        break;
    }

    return QVariant();
}

void ListModel::refreshData(const std::vector<ModelEntity> &newData)
{
    mData = newData;
    emit dataChanged(this->index(0, 0), this->index(rowCount()-1, 4));
}

