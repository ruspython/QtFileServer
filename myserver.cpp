#include "myserver.h"

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
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newuser()));
    if (!tcpServer->listen(QHostAddress::Any, 33333) && server_status==0) {
        qDebug() <<  QObject::tr("Unable to start the server: %1.").arg(tcpServer->errorString());
    } else {
        server_status=1;
        qDebug() << QString::fromUtf8("Сервер запущен!");
        startBtn->setText("Running");
    }
}

void MainWindow::on_stoping_clicked()
{
    if(server_status==1){
        foreach(int i,SClients.keys()){
            QTextStream os(SClients[i]);
            os.setAutoDetectUnicode(true);
            os << QDateTime::currentDateTime().toString() << "\n";
            SClients[i]->close();
            SClients.remove(i);
        }
        tcpServer->close();
        qDebug() << QString::fromUtf8("Сервер остановлен!");
        server_status=0;
    }
}


void MainWindow::newuser()
{
    if(server_status==1){
        qDebug() << QString::fromUtf8("У нас новое соединение!");
        QTcpSocket* clientSocket=tcpServer->nextPendingConnection();
        int idusersocs=clientSocket->socketDescriptor();
        SClients[idusersocs]=clientSocket;
        connect(SClients[idusersocs],SIGNAL(readyRead()),this, SLOT(slotReadClient()));
    }
}

void MainWindow::slotReadClient()
{
//    QTcpSocket* clientSocket = (QTcpSocket*)sender();
//    int idusersocs=clientSocket->socketDescriptor();
//    QTextStream os(clientSocket);
//    os.setAutoDetectUnicode(true);
//    os << "HTTP/1.0 200 Ok\r\n"
//          "Content-Type: text/html; charset=\"utf-8\"\r\n"
//          "\r\n";
//    QFile htmlFile(":templates/index.html");
//    htmlFile.open(QIODevice::ReadOnly);
//    QTextStream html(&htmlFile);
//    QString htmlText = html.readAll();
//    qDebug() << htmlText;
//    os << htmlText;

//    clientSocket->close();
//    SClients.remove(idusersocs);

    QTcpSocket *sender = (QTcpSocket*) this->sender();

    QDataStream in(sender);

    QString fName;
    QString size;

    in >> fName;
    in >> size;

    qDebug() << "file name:" << fName;
    qDebug() << "size" << size;

    progressBar->setMaximum(size.toInt());

    QFile newFile("/Users/vlad/Desktop/"+fName);

    if (newFile.open(QIODevice::ReadWrite))
    {
        int totalSize = 0;
        while(sender->bytesAvailable())
        {
             QByteArray z;
             in >> z;
             newFile.write(z);
             totalSize += z.size();

             progressBar->setValue(totalSize);

             qDebug () << "Received:" << z.size();
        }
        qDebug() << totalSize;
        qDebug() << size.toInt();

        if (totalSize != size.toInt()) {
            qDebug() << "download fail";
        }
        newFile.close();

    }
}
