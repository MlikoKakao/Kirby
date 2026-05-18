#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QKeyEvent>
#include <vector>
#include "player.h"

struct Platform
{
    int x;
    int y;
    int width;
    int height;
};

struct Block
{
    int x;
    int y;
    int width;
    int height;
};

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;


private:
    int gravity;
    std::vector<Platform> platforms;    
    std::vector<Block> blocks;
    Player player;

    bool leftPressed;
    bool rightPressed;


    void applyGravity();
    void checkPlatformVerticalCollision(int oldKirbyY, Platform platform, QRect playerRect);
    void checkBlockVerticalCollision(int oldKirbyY, Block block, QRect playerRect);
    void checkHorizontalCollision(int oldKirbyX, Block block, QRect playerRect);
    // Could overload this, but currently only blocks check for horizontal colision


    int cameraX;
    void updateCamera();

    void updateGame();
};

#endif // GAMEWIDGET_H