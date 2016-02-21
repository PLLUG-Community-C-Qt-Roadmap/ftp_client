#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    void doConnect();

    ~MainWindow();

public slots:
    void slotConnected();
    void slotError(QAbstractSocket::SocketError err);

private:
    void onChangeDir();
    void onDownloadFile();
    void onUploadFile();

private:
     void init();

private:
    QTcpSocket * socket;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
