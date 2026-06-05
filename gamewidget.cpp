#include "gamewidget.h"

#include <QPainter>
#include <QColor>
#include <QKeyEvent>
#include <QTimer>
#include <vector>

//-----------------------------------------------------------------//
//------------------------CONSTRUCTOR SECTION------------------------//
//------------------------CONSTRUCTOR SECTION------------------------//
//-----------------------------------------------------------------//

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent),
    gravity(1),
    cameraX(0),
    currentGameState(GameState::StartMenu),
    currentStage(0),
    currentStageWidth(1620),
    gameOverSelection(0),
    lives(3),
    score(0),
    player(100, 300, 80, 80, 3),
    leftPressed(false),
    rightPressed(false),
    upPressed(false),
    sparkAttack(false),
    fireAttack(false),
    sparkAttackTimer(0),
    inhaleAnimationTimer(0),
    selectedMenuAbility(KirbyState::FireAbility),
    abilityMenuOpen(false),
    selectedAbilityIndex(0)
{
    Assets::instance().loadAll();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameWidget::updateGame);
    timer->start(16);

    setFixedSize(1620, 1080);

    setFocusPolicy(Qt::StrongFocus);
    setFocus();
}

//-----------------------------------------------------------------//
//------------------------EVENT SECTION------------------------//
//------------------------EVENT SECTION------------------------//
//-----------------------------------------------------------------//

void GameWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);

    if (currentGameState != GameState::Playing)
    {
        drawMenus(painter);
        return;
    }
    drawBackground(painter);

    drawStageTiles(painter);
    drawStageObjects(painter);
    drawEnemies(painter);
    drawProjectiles(painter);

    if (player.getState() == KirbyState::Inhaling)
    {
        QRect inhaleRect = player.getInhaleRect();
        int offSet = player.getFacingRight() ? -30 : 30;

        int frame = (inhaleAnimationTimer / 6) % 6 + 1;
        QString spriteName = "inhale_" + QString::number(frame);

        painter.drawPixmap(
            inhaleRect.x() - cameraX + offSet,
            inhaleRect.y() + 20,
            inhaleRect.width(),
            inhaleRect.height(),
            Assets::instance().getPixmap(spriteName)
            );
    }

    player.drawFireAttack(painter, cameraX);
    player.draw(painter, cameraX);

    drawHUD(painter);
    drawAbilityMenu(painter);
}

void GameWidget::keyPressEvent(QKeyEvent *event)
{
    if (currentGameState == GameState::StartMenu)
    {
        handleStartMenuInput(event);
        return;
    }

    if (currentGameState == GameState::GameOver)
    {
        handleGameOverInput(event);
        return;
    }

    if (currentGameState != GameState::Playing)
    {
        return;
    }

    if (event->key() == Qt::Key_C)
    {
        abilityMenuOpen = !abilityMenuOpen;
        update();
        return;
    }

    if (abilityMenuOpen)
    {
        handleAbilityMenuInput(event);
        return;
    }

    handleGameplayInput(event);
}

void GameWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
    {
        return;
    }

    if (event->key() == Qt::Key_Right)
    {
        rightPressed = false;
    }

    if (event->key() == Qt::Key_Left)
    {
        leftPressed = false;
    }

    if (event->key() == Qt::Key_Up || event->key() == Qt::Key_W)
    {
        upPressed = false;

        if (!player.getIsGrounded())
        {
            player.releaseInAir();
        }
    }

    if (event->key() == Qt::Key_Down)
    {
        player.setCrouched(false);
    }

    if (event->key() == Qt::Key_X)
    {
        if (player.getState() == KirbyState::Inhaling)
        {
            player.setState(KirbyState::Normal);
        }
        else if (player.getState() == KirbyState::FireAbility)
        {
            fireAttack = false;
            player.stopFireAttackHold();
        }
        else if (player.getState() == KirbyState::SparkAbility)
        {
            sparkAttack = false;
            player.stopSparkAttackHold();
        }
    }
}


//-----------------------------------------------------------------//
//------------------------MAIN UPDATE SECTION------------------------//
//------------------------MAIN UPDATE SECTION------------------------//
//-----------------------------------------------------------------//

void GameWidget::updateGame()
{
    if (!canUpdateGame())
    {
        update();
        return;
    }

    if (handlePlayerDeath())
    {
        update();
        return;
    }

    updateTimers();

    int oldKirbyX = player.getX();

    updatePlayerBeforePhysics();
    updateEnemies();

    handleInhaling();

    handlePlayerInputMovement();
    handlePlayerAttacks();

    updateProjectiles();

    handlePlayerPhysicsAndCollisions(oldKirbyX);

    handleFallingDeath();

    updateCamera();
    update();
}

bool GameWidget::canUpdateGame() const
{
    return currentGameState == GameState::Playing && !abilityMenuOpen;
}

