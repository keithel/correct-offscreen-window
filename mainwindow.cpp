#include "mainwindow.h"
#include <QApplication>
#include <QScreen>
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

    // Ok - add some logic to move the window out of bounds.
}

MainWindow::~MainWindow() {}

void MainWindow::correctOutOfBounds()
{
    qDebug() << "Correcting geometry";
    setGeometry(correctOutOfBoundsGeometry(geometry()));
}

// Given a geometry that is partially or completely outside the bounds of the available virtual
// geometry, will return a rect that has the geometry such that the title bar of the window
// whose geometry this is representing will be visible on the displays that are currently active
// on the user's system.
QRect MainWindow::correctOutOfBoundsGeometry(const QRect &g)
{
    static const int sOffscreenWidthToShow = 20; // device pixels
    auto offscreenWidthToShow = [](const QScreen* screen)-> int {
        return sOffscreenWidthToShow * screen->devicePixelRatio();
    };
    const QScreen* primaryScreen = qApp->primaryScreen();

    // Get the available virtual geometry -- this is the union of all of the available
    // geometries of the QScreens that are part of this virtual desktop. Available means the
    // screen areas excluding window manager reserved areas like task bars and system menus.
    // NOTE: there is only ever one virtual desktop, so it doesn't matter which screen we get
    // the virtual geometry from.
    QRect availVirtalGeom = primaryScreen->availableVirtualGeometry();

    QRect correctedG(g);

    if (g.right() < availVirtalGeom.left())
        correctedG.setRight(availVirtalGeom.left() + sOffscreenWidthToShow);
    if (g.left() > availVirtalGeom.right())
        correctedG.setLeft(availVirtalGeom.right() - sOffscreenWidthToShow);
    if (g.bottom() < availVirtalGeom.top())
        correctedG.setBottom(availVirtalGeom.top() + g.height());
    if (g.top() > availVirtalGeom.bottom())
        correctedG.setTop(availVirtalGeom.bottom() - sOffscreenWidthToShow);

    return correctedG;
}
