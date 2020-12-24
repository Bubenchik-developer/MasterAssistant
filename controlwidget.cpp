#include "controlwidget.h"
#include <QGridLayout>
#include <QTimer>
#include <QLineEdit>
#include <QValidator>
#include <QtMath>

ControlWidget::ControlWidget(ViewWidget *view) :
    m_view(view),
    m_ctrl_pressed(false),
    m_phase(CW_POINT)
{
    scale = 0.5;
    QGridLayout *grid = new QGridLayout(this);
    cwView = new QGraphicsView();
    cwView->setMouseTracking(true);

    grid->addWidget(cwView, 0, 0);

    QWidget * zoneWidget = new QWidget();
    QGridLayout * zoneGrid = new QGridLayout(zoneWidget);

    pbRed = makeColorButton(Qt::red);
    zoneGrid->addWidget(pbRed,0,0);
    pbRed->setText("  ");
    pbRed->setChecked(true);
    m_checkedColor = Qt::red;
    pbOrange = makeColorButton(Qt::darkGray);
    zoneGrid->addWidget(pbOrange,0,1);
    pbYellow = makeColorButton(Qt::yellow);
    zoneGrid->addWidget(pbYellow,0,2);
    pbGreen = makeColorButton(Qt::green);
    zoneGrid->addWidget(pbGreen,0,3);
    pbGay = makeColorButton(Qt::cyan);
    zoneGrid->addWidget(pbGay,0,4);
    pbBlue = makeColorButton(Qt::blue);
    zoneGrid->addWidget(pbBlue,0,5);
    pbViolet = makeColorButton(Qt::magenta);
    zoneGrid->addWidget(pbViolet,0,6);

    QLabel *lLine = new QLabel("Line");

    pbLine30 = makeLineButton(30);
    pbLine60 = makeLineButton(60);
    pbLine120 = makeLineButton(120);
    pbLineCustom = makeLineButton();

    leLineCustom = new QLineEdit("");
    leLineCustom->setValidator(new QIntValidator(0, 999));
    leLineCustom->setFixedWidth(40);

    zoneGrid->addWidget(lLine, 1, 0);
    zoneGrid->addWidget(pbLine30, 1, 1);
    zoneGrid->addWidget(pbLine60, 1, 2);
    zoneGrid->addWidget(pbLine120, 1, 3);
    zoneGrid->addWidget(leLineCustom, 1, 4, 1, 2);
    zoneGrid->addWidget(pbLineCustom, 1, 6);
    cbTracking = new QComboBox();
    cbTracking->addItem("Square");
    cbTracking->addItem("Line");
    cbTracking->addItem("Intersect");
    zoneGrid->addWidget(cbTracking);

    grid->addWidget(zoneWidget, 0, 1);


 /*
    pbRed, , , , , , ;

    pbLine30, pbLine60, pbLine120, pbLineCustom;
    leLineCustom;

    pbCone15, pbCone30, pbCone60, pbConeCustom;
    leConeCustom;

    pbCube15, pbCube30, pbCube60, pbCubeCustom;
    leCubeCustom;

    pbSphere15, pbSphere30, pbSphere60, pbSphereCustom;
    leSphereCustom;
*/
    cwView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    cwView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    cwView->

    connect(cwView, SIGNAL(), this, SLOT());

    setMinimumHeight(400);
    setMinimumWidth(600);
    m_pics.clear();

}


QPushButton* ControlWidget::makeColorButton(QColor color)
{
    QPushButton *result = new QPushButton(" ");
    result->setFlat(true);
    result->setFixedSize(20,15);
    result->setCheckable(true);
    {
        QPalette pal = result->palette();
        QBrush br;
        result->setAutoFillBackground(true);
        br.setColor(color);
        br.setStyle(Qt::SolidPattern);
        pal.setBrush(QPalette::Button, br);
        result->setPalette(pal);
    }
    connect(result, SIGNAL(clicked()), this, SLOT(colorChanged()));
    m_ColorButtons.push_back(result);
    return result;
}


