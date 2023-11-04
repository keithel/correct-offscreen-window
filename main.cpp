#include "mainwindow.h"

#include <QApplication>
#include <QPushButton>
#include <QTimer>
#include <QWindow>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QPoint>
#include <QDebug>
#include <QCloseEvent>
#include <vector>

using std::vector;

class ButtonBox : public QGroupBox
{
    Q_OBJECT

protected:
    void closeEvent(QCloseEvent *event) override {
        qApp->quit();
    }

};

vector<QPushButton *> createMoveOffscreenButtons(QWidget* parent, QMainWindow* mw)
{
    vector<QPushButton *> moveOffscreenButtons;

    QRect availVirtGeom = qApp->primaryScreen()->availableVirtualGeometry();

    QPushButton *moveOffscreenButton = new QPushButton("Move Offscreen ↑", parent);
    QObject::connect(moveOffscreenButton, &QAbstractButton::clicked, mw, [mw, availVirtGeom](){
        QPoint newPos(availVirtGeom.width()/2-mw->width()/2, -200);
        qDebug() << "Moving MainWindow to" << newPos;
        mw->windowHandle()->setPosition(newPos);
    });
    moveOffscreenButtons.push_back(moveOffscreenButton);

    moveOffscreenButton = new QPushButton("Move Offscreen ↗", parent);
    QObject::connect(moveOffscreenButton, &QAbstractButton::clicked, mw, [mw, availVirtGeom](){
        QPoint newPos(availVirtGeom.right()+200, -200);
        qDebug() << "Moving MainWindow to" << newPos;
        mw->windowHandle()->setPosition(newPos);
    });
    moveOffscreenButtons.push_back(moveOffscreenButton);

    moveOffscreenButton = new QPushButton("Move Offscreen →", parent);
    QObject::connect(moveOffscreenButton, &QAbstractButton::clicked, mw, [mw, availVirtGeom](){
        QPoint newPos(availVirtGeom.right()+200, availVirtGeom.height()/2-mw->height()/2);
        qDebug() << "Moving MainWindow to" << newPos;
        mw->windowHandle()->setPosition(newPos);
    });
    moveOffscreenButtons.push_back(moveOffscreenButton);

    moveOffscreenButton = new QPushButton("Move Offscreen ↘", parent);
    QObject::connect(moveOffscreenButton, &QAbstractButton::clicked, mw, [mw, availVirtGeom](){
        QPoint newPos(availVirtGeom.right()+200, availVirtGeom.bottom()+200);
        qDebug() << "Moving MainWindow to" << newPos;
        mw->windowHandle()->setPosition(newPos);
    });
    moveOffscreenButtons.push_back(moveOffscreenButton);

    moveOffscreenButton = new QPushButton("Move Offscreen ↓", parent);
    QObject::connect(moveOffscreenButton, &QAbstractButton::clicked, mw, [mw, availVirtGeom](){
        QPoint newPos(availVirtGeom.center().x()-mw->width()/2, availVirtGeom.bottom()+200);
        qDebug() << "Moving MainWindow to" << newPos;
        mw->windowHandle()->setPosition(newPos);
    });
    moveOffscreenButtons.push_back(moveOffscreenButton);

    moveOffscreenButton = new QPushButton("Move Offscreen ↙", parent);
    QObject::connect(moveOffscreenButton, &QAbstractButton::clicked, mw, [mw, availVirtGeom](){
        QPoint newPos(-200, availVirtGeom.bottom()+200);
        qDebug() << "Moving MainWindow to" << newPos;
        mw->windowHandle()->setPosition(newPos);
    });
    moveOffscreenButtons.push_back(moveOffscreenButton);

    moveOffscreenButton = new QPushButton("Move Offscreen ←", parent);
    QObject::connect(moveOffscreenButton, &QAbstractButton::clicked, mw, [mw, availVirtGeom](){
        QPoint newPos(-400, availVirtGeom.center().y()-mw->height()/2);
        qDebug() << "Moving MainWindow to" << newPos;
        mw->windowHandle()->setPosition(newPos);
    });
    moveOffscreenButtons.push_back(moveOffscreenButton);

    moveOffscreenButton = new QPushButton("Move Offscreen ↖", parent);
    QObject::connect(moveOffscreenButton, &QAbstractButton::clicked, mw, [mw](){
        QPoint newPos(-200, -200);
        qDebug() << "Moving MainWindow to" << newPos;
        mw->windowHandle()->setPosition(newPos);
    });
    moveOffscreenButtons.push_back(moveOffscreenButton);

    return moveOffscreenButtons;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    ButtonBox buttonBox;

    vector<QPushButton *> moveOffscreenButtons = createMoveOffscreenButtons(&buttonBox, &w);

    QPushButton correctGeometryBut("Correct Geometry");
    QObject::connect(&correctGeometryBut, &QAbstractButton::clicked, &w, [&w](){
        w.correctOutOfBounds();
    });
    QVBoxLayout *buttonLayout = new QVBoxLayout();
    for(auto button : moveOffscreenButtons)
        buttonLayout->addWidget(button);
    buttonLayout->addWidget(&correctGeometryBut);
    buttonBox.setLayout(buttonLayout);


    buttonBox.show();
    w.show();

    return a.exec();
}

#include "main.moc"
