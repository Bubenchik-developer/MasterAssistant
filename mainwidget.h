#ifndef MAINWIDGET_H
#define MAINWIDGET_H
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QString>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QPixmap>
#include "controlwidget.h"
#include "QCheckBox"
class MainWidget : public QWidget
{
    Q_OBJECT
public:
    MainWidget();

private:

    QGridLayout * grid;
    QPushButton * pbNew;
    QPushButton * pbOpen;
    ControlWidget * cw;
    ViewWidget * vw;
    QString m_filename;
    QLineEdit *leNipWidth;
    QLineEdit *leNipHeight;
    QLineEdit *leSquareSize;
    QWidget* AskingW;

    bool m_fullscreen;
private slots:
    void openImage();
    void openImage2();
    void setFullscreen(bool toogle);
};

#endif // MAINWIDGET_H
