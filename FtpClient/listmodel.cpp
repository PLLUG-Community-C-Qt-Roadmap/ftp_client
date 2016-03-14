#include "listmodel.h"

ListModel::ListModel(QObject *parent) : QAbstractListModel(parent)
{
   // Sample data
   mData.push_back(ModelEntity("My Pictures", QDateTime::currentDateTime(), ModelEntity::Type::folder, 1037));
   mData.push_back(ModelEntity("image.jpg", QDateTime::currentDateTime(), ModelEntity::Type::file, 876));
   mData.push_back(ModelEntity("text.txt", QDateTime::currentDateTime(), ModelEntity::Type::file, 103));
   mData.push_back(ModelEntity("image2.png", QDateTime::currentDateTime(), ModelEntity::Type::file, 246));
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
    size_t columnIndex = static_cast<size_t>(index.column());

    switch(columnIndex)
    {
    case 0:
        return mData[rowIndex].mName;
    case 1:
        return mData[rowIndex].mDateModified;
    case 2:
        return mData[rowIndex].mType;
    case 3:
        return mData[rowIndex].mSize;
    default:
        break;
    }

    return QVariant();
}

bool ListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role != Qt::DisplayRole)
        return false;

    size_t rowIndex = static_cast<size_t>(index.row());
    size_t columnIndex = static_cast<size_t>(index.column());

    switch(columnIndex)
    {
    case 0:
        mData[rowIndex].mName = value.toString(); break;
    case 1:
        mData[rowIndex].mDateModified = value.toDateTime(); break;
    case 2:
        if(value.toString() == "file")
        {
             mData[rowIndex].mType = ModelEntity::Type::file; break;
        }
        else if (value.toString() == "folder")
        {
             mData[rowIndex].mType = ModelEntity::Type::folder; break;
        }
        else
        {
             mData[rowIndex].mType = ModelEntity::Type::undefined; break;
        }
    case 3:
        mData[rowIndex].mSize = value.toDouble(); break;
    default:
        break;
    }

    dataChanged(index, index, QVector<int>() << role);

    return true;
}

bool ListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, rowCount(parent), rowCount(parent) + 1);
    mData.push_back(ModelEntity("", QDateTime(), ModelEntity::Type::undefined, 0));
    endInsertRows();
    return true;
}

