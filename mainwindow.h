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

class MetadataDisplayDialog : public QDialog
{
    Q_OBJECT

public:
    MetadataDisplayDialog(QMainWindow *);

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
    QRect correctOutOfBoundsGeometry(const QPoint &winPos, const QSize &winSize);

    QWidget *mCenterWidget = nullptr;
    QGridLayout *mGridLayout = nullptr;
    MetadataDisplayDialog *metadataDisplay = nullptr;

    vector<vector<QLabel *>> mGridLabels;
};

#endif // MAINWINDOW_H