void GameWidget::updateTimers()
{
    player.updateInvincibility();
    player.updateAttackAnimation();
    player.updateFireAttackHold();
    player.updateSparkAttackHold();

    if (player.getState() == KirbyState::Inhaling)
    {
        inhaleAnimationTimer++;
    }
    else
    {
        inhaleAnimationTimer = 0;
    }

    if (sparkAttackTimer > 0)
    {
        sparkAttackTimer--;
    }
}

void GameWidget::updatePlayerBeforePhysics()
{
    player.setGrounded(false);
    player.applyHorizontalVelocity();

    checkItemCollision();
}

//-----------------------------------------------------------------//
//------------------------INPUT SECTION------------------------//
//------------------------INPUT SECTION------------------------//
//-----------------------------------------------------------------//

void GameWidget::handleAbilityMenuInput(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Left ||
        event->key() == Qt::Key_Right)
    {
        if (selectedMenuAbility == KirbyState::FireAbility)
        {
            selectedMenuAbility = KirbyState::SparkAbility;
        }
        else
        {
            selectedMenuAbility = KirbyState::FireAbility;
        }

        update();
        return;
    }

    if (event->key() == Qt::Key_Return)
    {
        equipSelectedAbility();
        abilityMenuOpen = false;
        update();
        return;
    }

    if (event->key() == Qt::Key_V)
    {
        dropSelectedAbility();
        update();
        return;
    }

    if (event->key() == Qt::Key_C)
    {
        abilityMenuOpen = false;
        update();
        return;
    }
}

void GameWidget::handleGameplayInput(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Right)
    {
        if (!player.getIsCrouched())
        {
            rightPressed = true;
        }

        return;
    }

    if (event->key() == Qt::Key_Left)
    {
        if (!player.getIsCrouched())
        {
            leftPressed = true;
        }

        return;
    }

    if (event->key() == Qt::Key_Up || event->key() == Qt::Key_W)
    {
        if (!checkPortalAndGoal())
        {
            if (player.getIsGrounded() && !isPlayerBusy())
            {
                player.jump();
                upPressed = false;
            }
            else if (!player.getIsGrounded() && !isPlayerBusy())
            {
                upPressed = true;
            }
        }

        return;
    }

    if (event->key() == Qt::Key_Down)
    {
        if (player.getState() == KirbyState::Mouthful_Enemy)
        {
            storeSwallowedEnemyAbility();
        }
        else
        {
            player.setCrouched(true);
        }

        return;
    }

    if (event->key() == Qt::Key_X)
    {
        handleAttackInput(event);
        return;
    }

    if (event->key() == Qt::Key_V)
    {
        storeCurrentAbility();
        update();
        return;
    }
}

void GameWidget::handleStartMenuInput(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return)
    {
        startGame();
    }
}

void GameWidget::handleAttackInput(QKeyEvent *event)
{
    if (player.getIsHovering())
    {
        spawnPlayerProjectile(ProjectileType::Air, 25, 12, 1);

        player.setHovering(false);
        upPressed = false;
        player.startAttackAnimation(10);
        return;
    }

    if (player.getState() == KirbyState::Normal)
    {
        player.setState(KirbyState::Inhaling);
        return;
    }

    if (player.getState() == KirbyState::Mouthful_Enemy)
    {
        if (event->isAutoRepeat())
        {
            return;
        }

        spawnPlayerProjectile(ProjectileType::Star, 30, 12, 1);

        player.startAttackAnimation(10);
        player.setState(KirbyState::Normal);
        return;
    }

    if (player.getState() == KirbyState::FireAbility)
    {
        if (event->isAutoRepeat())
        {
            return;
        }

        fireAttack = true;
        player.startFireAttackHold();
        return;
    }

    if (player.getState() == KirbyState::SparkAbility)
    {
        if (event->isAutoRepeat())
        {
            return;
        }

        sparkAttack = true;
        player.startSparkAttackHold();
        return;
    }

    player.setState(KirbyState::Inhaling);
}

void GameWidget::handleGameOverInput(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down)
    {
        gameOverSelection = 1 - gameOverSelection;
        update();
        return;
    }

    if (event->key() != Qt::Key_Return)
    {
        return;
    }

    if (gameOverSelection == 0)
    {
        currentGameState = GameState::StartMenu;
    }
    else
    {
        close();
    }

    update();
}

//-----------------------------------------------------------------//
//------------------------PLAYER SECTION------------------------//
//------------------------PLAYER SECTION------------------------//
//-----------------------------------------------------------------//

