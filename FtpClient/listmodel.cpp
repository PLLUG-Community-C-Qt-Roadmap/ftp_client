#include "listmodel.h"

ListModel::ListModel(const std::vector<ModelEntity> &initialData, QObject *parent) : QAbstractListModel(parent)
{
   refreshData(initialData);
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
        return ModelEntity::getStrType(mData[rowIndex].mType);
    case Size:
    {
        if(mData[rowIndex].mType == ModelEntity::Type::Folder)
        {
            return QString();
        }
        else
        {
            return mData[rowIndex].mSize;
        }
    }
    default:
        break;
    }

    return QVariant();
}

QVariant ListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
            return QVariant();

        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case 0:
                return tr("Name");
            case 1:
                return tr("Date Modified");
            case 2:
                return tr("Type");
            case 3:
                return tr("Size");
            }
        }
        return QVariant();
}

void ListModel::refreshData(const std::vector<ModelEntity> &newData)
{
    mData = newData;
    emit dataChanged(this->index(0, 0), this->index(rowCount(QModelIndex())-1, 4));
}

