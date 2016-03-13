#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <vector>

struct NameAge
{
    NameAge(QString name, int age) : _name(name), _age(age) {}

    QString _name;
    int _age;

};

struct ITest
{
    virtual void testMethod() = 0;
};

class ListModel : public QAbstractListModel, public ITest
{
    Q_OBJECT
public:
    explicit ListModel(QObject *parent = nullptr);

    virtual void testMethod() override { qDebug("sadasd"); }

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    virtual bool insertRows(int row, int count, const QModelIndex &parent) override;

private:
    std::vector<NameAge> _data;

};

#endif // LISTMODEL_H
