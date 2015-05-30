#include "myserver.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    startBtn = new QPushButton(this);
    startBtn->setText("Connect");

//    progressBar = new QProgressBar(this);

    layout = new QGridLayout;
    layout->addWidget(startBtn, 0, 0);
//    layout->addWidget(progressBar, 1, 0);

    connect(startBtn, &QPushButton::clicked, this, &MainWindow::on_starting_clicked);

    setCentralWidget (new QWidget (this));
    centralWidget()->setLayout(layout);
}

MainWindow::~MainWindow()
{
    server_status=0;
}

void MainWindow::on_starting_clicked()
{
    startBtn->setText("Connecting...");
    tcpServer = new QTcpServer(this);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    if (!tcpServer->listen(QHostAddress::Any, 33333) && server_status==0) {
        qDebug() <<  QObject::tr("Unable to start the server: %1.").arg(tcpServer->errorString());
    } else {
        server_status=1;
        qDebug() << QString::fromUtf8("Сервер запущен!");
        startBtn->setText("Running");
    }
}

void MainWindow::acceptConnection()
{

    qDebug() << QString::fromUtf8("У нас новое соединение!");
    tcpServerConnection = tcpServer->nextPendingConnection();
    connect(tcpServerConnection,SIGNAL(readyRead()),this, SLOT(slotReadClient()));
//    tcpServer->close();

    QDir::setCurrent("/Users/vlad/Desktop/");
    QString fileName();
    QString fileSize;

}


void MainWindow::slotReadClient()
{
    QDataStream in(tcpServerConnection);
    QByteArray tmpByteArray;

    if (!isInfoGot) {
        isInfoGot = true;
        in >> fileName;
        qDebug() << fileName;
        in >> fileSize;
        qDebug() << fileSize;
    }
    QFile loadedFile(fileName);

    if (loadedFile.open(QIODevice::Append))
    {
        while (tcpServerConnection->bytesAvailable())
        {
            qDebug() << "bytesAvailable:" << tcpServerConnection->bytesAvailable();
            in >> tmpByteArray;
            QString some(tmpByteArray);
            if (some=="#END") {
                isInfoGot = false;
                QFileInfo fileInfo(loadedFile);
                qDebug() << "File size:" << loadedFile.size();
                if (loadedFile.size() == fileSize.toInt()) {
                    qDebug() << "Success upload";
                    sendMessage(1);
                } else {
                    qDebug() << "Fail upload";
                    sendMessage(0);
                }

                break;
            }
            loadedFile.write(tmpByteArray);
        }
        loadedFile.close();
    }
}

// TODO: add hostAddr, hostPort to the function
void MainWindow::sendMessage(int code) {
    QTcpSocket *sock = new QTcpSocket(this);
    sock->connectToHost("127.0.0.1", 44444);
//    QDataStream out(sock);
//    out << code << "hello";
    sock->write(QString::number(code).toUtf8().constData());
    sock->write("hren");
}