void GameWidget::handlePlayerPhysicsAndCollisions(int oldKirbyX)
{
    checkPlayerBounds();

    for (int i = 0; i < static_cast<int>(blocks.size()); i++)
    {
        checkEntityHorizontalCollision(player, oldKirbyX, blocks.at(i));
    }

    for (int i = 0; i < static_cast<int>(floors.size()); i++)
    {
        checkEntityHorizontalCollision(player, oldKirbyX, floors.at(i));
    }

    int oldKirbyY = player.getY();

    player.applyGravity(gravity);

    checkEnemyCollision();

    for (int i = 0; i < static_cast<int>(platforms.size()); i++)
    {
        checkEntityPlatformVerticalCollision(player, oldKirbyY, platforms.at(i));
    }

    for (int i = 0; i < static_cast<int>(blocks.size()); i++)
    {
        checkEntityBlockVerticalCollision(player, oldKirbyY, blocks.at(i));
    }

    for (int i = 0; i < static_cast<int>(floors.size()); i++)
    {
        checkEntityBlockVerticalCollision(player, oldKirbyY, floors.at(i));
    }
}

void GameWidget::handlePlayerInputMovement()
{
    bool movingNow = false;

    if (!isPlayerBusy())
    {
        if (leftPressed)
        {
            player.move(-5, 0);
            player.setFacingRight(false);
            movingNow = true;
        }
        else if (rightPressed)
        {
            player.move(5, 0);
            player.setFacingRight(true);
            movingNow = true;
        }
    }

    player.updateAnimation(movingNow);

    if (upPressed && !player.getIsGrounded() && !isPlayerBusy())
    {
        player.fly();
    }
}

void GameWidget::handleFallingDeath()
{
    if (player.getY() <= height())
    {
        return;
    }

    lives--;

    if (lives <= 0)
    {
        currentGameState = GameState::GameOver;
        gameOverSelection = 0;
    }
    else
    {
        respawnPlayer();
    }
}

void GameWidget::checkPlayerBounds()
{
    if (player.getX() < 0)
    {
        player.setX(0);
    }

    int maxPlayerX = getCurrentStageWidth() - player.getRect().width();

    if (player.getX() > maxPlayerX)
    {
        player.setX(maxPlayerX);
    }

    if (player.getY() < 0)
    {
        player.setY(0);
    }
}

void GameWidget::damagePlayer(int damage, int sourceX)
{
    if (player.getInvincibleTimer() > 0)
    {
        return;
    }

    player.takeDamage(damage);
    player.setInvincibleTimer(120);

    stopPlayerActions();

    if (player.getX() < sourceX)
    {
        player.knockback(-12.0f, -10.0f);
    }
    else
    {
        player.knockback(12.0f, -10.0f);
    }
}

void GameWidget::stopPlayerActions()
{
    player.setState(KirbyState::Normal);
    player.setHovering(false);
    player.stopAttackAnimation();

    fireAttack = false;
    sparkAttack = false;
}

bool GameWidget::isPlayerBusy() const
{
    return player.getState() == KirbyState::Inhaling ||
           player.getIsAttacking() ||
           player.getIsCrouched() ||
           fireAttack ||
           sparkAttack;
}

bool GameWidget::handlePlayerDeath()
{
    if (player.getHealth() > 0)
    {
        return false;
    }

    lives--;

    if (lives <= 0)
    {
        currentGameState = GameState::GameOver;
        gameOverSelection = 0;
    }
    else
    {
        respawnPlayer();
    }

    return true;
}


//-----------------------------------------------------------------//
//------------------------ENEMY SECTION------------------------//
//------------------------ENEMY SECTION------------------------//
//-----------------------------------------------------------------//

void GameWidget::updateEnemies()
{
    for (int i = 0; i < static_cast<int>(enemies.size()); i++)
    {
        int oldEnemyX = enemies.at(i).getX();
        int oldEnemyY = enemies.at(i).getY();

        enemies.at(i).updateAI(player.getRect());
        enemies.at(i).move(1);
        enemies.at(i).updateAnimation();

        shootEnemyProjectileIfNeeded(enemies.at(i));

        if (enemies.at(i).getType() != EnemyType::Gordo)
        {
            applyEnemyPhysics(enemies.at(i), oldEnemyX, oldEnemyY);
        }
    }

    checkEnemyAbilityCollision();
}

void GameWidget::shootEnemyProjectileIfNeeded(Enemy &enemy)
{
    if (!enemy.shouldShootProjectile(player.getRect()))
    {
        return;
    }

    int projectileSpeed = enemy.getDirection() > 0 ? 7 : -7;

    int projectileX = enemy.getDirection() > 0
                          ? enemy.getX() + enemy.getRect().width()
                          : enemy.getX() - 40;

    int projectileY =
        enemy.getY() + enemy.getRect().height() / 2 - 15;

    projectiles.push_back(
        Projectile(
            projectileX,
            projectileY,
            35,
            projectileSpeed,
            ProjectileType::EnemyFire,
            1
            )
        );
}

