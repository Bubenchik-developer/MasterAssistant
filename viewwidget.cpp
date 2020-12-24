#include "viewwidget.h"
#include <QApplication>

ViewWidget::ViewWidget() : NIP_W(1280), NIP_H(720)
{
    m_x = 0;
    m_y = 0;
    setWindowTitle("View");
    grView = new QGraphicsView(this);
//    grView->setFixedSize(NIP_W,NIP_H);
    grScene = new QGraphicsScene();
    grView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    grView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    grView->setOptimizationFlags(QGraphicsView::DontSavePainterState | QGraphicsView::DontAdjustForAntialiasing | QGraphicsView::IndirectPainting);
    grView->setScene(grScene);

    QShortcut * f11 = new QShortcut(Qt::Key_F11, this);
    connect(f11, SIGNAL(activated()), this, SLOT(showFullScreen()));
    QShortcut * f12 = new QShortcut(Qt::Key_F12, this);
    connect(f12, SIGNAL(activated()), this, SLOT(showNormal()));

}

void ViewWidget::setImage(QString path)
{
    m_pix.load(path);
    m_pixItem = new QGraphicsPixmapItem();
    m_pixItem->setPixmap(m_pix);
    grScene->addItem(m_pixItem);
    grView->setMinimumSize(NIP_W, NIP_H);
    m_pixItem->setPos(0,0);
    //updateImage();
}


//void ViewWidget::updateImage() {
//}


