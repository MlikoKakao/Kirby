#include <QDebug>
#include <cmath>
#include "enemy.h"

Enemy::Enemy(int startX, int startY, int startWidth, int startHeight, int startSpeed, int startHealth, EnemyType startType):
    x(startX),
    y(startY),
    width(startWidth),
    height(startHeight),
    speed(startSpeed),
    health(startHealth),
    type(startType),
    moveTimer(QRandomGenerator::global()->bounded(0, 240)),
    turnTime(QRandomGenerator::global()->bounded(160, 321)),
    direction(QRandomGenerator::global()->bounded(0, 2) == 0 ? -1 : 1),
    velocityY(0.0f),
    isGrounded(false),
    animationTimer(QRandomGenerator::global()->bounded(0, 30)),
    animationFrame(0),
    projectileCooldown(QRandomGenerator::global()->bounded(0, 90)),
    aiState(EnemyAIState::Walking),
    attackTimer(0),
    attackCooldown(QRandomGenerator::global()->bounded(0, 120)),
    prepareAttackTimer(0),
    jumpCooldown(QRandomGenerator::global()->bounded(0, 90)),
    attackAnimationTimer(0)
{}

QRect Enemy::getRect() const { return QRect(x, y, width, height); }
int Enemy::getX() const { return x; }
int Enemy::getY() const { return y; }
float Enemy::getVelocityY() const { return velocityY; }
int Enemy::getDirection() const { return direction; }

QString Enemy::directionalSprite(const QString& right, const QString& left) const { return direction > 0 ? right : left; }
bool Enemy::isAttacking() const { return aiState == EnemyAIState::Attacking; }
void Enemy::setX(int newX) { x = newX; }
void Enemy::setY(int newY) { y = newY; }
void Enemy::setVelocityY(float newVelocityY) { velocityY = newVelocityY; }
void Enemy::setGrounded(bool grounded) { isGrounded = grounded; }
void Enemy::setMoveTimer(int mt) {moveTimer = mt; }


void Enemy::takeDamage(int damage) { health -= damage; }
int Enemy::getHealth() const { return health; }
// I think all Kirby enemies have 1 health so this might be useless
// But I'm not sure and this makes it possible later to do if I'm wrong

EnemyType Enemy::getType() const { return type; }
bool Enemy::canBeInhaled() const { return type != EnemyType::Gordo; }
bool Enemy::canBeDestroyed() const { return type != EnemyType::Gordo; }

void Enemy::move(int speed)
{
    // enemies should not move while preparing/attacking
    if (aiState == EnemyAIState::PreparingAttack ||
        aiState == EnemyAIState::Attacking)
    {
        return;
    }

    // special movement for Sparky
    if (type == EnemyType::Sparky)
    {
        const float sparkyJumpPower = -10.0f;


        moveTimer++;

        if (jumpCooldown > 0)
        {
            jumpCooldown--;
        }

        if (isGrounded && jumpCooldown <= 0)
        {
            velocityY = sparkyJumpPower;
            jumpCooldown = QRandomGenerator::global()->bounded(70, 131);
            turnAround();
            isGrounded = false;
        }

        if (!isGrounded && moveTimer % 3 == 0)
        {
            x += direction * 3;
        }

        return;
    }

    // normal walking movement for Waddle Dee / Hot Head / etc.
    moveTimer++;

    if (moveTimer % 2 == 0)
    {
        x += speed * direction;
    }

    if (moveTimer > turnTime)
    {
        turnAround();

        moveTimer = 0;
        turnTime = QRandomGenerator::global()->bounded(160, 321);
    }
}

void Enemy::draw(QPainter &painter, int cameraX) const
{
    QString spriteName = getCurrentSpriteName();

    if (type == EnemyType::Sparky && aiState == EnemyAIState::PreparingAttack)
    {
        painter.drawPixmap(
            x - cameraX,
            y,
            width,
            height,
            Assets::instance().getPixmap(spriteName)
            );

        return;
    }

    if (type == EnemyType::Sparky && aiState == EnemyAIState::Attacking)
    {
        painter.drawPixmap(
            x - cameraX,
            y,
            width,
            height,
            Assets::instance().getPixmap(spriteName)
            );

        return;
    }

    painter.drawPixmap(
        x - cameraX,
        y,
        width,
        height,
        Assets::instance().getPixmap(spriteName)
        );
}

