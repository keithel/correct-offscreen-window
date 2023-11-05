#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <vector>

using std::vector;

class QCloseEvent;
class QShowEvent;
class QMoveEvent;
class QResizeEvent;
class QGridLayout;
class QLabel;
class QTimer;

class MetadataDisplayWindow : public QWidget
{
    Q_OBJECT

public:
    MetadataDisplayWindow(QMainWindow *);

    void delayedUpdate();

public slots:
    void update();

private:
    QMainWindow *mwParent = nullptr;
    QLabel *posLabel = nullptr;
    QLabel *posValLabel = nullptr;
    QLabel *frameGeomLabel = nullptr;
    QLabel *frameGeomValLabel = nullptr;
    QLabel *frameMarginsLabel = nullptr;
    QLabel *frameMarginsValLabel = nullptr;
    QLabel *geomLabel = nullptr;
    QLabel *geomValLabel = nullptr;
    QLabel *availVirtGeomLabel = nullptr;
    QLabel *availVirtGeomValLabel = nullptr;
    QTimer *updateMetadataDisplayTimer = nullptr;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void correctOutOfBounds();

protected:
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void moveEvent(QMoveEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QRect correctOutOfBoundsGeometry(const QRect &winGeom);

    QWidget *mCenterWidget = nullptr;
    QGridLayout *mGridLayout = nullptr;
    MetadataDisplayWindow *metadataDisplay = nullptr;

    vector<vector<QLabel *>> mGridLabels;
};

#endif // MAINWINDOW_H
