#include "mainwindow.h"
#include <QLabel>
#include <QGridLayout>
#include <QTimer>
#include <QApplication>
#include <QScreen>
#include <QCloseEvent>
#include <QMoveEvent>
#include <QResizeEvent>
#include <QWindow>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    mCenterWidget = new QWidget(parent);
    mGridLayout = new QGridLayout(this);
    mGridLayout->setObjectName("Grid Layout");

    for(auto x = 0; x < 3; x++) {
        mGridLabels.push_back(vector<QLabel*>());
        for(auto y = 0; y < 3; y++) {
            QString colStr, rowStr;
            switch(x) {
            case 0:  colStr = "TOP"; break;
            case 1:  colStr = ""; break;
            case 2:  colStr = "BOTTOM"; break;
            default: colStr = "UNKNOWN"; break;
            }
            switch(y) {
            case 0:  rowStr = "LEFT"; break;
            case 1:  rowStr = (x == 1) ? "Hello, World" : ""; break;
            case 2:  rowStr = "RIGHT"; break;
            default: rowStr = "UNKNOWN"; break;
            }

            QLabel *l = new QLabel(colStr + " " + rowStr, this);
            l->setAlignment(Qt::AlignCenter);
            mGridLayout->addWidget(l, x, y);
            mGridLabels[x].push_back(l);
        }
    }

    mCenterWidget->setLayout(mGridLayout);
    setCentralWidget(mCenterWidget);

    metadataDisplay = new MetadataDisplayWindow(this);
    metadataDisplay->show();
}

MainWindow::~MainWindow()
{
    delete metadataDisplay;
}

void MainWindow::correctOutOfBounds()
{
    qDebug() << "Correcting geometry";
    QWindow *win = windowHandle();
    QRect correctedGeom = correctOutOfBoundsGeometry(win->frameGeometry());

    move(correctedGeom.topLeft());
    // Resize to the corrected window size, subtracting out the titlebar and any other margins.
    // as the resize method expects a geometry that does not include these.
    resize((correctedGeom - win->frameMargins()).size());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qApp->quit();
}

void MainWindow::showEvent(QShowEvent *event)
{
    metadataDisplay->update();
}

void MainWindow::moveEvent(QMoveEvent *event)
{
    metadataDisplay->delayedUpdate();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    metadataDisplay->delayedUpdate();
}


// Given a geometry that is partially or completely outside the bounds of the available virtual
// geometry, will return a rect that has the geometry such that the title bar of the window
// whose geometry this is representing will be visible on the displays that are currently active
// on the user's system.
// frameGeometry() cannot be used because the size of the window will not be correct when setting
// the corrected size to the window, as all size setting methods only expect the geometry without
// the frame.
//
// winGeom - the geometry of the window, including the titlebar (`QWindow::frameGeometry()`)
QRect MainWindow::correctOutOfBoundsGeometry(const QRect &winGeom)
{
    static const int sOffscreenWidthToShow = 80; // device pixels

    // Get the available virtual geometry -- this is the union of all of the available
    // geometries of the QScreens that are part of this virtual desktop. Available means the
    // screen areas excluding window manager reserved areas like task bars and system menus.
    // NOTE: there is only ever one virtual desktop, so it doesn't matter which screen we get
    // the virtual geometry from.
    QRect availVirtalGeom = qApp->primaryScreen()->availableVirtualGeometry();

    QRect correctedG(winGeom);

    if (winGeom.right() < availVirtalGeom.left())
        correctedG.moveRight(availVirtalGeom.left() + sOffscreenWidthToShow);
    if (winGeom.left() > availVirtalGeom.right())
        correctedG.moveLeft(availVirtalGeom.right() - sOffscreenWidthToShow);
    if (winGeom.bottom() < availVirtalGeom.top())
        correctedG.moveTop(availVirtalGeom.top());
    if (winGeom.top() > availVirtalGeom.bottom())
        correctedG.moveTop(availVirtalGeom.bottom() - sOffscreenWidthToShow);

    return correctedG;
}

MetadataDisplayWindow::MetadataDisplayWindow(QMainWindow *parent)
    : QWidget()
    , mwParent(parent)
{
    posLabel = new QLabel(this);
    posValLabel = new QLabel(this);
    frameGeomLabel = new QLabel(this);
    frameGeomValLabel = new QLabel(this);
    frameMarginsLabel = new QLabel(this);
    frameMarginsValLabel = new QLabel(this);
    geomLabel = new QLabel(this);
    geomValLabel = new QLabel(this);
    availVirtGeomLabel = new QLabel(this);
    availVirtGeomValLabel = new QLabel(this);

    posLabel->setText("Position");
    frameGeomLabel->setText("Frame Geometry");
    frameMarginsLabel->setText("Frame Margins");
    geomLabel->setText("Geometry");
    availVirtGeomLabel->setText("Available Virtual Geometry");

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(posLabel, 0, 0);
    layout->addWidget(posValLabel, 0, 1);
    layout->addWidget(frameGeomLabel, 1, 0);
    layout->addWidget(frameGeomValLabel, 1, 1);
    layout->addWidget(frameMarginsLabel, 2, 0);
    layout->addWidget(frameMarginsValLabel, 2, 1);
    layout->addWidget(geomLabel, 3, 0);
    layout->addWidget(geomValLabel, 3, 1);
    layout->addWidget(availVirtGeomLabel, 4, 0);
    layout->addWidget(availVirtGeomValLabel, 4, 1);
    setLayout(layout);

    updateMetadataDisplayTimer = new QTimer(this);
    updateMetadataDisplayTimer->setInterval(100);
    updateMetadataDisplayTimer->setSingleShot(true);
    connect(updateMetadataDisplayTimer, &QTimer::timeout, this, &MetadataDisplayWindow::update);
}

void MetadataDisplayWindow::delayedUpdate()
{
    updateMetadataDisplayTimer->start();
}

void MetadataDisplayWindow::update()
{
    QWindow *mwWindow = mwParent->windowHandle();
    QPoint pos = mwParent->pos();
    QRect fGeom = mwWindow->frameGeometry();
    QMargins fMargins = mwWindow->frameMargins();
    QRect geom = mwWindow->geometry();
    QRect avGeom = qApp->primaryScreen()->availableVirtualGeometry();

    posValLabel->setText(QString("(%1, %2)").arg(pos.x()).arg(pos.y()));
    frameGeomValLabel->setText(QString("(%1, %2 %3x%4)").arg(fGeom.x()).arg(fGeom.y()).arg(fGeom.width()).arg(fGeom.height()));
    frameMarginsValLabel->setText(QString("(%1, %2, %3, %4)").arg(fMargins.left()).arg(fMargins.top()).arg(fMargins.right()).arg(fMargins.bottom()));
    geomValLabel->setText(QString("(%1, %2 %3x%4)").arg(geom.x()).arg(geom.y()).arg(geom.width()).arg(geom.height()));
    availVirtGeomValLabel->setText(QString("(%1, %2 %3x%4)").arg(avGeom.x()).arg(avGeom.y()).arg(avGeom.width()).arg(avGeom.height()));
}
