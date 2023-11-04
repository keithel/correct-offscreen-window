#include "mainwindow.h"
#include <QApplication>
#include <QScreen>
#include <QWindow>
#include <QCloseEvent>
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
}

MainWindow::~MainWindow() {}

void MainWindow::correctOutOfBounds()
{
    qDebug() << "Correcting geometry";
    QRect correctedGeom = correctOutOfBoundsGeometry(pos(), size());
    resize(correctedGeom.size());
    move(correctedGeom.topLeft());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qApp->quit();
}

// Given a geometry that is partially or completely outside the bounds of the available virtual
// geometry, will return a rect that has the geometry such that the title bar of the window
// whose geometry this is representing will be visible on the displays that are currently active
// on the user's system.
// frameGeometry() cannot be used because the size of the window will not be correct when setting
// the corrected size to the window, as all size setting methods only expect the geometry without
// the frame.
//
// winPos - the position of the top left corner of the window frame (QWidget::pos())
// winSize - the size of the window, not including the frame (QWidget::size())
QRect MainWindow::correctOutOfBoundsGeometry(const QPoint &winPos, const QSize &winSize)
{
    static const int sOffscreenWidthToShow = 80; // device pixels

    // Get the available virtual geometry -- this is the union of all of the available
    // geometries of the QScreens that are part of this virtual desktop. Available means the
    // screen areas excluding window manager reserved areas like task bars and system menus.
    // NOTE: there is only ever one virtual desktop, so it doesn't matter which screen we get
    // the virtual geometry from.
    QRect availVirtalGeom = qApp->primaryScreen()->availableVirtualGeometry();

    QRect g(winPos, winSize);
    QRect correctedG(g);

    if (g.right() < availVirtalGeom.left())
        correctedG.moveRight(availVirtalGeom.left() + sOffscreenWidthToShow);
    if (g.left() > availVirtalGeom.right())
        correctedG.moveLeft(availVirtalGeom.right() - sOffscreenWidthToShow);
    if (g.bottom() < availVirtalGeom.top())
        correctedG.moveTop(availVirtalGeom.top());
    if (g.top() > availVirtalGeom.bottom())
        correctedG.moveTop(availVirtalGeom.bottom() - sOffscreenWidthToShow);

    return correctedG;
}
