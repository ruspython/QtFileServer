#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QTcpSocket>
#include <QObject>
#include <QByteArray>
#include <QDebug>
#include <QPushButton>
#include <QGridLayout>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QIODevice>
#include <QProgressBar>

namespace Ui {
    class MainWindow;
}

class QTcpServer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_starting_clicked();
    void on_stoping_clicked();
    void newuser();
    void slotReadClient();

private:
    QTcpServer *tcpServer;
    QProgressBar *progressBar;
    QPushButton *startBtn;
    QGridLayout *layout;
    int server_status;
    QMap<int,QTcpSocket *> SClients;
};

#endif // MAINWINDOW_H
