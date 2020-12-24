#include "saveandopenwidget.h"
#include <fcntl.h>
#include <QDebug>

//SaveAndOpenWidget::SaveAndOpenWidget()
//{
//    QGridLayout * grid;
//    QPushButton * pbOpenSession;
////    connect(pbOpenSession);
////    grid->addWidget()
//}


//void SaveAndOpenWidget::readFile(QString filename)
//{
//    filename = "KDE34.png";
//    int fd = open(filename.toStdString().c_str(), O_RDONLY);
//    size_t size = lseek(fd, 0, SEEK_END);
//    uchar *buf = new uchar[size];
//    qDebug() << pread(fd, buf, size, 0);
//    ::close(fd);

//    //Файл прочитан, заполняем структуру. Пока только картинка

//    readImage(buf, size);



//}

//void SaveAndOpenWidget::readImage(uchar* buf, size_t size)
//{
//    QByteArray ar;
//    ar = QByteArray::fromRawData((const char*)buf, size);

//    QImage img;
//    img.loadFromData(ar);
//    QLabel * label = new QLabel();
//    label->setFixedSize(100,100);
//    label->setPixmap(QPixmap::fromImage(img));
//    label->show();
//}