void GameWidget::applyEnemyPhysics(Enemy &enemy, int oldEnemyX, int oldEnemyY)
{
    enemy.setGrounded(false);

    if (enemy.getType() == EnemyType::Sparky)
    {
        enemy.applyGravity(0.25f);
    }
    else
    {
        enemy.applyGravity(1.0f);
    }

    bool hitWall = false;

    for (int j = 0; j < static_cast<int>(blocks.size()); j++)
    {
        checkEntityBlockVerticalCollision(enemy, oldEnemyY, blocks.at(j));

        if (checkEntityHorizontalCollisionAndReturn(enemy, oldEnemyX, blocks.at(j)))
        {
            hitWall = true;
        }
    }

    for (int i = 0; i < static_cast<int>(platforms.size()); i++)
    {
        checkEntityPlatformVerticalCollision(enemy, oldEnemyY, platforms.at(i));
    }

    for (int j = 0; j < static_cast<int>(floors.size()); j++)
    {
        checkEntityBlockVerticalCollision(enemy, oldEnemyY, floors.at(j));

        if (checkEntityHorizontalCollisionAndReturn(enemy, oldEnemyX, floors.at(j)))
        {
            hitWall = true;
        }
    }

    if (hitWall)
    {
        enemy.turnAround();
    }
}

//-----------------------------------------------------------------//
//------------------------ATTACK SECTION------------------------//
//------------------------ATTACK SECTION------------------------//
//-----------------------------------------------------------------//

void GameWidget::handlePlayerAttacks()
{
    if (fireAttack)
    {
        executeFireAttack();
    }

    if (player.getState() == KirbyState::SparkAbility &&
        player.getIsAttacking())
    {
        executeSparkAttack();
    }
}

void GameWidget::handleInhaling()
{
    if (player.getState() != KirbyState::Inhaling)
    {
        return;
    }

    QRect inhaleRect = player.getInhaleRect();

    int playerCenterX = player.getX() + player.getRect().width() / 2;
    int playerCenterY = player.getY() + player.getRect().height() / 2;

    for (int i = 0; i < static_cast<int>(enemies.size()); i++)
    {
        if (!inhaleRect.intersects(enemies.at(i).getRect()))
        {
            continue;
        }

        if (!enemies.at(i).canBeInhaled())
        {
            continue;
        }

        enemies.at(i).moveToward(playerCenterX, playerCenterY, 3);

        if (player.getRect().intersects(enemies.at(i).getRect()))
        {
            player.setSwallowedEnemyType(enemies.at(i).getType());
            enemies.erase(enemies.begin() + i);
            score += 100;
            player.setState(KirbyState::Mouthful_Enemy);
            return;
        }
    }
}

void GameWidget::damageEnemiesInRect(QRect attackRect, int damage)
{
    for (int i = 0; i < static_cast<int>(enemies.size()); i++)
    {
        if (attackRect.intersects(enemies.at(i).getRect()) &&
            enemies.at(i).canBeDestroyed())
        {
            enemies.at(i).takeDamage(damage);

            if (enemies.at(i).getHealth() <= 0)
            {
                enemies.erase(enemies.begin() + i);
                score += 100;
                i--;
            }
        }
    }
}

void GameWidget::executeFireAttack()
{
    std::vector<QRect> fireHitboxes = player.getFireAttackHitboxes();

    for (int i = 0; i < static_cast<int>(fireHitboxes.size()); i++)
    {
        damageEnemiesInRect(fireHitboxes.at(i), 1);
    }
}

void GameWidget::executeSparkAttack()
{
    QRect sparkAttackRect(
        player.getX() - 80,
        player.getY() - 80,
        player.getRect().width() + 160,
        player.getRect().height() + 160
        );

    damageEnemiesInRect(sparkAttackRect, 1);
    sparkAttackTimer = 10;
}

//-------------------------------------------------------------//
//------------------PROJECTILE SECTION------------------------//
//------------------PROJECTILE SECTION------------------------//
//------------------------------------------------------------//

void GameWidget::updateProjectiles()
{
    checkProjectileTooFar();
    checkProjectileSolidCollision();
    checkProjectileEnemyCollision();
    checkEnemyProjectilePlayerCollision();
}

void GameWidget::spawnPlayerProjectile(ProjectileType type, int size, int speed, int damage)
{
    int projectileSpeed = player.getFacingRight() ? speed : -speed;

    int projectileX = player.getFacingRight()
                          ? player.getX() + player.getRect().width()
                          : player.getX() - 30;

    int projectileY = player.getY() + player.getRect().height() / 2;

    projectiles.push_back(
        Projectile(
            projectileX,
            projectileY,
            size,
            projectileSpeed,
            type,
            damage
            )
        );
}

void GameWidget::checkProjectileTooFar()
{
    for (int i = 0; i < static_cast<int>(projectiles.size()); i++)
    {
        projectiles.at(i).move();

        int stageWidth = getCurrentStageWidth();

        if (projectiles.at(i).shouldBeRemoved() ||
            projectiles.at(i).getX() < -500 ||
            projectiles.at(i).getX() > stageWidth + 500)
        {
            projectiles.erase(projectiles.begin() + i);
            i--;
        }
    }
}