void ControlWidget::colorChanged()
{
    foreach(QPushButton *pb, m_ColorButtons) {
        if(pb->isChecked() && pb->text().length() == 1) {
            pb->setText("  ");
            m_checkedColor = pb->palette().brush(QPalette::Button).color();
        } else {
            pb->setText(" ");
            pb->setChecked(false);
        }
    }
}

QPushButton* ControlWidget::makeFeetButton(int feets)
{
    QPushButton * result;
    if (feets == 0) {
        //custom
        result = new QPushButton("*");
        result->setFixedSize(20,15);
        result->setCheckable(true);
    } else {
        result = new QPushButton(QString::number(feets));
        result->setFixedSize(20,15);
        result->setCheckable(true);
    }
    return result;
}

QPushButton* ControlWidget::makeLineButton(int feets )
{
    QPushButton * result = makeFeetButton(feets);
    m_LineButtons.push_back(result);
    connect(result, SIGNAL(clicked()), this, SLOT(drawLine()));
    return result;
}

void ControlWidget::enableButtons(bool enable) {
    foreach(QPushButton *pb, m_LineButtons) pb->setEnabled(enable);
}

void ControlWidget::drawLine()
{
    if(m_phase == CW_POINT) {
        m_phase = CW_DRAW_LINE;
        enableButtons(false);
    }
    foreach(QPushButton *pb, m_LineButtons) {
        if(pb->isChecked()) {
            if(pb->text() == "*") {
                bool ok;
                m_feet = leLineCustom->text().toInt(&ok);
                if(!ok) m_feet = 0;
            } else {
                m_feet = pb->text().toInt();
            }

        }
    }
}


void ControlWidget::setImage(QString path) {
    QPixmap *pix = new QPixmap();
    pix->load(path);
    m_pixItem = new QGraphicsPixmapItem();
    m_pixItem->setPixmap(*pix);
    cwScene = new ControlGraphicsScene(m_view, this, NIP_W, NIP_H, SQ_Size);
    cwView->setScene(cwScene);
    cwScene->addItem(m_pixItem);
    cwView->scale(scale, scale);
    m_curPix = pix;
    m_pics.push_back(pix);
    m_view->setImage(path);
}

///------------------------------- ControlGraphicsScene

ControlGraphicsScene::ControlGraphicsScene(ViewWidget *viewWidget, ControlWidget *controlWidget, int nipWidth, int nip_height, int square_size)
    : m_viewWidget(viewWidget), m_controlWidget(controlWidget), NIP_H(nip_height), NIP_W (nipWidth), SQ_SIZE(square_size), lineFirstPoint(10000, 10000)
{
    m_spot = new QGraphicsEllipseItem();
    m_spotView = new QGraphicsEllipseItem();
    {
        QPen pen;
        pen.setColor(Qt::red);
        pen.setWidth(10);
        pen.setStyle(Qt::SolidLine);
        m_spot->setPen(pen);
        m_spotView->setPen(pen);
    }

    m_viewRect = new QGraphicsRectItem(0,0,nipWidth,nip_height);
    {
        QPen pen;
        pen.setColor(Qt::green);
        pen.setWidth(10);
        pen.setStyle(Qt::DashLine);
        m_viewRect->setPen(pen);
    }

    m_viewIntersect = new QGraphicsEllipseItem();
    m_start = new QGraphicsEllipseItem();

    {
        QPen pen;
        pen.setColor(Qt::red);
        pen.setWidth(10);
        pen.setStyle(Qt::SolidLine);
        m_viewIntersect->setPen(pen);
    }


    m_viewRect->setZValue(10000);
    m_spot->setZValue(10001);
    m_spotView->setZValue(10001);
    m_viewIntersect->setZValue(10000);
    m_start->setZValue(10001);

    addItem(m_viewRect);
    addItem(m_spot);
    addItem(m_viewIntersect);
    m_viewWidget->getScene()->addItem(m_spotView);

    QTimer * timer = new QTimer(this);
    timer->setInterval(100);
    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
    timer->start();

    QShortcut * f11 = new QShortcut(Qt::Key_F11, controlWidget);
    connect(f11, SIGNAL(activated()), controlWidget, SLOT(showFullScreen()));
    QShortcut * f12 = new QShortcut(Qt::Key_F12, controlWidget);
    connect(f12, SIGNAL(activated()), controlWidget, SLOT(showNormal()));
}

void ControlGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    QPoint position = mouseEvent->scenePos().toPoint();
    // Подстраиваемся под клетку
    const int square_size = m_controlWidget->getSquareSize();
    int x = 0, y = 0;

    switch(m_controlWidget->phase()) {
        case ControlWidget::CW_POINT: {
            findSquare(position,x,y);
            QPen p;
            QBrush b;
            b.setColor(Qt::red);
            p.setWidth(10);
            p.setStyle(Qt::SolidLine);
            p.setBrush(b);
            m_spot->setRect(x+square_size/4, y+square_size/4, square_size/2, square_size/2);
            m_spot->setOpacity(1.0);
            m_spotView->setRect(x+square_size/4, y+square_size/4, square_size/2, square_size/2);
            m_spotView->setOpacity(1.0);
            break;
        }
        case ControlWidget::CW_DRAW_LINE: {

            int x = 0, y = 0;
            switch(m_controlWidget->cbTracking->currentIndex())
            {
                case 0:
                    trackSquareCenter(position,x,y);
                break;
                case 1:
                    trackLines(position,x,y);
                break;
                case 2:
                    trackIntersection(position,x,y);
                break;
            }

            if(lineFirstPoint.x() == 10000 && lineFirstPoint.y() == 10000) {
                lineFirstPoint.setX(x);
                lineFirstPoint.setY(y);
                QPen p;
                QBrush b;
                b.setColor(m_controlWidget->getColor());
                p.setWidth(SQ_SIZE/5);
                p.setStyle(Qt::SolidLine);
                p.setColor(Qt::black);
                b.setStyle(Qt::Dense3Pattern);
                p.setBrush(b);
                QGraphicsEllipseItem* startingPoint = new QGraphicsEllipseItem(x-SQ_SIZE/10,y-SQ_SIZE/10,SQ_SIZE/5,SQ_SIZE/5);
                startingPoint->setPen(p);
                startingPoint->setOpacity(0.4);
                addItem(startingPoint);
            } else {
                QPen p;
                QBrush b;
                b.setColor(m_controlWidget->getColor());
                p.setWidth(SQ_SIZE);
                p.setStyle(Qt::SolidLine);
                p.setColor(Qt::black);
                b.setStyle(Qt::Dense3Pattern);
                p.setBrush(b);

                QGraphicsLineItem *line = new QGraphicsLineItem(lineFirstPoint.x(), lineFirstPoint.y(),
                                                                lineFirstPoint.x() + (square_size * (m_controlWidget->feet()/5 - 1)), lineFirstPoint.y());

                line->setOpacity(0.4);
                line->setPen(p);
                QLineF l1 = line->line();
                QLineF l2(lineFirstPoint, QPoint(x,y));
                line->setTransformOriginPoint(QPointF(lineFirstPoint.x(), lineFirstPoint.y()));
                double angle = l2.angleTo(l1);
                qDebug() << angle;
                double koef = 1.0;
                if(angle < 90) {
                    if(angle <= 45)
                        koef = qCos(angle * M_PI/180);
                    else
                        koef = qSin(angle * M_PI/180);
                } else if(angle > 90 && angle < 180) {
                    if(angle <= 135)
                        koef = qSin((180 - angle) * M_PI/180);
                    else
                        koef = qCos((180 - angle) * M_PI/180);
                } else if (angle < 270 && angle > 180) {
                    if(angle <= 225)
                        koef = qCos((angle - 180) * M_PI/180);
                    else
                        koef = qSin((angle - 180) * M_PI/180);
                } else if (angle > 270 && angle < 360) {
                    if(angle <= 315)
                        koef = qSin((360-angle) * M_PI/180);
                    else
                        koef = qCos((360-angle) * M_PI/180);

                }

                switch(m_controlWidget->cbTracking->currentIndex()) {
                    case 0:
                        line->setLine(lineFirstPoint.x() + SQ_SIZE , lineFirstPoint.y(),
                                  (int)(lineFirstPoint.x() + (square_size * (m_controlWidget->feet()/5)) / koef), lineFirstPoint.y());
                    break;
                    case 1:
                        line->setLine(lineFirstPoint.x() + SQ_SIZE/2, lineFirstPoint.y(),
                                  (int)(lineFirstPoint.x() + (square_size * (m_controlWidget->feet()/5) - SQ_SIZE/2) / koef), lineFirstPoint.y());
                    break;
                    case 2:
                        line->setLine(lineFirstPoint.x() + SQ_SIZE/2, lineFirstPoint.y(),
                                  (int)(lineFirstPoint.x() + (square_size * (m_controlWidget->feet()/5) - SQ_SIZE/2) / koef), lineFirstPoint.y());
                    break;

                }
                line->setRotation(angle);
                addItem(line);

                if(mouseEvent->button() & Qt::LeftButton) {
                    QGraphicsLineItem *line = new QGraphicsLineItem(lineFirstPoint.x() + square_size/2, lineFirstPoint.y() + square_size/2, lineFirstPoint.x() + square_size/2 + (square_size * (m_controlWidget->feet()/5 - 1)), lineFirstPoint.y() + square_size/2);
                    QLineF l1 = line->line();
                    QLineF l2(lineFirstPoint, QPoint(x,y));
                    line->setTransformOriginPoint(QPointF(lineFirstPoint.x() + square_size/2, lineFirstPoint.y() + square_size/2));
                    line->setRotation(-l1.angleTo(l2));
                    line->setPen(p);
                    m_viewWidget->getScene()->addItem(line);
                }

                lineFirstPoint.setX(10000);
                lineFirstPoint.setY(10000);
                m_controlWidget->endPhase();
            }
        }
    }
}

