#include "stagedata.h"

StageData createStage1()
{
    StageData stage;

    stage.width = 4860;
    stage.playerStartX = 100;
    stage.playerStartY = 700;

    stage.terrainSprites.push_back({0, 800, 1570, 310, "stage1_1"});
    stage.terrainSprites.push_back({1870, 280, 1570, 829, "stage1_2"});
    stage.terrainSprites.push_back({3690, 750, 1570, 418, "stage1_3"});

    // Floors
    stage.floors.push_back({0, 850, 1570, 650});

    stage.floors.push_back({1900, 850, 1520, 650});
    stage.floors.push_back({2100, 750, 1330, 650});
    stage.floors.push_back({2750, 300, 150, 800});
    stage.floors.push_back({2900, 520, 100, 300});

    stage.floors.push_back({3700, 900, 1570, 300});
    stage.floors.push_back({4150, 800, 630, 300});

    // Platforms
    stage.platforms.push_back({300, 690, 260, 30});
    stage.platforms.push_back({1150, 700, 260, 30});
    stage.platforms.push_back({1650, 550, 260, 30});

    stage.platforms.push_back({2350, 560, 220, 30});
    stage.platforms.push_back({2550, 420, 180, 30});

    stage.platforms.push_back({3800, 530, 220, 30});
    stage.platforms.push_back({4200, 680, 220, 30});


    // Blocks
    stage.blocks.push_back({650, 770, 80, 80});
    stage.blocks.push_back({1450, 770, 80, 80});
    stage.blocks.push_back({2200, 670, 80, 80});

    stage.blocks.push_back({4450, 720, 80, 80});
    stage.blocks.push_back({4450, 640, 80, 80});
    stage.blocks.push_back({4450, 560, 80, 80});
    stage.blocks.push_back({4450, 480, 80, 80});
    stage.blocks.push_back({4450, 400, 80, 80});

    // Items
    stage.items.push_back({2950, 450, 50, 50, ItemType::MaximTomato});

    // Enemies
    stage.enemies.push_back({500, 760, 50, 50, 1, 1, EnemyType::WaddleDee});
    stage.enemies.push_back({1050, 760, 50, 50, 1, 1, EnemyType::WaddleDee});

    stage.enemies.push_back({1650, 680, 50, 50, 1, 999, EnemyType::Gordo});

    stage.enemies.push_back({2150, 700, 50, 50, 1, 1, EnemyType::WaddleDee});
    stage.enemies.push_back({3000, 470, 50, 50, 1, 1, EnemyType::WaddleDee});

    stage.enemies.push_back({3350, 500, 50, 50, 1, 999, EnemyType::Gordo});

    stage.enemies.push_back({3900, 850, 50, 50, 1, 1, EnemyType::WaddleDee});
    stage.enemies.push_back({4350, 750, 50, 50, 1, 1, EnemyType::WaddleDee});

    stage.portalRect = QRect(4600, 650, 150, 150);
    stage.goalRect = QRect();

    return stage;
}