void GameWidget::checkEnemyProjectilePlayerCollision()
{
    if (player.getInvincibleTimer() > 0)
    {
        return;
    }

    for (int i = 0; i < static_cast<int>(projectiles.size()); i++)
    {
        if (projectiles.at(i).getType() != ProjectileType::EnemyFire)
        {
            continue;
        }

        if (projectiles.at(i).getRect().intersects(player.getRect()))
        {
            int sourceX = projectiles.at(i).getX();
            int damage = projectiles.at(i).getDamage();

            damagePlayer(damage, sourceX);

            projectiles.erase(projectiles.begin() + i);
            return;
        }
    }
}

void GameWidget::checkProjectileEnemyCollision()
{
    for (int i = 0; i < static_cast<int>(projectiles.size()); i++)
    {
        if (projectiles.at(i).getType() == ProjectileType::EnemyFire)
        {
            continue;
        }
        for (int j = 0; j < static_cast<int>(enemies.size()); j++)
        {
            if (projectiles.at(i).getRect().intersects(enemies.at(j).getRect()))
            {
                if (enemies.at(j).canBeDestroyed())
                {
                    enemies.at(j).takeDamage(projectiles.at(i).getDamage());

                    if (enemies.at(j).getHealth() <= 0)
                    {
                        enemies.erase(enemies.begin() + j);
                        score += 100;
                    }
                }

                projectiles.erase(projectiles.begin() + i);
                return;
            }
        }
    }
}


void GameWidget::checkProjectileSolidCollision()
{
    for (int i = 0; i < static_cast<int>(projectiles.size()); i++)
    {
        QRect projectileRect = projectiles.at(i).getRect();

        // Check collision with blocks
        for (int j = 0; j < static_cast<int>(blocks.size()); j++)
        {
            QRect blockRect(
                blocks.at(j).x,
                blocks.at(j).y,
                blocks.at(j).width,
                blocks.at(j).height
                );

            if (projectileRect.intersects(blockRect))
            {
                projectiles.erase(projectiles.begin() + i);
                i--;
                return;
            }
        }

        // Check collision with floors
        for (int j = 0; j < static_cast<int>(floors.size()); j++)
        {
            QRect floorRect(
                floors.at(j).x,
                floors.at(j).y,
                floors.at(j).width,
                floors.at(j).height
                );

            if (projectileRect.intersects(floorRect))
            {
                projectiles.erase(projectiles.begin() + i);
                i--;
                return;
            }
        }

        // Check collision with platforms
        for (int j = 0; j < static_cast<int>(platforms.size()); j++)
        {
            QRect platformRect(
                platforms.at(j).x,
                platforms.at(j).y,
                platforms.at(j).width,
                platforms.at(j).height
                );

            if (projectileRect.intersects(platformRect))
            {
                projectiles.erase(projectiles.begin() + i);
                i--;
                return;
            }
        }
    }
}

//-----------------------------------------------------------------//
//------------------------COLLISION SECTION------------------------//
//------------------------COLLISION SECTION------------------------//
//-----------------------------------------------------------------//

void GameWidget::checkEnemyCollision()
{
    for (int i = 0; i < static_cast<int>(enemies.size()); i++)
    {
        if (!player.getRect().intersects(enemies.at(i).getRect()))
        {
            continue;
        }

        bool kirbyIsConsuming =
            player.getState() == KirbyState::Inhaling ||
            player.getState() == KirbyState::Mouthful_Enemy;

        bool enemyCanBeConsumed = enemies.at(i).canBeInhaled();

        if (kirbyIsConsuming && enemyCanBeConsumed)
        {
            continue;
        }

        damagePlayer(1, enemies.at(i).getX());
    }
}

void GameWidget::checkEnemyAbilityCollision()
{
    if (player.getInvincibleTimer() > 0)
    {
        return;
    }

    for (int i = 0; i < static_cast<int>(enemies.size()); i++)
    {
        if (enemies.at(i).getType() == EnemyType::HotHead)
        {
            std::vector<QRect> fireHitboxes = enemies.at(i).getFireHitboxes();

            for (int j = 0; j < static_cast<int>(fireHitboxes.size()); j++)
            {
                if (player.getRect().intersects(fireHitboxes.at(j)))
                {
                    damagePlayer(1, enemies.at(i).getX());
                    return;
                }
            }
        }

        if (enemies.at(i).getType() == EnemyType::Sparky)
        {
            QRect sparkHitbox = enemies.at(i).getSparkHitbox();

            if (player.getRect().intersects(sparkHitbox))
            {
                damagePlayer(1, enemies.at(i).getX());
                return;
            }
        }
    }
}

void GameWidget::checkItemCollision()
{
    for (int i = 0; i < static_cast<int>(items.size()); i++)
    {
        if (player.getRect().intersects(items.at(i).getRect()))
        {
            if (items.at(i).getType() == ItemType::MaximTomato)
            {
                player.setHealth(3);
            }
            else if (items.at(i).getType() == ItemType::OneUp)
            {
                if (lives < 3)
                {
                    lives++;
                }
            }
            items.erase(items.begin() + i);
            score += 300;
            i--;
        }
    }
}

