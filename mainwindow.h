#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QGridLayout>
#include <vector>

using std::vector;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void correctOutOfBounds();

private:
    QRect correctOutOfBoundsGeometry(const QRect&);

    QWidget *mCenterWidget;
    QGridLayout *mGridLayout;

    vector<vector<QLabel *>> mGridLabels;
};
#endif // MAINWINDOW_H
