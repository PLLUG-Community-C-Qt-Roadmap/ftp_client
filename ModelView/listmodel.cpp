#include "listmodel.h"

ListModel::ListModel(QObject *parent) : QAbstractListModel(parent)
{
    for(int i = 0; i < 4; ++i)
    {
        _data.emplace_back(QString("Name: %1").arg(i), rand() % 25 + 18);
    }
}

int ListModel::rowCount(const QModelIndex &parent) const
{
    return _data.size();
}

int ListModel::columnCount(const QModelIndex &parent) const
{
    return 2;
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
        return _data[rowIndex]._name;
    case 1:
        return _data[rowIndex]._age;
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
        _data[rowIndex]._name = value.toString(); break;
    case 1:
        _data[rowIndex]._age = value.toInt(); break;
    default:
        return false;
    }

    dataChanged(index, index, QVector<int>() << role);

    return true;
}

bool ListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, rowCount(parent), rowCount(parent) + 1);
    _data.emplace_back("", -1);
    endInsertRows();
    return true;
}
