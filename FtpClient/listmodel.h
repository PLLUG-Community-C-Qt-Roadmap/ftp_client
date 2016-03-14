#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <vector>
#include <QString>
#include <QDateTime>

struct ModelEntity
{
    enum Type
    {
        Undefined = 0,
        Folder = 1,
        File = 2,
    };

    ModelEntity ():
        mName(),
        mDateModified(),
        mType(),
        mSize()
    {}
    ModelEntity (const QString name, const QDateTime dateModified, const Type type, const double size):
        mName(name),
        mDateModified(dateModified),
        mType(type),
        mSize(size)
    {}

    QString mName;
    QDateTime mDateModified;
    Type mType;
    double mSize;
};

class IListModel
{
public:
    virtual void refreshData(const std::vector<ModelEntity> &ent) = 0;
};

class ListModel : public QAbstractListModel, public IListModel
{
    Q_OBJECT
public:
    explicit ListModel(QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;

    virtual void refreshData(const std::vector<ModelEntity> &newData) override;

    enum Column
    {
        Name = 0,
        DateModified = 1,
        Type = 2,
        Size = 3
    };

private:
    std::vector<ModelEntity> mData;
};

#endif // LISTMODEL_H
