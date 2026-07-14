#include "updated_duct.h"

/**
 * @brief 덕트 사용시 동작
 */
void DuctTag(String tag_player)
{
    if (duct_available)
    {
        tag_player_name = tag_player;
        use_duct_num++;
        CooltimeCalculation();
        DuctOpen();
        TagPlayerSend();
    }
    else
    {
        CooltimeMp3();
    }
}

void DuctOpen(bool switch_push)
{
    if (duct_available || switch_push)
    {
        if (cooltime_timer.isEnabled(cooltime_timer_id))
        {
            cooltime_timer.deleteTimer(cooltime_timer_id);
        }
        Mp3PlayLargeFolder(1, 2);
        switch_available = false;
        duct_available = false;
        pixels_line.lightColor(line_red);
        pixels_switch.lightColor(red);
        pixels_round.lightColor(red);
        digitalWrite(RELAY_PIN, HIGH);
        duct_close_timer_id = duct_close_timer.setTimeout(4000, DuctClose);
    }
}

void DuctClose()
{
    digitalWrite(RELAY_PIN, LOW);
    pixels_line.lightColor(line_red);
    pixels_switch.lightColor(yellow);
    switch_available = true;
    current_time = 0;
    cool_time_neo_bool = true;
    if (!cooltime_timer.isEnabled(cooltime_timer_id))
    {
        cooltime_timer_id = cooltime_timer.setInterval(1000, CooltimeTimerFunc);
    }
    has2wifi.Send((String)(const char *)my["device_name"], "device_state", "lock");
}

/**
 * @brief 덕트 사용횟수에 따른 쿨타임 계산 함수
 */
void CooltimeCalculation()
{
    switch (use_duct_num)
    {
    case 1:
    case 2:
        cooltime = cooltime_set + cooltime_add * 0;
        break;
    case 3:
    case 4:
        cooltime = cooltime_set + cooltime_add * 1;
        break;
    case 5:
    case 6:
        cooltime = cooltime_set + cooltime_add * 2;
        break;
    case 7:
    case 8:
        cooltime = cooltime_set + cooltime_add * 3;
        break;
    case 9:
    case 10:
        cooltime = cooltime_set + cooltime_add * 4;
        break;
    default:
        break;
    }
}

void TagPlayerSend()
{
    has2wifi.Send((String)(const char *)my["device_name"], "tag_player", tag_player_name);
}

void DuctKill()
{
    // 가장 최근 태그한 플레이어 정보를 DB에서 가져옴
    String kill_player = (String)(const char *)my["tag_player"];

    has2wifi.Receive(kill_player);

    if (kill_player.startsWith("G"))
    {
        if ((String)(const char *)tag["role"] == "player")
        {
            duct_kill_bool = true;
            Serial.println("Duct Kill!");
            if ((String)(const char *)shift_machine["selected_language"] == "EN")
            {
                Mp3PlayLargeFolder(4, 3);
            }
            cool_time_neo_bool = false;
            pixels_line.lightColor(purple);
            pixels_round.lightColor(purple);
            delay(500);
            pixels_line.clear(); pixels_line.show();
            pixels_round.clear(); pixels_round.show();
            delay(500);
            pixels_line.lightColor(purple);
            pixels_round.lightColor(purple);
            delay(500);
            pixels_line.clear(); pixels_line.show();
            pixels_round.clear(); pixels_round.show();
            delay(500);
            if (tagger_mode)
            {   // "이로운 효과" 동결 중이면 kill 후에도 보라색 freeze 외관 유지
                pixels_line.lightColor(line_purple);
                pixels_round.lightColor(purple);
                pixels_switch.lightColor(purple);
            }
            else if (duct_available)
            {
                pixels_line.lightColor(line_yellow);
                pixels_round.lightColor(yellow);
                pixels_switch.lightColor(yellow);
            }
            else
            {
                pixels_round.lightColor(red);
                pixels_line.lightColor(line_red);
            }

            // 서버 taken 핸들러 규칙: value=술래(tagger), key=피해자(생존자)
            // (Situation(arg1→value, table, arg3→key) 순서. arg1에 술래, arg3에 생존자)
            has2wifi.Situation(tagger_name, "taken", kill_player);

            cool_time_neo_bool = true;
        }
    }
}

void MmmmOpen()
{
    if (mmmm_open) return;
    mmmm_open = true;

    mmmm_prev_duct_available     = duct_available;
    mmmm_prev_cooltime_running   = cooltime_timer.isEnabled(cooltime_timer_id);
    mmmm_prev_current_time       = current_time;
    mmmm_prev_cool_time_neo_bool = cool_time_neo_bool;

    if (mmmm_prev_cooltime_running)
        cooltime_timer.deleteTimer(cooltime_timer_id);

    switch_available = false;
    duct_available   = false;
    Mp3PlayLargeFolder(1, 2);
    pixels_line.lightColor(line_red);
    pixels_switch.lightColor(red);
    pixels_round.lightColor(red);
    digitalWrite(RELAY_PIN, HIGH);
    duct_close_timer_id = duct_close_timer.setTimeout(4000, MmmmClose);
}

void MmmmClose()
{
    digitalWrite(RELAY_PIN, LOW);
    mmmm_open        = false;
    switch_available = true;

    if (mmmm_prev_duct_available)
    {
        duct_available    = true;
        cool_time_neo_bool = false;
        pixels_line.lightColor(line_yellow);
        pixels_round.lightColor(yellow);
        pixels_switch.lightColor(yellow);
    }
    else
    {
        duct_available    = false;
        current_time      = mmmm_prev_current_time;
        cool_time_neo_bool = mmmm_prev_cool_time_neo_bool;
        pixels_line.clear();
        pixels_line.lightColor(line_red, NUMPIXELS_LINE * (cooltime - current_time) / cooltime);
        pixels_switch.lightColor(yellow);
        if (mmmm_prev_cooltime_running)
            cooltime_timer_id = cooltime_timer.setInterval(1000, CooltimeTimerFunc);
        has2wifi.Send((String)(const char *)my["device_name"], "device_state", "lock");
    }
}