StageData createStage2()
{
    StageData stage;

    stage.width = 8100;
    stage.playerStartX = 100;
    stage.playerStartY = 500;

    stage.terrainSprites.push_back({0, 800, 1358, 296, "stage2_1"});
    stage.terrainSprites.push_back({1358, 800, 1358, 296, "stage2_2"});
    stage.terrainSprites.push_back({2716, 800, 1358, 296, "stage2_1"});
    stage.terrainSprites.push_back({4074, 800, 1358, 296, "stage2_2"});
    stage.terrainSprites.push_back({5432, 800, 1358, 296, "stage2_1"});
    stage.terrainSprites.push_back({6790, 800, 1358, 296, "stage2_2"});

    // Floors
    stage.floors.push_back({0, 830, 8100, 650});

    // Platforms
    stage.platforms.push_back({300, 720, 240, 30});
    stage.platforms.push_back({650, 640, 220, 30});
    stage.platforms.push_back({1000, 560, 220, 30});
    stage.platforms.push_back({1250, 700, 240, 30});

    stage.platforms.push_back({1550, 720, 220, 30});
    stage.platforms.push_back({1850, 620, 220, 30});
    stage.platforms.push_back({2150, 520, 200, 30});
    stage.platforms.push_back({2450, 650, 220, 30});

    stage.platforms.push_back({3170, 650, 200, 30});
    stage.platforms.push_back({3450, 560, 200, 30});
    stage.platforms.push_back({3750, 470, 180, 30});

    stage.platforms.push_back({4450, 720, 240, 30});
    stage.platforms.push_back({4800, 630, 220, 30});
    stage.platforms.push_back({5150, 540, 200, 30});
    stage.platforms.push_back({5500, 680, 240, 30});

    stage.platforms.push_back({5900, 730, 220, 30});
    stage.platforms.push_back({6250, 630, 220, 30});
    stage.platforms.push_back({6600, 530, 200, 30});
    stage.platforms.push_back({6950, 650, 220, 30});
    stage.platforms.push_back({7300, 720, 240, 30});
    stage.platforms.push_back({7600, 650, 220, 30});

    // Blocks
    stage.blocks.push_back({850, 750, 80, 80});
    stage.blocks.push_back({930, 750, 80, 80});

    stage.blocks.push_back({1700, 750, 80, 80});
    stage.blocks.push_back({2300, 750, 80, 80});

    stage.blocks.push_back({3000, 750, 80, 80});
    stage.blocks.push_back({3080, 750, 80, 80});
    stage.blocks.push_back({3080, 670, 80, 80});

    stage.blocks.push_back({4000, 750, 80, 80});
    stage.blocks.push_back({4000, 670, 80, 80});

    stage.blocks.push_back({5000, 750, 80, 80});
    stage.blocks.push_back({5080, 750, 80, 80});

    stage.blocks.push_back({6100, 750, 80, 80});
    stage.blocks.push_back({6100, 670, 80, 80});

    stage.blocks.push_back({7050, 750, 80, 80});
    stage.blocks.push_back({7130, 750, 80, 80});
    stage.blocks.push_back({7450, 750, 80, 80});

    // Items
    stage.items.push_back({2200, 360, 50, 50, ItemType::OneUp});

    // Enemies
    stage.enemies.push_back({500, 760, 50, 50, 1, 1, EnemyType::WaddleDee});
    stage.enemies.push_back({1000, 510, 50, 50, 1, 1, EnemyType::WaddleDee});
    stage.enemies.push_back({1250, 560, 50, 50, 1, 999, EnemyType::Gordo});

    stage.enemies.push_back({1850, 570, 50, 50, 1, 1, EnemyType::HotHead});
    stage.enemies.push_back({2450, 600, 50, 50, 1, 1, EnemyType::WaddleDee});
    stage.enemies.push_back({2700, 760, 50, 50, 1, 999, EnemyType::Gordo});
    stage.enemies.push_back({2150, 510, 50, 50, 1, 1, EnemyType::Sparky});

    stage.enemies.push_back({3150, 600, 50, 50, 1, 1, EnemyType::WaddleDee});
    stage.enemies.push_back({3450, 510, 50, 50, 1, 1, EnemyType::Sparky});
    stage.enemies.push_back({3750, 420, 50, 50, 1, 1, EnemyType::WaddleDee});
    stage.enemies.push_back({4100, 560, 50, 50, 1, 999, EnemyType::Gordo});

    stage.enemies.push_back({4550, 670, 50, 50, 1, 1, EnemyType::HotHead});
    stage.enemies.push_back({5000, 580, 50, 50, 1, 1, EnemyType::WaddleDee});
    stage.enemies.push_back({5300, 760, 50, 50, 1, 1, EnemyType::Sparky});
    stage.enemies.push_back({5600, 630, 50, 50, 1, 999, EnemyType::Gordo});

    stage.enemies.push_back({6000, 680, 50, 50, 1, 1, EnemyType::WaddleDee});
    stage.enemies.push_back({6250, 580, 50, 50, 1, 1, EnemyType::HotHead});
    stage.enemies.push_back({6600, 480, 50, 50, 1, 1, EnemyType::Sparky});
    stage.enemies.push_back({6950, 600, 50, 50, 1, 999, EnemyType::Gordo});

    stage.enemies.push_back({7300, 670, 50, 50, 1, 1, EnemyType::WaddleDee});
    stage.enemies.push_back({7600, 600, 50, 50, 1, 1, EnemyType::HotHead});

    stage.portalRect = QRect();
    stage.goalRect = QRect(7900, 630, 120, 200);

    return stage;
}