QString Enemy::getCurrentSpriteName() const
{
    if (type == EnemyType::Gordo)
    {
        return animationFrame == 0 ? "gordo1" : "gordo2";
    }

    if (type == EnemyType::WaddleDee)
    {
        if (direction > 0)
        {
            if (animationFrame == 0)
            {
                return "waddle_dee0_r";
            }
            else if (animationFrame == 1)
            {
                return "waddle_dee1_r";
            }
            else
            {
                return "waddle_dee2_r";
            }
        }
        else
        {
            if (animationFrame == 0)
            {
                return "waddle_dee0_l";
            }
            else if (animationFrame == 1)
            {
                return "waddle_dee1_l";
            }
            else
            {
                return "waddle_dee2_l";
            }
        }
    }

    if (type == EnemyType::Sparky)
    {
        if (aiState == EnemyAIState::PreparingAttack)
        {
            return "sparky_attack1";
        }

        if (aiState == EnemyAIState::Attacking)
        {
            return "sparky_attack1";
        }

        if (animationFrame == 0)
        {
            return directionalSprite("sparky1_r", "sparky1_l");
        }

        return directionalSprite("sparky2_r", "sparky2_l");
    }

    if (type == EnemyType::HotHead)
    {
        if (aiState == EnemyAIState::PreparingAttack)
        {
            return directionalSprite("hot_head_stop_r", "hot_head_stop_l");
        }

        if (aiState == EnemyAIState::Attacking)
        {
            return directionalSprite("hot_head_attack_r", "hot_head_attack_l");
        }

        return directionalSprite(
            "hot_head_walk_r",
            "hot_head_walk_l"
            );
    }

    return "";
}

void Enemy::moveToward(int targetX, int targetY, int speed)
{
    int enemyCenterX = x + width / 2;
    int enemyCenterY = y + height / 2;

    if (enemyCenterX < targetX) {
        x += speed;
    } else if (enemyCenterX > targetX) {
        x -= speed;
    }

    if (enemyCenterY < targetY) {
        y += speed;
    } else if (enemyCenterY > targetY) {
        y -= speed;
    }
}

void Enemy::applyGravity(float gravity)
{
    velocityY += gravity;

    float maxFallSpeed = 12.0f;

    if (type == EnemyType::Sparky)
    {
        maxFallSpeed = 6.0f;
    }

    if (velocityY > maxFallSpeed)
    {
        velocityY = maxFallSpeed;
    }

    y += static_cast<int>(velocityY);
}

void Enemy::updateAnimation()
{
    animationTimer++;

    int animationSpeed = 15;
    int maxFrame = 1;

    if (type == EnemyType::WaddleDee)
    {
        animationSpeed = 10;
        maxFrame = 2;
    }

    if(animationTimer >= animationSpeed)
    {
        animationFrame++;

        if (animationFrame > maxFrame)
        {
            animationFrame = 0;
        }

        animationTimer = 0;
    }
}

void Enemy::updateAI(const QRect& playerRect)
{
    if (type != EnemyType::HotHead && type != EnemyType::Sparky)
    {
        return;
    }

    if (attackCooldown > 0)
    {
        attackCooldown--;
    }

    int enemyCenterX = x + width / 2;
    int enemyCenterY = y + height / 2;

    int playerCenterX = playerRect.x() + playerRect.width() / 2;
    int playerCenterY = playerRect.y() + playerRect.height() / 2;

    int distanceX = std::abs(playerCenterX - enemyCenterX);
    int distanceY = std::abs(playerCenterY - enemyCenterY);

    if (type == EnemyType::HotHead)
    {
        bool playerCloseEnough = distanceX < 500;
        bool playerSameHeight = distanceY < 120;

        if (aiState == EnemyAIState::PreparingAttack)
        {
            prepareAttackTimer--;

            direction = playerCenterX > enemyCenterX ? 1 : -1;

            if (prepareAttackTimer <= 0)
            {
                aiState = EnemyAIState::Attacking;
                attackTimer = 45;
                attackAnimationTimer = 0;
            }

            return;
        }

        if (aiState == EnemyAIState::Attacking)
        {
            attackTimer--;
            attackAnimationTimer++;

            if (attackTimer <= 0)
            {
                aiState = EnemyAIState::Walking;
                attackCooldown = 90;
            }

            return;
        }

        if (aiState == EnemyAIState::Walking)
        {
            if (playerCloseEnough && playerSameHeight && attackCooldown <= 0)
            {
                direction = playerCenterX > enemyCenterX ? 1 : -1;

                aiState = EnemyAIState::PreparingAttack;
                prepareAttackTimer = 50;
            }
        }

        return;
    }

    if (type == EnemyType::Sparky)
    {
        if (aiState == EnemyAIState::PreparingAttack)
        {
            prepareAttackTimer--;

            if (prepareAttackTimer <= 0)
            {
                aiState = EnemyAIState::Attacking;
                attackTimer = 40;
                attackAnimationTimer = 0;
            }

            return;
        }

        if (aiState == EnemyAIState::Attacking)
        {
            attackTimer--;
            attackAnimationTimer++;

            if (attackTimer <= 0)
            {
                aiState = EnemyAIState::Walking;
                attackCooldown = 120;
            }

            return;
        }

        if (aiState == EnemyAIState::Walking)
        {
            if (attackCooldown <= 0 && isGrounded)
            {
                aiState = EnemyAIState::PreparingAttack;
                prepareAttackTimer = 25;

                velocityY = 0;
            }
        }

        return;
    }
}

