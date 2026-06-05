#include "assets.h"
#include <QDebug>

Assets& Assets::instance()
{
    static Assets assets;
    return assets;
}

void Assets::loadPixmap(const QString& name, const QString& path)
{
    QPixmap pixmap;

    if (!pixmap.load(path)) {
        qDebug() << "Failed to load pixmap:" << path;
    }

    pixmaps.insert(name, pixmap);
}

const QPixmap& Assets::getPixmap(const QString& name) const
{
    auto it = pixmaps.find(name);

    if (it == pixmaps.end()) {
        static QPixmap empty;
        qDebug() << "Missing pixmap:" << name;
        return empty;
    }

    return it.value();
}

void Assets::loadAll()
{
    loadPixmap(
        "kirby_idle_right",
        ":/images/Kirby_normal/kirby_stop_R.png"
        );

    loadPixmap(
        "kirby_idle_left",
        ":/images/Kirby_normal/kirby_stop_L.png"
        );

    loadPixmap(
        "kirby_run1_right",
        ":/images/Kirby_normal/kirby_run_1_R.png"
        );

    loadPixmap(
        "kirby_run1_left",
        ":/images/Kirby_normal/kirby_run_1_L.png"
        );

    loadPixmap(
        "kirby_run2_right",
        ":/images/Kirby_normal/kirby_run_2_R.png"
        );

    loadPixmap(
        "kirby_run2_left",
        ":/images/Kirby_normal/kirby_run_2_L.png"
        );

    loadPixmap(
        "kirby_run3_right",
        ":/images/Kirby_normal/kirby_run_3_R.png"
        );

    loadPixmap(
        "kirby_run3_left",
        ":/images/Kirby_normal/kirby_run_3_L.png"
        );

    loadPixmap(
        "kirby_run4_right",
        ":/images/Kirby_normal/kirby_run_4_R.png"
        );

    loadPixmap(
        "kirby_run4_left",
        ":/images/Kirby_normal/kirby_run_4_L.png"
        );

    loadPixmap(
        "kirby_attack_right",
        ":/images/Kirby_normal/kirby_attack_R.png"
        );

    loadPixmap(
        "kirby_attack_left",
        ":/images/Kirby_normal/kirby_attack_L.png"
        );

    loadPixmap(
        "kirby_jump1_r",
        ":/images/Kirby_normal/kirby_jump(1)_R.png"
        );

    loadPixmap(
        "kirby_jump1_l",
        ":/images/Kirby_normal/kirby_jump(1)_L.png"
        );

    loadPixmap(
        "kirby_jump2",
        ":/images/Kirby_normal/kirby_jump(2).png"
        );

    loadPixmap(
        "kirby_jump2_l",
        ":/images/Kirby_normal/kirby_jump(2)_L.png"
        );

    loadPixmap(
        "kirby_jump3",
        ":/images/Kirby_normal/kirby_jump(3).png"
        );

    loadPixmap(
        "kirby_jump3_l",
        ":/images/Kirby_normal/kirby_jump(3)_L.png"
        );

    loadPixmap(
        "kirby_star",
        ":/images/Kirby_normal/kirby_attack_star(2).png"
        );

    loadPixmap(
        "inhale_1", ":/images/Kirby_normal/inhale_1.png"
        );

    loadPixmap(
        "inhale_2", ":/images/Kirby_normal/inhale_2.png"
        );

    loadPixmap(
        "inhale_3", ":/images/Kirby_normal/inhale_3.png"
        );

    loadPixmap(
        "inhale_4", ":/images/Kirby_normal/inhale_4.png"
        );

    loadPixmap(
        "inhale_5", ":/images/Kirby_normal/inhale_5.png"
        );

    loadPixmap(
        "inhale_6", ":/images/Kirby_normal/inhale_6.png"
        );

    loadPixmap(
        "kirby_star_l",
        ":/images/Kirby_normal/kirby_attack_star_L(1).png"
        );

    loadPixmap(
        "kirby_star_r",
        ":/images/Kirby_normal/kirby_attack_star_R(1).png"
        );

    loadPixmap(
        "kirby_fire_idle_l",
        ":/images/Kirby_fire/kirbyfire_stop_L.png"
        );

    loadPixmap(
        "kirby_fire_idle_r",
        ":/images/Kirby_fire/kirbyfire_stop_R.png"
        );

    loadPixmap(
        "kirby_fire_down_l",
        ":/images/Kirby_fire/kirbyfire_down_L.png"
        );

    loadPixmap(
        "kirby_fire_down_r",
        ":/images/Kirby_fire/kirbyfire_down_R.png"
        );

    loadPixmap(
        "kirby_fire_fly1_l",
        ":/images/Kirby_fire/kirbyfire_fly(1)_L.png"
        );

    loadPixmap(
        "kirby_fire_fly1_r",
        ":/images/Kirby_fire/kirbyfire_fly(1)_R.png"
        );

    loadPixmap(
        "kirby_fire_fly2_l",
        ":/images/Kirby_fire/kirbyfire_fly(2)_L.png"
        );

    loadPixmap(
        "kirby_fire_fly2_r",
        ":/images/Kirby_fire/kirbyfire_fly(2)_R.png"
        );

    loadPixmap(
        "kirby_crouch_r",
        ":/images/Kirby_normal/kirby_down_R.png"
        );

    loadPixmap(
        "kirby_crouch_l",
        ":/images/Kirby_normal/kirby_down_L.png"
        );

    loadPixmap(
        "kirby_fly1_r",
        ":/images/Kirby_normal/kirby_fly_1_R.png"
        );

    loadPixmap(
        "kirby_fly1_l",
        ":/images/Kirby_normal/kirby_fly_1_L.png"
        );

    loadPixmap(
        "kirby_fly2_r",
        ":/images/Kirby_normal/kirby_fly_2_R.png"
        );

    loadPixmap(
        "kirby_fly2_l",
        ":/images/Kirby_normal/kirby_fly_2_L.png"
        );

    loadPixmap(
        "kirby_fire_down_l",
        ":/images/Kirby_fire/kirbyfire_down_L.png"
        );

    loadPixmap(
        "kirby_fire_down_r",
        ":/images/Kirby_fire/kirbyfire_down_R.png"
        );

    loadPixmap(
        "kirby_fire_fly1_l",
        ":/images/Kirby_fire/kirbyfire_fly(1)_L.png"
        );

    loadPixmap(
        "kirby_fire_fly1_r",
        ":/images/Kirby_fire/kirbyfire_fly(1)_R.png"
        );

    loadPixmap(
        "kirby_fire_fly2_l",
        ":/images/Kirby_fire/kirbyfire_fly(2)_L.png"
        );

    loadPixmap(
        "kirby_fire_fly2_r",
        ":/images/Kirby_fire/kirbyfire_fly(2)_R.png"
        );

    loadPixmap(
        "kirby_fire_run1_l",
        ":/images/Kirby_fire/kirbyfire_run(1)_L.png"
        );

    loadPixmap(
        "kirby_fire_run1_r",
        ":/images/Kirby_fire/kirbyfire_run(1)_R.png"
        );

    loadPixmap(
        "kirby_fire_run2_l",
        ":/images/Kirby_fire/kirbyfire_run(2)_L.png"
        );

    loadPixmap(
        "kirby_fire_run2_r",
        ":/images/Kirby_fire/kirbyfire_run(2)_R.png"
        );

    loadPixmap(
        "kirby_fire_run3_l",
        ":/images/Kirby_fire/kirbyfire_run(3)_L.png"
        );

    loadPixmap(
        "kirby_fire_run3_r",
        ":/images/Kirby_fire/kirbyfire_run(3)_R.png"
        );

    loadPixmap(
        "kirby_fire_idle_l",
        ":/images/Kirby_fire/kirbyfire_stop_L.png"
        );

    loadPixmap(
        "kirby_fire_idle_r",
        ":/images/Kirby_fire/kirbyfire_stop_R.png"
        );

    loadPixmap(
        "kirby_fire_attack_l",
        ":/images/Kirby_fire/kirbyfire_attack_L.png"
        );

    loadPixmap(
        "kirby_fire_attack_r",
        ":/images/Kirby_fire/kirbyfire_attack_R.png"
        );

    loadPixmap(
        "fire_icon",
        ":/images/Kirby_fire/kirbyfire_board.png"
        );

    loadPixmap(
        "kirby_spark_attack1",
        ":/images/Kirby_spark/Kirby_spark_attack(1).png"
        );

    loadPixmap(
        "kirby_spark_attack2",
        ":/images/Kirby_spark/Kirby_spark_attack(2).png"
        );

    loadPixmap(
        "kirby_spark_attack3",
        ":/images/Kirby_spark/Kirby_spark_attack(3).png"
        );

    loadPixmap(
        "kirby_spark_down_l",
        ":/images/Kirby_spark/Kirby_spark_down_L.png"
        );

    loadPixmap(
        "kirby_spark_down_r",
        ":/images/Kirby_spark/Kirby_spark_down_R.png"
        );

    loadPixmap(
        "kirby_spark_fly1_l",
        ":/images/Kirby_spark/Kirby_spark_fly(1)_L.png"
        );

    loadPixmap(
        "kirby_spark_fly1_r",
        ":/images/Kirby_spark/Kirby_spark_fly(1)_R.png"
        );

    loadPixmap(
        "kirby_spark_fly2_l",
        ":/images/Kirby_spark/Kirby_spark_fly(2)_L.png"
        );

    loadPixmap(
        "kirby_spark_fly2_r",
        ":/images/Kirby_spark/Kirby_spark_fly(2)_R.png"
        );

    loadPixmap(
        "kirby_spark_run1_l",
        ":/images/Kirby_spark/Kirby_spark_run(1)_L.png"
        );

    loadPixmap(
        "kirby_spark_run1_r",
        ":/images/Kirby_spark/Kirby_spark_run(1)_R.png"
        );

    loadPixmap(
        "kirby_spark_run2_l",
        ":/images/Kirby_spark/Kirby_spark_run(2)_L.png"
        );

    loadPixmap(
        "kirby_spark_run2_r",
        ":/images/Kirby_spark/Kirby_spark_run(2)_R.png"
        );

    loadPixmap(
        "kirby_spark_idle_l",
        ":/images/Kirby_spark/Kirby_spark_stop_L.png"
        );

    loadPixmap(
        "kirby_spark_idle_r",
        ":/images/Kirby_spark/Kirby_spark_stop_R.png"
        );

    loadPixmap(
        "spark_icon",
        ":/images/Kirby_spark/Kirby_spark_board.png"
        );

    loadPixmap(
        "stage1_1",
        ":/images/background/Stage1(1).png"
        );

    loadPixmap(
        "stage1_2",
        ":/images/background/Stage1(2).png"
        );

    loadPixmap(
        "stage1_3",
        ":/images/background/Stage1(3).png"
        );

    loadPixmap(
        "stage2_1",
        ":/images/background/Stage2(1).png"
        );

    loadPixmap(
        "stage2_2",
        ":/images/background/Stage2(2).png"
        );

    loadPixmap(
        "game_over_continue",
        ":/images/background/game_over_continue.png"
        );

    loadPixmap(
        "game_over_quit",
        ":/images/background/game_over_quit.png"
        );

    loadPixmap(
        "start_menu",
        ":/images/background/start.png"
        );

    loadPixmap(
        "game_clear",
        ":/images/background/clear_screen.png"
        );

    loadPixmap(
        "background_1",
        ":/images/background/background_1_scale.png"
        );

    loadPixmap(
        "background_2",
        ":/images/background/background_2_scale.png"
        );

    loadPixmap(
        "menu_screen",
        ":/images/background/menu_screen.png"
        );

    loadPixmap(
        "menu_screen_select_fire_nofire",
        ":/images/background/menu_screen_select_fire_nofire.png"
        );

    loadPixmap(
        "menu_screen_select_fire_nofire_nospark",
        ":/images/background/menu_screen_select_fire_nofire_nospark.png"
        );

    loadPixmap(
        "menu_screen_select_fire_no_spark",
        ":/images/background/menu_screen_select_fire_no_spark.png"
        );

    loadPixmap(
        "menu_screen_spark",
        ":/images/background/menu_screen_spark.png"
        );

    loadPixmap(
        "menu_screen_spark_no_fire",
        ":/images/background/menu_screen_spark_no_fire.png"
        );

    loadPixmap(
        "menu_screen_spark_no_fire_no_spark",
        ":/images/background/menu_screen_spark_no_fire_no_spark.png"
        );

    loadPixmap(
        "menu_screen_spark_no_spark",
        ":/images/background/menu_screen_spark_no_spark.png"
        );

    loadPixmap(
        "hud_score",
        ":/images/background/hud_score.png"
        );

    loadPixmap(
        "goal_door",
        ":/images/item/goal_door.png"
        );

    loadPixmap(
        "door",
        ":/images/item/door.png"
        );

    loadPixmap(
        "maxim_tomato",
        ":/images/item/Maxim Tomato.png"
        );

    loadPixmap(
        "1up",
        ":/images/item/1UP.png"
        );

    loadPixmap(
        "life",
        ":/images/item/life.png"
        );

    loadPixmap(
        "hp_0",
        ":/images/item/HP_0.png"
        );

    loadPixmap(
        "hp_1",
        ":/images/item/HP_1.png"
        );

    loadPixmap(
        "block",
        ":/images/item/brick.png"
        );

    loadPixmap(
        "platform",
        ":/images/item/floor.png"
        );

    loadPixmap(
        "waddle_dee_stop_l",
        ":/images/Waddle Dee/Waddle_Dee_stop_l.png"
        );

    loadPixmap(
        "waddle_dee_stop_r",
        ":/images/Waddle Dee/Waddle_Dee_stop_r.png"
        );

    loadPixmap(
        "waddle_dee0_l",
        ":/images/Waddle Dee/Waddle_Dee0_l.png"
        );

    loadPixmap(
        "waddle_dee1_l",
        ":/images/Waddle Dee/Waddle_Dee1_l.png"
        );

    loadPixmap(
        "waddle_dee2_l",
        ":/images/Waddle Dee/Waddle_Dee2_l.png"
        );

    loadPixmap(
        "waddle_dee0_r",
        ":/images/Waddle Dee/Waddle_Dee0_r.png"
        );

    loadPixmap(
        "waddle_dee1_r",
        ":/images/Waddle Dee/Waddle_Dee1_r.png"
        );

    loadPixmap(
        "waddle_dee2_r",
        ":/images/Waddle Dee/Waddle_Dee2_r.png"
        );

    loadPixmap(
        "gordo1",
        ":/images/Gordo/Gordo(0).png"
        );

    loadPixmap(
        "gordo2",
        ":/images/Gordo/Gordo(1).png"
        );

    loadPixmap(
        "hot_head_walk_r",
        ":/images/Hot Head/Hot_head_run_R.png"
        );

    loadPixmap(
        "hot_head_walk_l",
        ":/images/Hot Head/Hot_head_run_L.png"
        );

    loadPixmap(
        "hot_head_fire",
        ":/images/Hot Head/Hot_head_fire(1).png"
        );

    loadPixmap(
        "hot_head_fire1_r",
        ":/images/Hot Head/Hot_head_fire(2)_R.png"
        );

    loadPixmap(
        "hot_head_fire1_l",
        ":/images/Hot Head/Hot_head_fire(2)_L.png"
        );

    loadPixmap(
        "hot_head_fire2_r",
        ":/images/Hot Head/Hot_head_fire(3)_R.png"
        );

    loadPixmap(
        "hot_head_fire2_l",
        ":/images/Hot Head/Hot_head_fire(3)_L.png"
        );

    loadPixmap(
        "hot_head_attack_r",
        ":/images/Hot Head/Hot_head_attack_R.png"
        );

    loadPixmap(
        "hot_head_attack_l",
        ":/images/Hot Head/Hot_head_attack_L.png"
        );

    loadPixmap(
        "hot_head_stop_r",
        ":/images/Hot Head/Hot_head_stop_R.png"
        );

    loadPixmap(
        "hot_head_stop_l",
        ":/images/Hot Head/Hot_head_stop_L.png"
        );

    loadPixmap(
        "sparky1_r",
        ":/images/Sparky/Sparky_ritht_1.png"
        );

    loadPixmap(
        "sparky2_r",
        ":/images/Sparky/Sparky_ritht_2.png"
        );

    loadPixmap(
        "sparky1_l",
        ":/images/Sparky/Sparky_left_1.png"
        );

    loadPixmap(
        "sparky2_l",
        ":/images/Sparky/Sparky_left_2.png"
        );

    loadPixmap(
        "sparky_attack1",
        ":/images/Sparky/Sparky_attack_1.png"
        );

    loadPixmap(
        "sparky_attack2",
        ":/images/Sparky/Sparky_attack_2.png"
        );

    loadPixmap(
        "kirbyfire_fire1_left",
        ":/images/Kirby_fire/kirbyfire_fire(1)_L.png"
        );

    loadPixmap(
        "kirbyfire_fire1_right",
        ":/images/Kirby_fire/kirbyfire_fire(1)_R.png"
        );

    loadPixmap(
        "kirbyfire_fire2_left",
        ":/images/Kirby_fire/kirbyfire_fire(2)_L.png"
        );

    loadPixmap(
        "kirbyfire_fire2_right",
        ":/images/Kirby_fire/kirbyfire_fire(2)_R.png"
        );

    loadPixmap(
        "kirbyfire_fire3_left",
        ":/images/Kirby_fire/kirbyfire_fire(3)_L.png"
        );

    loadPixmap(
        "kirbyfire_fire3_right",
        ":/images/Kirby_fire/kirbyfire_fire(3)_R.png"
        );

    loadPixmap(
        "kirbyspark_2",
        ":/images/Kirby_spark/Kirby_spark_attack(2).png"
        );


}