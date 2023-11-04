#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QGridLayout>
#include <vector>

using std::vector;

class QCloseEvent;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void correctOutOfBounds();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QRect correctOutOfBoundsGeometry(const QPoint &winPos, const QSize &winSize);

    QWidget *mCenterWidget;
    QGridLayout *mGridLayout;

    vector<vector<QLabel *>> mGridLabels;
};
#endif // MAINWINDOW_H
