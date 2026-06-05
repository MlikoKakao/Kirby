#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QKeyEvent>
#include <QPainter>
#include <vector>

#include "player.h"
#include "enemy.h"
#include "projectile.h"
#include "assets.h"
#include "item.h"
#include "stagedata.h"

enum class GameState
{
    StartMenu,
    Playing,
    GameOver,
    Win
};

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr);

protected:
    //-----------------------------------------------------------------
    //------------------------EVENT SECTION------------------------//
    //------------------------EVENT SECTION------------------------//
    //-----------------------------------------------------------------

    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    //-----------------------------------------------------------------
    //------------------------MAIN UPDATE SECTION------------------------//
    //------------------------MAIN UPDATE SECTION------------------------//
    //-----------------------------------------------------------------

    void updateGame();
    bool canUpdateGame() const;
    void updateTimers();
    void updatePlayerBeforePhysics();

    //-----------------------------------------------------------------
    //------------------------INPUT SECTION------------------------//
    //------------------------INPUT SECTION------------------------//
    //-----------------------------------------------------------------

    void handleStartMenuInput(QKeyEvent *event);
    void handleGameOverInput(QKeyEvent *event);
    void handleAbilityMenuInput(QKeyEvent *event);
    void handleGameplayInput(QKeyEvent *event);
    void handleAttackInput(QKeyEvent *event);

    //-----------------------------------------------------------------
    //------------------------PLAYER SECTION------------------------//
    //------------------------PLAYER SECTION------------------------//
    //-----------------------------------------------------------------

    void handlePlayerInputMovement();
    void handlePlayerPhysicsAndCollisions(int oldKirbyX);
    void checkPlayerBounds();

    void damagePlayer(int damage, int sourceX);
    void stopPlayerActions();
    bool isPlayerBusy() const;

    bool handlePlayerDeath();
    void handleFallingDeath();

    //-----------------------------------------------------------------
    //------------------------ENEMY SECTION------------------------//
    //------------------------ENEMY SECTION------------------------//
    //-----------------------------------------------------------------

    void updateEnemies();
    void shootEnemyProjectileIfNeeded(Enemy &enemy);
    void applyEnemyPhysics(Enemy &enemy, int oldEnemyX, int oldEnemyY);

    //-----------------------------------------------------------------
    //------------------------ATTACK SECTION------------------------//
    //------------------------ATTACK SECTION------------------------//
    //-----------------------------------------------------------------

    void handlePlayerAttacks();
    void handleInhaling();
    void executeFireAttack();
    void executeSparkAttack();
    void damageEnemiesInRect(QRect attackRect, int damage);

    //-----------------------------------------------------------------
    //------------------------PROJECTILE SECTION------------------------//
    //------------------------PROJECTILE SECTION------------------------//
    //-----------------------------------------------------------------

    void updateProjectiles();
    void spawnPlayerProjectile(ProjectileType type, int size, int speed, int damage);

    void checkProjectileTooFar();
    void checkProjectileEnemyCollision();
    void checkProjectileSolidCollision();
    void checkEnemyProjectilePlayerCollision();

    //-----------------------------------------------------------------
    //------------------------COLLISION SECTION------------------------//
    //------------------------COLLISION SECTION------------------------//
    //-----------------------------------------------------------------

    void checkEnemyCollision();
    void checkEnemyAbilityCollision();
    void checkItemCollision();

    template <typename Entity>
    void checkEntityBlockVerticalCollision(Entity &entity, int oldY, Block block);

    template <typename Entity>
    void checkEntityHorizontalCollision(Entity &entity, int oldX, Block block);

    template <typename Entity>
    void checkEntityPlatformVerticalCollision(Entity &entity, int oldY, Platform platform);

    template <typename Entity>
    bool checkEntityHorizontalCollisionAndReturn(Entity &entity, int oldX, Block block);
    //-----------------------------------------------------------------
    //------------------------GAME FLOW SECTION------------------------//
    //------------------------GAME FLOW SECTION------------------------//
    //-----------------------------------------------------------------

    void startGame();
    void respawnPlayer();
    bool checkPortalAndGoal();

    //-----------------------------------------------------------------
    //------------------------STAGE SECTION------------------------//
    //------------------------STAGE SECTION------------------------//
    //-----------------------------------------------------------------

    void loadStageData(const StageData &stage);
    void loadStage1();
    void loadStage2();

    void resetKirbyForStage();
    void clearStage();
    int getCurrentStageWidth() const;

    //-----------------------------------------------------------------
    //------------------------DRAW SECTION------------------------//
    //------------------------DRAW SECTION------------------------//
    //-----------------------------------------------------------------

    void drawMenus(QPainter &painter);
    void drawHUD(QPainter &painter);
    void drawAbilityMenu(QPainter &painter);
    void drawStageTiles(QPainter &painter);
    void drawStageObjects(QPainter &painter);
    void drawEnemies(QPainter &painter);
    void drawProjectiles(QPainter &painter);
    void drawFireAttack(QPainter &painter);
    void drawBackground(QPainter &painter);


    //-----------------------------------------------------------------
    //------------------------ABILITY MENU SECTION------------------------//
    //------------------------ABILITY MENU SECTION------------------------//
    //-----------------------------------------------------------------

    void storeSwallowedEnemyAbility();

    bool isAbilityState(KirbyState state) const;
    bool hasStoredAbility(KirbyState ability) const;

    void addAbilityToStorage(KirbyState ability);
    void equipSelectedAbility();
    void dropSelectedAbility();
    void storeCurrentAbility();

    QString getAbilityMenuSpriteName() const;

    //-----------------------------------------------------------------
    //------------------------CAMERA SECTION------------------------//
    //------------------------CAMERA SECTION------------------------//
    //-----------------------------------------------------------------

    void updateCamera();

