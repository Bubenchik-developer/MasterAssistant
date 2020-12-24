#ifndef VIEWWIDGET_H
#define VIEWWIDGET_H
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QString>
#include <QLabel>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QShortcut>
#include <QKeyEvent>
#include <QDebug>
#include <QFileDialog>
#include <QPixmap>
#include <QPixmap>
#include <QPixmap>
class ViewWidget : public QWidget
{
    Q_OBJECT
public:
    ViewWidget();
    void setImage(QString path);
    void setNipSize(int w, int h) {
        NIP_W = w;
        NIP_H = h;
    }

    void moveBy(int dx, int dy) {
        if( ((m_x + dx) >= 0 && ((m_x + NIP_W + dx) <= m_pix.width())) &&
                ((m_y + dy >= 0) && ((m_y + NIP_H + dy) <= m_pix.height()) ))
        {
            m_x += dx;
            m_y += dy;
            grView->fitInView(m_x, m_y, NIP_W, NIP_H);
        }
    }

    QGraphicsScene* getScene() {
        return grScene;
    }

    int X() { return m_x; }
    int Y() { return m_y; }

    QPixmap getPixmap () { return m_pix; }

private:
    QGraphicsView * grView;
    QGraphicsScene * grScene;

    int NIP_W, NIP_H;
    int m_x, m_y;

    QString m_imagePath;
    QGraphicsPixmapItem *m_pixItem;
    QPixmap m_pix;
};

#endif // VIEWWIDGET_H
