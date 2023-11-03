#include "mainwindow.h"

#include <QApplication>
#include <QPushButton>
#include <QTimer>
#include <QWindow>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QPoint>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QGroupBox buttonBox;
    QPushButton correctGeometryBut("Correct Geometry");
    QPushButton moveOffscreenBut("Move offscreen");
    QObject::connect(&correctGeometryBut, &QAbstractButton::clicked, &w, [&w](){
        w.correctOutOfBounds();
    });
    QVBoxLayout *buttonLayout = new QVBoxLayout();
    buttonLayout->addWidget(&correctGeometryBut);
    buttonLayout->addWidget(&moveOffscreenBut);
    buttonBox.setLayout(buttonLayout);
    QObject::connect(&moveOffscreenBut, &QAbstractButton::clicked, &w, [&w](){
        QPoint newPos(-200, -200);
        qDebug() << "Moving MainWindow to" << newPos;
        w.windowHandle()->setPosition(newPos);
    });


    buttonBox.show();
    w.show();
    QTimer::singleShot(100, &w, [&w](){
        w.windowHandle()->setPosition(-200, -200);
        // w.setGeometry(10, -200, w.width(), w.height());
    });
    return a.exec();
}