private:

    int gravity;
    int cameraX;

    GameState currentGameState;
    int currentStage;
    int currentStageWidth;
    int gameOverSelection;
    int lives;
    int score;

    std::vector<Platform> platforms;
    std::vector<Block> blocks;
    std::vector<Block> floors;
    std::vector<Enemy> enemies;
    std::vector<Projectile> projectiles;
    std::vector<Item> items;
    std::vector<TerrainSprite> terrainSprites;

    QRect portalRect;
    QRect goalRect;

    Player player;

    bool leftPressed;
    bool rightPressed;
    bool upPressed;

    bool sparkAttack;
    bool fireAttack;
    int sparkAttackTimer;
    int inhaleAnimationTimer;

    std::vector<KirbyState> storedAbilities;
    KirbyState selectedMenuAbility;
    bool abilityMenuOpen;
    int selectedAbilityIndex;

};


// Seems template has to be here and not in .cpp file
template <typename Entity>
void GameWidget::checkEntityBlockVerticalCollision(Entity &entity, int oldY, Block block)
{
    QRect entityRect = entity.getRect();

    int oldBottom = oldY + entityRect.height();

    // Without this, Entity glitches to the top if it hits the upper half of the side of a platform.
    bool wasAboveBlock = oldBottom <= block.y;

    bool crossedBlockTop = entityRect.bottom() >= block.y &&
                           entityRect.bottom() <= block.y + block.height;

    bool horizontalOverlap = entity.getX() < block.x + block.width &&
                             entityRect.right() > block.x;

    if (entity.getVelocityY() >= 0 &&
        wasAboveBlock &&
        crossedBlockTop &&
        horizontalOverlap)
    {
        entity.setY(block.y - entityRect.height());
        entity.setVelocityY(0);
        entity.setGrounded(true);
    }

    // Logic so Entity gets blocked when jumping up.
    bool wasBelowBlock = oldY >= block.y + block.height;
    bool crossedBlockBottom = entity.getY() <= block.y + block.height;

    if (entity.getVelocityY() < 0 &&
        horizontalOverlap &&
        wasBelowBlock &&
        crossedBlockBottom)
    {
        entity.setY(block.y + block.height);
        entity.setVelocityY(0);
    }
}

template <typename Entity>
void GameWidget::checkEntityHorizontalCollision(Entity &entity, int oldX, Block block)
{
    QRect entityRect = entity.getRect();

    bool horizontalOverlap = entity.getX() < block.x + block.width &&
                             entityRect.right() > block.x;

    bool verticalOverlap = entity.getY() < block.y + block.height &&
                           entityRect.bottom() > block.y;

    if (horizontalOverlap && verticalOverlap)
    {
        entity.setX(oldX);
    }
}

template <typename Entity>
void GameWidget::checkEntityPlatformVerticalCollision(Entity &entity, int oldY, Platform platform)
{
    QRect entityRect = entity.getRect();

    int oldBottom = oldY + entityRect.height();

    // Without this, Kirby glitches to the top if he hits the upper half of the side of a platform.
    bool wasAbovePlatform = oldBottom <= platform.y;

    bool crossedPlatformTop = entityRect.bottom() >= platform.y &&
                              entityRect.bottom() <= platform.y + platform.height;

    bool horizontalOverlap = entity.getX() < platform.x + platform.width &&
                             entityRect.right() > platform.x;

    if (entity.getVelocityY() >= 0 &&
        wasAbovePlatform &&
        crossedPlatformTop &&
        horizontalOverlap)
    {
        entity.setY(platform.y - entityRect.height());
        entity.setVelocityY(0);
        entity.setGrounded(true);
    }
}

template <typename Entity>
bool GameWidget::checkEntityHorizontalCollisionAndReturn(Entity &entity, int oldX, Block block)
{
    QRect entityRect = entity.getRect();

    bool horizontalOverlap = entity.getX() < block.x + block.width &&
                             entityRect.right() > block.x;

    bool verticalOverlap = entity.getY() < block.y + block.height &&
                           entityRect.bottom() > block.y;

    if (horizontalOverlap && verticalOverlap)
    {
        entity.setX(oldX);
        return true;
    }

    return false;
}

#endif // GAMEWIDGET_H