void ControlGraphicsScene::keyPressEvent(QKeyEvent *ev) {
    int dx = 0, dy = 0;
    switch(ev->key())
    {
    case Qt::Key_Up:
        dy -= (m_ctrl_pressed ? 1 : SQ_SIZE);
        break;
    case Qt::Key_Down:
        dy += (m_ctrl_pressed ? 1 : SQ_SIZE);
        break;
    case Qt::Key_Left:
        dx -= (m_ctrl_pressed ? 1 : SQ_SIZE);
        break;
    case Qt::Key_Right:
        dx += (m_ctrl_pressed ? 1 : SQ_SIZE);
        break;
    case Qt::Key_Control:
        m_ctrl_pressed = true;
        break;
    }
    qDebug() << dx << dy;
    m_viewWidget->moveBy(dx,dy);
    m_viewRect->setRect(m_viewWidget->X(), m_viewWidget->Y(), NIP_W, NIP_H);
}

void ControlGraphicsScene::keyReleaseEvent(QKeyEvent *ev) {
    switch(ev->key()) {
        case Qt::Key_Control:
            m_ctrl_pressed = false;
        break;
    }
}

void ControlGraphicsScene::trackLines(QPoint position, int &x, int &y)
{
    x = 0, y = 0;
    // находим клетку
    findSquare(position,x,y);
    //находим центр линии ближайший

    int top_x = x + SQ_SIZE/2, top_y = y;
    int left_x = x, left_y = y + SQ_SIZE/2;
    int bot_x = x + SQ_SIZE/2, bot_y = y + SQ_SIZE;
    int right_x = x + SQ_SIZE, right_y = y + SQ_SIZE/2;

    int length_to_top = qSqrt(qPow(qAbs(top_x - position.x()), 2) + qPow(qAbs(top_y - position.y()), 2));
    int length_to_left = qSqrt(qPow(qAbs(left_x - position.x()), 2) + qPow(qAbs(left_y - position.y()), 2));
    int length_to_bot = qSqrt(qPow(qAbs(bot_x - position.x()), 2) + qPow(qAbs(bot_y - position.y()), 2));
    int length_to_right = qSqrt(qPow(qAbs(right_x - position.x()), 2) + qPow(qAbs(right_y - position.y()), 2));

    QVector<int> length;
    length.push_back(length_to_top);
    length.push_back(length_to_right);
    length.push_back(length_to_bot);
    length.push_back(length_to_left);
    qSort(length);
    if(length[0] == length_to_top) {
        x = top_x;
        y = top_y;
    } else if(length[0] == length_to_right) {
        x = right_x;
        y = right_y;
    } else if(length[0] == length_to_bot) {
        x = bot_x;
        y = bot_y;
    } else {
        x = left_x;
        y = left_y;
    }
}

