#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QAbstractSocket>
#include <QDebug>
#include <QModelIndex>

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

    void showConnectButton();
    void hideConnectButton();

    void showUploadButton();
    void hideUploadButton();

    void showFilesView();
    void hideFilesView();

signals:
    void connectButtonClicked();
    void uploadButtonClicked();
    void viewItemClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
