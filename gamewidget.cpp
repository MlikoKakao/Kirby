#include "gamewidget.h"

#include <QPainter>
#include <QColor>
#include <QKeyEvent>
#include <QTimer>
#include <vector>

// Constructor for Kirby and key values, like gravity, camera
GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent),
    gravity(1),
    player(100, 300, 80, 80)
{
    cameraX = 0;
    leftPressed = false;
    rightPressed = false;

    // Creating platforms and blocks inside a vector with coordinates and sizes (x, y, width, height)
    platforms.push_back({100, 750, 300, 30});
    blocks.push_back({0, 900, 4000, 650}); // Floor
    blocks.push_back({400, 750, 300, 100});
    blocks.push_back({700, 700, 50, 50});

    enemies.push_back({600, 600, 50, 50, 5});


    // Setting up FPS
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameWidget::updateGame);

    // Set up update window - 16 is 60 FPS
    timer->start(16);

    setFixedSize(1620, 1080); // Set up window size

    // Focus Kirby so key presses go to him
    setFocusPolicy(Qt::StrongFocus);
    setFocus();
}

// Draw everything on screen
void GameWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    painter.setBrush(QColor(100,100,100));
    int platformsSize = platforms.size();
    for (int i = 0; i < platformsSize; i++)
    {
        painter.drawRect(platforms.at(i).x - cameraX, platforms.at(i).y, platforms.at(i).width, platforms.at(i).height);
    }

    painter.setBrush(QColor(100,150,40));
    int blocksSize = blocks.size();
    for (int i = 0; i < blocksSize; i++)
    {
        painter.drawRect(blocks.at(i).x - cameraX, blocks.at(i).y, blocks.at(i).width, blocks.at(i).height);
    }

    painter.setBrush(QColor(250,0,0));
    int enemiesSize = enemies.size();
    for (int i = 0; i < enemiesSize; i++)
    {
        enemies.at(i).draw(painter, cameraX);
    }

    player.draw(painter, cameraX);
}


// Make Kirby take key presses - if done by just changin cords, movement is bad since depends on OS refresh
void GameWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Right) {
        rightPressed = true;
    }

    if (event->key() == Qt::Key_Left) {
        leftPressed = true;
    }
    if (event->key() == Qt::Key_Up) {
        player.jump();
    }
    if (event->key() == Qt::Key_Down) {
        player.setCrouched(true);
    }
}

// Make Kirby stop on key presses release
void GameWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Right) {
        rightPressed = false;
    }

    if (event->key() == Qt::Key_Left) {
        leftPressed = false;
    }
    if (event->key() == Qt::Key_Down) {
        player.setCrouched(false);
    }
}


// Whole game loop
void GameWidget::updateGame()
{
    player.setGrounded(false); // Always assume Kirby not grounded

    int oldKirbyX = player.getX(); // Use for walls etc for horizontal collision.

    int enemiesSize = enemies.size();
    for (int i = 0; i < enemiesSize; i++)
    {
        enemies.at(i).move(1);
    }

    // Movement
    if (leftPressed) {
        player.move(-5, 0);
    }
    if (rightPressed) {
        player.move(5, 0);
    }

    // Getting kirby coords and position without needing to write 20 getters/setters in player.cpp
    QRect playerRect = player.getRect();

    int blocksSize = blocks.size();
    for (int i = 0; i < blocksSize; i++) {
        checkHorizontalCollision(oldKirbyX, blocks.at(i), playerRect);
    } // Draw blocks

    int oldKirbyY = player.getY(); // Used for collision

    player.applyGravity(gravity);

    playerRect = player.getRect();

    int platformsSize = platforms.size();
    for (int i = 0; i < platformsSize; i++) {
        checkPlatformVerticalCollision(oldKirbyY, platforms.at(i), playerRect);
    } // Draw platforms

    playerRect = player.getRect(); // Need to update after movement, gravity etc

    for (int i = 0; i < blocksSize; i++) {
        checkBlockVerticalCollision(oldKirbyY, blocks.at(i), playerRect);
    }

    updateCamera();
    update();
}

void GameWidget::checkPlatformVerticalCollision(int oldKirbyY, Platform platform, QRect playerRect)
{
    int oldKirbyBottom = oldKirbyY + playerRect.height();

    // Without this Kirby glitches to the top if hits the upper half of the side of a platform
    bool wasAbovePlatform = oldKirbyBottom <= platform.y;

    bool crossedPlatformTop = playerRect.bottom() >= platform.y &&
                              playerRect.bottom() <= platform.y + platform.height;

    bool horizontalOverlap = player.getX() < platform.x + platform.width &&
                             playerRect.right() > platform.x;

    if (player.getVelocityY() >= 0 &&
        wasAbovePlatform &&
        crossedPlatformTop &&
        horizontalOverlap) {
        player.setY(platform.y - playerRect.height());
        player.setVelocityY(0);
        player.setGrounded(true);
    }
}

void GameWidget::checkBlockVerticalCollision(int oldKirbyY, Block block, QRect playerRect)
{
    int oldKirbyBottom = oldKirbyY + playerRect.height();

    // Without this Kirby glitches to the top if hits the upper half of the side of a platform
    bool wasAboveBlock = oldKirbyBottom <= block.y;

    bool crossedBlockTop = playerRect.bottom() >= block.y &&
                              playerRect.bottom() <= block.y + block.height;

    bool horizontalOverlap = player.getX() < block.x + block.width &&
                             playerRect.right() > block.x;

    if (player.getVelocityY() >= 0 &&
        wasAboveBlock &&
        crossedBlockTop &&
        horizontalOverlap)
    {
        player.setY(block.y - playerRect.height());
        player.setVelocityY(0);
        player.setGrounded(true);
    }

    // Logic so Kirby gets blocked when jumping up
    bool wasBelowBlock = oldKirbyY >= block.y + block.height;
    bool crossedBlockBottom = player.getY() <= block.y + block.height;

    if (player.getVelocityY() < 0 &&
        horizontalOverlap &&
        wasBelowBlock &&
        crossedBlockBottom)
    {
        player.setY(block.y + block.height);
        player.setVelocityY(0);
    }
}


void GameWidget::checkHorizontalCollision(int oldKirbyX, Block block, QRect playerRect)
{
    bool horizontalOverlap = player.getX() < block.x + block.width &&
                             playerRect.right() > block.x;

    bool verticalOverlap = (player.getY() < block.y + block.height &&
                           playerRect.bottom() > block.y);

    if (horizontalOverlap && verticalOverlap) {
        player.setX(oldKirbyX);
    }
}

void GameWidget::updateCamera() // Player starts on left, then is centerd
{
    int screenCenter = 810;
    cameraX = player.getX() - screenCenter;
    if (cameraX < 0) {
        cameraX = 0;
    }
}