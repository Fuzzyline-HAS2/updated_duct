#include "duct.h"

//************************************************ Core1 ********************************************************************
/**
 * @brief  Duct Initialize
 */
void DuctInit()
{
  Serial.begin(115200);
  SensorInit();
  has2wifi.Setup("KT_GiGA_6C64", "ed46zx1198");
  // has2wifi.Setup();
  DataChange();
  TimerInit();
}

/**
 * @brief 아두이노 기본 문법 (전원이 켜지면 한번만 실행)
 */
void setup()
{
  delay(1000);
  Serial.begin(115200);
  DuctInit();
}

/**
 * @brief 아두이노 기본 문법 (전원이 켜져있는동안 Core1에서 계속 실행)
 */
void loop()
{
  TimerRun();

  EmegencyPush();

  if(game_state == activate){ ActivateFunc(); }
  else{  // 세팅 모드에서는 덕트가 쿨타임 없이 열리고 닫힐 수 있음
    RfidLoop();
    if(!digitalRead(SW_PIN)){
      Serial.println("staff");
      digitalWrite(RELAY_PIN, HIGH);
      delay(5000);
      digitalWrite(RELAY_PIN, LOW);
    }
  }
}