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
        folder = 0,
        file = 1,
        undefined = 2
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
    ModelEntity (const ModelEntity  &copy):
        mName(copy.mName),
        mDateModified(copy.mDateModified),
        mType(copy.mType),
        mSize(copy.mSize)
    {}

    QString mName;
    QDateTime mDateModified;
    Type mType;
    double mSize;
};

class ListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ListModel(QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    virtual bool insertRows(int row, int count, const QModelIndex &parent) override;

private:
    std::vector<ModelEntity> mData;
};

#endif // LISTMODEL_H
