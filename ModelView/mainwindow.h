#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractItemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setModel(QAbstractItemModel *model);

signals:
    void itemDoubleClicked(const QModelIndex &index);
    void addItemClicked(QString name, int age);

private:
    void onAddItem();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
