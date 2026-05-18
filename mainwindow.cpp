#include "mainwindow.h"
#include "gamewidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    GameWidget *gameWidget = new GameWidget(this);
    setCentralWidget(gameWidget);
}

MainWindow::~MainWindow() = default;