void ControlGraphicsScene::findSquare(QPoint position, int &x, int &y)
{
    for(;;) {
        if(x < position.x() - SQ_SIZE) {
            x+=SQ_SIZE;
            continue;
        }
        if(y < position.y() - SQ_SIZE) {
            y+=SQ_SIZE;
            continue;
        }
        break;
    }
}

void ControlGraphicsScene::trackSquareCenter(QPoint position, int &x, int &y) {
    findSquare(position,x,y);
    x+=SQ_SIZE/2;
    y+=SQ_SIZE/2;
}

void ControlGraphicsScene::trackIntersection(QPoint position, int &x, int &y)
{
    x = 0, y = 0;
    // находим клетку
    findSquare(position,x,y);
    //находим центр линии ближайший

    int left_top_x = x, left_top_y = y; //верхняя левая
    int right_top_x = x + SQ_SIZE, right_top_y = y; //в. правая
    int right_bot_x = x + SQ_SIZE, right_bot_y = y + SQ_SIZE; //н. правая
    int left_bot_x = x, left_bot_y = y + SQ_SIZE;

    int length_to_top = qSqrt(qPow(left_top_x - position.x(), 2) + qPow(left_top_y - position.y(), 2));
    int length_to_left = qSqrt(qPow(right_top_x - position.x(), 2) + qPow(right_top_y - position.y(), 2));
    int length_to_bot = qSqrt(qPow(left_bot_x - position.x(), 2) + qPow(left_bot_y - position.y(), 2));
    int length_to_right = qSqrt(qPow(right_bot_x - position.x(), 2) + qPow(right_bot_y - position.y(), 2));

    QVector<int> length;
    length.push_back(length_to_top);
    length.push_back(length_to_right);
    length.push_back(length_to_bot);
    length.push_back(length_to_left);
    qSort(length);
    if(length[0] == length_to_top) {
        x = left_top_x;
        y = left_top_y;
    } else if(length[0] == length_to_right) {
        x = right_bot_x;
        y = right_bot_y;
    } else if(length[0] == length_to_bot) {
        x = left_bot_x;
        y = left_bot_y;
    } else {
        x = right_top_x;
        y = right_top_y;
    }
}


void ControlGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *ev)
{
    QPoint position = ev->scenePos().toPoint();
    int x = 0, y = 0;
    switch(m_controlWidget->cbTracking->currentIndex())
    {
        case 0:
            trackSquareCenter(position,x,y);
        break;
        case 1:
            trackLines(position,x,y);
        break;
        case 2:
            trackIntersection(position,x,y);
        break;
    }

    QPen p;
    p.setColor(Qt::red);
    p.setWidth(5);
    m_viewIntersect->setPen(p);

    m_viewIntersect->setRect(x-2, y-2, 4, 4);

}

void ControlGraphicsScene::tick()
{
   m_spot->setOpacity(m_spot->opacity() - 0.05);
   m_spotView->setOpacity(m_spot->opacity() - 0.05);
}
