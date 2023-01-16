#include "duct.h"

/**
 * @brief DB gamestate가 setting 일 때 한번 동작하는 코드
 */
void SettingFunc()
{
    game_state = setting;

    use_duct_num = 0;
    duct_available = true;
    current_time = 0;
    cooltime = 0;
    cooltime_timer.deleteTimer(cooltime_timer_id);

    pixels_line.lightColor(line_white);
    pixels_round.lightColor(white);
    pixels_switch.lightColor(white);
}

/**
 * @brief DB gamestate가 ready 일 때 한번 동작하는 코드
 */
void ReadyFunc()
{
    game_state = ready;

    use_duct_num = 0;
    duct_available = true;
    current_time = 0;
    cooltime = 0;    
    cooltime_timer.deleteTimer(cooltime_timer_id);

    pixels_line.lightColor(line_red);
    pixels_round.lightColor(red);
    pixels_switch.lightColor(red);
}

/**
 * @brief DB gamestate가 activate 일 때 반복 동작하는 코드
 */
void ActivateFunc()
{
    RfidLoop();
    if(!digitalRead(SW_PIN) && switch_available) { DuctOpen(true); }
    if(!digitalRead(EMCHECK_PIN)){ duct_kill_bool = false; }
}

/**
 * @brief DB gamestate가 activate 일 때 한번 동작하는 코드
 */
void ActivateRunOnce()
{
    game_state = activate;

    // 쿨타임과 쿨타임 증가량을 DB에서 읽어 사용할 수 있음
    cooltime_set = (int)my["cool_time"];
    cooltime_add = (int)my["cool_time_add"];

    pixels_line.lightColor(line_yellow);
    pixels_round.lightColor(yellow);
    pixels_switch.lightColor(yellow);
}

/**
 * @brief 주기적으로 DB를 읽어옴
 */
void DataChange()
{
    static StaticJsonDocument<1000> cur;

    if ((String)(const char *)my["game_state"] != (String)(const char *)cur["game_state"]){
        if ((String)(const char *)my["game_state"] == "setting"){
            SettingFunc();
        }
        else if ((String)(const char *)my["game_state"] == "ready"){
            ReadyFunc();
        }
        else if ((String)(const char *)my["game_state"] == "activate"){
            ActivateRunOnce();
        }
    }

    if((String)(const char *)my["manage_state"] != (String)(const char *)cur["manage_state"]){
        if((String)(const char *)my["manage_state"] == "mo"){
            DuctOpen();
        }
    }

    Serial.println("Data Change");
    cur = my;
}