#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QString>
#include <QDateTime>
#include <vector>

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
    ModelEntity (const QString &name, const QDateTime &dateModified, Type type, double size):
        mName(name),
        mDateModified(dateModified),
        mType(type),
        mSize(size)
    {}
    ModelEntity(const QStringList &properties)
    {
        auto it = properties.begin();

        mName = *it;
        mDateModified = QDateTime::fromString(*(++it), "ddd MMM d HH:mm:ss yyyy");
        mType = getEnumType(*(++it));
        mSize = (*(++it)).toInt();
    }

    static ModelEntity::Type getEnumType(const QString &type)
    {
        if(type == "File")
        {
            return Type::File;
        }
        else if(type == "Folder")
        {
            return Type::Folder;
        }
        else return Type::Undefined;
    }

    static QString getStrType(Type type)
    {
        switch(type)
        {
        case 1: return "Folder";
        case 2: return "File";
        default: return "Undefined";
        }
    }

    QString mName;
    QDateTime mDateModified;
    Type mType;
    int mSize;
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
    explicit ListModel(const std::vector<ModelEntity> &initialData, QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

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