//-----------------------------------------------------------------//
//------------------------GAME FLOW SECTION------------------------//
//------------------------GAME FLOW SECTION------------------------//
//-----------------------------------------------------------------//

void GameWidget::startGame()
{
    lives = 3;
    player.setHealth(3);
    score = 0;

    storedAbilities.clear();
    selectedAbilityIndex = 0;
    abilityMenuOpen = false;

    currentStage = 1;
    currentGameState = GameState::Playing;

    loadStage1();
}

void GameWidget::respawnPlayer()
{
    abilityMenuOpen = false;
    player.setVelocityX(0);
    player.setHealth(3);

    if (currentStage == 1)
    {
        loadStage1();
    } else if (currentStage == 2)
    {
        loadStage2();
    }
}

bool GameWidget::checkPortalAndGoal()
{
    if (currentGameState != GameState::Playing)
    {
        return false;
    }

    if (currentStage == 1 && player.getRect().intersects(portalRect))
    {
        currentStage = 2;
        score += 1000;
        loadStage2();
        return true;
    }

    if (currentStage == 2 && player.getRect().intersects(goalRect))
    {
        currentGameState = GameState::Win;
        return true;
    }
    return false;
}

//-----------------------------------------------------------------//
//------------------------STAGE SECTION------------------------//
//------------------------STAGE SECTION------------------------//
//-----------------------------------------------------------------//

void GameWidget::loadStageData(const StageData &stage)
{
    resetKirbyForStage();
    clearStage();

    currentStageWidth = stage.width;

    player.setPosition(stage.playerStartX, stage.playerStartY);

    portalRect = stage.portalRect;
    goalRect = stage.goalRect;

    platforms = stage.platforms;
    blocks = stage.blocks;
    floors = stage.floors;
    items = stage.items;
    enemies = stage.enemies;
    terrainSprites = stage.terrainSprites;
}

void GameWidget::resetKirbyForStage()
{
    abilityMenuOpen = false;
    player.setState(KirbyState::Normal);
    player.setHovering(false);
    player.setCrouched(false);


    player.setInvincibleTimer(0);
    player.setVelocityX(0);
    player.setVelocityY(0);
    player.stopAttackAnimation();

    sparkAttackTimer = 0;
}

void GameWidget::clearStage()
{
    platforms.clear();
    blocks.clear();
    items.clear();
    enemies.clear();
    projectiles.clear();
    floors.clear();
    terrainSprites.clear();

    player.setVelocityX(0);
    cameraX = 0;
}

int GameWidget::getCurrentStageWidth() const
{
    return currentStageWidth;
}

void GameWidget::loadStage1()
{
    loadStageData(createStage1());
}

void GameWidget::loadStage2()
{
    loadStageData(createStage2());
}

//-------------------------------------------------------------//
//------------------------DRAW SECTION------------------------//
//------------------------DRAW SECTION------------------------//
//------------------------------------------------------------//

void GameWidget::drawMenus(QPainter &painter)
{
    if(currentGameState == GameState::StartMenu)
    {
        painter.drawPixmap(0, 0, width(), height(),
                           Assets::instance().getPixmap("start_menu"));
        return;
    } else if (currentGameState == GameState::GameOver)
    {
        if (gameOverSelection == 0)
        {
            painter.drawPixmap(0, 0, width(), height(),
                               Assets::instance().getPixmap("game_over_continue"));
            return;
        } else if (gameOverSelection == 1)
        {
            painter.drawPixmap(0, 0, width(), height(),
                               Assets::instance().getPixmap("game_over_quit"));
            return;
        }
    } else if (currentGameState == GameState::Win)
    {
        painter.drawPixmap(0, 0, width(), height(),
                           Assets::instance().getPixmap("game_clear"));
        return;
    }

}

void GameWidget::drawStageTiles(QPainter &painter)
{
    painter.setBrush(QColor(100,150,40));
    for (int i = 0; i < static_cast<int>(floors.size()); i++)
    {
        painter.drawRect(floors.at(i).x - cameraX, floors.at(i).y, floors.at(i).width, floors.at(i).height);
    }

    for (const TerrainSprite& terrain : terrainSprites)
    {
        painter.drawPixmap(
            terrain.x - cameraX,
            terrain.y,
            terrain.width,
            terrain.height,
            Assets::instance().getPixmap(terrain.spriteName)
            );
    }

    for (int i = 0; i < static_cast<int>(platforms.size()); i++)
    {
        painter.drawPixmap(platforms.at(i).x - cameraX,
                           platforms.at(i).y,
                           platforms.at(i).width,
                           platforms.at(i).height,
                           Assets::instance().getPixmap("platform"));
    }

    for (int i = 0; i < static_cast<int>(blocks.size()); i++)
    {
        painter.drawPixmap(blocks.at(i).x - cameraX,
                           blocks.at(i).y,
                           blocks.at(i).width,
                           blocks.at(i).height,
                           Assets::instance().getPixmap("block"));
    }
}

