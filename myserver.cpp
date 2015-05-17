#include "myserver.h"

int Z = 0;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    startBtn = new QPushButton(this);
    startBtn->setText("Connect");

    progressBar = new QProgressBar(this);

    layout = new QGridLayout;
    layout->addWidget(startBtn, 0, 0);
    layout->addWidget(progressBar, 1, 0);

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
    QString fileName;
    QString fileSize;

}


void MainWindow::slotReadClient()
{
    QDataStream in(tcpServerConnection);
    QByteArray z;

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
            in >> z;
            loadedFile.write(z);

        }
        loadedFile.close();
    }


}
