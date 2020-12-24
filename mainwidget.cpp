#include "mainwidget.h"

MainWidget::MainWidget()
{
    m_fullscreen = true;
    grid = new QGridLayout(this);
    pbOpen = new QPushButton("Open");
    connect(pbOpen, SIGNAL(clicked()), this, SLOT(openImage()));
    //grid->addWidget(pbOpen, 0, 0);
    vw = new ViewWidget();
    cw = new ControlWidget(vw);
    grid->addWidget(cw, 1, 0);
    vw->setNipSize(1280, 720);
    cw->setNipSize(1280, 720);
    cw->setSquareSize(70);
    cw->setImage("crossroad.png");
    cw->show();
    cw->setFocus();
    vw->show();
}

void MainWidget::openImage() {
    m_filename = QFileDialog::getOpenFileName();
    if(m_filename.length() > 0) {
        AskingW = new QWidget();
        QGridLayout* gr = new QGridLayout(AskingW);
        QCheckBox *cbFullscreenNIP  = new QCheckBox("Fullsreen");
        cbFullscreenNIP->setChecked(true);
        connect(cbFullscreenNIP, SIGNAL(toogled(bool)), this, SLOT(setFullscreen(bool)));
        leNipWidth       = new QLineEdit("1920");
        leNipHeight      = new QLineEdit("1080");
        leSquareSize    = new QLineEdit("20");
        gr->addWidget(new QLabel("NIP Width"), 0, 0, 1, 1);
        gr->addWidget(leNipWidth, 0, 1, 1, 1);
        gr->addWidget(new QLabel("NIP Height"), 1, 0, 1, 1);
        gr->addWidget(leNipHeight, 1, 1, 1, 1);
        gr->addWidget(new QLabel("Square Size"), 2, 0, 1, 1);
        gr->addWidget(leSquareSize, 2, 1, 1, 1);
        QPushButton *pbProceed = new QPushButton("Proceed");
        connect(pbProceed, SIGNAL(clicked()), this, SLOT(openImage2()));
        gr->addWidget(pbProceed,3,0,1,2);
        grid->addWidget(AskingW, 1, 0);
    }
}



void MainWidget::openImage2() {
    AskingW->hide();
    //vw->setFullscreen(m_fullscreen);
    vw->setNipSize(leNipWidth->text().toInt(), leNipHeight->text().toInt());
    cw->setNipSize(leNipWidth->text().toInt(), leNipHeight->text().toInt());
    cw->setSquareSize(leSquareSize->text().toInt());
    cw->setImage(m_filename);
    cw->show();
    cw->setFocus();
    vw->show();
}

void MainWidget::setFullscreen(bool toogle) {
    m_fullscreen = toogle;
}