void GameWidget::drawStageObjects(QPainter &painter)
{
    if (currentStage == 1)
    {
        painter.drawPixmap(portalRect.x() - cameraX,
                           portalRect.y(),
                           portalRect.width(),
                           portalRect.height(),
                           Assets::instance().getPixmap("door"));
    }
    else if (currentStage == 2)
    {
        painter.drawPixmap(goalRect.x() - cameraX,
                           goalRect.y(),
                           goalRect.width(),
                           goalRect.height(),
                           Assets::instance().getPixmap("goal_door"));
    }

    for (int i = 0; i < static_cast<int>(items.size()); i++)
    {
        items.at(i).draw(painter, cameraX);
    }
}

void GameWidget::drawEnemies(QPainter &painter)
{
    for (int i = 0; i < static_cast<int>(enemies.size()); i++)
    {
        enemies.at(i).drawSparkyAttack(painter, cameraX);
    }

    for (int i = 0; i < static_cast<int>(enemies.size()); i++)
    {
        enemies.at(i).draw(painter, cameraX);
    }

    for (int i = 0; i < static_cast<int>(enemies.size()); i++)
    {
        enemies.at(i).drawFire(painter, cameraX);
    }
}

void GameWidget::drawProjectiles(QPainter &painter)
{
    for (int i = 0; i < static_cast<int>(projectiles.size()); i++)
    {
        projectiles.at(i).draw(painter, cameraX);
    }
}

void GameWidget::drawHUD(QPainter &painter)
{
    int startX = 600;
    int startY = height() - 100;

    int lifeIconWidth = 90;
    int lifeIconHeight = 60;

    int hpIconWidth = 50;
    int hpIconHeight = 80;

    int currentHp = player.getHealth();
    int maxHp = 3;

    painter.drawPixmap(
        startX,
        startY + 15,
        lifeIconWidth,
        lifeIconHeight,
        Assets::instance().getPixmap("life")
        );

    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 26, QFont::Bold));

    int livesTextX = startX + lifeIconWidth + 10;

    painter.drawText(
        livesTextX,
        startY + 48,
        "x " + QString::number(lives)
        );

    int hpStartX = livesTextX + 70 + 50;

    for (int i = 0; i < maxHp; i++)
    {
        QString spriteName;

        if (i < currentHp)
        {
            spriteName = "hp_1";
        }
        else
        {
            spriteName = "hp_0";
        }

        painter.drawPixmap(
            hpStartX + i * 60,
            startY,
            hpIconWidth,
            hpIconHeight,
            Assets::instance().getPixmap(spriteName)
            );
    }

    painter.drawPixmap(
        1320,
        1008,
        250,
        60,
        Assets::instance().getPixmap("hud_score")
        );

    painter.drawText(1350, 1050, "Score: " + QString::number(score));

    if (player.getState() == KirbyState::FireAbility)
    {
        painter.drawPixmap(50, 900, 150, 160, Assets::instance().getPixmap("fire_icon"));
    }
    else if (player.getState() == KirbyState::SparkAbility)
    {
        painter.drawPixmap(50, 900, 150, 160, Assets::instance().getPixmap("spark_icon"));
    }
}

void GameWidget::drawAbilityMenu(QPainter &painter)
{
    if (!abilityMenuOpen)
    {
        return;
    }

    painter.setBrush(QColor(0, 0, 0, 120));
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, 0, width(), height());

    QString menuSprite = getAbilityMenuSpriteName();

    painter.drawPixmap(
        0,
        0,
        width(),
        height(),
        Assets::instance().getPixmap(menuSprite)
        );
}

void GameWidget::drawBackground(QPainter &painter)
{
    QString backgroundName;

    if (currentStage == 1)
    {
        backgroundName = "background_1";
    }
    else if (currentStage == 2)
    {
        backgroundName = "background_2";
    }

    QPixmap background = Assets::instance().getPixmap(backgroundName);

    int bgWidth = background.width();

    int maxCameraX = currentStageWidth - width();
    int maxBgScroll = bgWidth - width();

    int bgX = 0;

    if (maxCameraX > 0 && maxBgScroll > 0)
    {
        bgX = cameraX * maxBgScroll / maxCameraX;
    }

    painter.drawPixmap(
        0,
        0,
        width(),
        height(),
        background,
        bgX,
        0,
        width(),
        height()
        );
}

//-------------------------------------------------------------//
//-----------------ABILITY MENU SECTION------------------------//
//-----------------ABILITY MENU SECTION------------------------//
//------------------------------------------------------------//

