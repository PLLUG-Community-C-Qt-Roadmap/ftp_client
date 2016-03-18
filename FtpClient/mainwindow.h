#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QAbstractSocket>
#include <QDebug>
#include <QPushButton>
#include <QAbstractItemModel>
#include <QTreeView>

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

    QPushButton* getUploadPushButton();
    QPushButton* getConnectPushButton();
    QTreeView* getFilesView();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