// Basically same as Kirby logic but idk how to use it for both,
// because then makes conflicts where it should live
std::vector<QRect> Enemy::getFireHitboxes() const
{
    std::vector<QRect> hitboxes;

    if (type != EnemyType::HotHead || aiState != EnemyAIState::Attacking)
    {
        return hitboxes;
    }

    int enemyFrontX = direction > 0
                          ? x + width
                          : x;

    int enemyCenterY = y + height / 2;

    QRect closeFire(
        direction > 0 ? enemyFrontX : enemyFrontX - 90,
        enemyCenterY - 60,
        90,
        120
        );

    QRect middleFire(
        direction > 0 ? enemyFrontX + 70 : enemyFrontX - 170,
        enemyCenterY - 45,
        100,
        90
        );

    QRect farFire(
        direction > 0 ? enemyFrontX + 150 : enemyFrontX - 240,
        enemyCenterY - 30,
        90,
        60
        );

    hitboxes.push_back(closeFire);
    hitboxes.push_back(middleFire);
    hitboxes.push_back(farFire);

    return hitboxes;
}

// This is Hot Head fire
void Enemy::drawFire(QPainter& painter, int cameraX) const
{
    if (type != EnemyType::HotHead || aiState != EnemyAIState::Attacking)
    {
        return;
    }

    QString spriteName = getCurrentFireSpriteName();

    if (spriteName == "")
    {
        return;
    }

    int fireWidth = 240;
    int fireHeight = 130;

    int fireX;
    int fireY = y + height / 2 - fireHeight / 2;

    if (direction > 0)
    {
        fireX = x + width - 20;
    }
    else
    {
        fireX = x - fireWidth + 20;
    }

    painter.drawPixmap(
        fireX - cameraX,
        fireY,
        fireWidth,
        fireHeight,
        Assets::instance().getPixmap(spriteName)
        );
}

QString Enemy::getCurrentFireSpriteName() const
{
    if (type != EnemyType::HotHead || aiState != EnemyAIState::Attacking)
    {
        return "";
    }

    int fireFrame = attackTimer / 8;


    if (fireFrame % 3 == 1)
    {
        return directionalSprite("hot_head_fire1_r", "hot_head_fire1_l");
    }

    return directionalSprite("hot_head_fire2_r", "hot_head_fire2_l");
}

bool Enemy::shouldShootProjectile(const QRect& playerRect)
{
    if (type != EnemyType::HotHead)
    {
        return false;
    }

    if (projectileCooldown > 0)
    {
        projectileCooldown--;
        return false;
    }

    int enemyCenterX = x + width / 2;
    int enemyCenterY = y + height / 2;

    int playerCenterX = playerRect.x() + playerRect.width() / 2;
    int playerCenterY = playerRect.y() + playerRect.height() / 2;

    int distanceX = std::abs(playerCenterX - enemyCenterX);
    int distanceY = std::abs(playerCenterY - enemyCenterY);

    bool playerSameY = distanceY < 80;

    int flamethrowerRange = 260;
    int projectileRange = 500;

    bool tooFarForFlamethrower = distanceX > flamethrowerRange;
    bool closeEnoughForProjectile = distanceX < projectileRange;

    if (playerSameY &&
        tooFarForFlamethrower &&
        closeEnoughForProjectile)
    {
        direction = playerCenterX > enemyCenterX ? 1 : -1;

        projectileCooldown = 90;
        return true;
    }

    return false;
}

QRect Enemy::getSparkHitbox() const
{
    if (type != EnemyType::Sparky || aiState != EnemyAIState::Attacking)
    {
        return QRect();
    }

    return QRect(
        x - 70,
        y - 70,
        width + 140,
        height + 140
        );
}

void Enemy::drawSparkyAttack(QPainter& painter, int cameraX) const
{
    if (type != EnemyType::Sparky || aiState != EnemyAIState::Attacking)
    {
        return;
    }

    QRect sparkRect = getSparkHitbox();

    painter.drawPixmap(
        sparkRect.x() - cameraX,
        sparkRect.y(),
        sparkRect.width(),
        sparkRect.height(),
        Assets::instance().getPixmap("sparky_attack2")
        );
}

void Enemy::turnAround()
{
    direction *= -1;
}