void GameWidget::storeSwallowedEnemyAbility()
{
    if (player.getSwallowedEnemyType() == EnemyType::HotHead)
    {
        addAbilityToStorage(KirbyState::FireAbility);
    }
    else if (player.getSwallowedEnemyType() == EnemyType::Sparky)
    {
        addAbilityToStorage(KirbyState::SparkAbility);
    }
    else
    {
        player.setState(KirbyState::Normal);
    }
}

bool GameWidget::isAbilityState(KirbyState state) const
{
    return state == KirbyState::FireAbility ||
           state == KirbyState::SparkAbility;
}

bool GameWidget::hasStoredAbility(KirbyState ability) const
{
    for (int i = 0; i < static_cast<int>(storedAbilities.size()); i++)
    {
        if (storedAbilities.at(i) == ability)
        {
            return true;
        }
    }

    return false;
}

void GameWidget::addAbilityToStorage(KirbyState ability)
{
    if (!isAbilityState(ability))
    {
        return;
    }

    selectedMenuAbility = ability;

    for (int i = 0; i < static_cast<int>(storedAbilities.size()); i++)
    {
        if (storedAbilities.at(i) == ability)
        {
            selectedAbilityIndex = i;
            player.setState(ability);
            return;
        }
    }

    storedAbilities.push_back(ability);
    selectedAbilityIndex = storedAbilities.size() - 1;

    player.setState(ability);
}

void GameWidget::equipSelectedAbility()
{
    if (!hasStoredAbility(selectedMenuAbility))
    {
        return;
    }

    player.setState(selectedMenuAbility);

    for (int i = 0; i < static_cast<int>(storedAbilities.size()); i++)
    {
        if (storedAbilities.at(i) == selectedMenuAbility)
        {
            selectedAbilityIndex = i;
            break;
        }
    }

    fireAttack = false;
    sparkAttack = false;
    player.stopAttackAnimation();
}

void GameWidget::dropSelectedAbility()
{
    if (!hasStoredAbility(selectedMenuAbility))
    {
        return;
    }

    for (int i = 0; i < static_cast<int>(storedAbilities.size()); i++)
    {
        if (storedAbilities.at(i) == selectedMenuAbility)
        {
            storedAbilities.erase(storedAbilities.begin() + i);
            break;
        }
    }

    if (player.getState() == selectedMenuAbility)
    {
        player.setState(KirbyState::Normal);
    }

    if (selectedAbilityIndex >= static_cast<int>(storedAbilities.size()))
    {
        selectedAbilityIndex = storedAbilities.size() - 1;
    }

    if (storedAbilities.empty())
    {
        selectedAbilityIndex = 0;
    }

    fireAttack = false;
    sparkAttack = false;
    player.stopAttackAnimation();
}

QString GameWidget::getAbilityMenuSpriteName() const
{
    bool hasFire = hasStoredAbility(KirbyState::FireAbility);
    bool hasSpark = hasStoredAbility(KirbyState::SparkAbility);

    if (selectedMenuAbility == KirbyState::FireAbility)
    {
        if (hasFire && hasSpark)
        {
            return "menu_screen";
        }
        else if (!hasFire && hasSpark)
        {
            return "menu_screen_select_fire_nofire";
        }
        else if (hasFire && !hasSpark)
        {
            return "menu_screen_select_fire_no_spark";
        }
        else
        {
            return "menu_screen_select_fire_nofire_nospark";
        }
    }

    if (selectedMenuAbility == KirbyState::SparkAbility)
    {
        if (hasFire && hasSpark)
        {
            return "menu_screen_spark";
        }
        else if (!hasFire && hasSpark)
        {
            return "menu_screen_spark_no_fire";
        }
        else if (hasFire && !hasSpark)
        {
            return "menu_screen_spark_no_spark";
        }
        else
        {
            return "menu_screen_spark_no_fire_no_spark";
        }
    }

    return "menu_screen_select_fire_nofire_nospark";
}

void GameWidget::storeCurrentAbility()
{
    if (!isAbilityState(player.getState()))
    {
        return;
    }

    KirbyState currentAbility = player.getState();

    // Make sure the current ability is stored.
    if (!hasStoredAbility(currentAbility))
    {
        storedAbilities.push_back(currentAbility);
    }

    // Keep menu selection on the stored ability.
    selectedMenuAbility = currentAbility;

    // Kirby returns to normal, but the ability is not deleted.
    player.setState(KirbyState::Normal);

    fireAttack = false;
    sparkAttack = false;
    player.stopAttackAnimation();
}

//-----------------------------------------------------------------//
//------------------------CAMERA SECTION------------------------//
//------------------------CAMERA SECTION------------------------//
//-----------------------------------------------------------------//

void GameWidget::updateCamera()
{
    int screenCenter = width() / 2;

    cameraX = player.getX() - screenCenter;

    if (cameraX < 0)
    {
        cameraX = 0;
    }

    int maxCameraX = getCurrentStageWidth() - width();

    if (cameraX > maxCameraX)
    {
        cameraX = maxCameraX;
    }
}