#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H
#include <QWidget>
#include <QObject>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include "viewwidget.h"
#include <QGraphicsSceneMouseEvent>
#include <QComboBox>


class ControlGraphicsScene;

class ControlWidget : public QWidget
{
    Q_OBJECT
public:
    ControlWidget();
    ControlWidget(ViewWidget * view);
    void setNipSize(int w, int h) {
        NIP_W = w;
        NIP_H = h;
    }

    void setSquareSize(int size) {
        SQ_Size = size;
    }

    int getSquareSize() {
        return SQ_Size;
    }

    enum ActionPhase {
      CW_POINT,
      CW_DRAW_LINE
    };

    void setImage(QString path);

    QColor getColor() { return m_checkedColor; }

    ActionPhase phase() {
        return m_phase;
    }

    void endPhase() {
        m_phase = CW_POINT;
        enableButtons(true);
        foreach(QPushButton* pb, m_LineButtons) pb->setChecked(false);
    }

    int feet() {
        return m_feet;
    }

    QComboBox * cbTracking;
private:
    ViewWidget * m_view;
    QGraphicsView * cwView;
    ControlGraphicsScene * cwScene;
    QGraphicsPixmapItem *m_pixItem;

    QPushButton *makeColorButton(QColor color);

    //colors
    QPushButton *   pbRed, *pbOrange, *pbYellow, *pbGreen, *pbGay, *pbBlue, *pbViolet;
    //line
    QPushButton *   pbLine30, *pbLine60, *pbLine120, *pbLineCustom;
    QLineEdit *     leLineCustom;
    //cone
    QPushButton *   *pbCone15, *pbCone30, *pbCone60, *pbConeCustom;
    QLineEdit *     leConeCustom;
    //cube
    QPushButton *   *pbCube15, *pbCube30, *pbCube60, *pbCubeCustom;
    QLineEdit *     leCubeCustom;
    //sphere or cil
    QPushButton *   *pbSphere15, *pbSphere30, *pbSphere60, *pbSphereCustom;
    QLineEdit *     leSphereCustom;
    double scale;


    int NIP_W, NIP_H;
    int SQ_Size;

    QVector<QPixmap*> m_pics;
    QPixmap *m_curPix;
    QVector<QPushButton*> m_ColorButtons, m_LineButtons;
    QLabel *coord;
    QColor m_checkedColor;
    int m_feet;

    bool m_ctrl_pressed;


    ActionPhase m_phase;

    QPushButton *makeFeetButton(int feets = 0);
    QPushButton *makeLineButton(int feets = 0);
    void enableButtons(bool enable);
private slots:
    void colorChanged();

    void drawLine();
};

class ControlGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    ControlGraphicsScene(ViewWidget * viewWidget, ControlWidget * controlWidget, int nipWidth, int nip_height, int square_size);

private:

    ViewWidget * m_viewWidget;
    ControlWidget * m_controlWidget;
    QVector<QGraphicsItem*> m_items;
    QGraphicsRectItem * m_viewRect;
    QGraphicsEllipseItem * m_spot;
    QGraphicsEllipseItem * m_spotView;
    QGraphicsEllipseItem * m_viewIntersect;
    QGraphicsEllipseItem * m_start;
    QPoint lineFirstPoint;


    const int NIP_H, NIP_W, SQ_SIZE;
    bool m_ctrl_pressed;
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void keyPressEvent(QKeyEvent * keyEvent) override;
    void keyReleaseEvent(QKeyEvent *ev) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *ev) override;

    void trackLines(QPoint position, int &res_x, int &res_y);
    void trackSquareCenter(QPoint position, int &x, int &y);
    void trackIntersection(QPoint position, int &x, int &y);
    void findSquare(QPoint position, int &x, int &y);
private slots:
    void tick();
};



#endif // CONTROLWIDGET_H
