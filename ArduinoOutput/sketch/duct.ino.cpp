#include <Arduino.h>
#line 1 "c:\\Users\\Fuzzy line Studio\\Desktop\\YuBin\\Programing Code\\Done_code\\duct\\duct.ino"
#include "duct.h"

//************************************************ Core1 ********************************************************************
/**
 * @brief  Duct Initialize
 */
#line 7 "c:\\Users\\Fuzzy line Studio\\Desktop\\YuBin\\Programing Code\\Done_code\\duct\\duct.ino"
void DuctInit();
#line 20 "c:\\Users\\Fuzzy line Studio\\Desktop\\YuBin\\Programing Code\\Done_code\\duct\\duct.ino"
void setup();
#line 30 "c:\\Users\\Fuzzy line Studio\\Desktop\\YuBin\\Programing Code\\Done_code\\duct\\duct.ino"
void loop();
#line 6 "c:\\Users\\Fuzzy line Studio\\Desktop\\YuBin\\Programing Code\\Done_code\\duct\\duct_function.ino"
void DuctTag(String tag_player);
#line 19 "c:\\Users\\Fuzzy line Studio\\Desktop\\YuBin\\Programing Code\\Done_code\\duct\\duct_function.ino"
void DuctOpen(bool switch_push);
#line 36 "c:\\Users\\Fuzzy line Studio\\Desktop\\YuBin\\Programing Code\\Done_code\\duct\\duct_function.ino"
void DuctClose();
#line 52 "c:\\Users\\Fuzzy line Studio\\Desktop\\YuBin\\Programing Code\\Done_code\\duct\\duct_function.ino"
void CooltimeCalculation();
#line 76 "c:\\Users\\Fuzzy line Studio\\Desktop\\YuBin\\Programing Code\\Done_code\\duct\\duct_function.ino"
void TagPlayerSend();
#line 92 "c:\\Users\\Fuzzy line Studio\\Desktop\\YuBin\\Programing Code\\Done_code\\duct\\duct_function.ino"
void DuctKill();
#line 6 "c:\\Users\\Fuzzy line Studio\\Desktop\\YuBin\\Programing Code\\Done_code\\duct\\game_state.ino"
void SettingFunc();
#line 24 "c:\\Users\\Fuzzy line Studio\\Desktop\\YuBin\\Programing Code\\Done_code\\duct\\game_state.ino"
void ReadyFunc();
#line 42 "c:\\Users\\Fuzzy line Studio\\Desktop\\YuBin\\Programing Code\\Done_code\\duct\\game_state.ino"
void ActivateFunc();
#line 51 "c:\\Users\\Fuzzy line Studio\\Desktop\\YuBin\\Programing Code\\Done_code\\duct\\game_state.ino"
void ActivateRunOnce();
#line 66 "c:\\Users\\Fuzzy line Studio\\Desktop\\YuBin\\Programing Code\\Done_code\\duct\\game_state.ino"
void DataChange();
#line 4 "c:\\Users\\Fuzzy line Studio\\Desktop\\YuBin\\Programing Code\\Done_code\\duct\\sensor.ino"
void SensorInit();
#line 37 "c:\\Users\\Fuzzy line Studio\\Desktop\\YuBin\\Programing Code\\Done_code\\duct\\sensor.ino"
void RfidInit();
#line 56 "c:\\Users\\Fuzzy line Studio\\Desktop\\YuBin\\Programing Code\\Done_code\\duct\\sensor.ino"
void RfidLoop();
#line 82 "c:\\Users\\Fuzzy line Studio\\Desktop\\YuBin\\Programing Code\\Done_code\\duct\\sensor.ino"
void CardChecking(uint8_t rfidData[32]);
#line 116 "c:\\Users\\Fuzzy line Studio\\Desktop\\YuBin\\Programing Code\\Done_code\\duct\\sensor.ino"
void Mp3Init();
#line 148 "c:\\Users\\Fuzzy line Studio\\Desktop\\YuBin\\Programing Code\\Done_code\\duct\\sensor.ino"
void Mp3PlayLargeFolder(uint8_t folder_number, uint16_t file_number);
#line 171 "c:\\Users\\Fuzzy line Studio\\Desktop\\YuBin\\Programing Code\\Done_code\\duct\\sensor.ino"
void Mp3Check();
#line 177 "c:\\Users\\Fuzzy line Studio\\Desktop\\YuBin\\Programing Code\\Done_code\\duct\\sensor.ino"
void CooltimeMp3();
#line 201 "c:\\Users\\Fuzzy line Studio\\Desktop\\YuBin\\Programing Code\\Done_code\\duct\\sensor.ino"
void EmegencyPush();
#line 2 "c:\\Users\\Fuzzy line Studio\\Desktop\\YuBin\\Programing Code\\Done_code\\duct\\timer.ino"
void TimerInit();
#line 7 "c:\\Users\\Fuzzy line Studio\\Desktop\\YuBin\\Programing Code\\Done_code\\duct\\timer.ino"
void TimerRun();
#line 15 "c:\\Users\\Fuzzy line Studio\\Desktop\\YuBin\\Programing Code\\Done_code\\duct\\timer.ino"
void CooltimeTimerFunc();
#line 35 "c:\\Users\\Fuzzy line Studio\\Desktop\\YuBin\\Programing Code\\Done_code\\duct\\timer.ino"
void RfidTagTimerFunc();
#line 40 "c:\\Users\\Fuzzy line Studio\\Desktop\\YuBin\\Programing Code\\Done_code\\duct\\timer.ino"
void WifiTimerFunc();
#line 7 "c:\\Users\\Fuzzy line Studio\\Desktop\\YuBin\\Programing Code\\Done_code\\duct\\duct.ino"
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
#line 1 "c:\\Users\\Fuzzy line Studio\\Desktop\\YuBin\\Programing Code\\Done_code\\duct\\duct_function.ino"
#include "duct.h"

/**
 * @brief 덕트 사용시 DB에 
 */
void DuctTag(String tag_player){
    if(duct_available){
        tag_player_name = tag_player;
        TagPlayerSend();
        use_duct_num++;
        CooltimeCalculation();
        DuctOpen();
    }
    else{
        CooltimeMp3();
    }
}

void DuctOpen(bool switch_push)
{   
    if(duct_available || switch_push){
        if(cooltime_timer.isEnabled(cooltime_timer_id)){
            cooltime_timer.deleteTimer(cooltime_timer_id);
        }
        Mp3PlayLargeFolder(1,2);
        switch_available = false;
        duct_available = false;
        pixels_line.lightColor(line_red);
        pixels_switch.lightColor(red);
        pixels_round.lightColor(red);
        digitalWrite(RELAY_PIN, HIGH);
        duct_close_timer_id = duct_close_timer.setTimeout(5000, DuctClose);
    }
}

void DuctClose()
{
    digitalWrite(RELAY_PIN, LOW);
    pixels_line.lightColor(line_red);
    pixels_switch.lightColor(yellow);
    switch_available = true;
    current_time = 0;
    has2wifi.Send((String)(const char*)my["device_name"], "device_state", "lock");
    if(!cooltime_timer.isEnabled(cooltime_timer_id)){
        cooltime_timer_id = cooltime_timer.setInterval(1000, CooltimeTimerFunc);
    }
}

/**
 * @brief 덕트 사용횟수에 따른 쿨타임 계산 함수
 */
void CooltimeCalculation()
{
    switch (use_duct_num)
    {
    case 1 : case 2 :
        cooltime = cooltime_set + cooltime_add * 0;
        break;
    case 3 : case 4 :
        cooltime = cooltime_set + cooltime_add * 1;
        break;
    case 5 : case  6:
        cooltime = cooltime_set + cooltime_add * 2;
        break;    
    case 7 : case  8:
        cooltime = cooltime_set + cooltime_add * 3;
        break;
    case 9 : case  10:
        cooltime = cooltime_set + cooltime_add * 4;
        break;    
    default:
        break;
    }
}

void TagPlayerSend()
{
    if((String)(const char*)my["device_name"] == "BV1" || (String)(const char*)my["device_name"] == "HV1"){
        has2wifi.Send("BV1", "tag_player", tag_player_name);
        has2wifi.Send("HV1", "tag_player", tag_player_name);
    }
    else if((String)(const char*)my["device_name"] == "BV2" || (String)(const char*)my["device_name"] == "GV1"){
        has2wifi.Send("BV2", "tag_player", tag_player_name);
        has2wifi.Send("GV1", "tag_player", tag_player_name);
    }
    else if((String)(const char*)my["device_name"] == "GV2" || (String)(const char*)my["device_name"] == "FV"){
        has2wifi.Send("FV", "tag_player", tag_player_name);
        has2wifi.Send("GV2", "tag_player", tag_player_name);
    }
}
//TODO 덕트킬 추가
void DuctKill()
{
    // 가장 최근 태그한 플레이어 정보를 DB에서 가져옴
    String kill_player = (String)(const char*)my["tag_player"];

    has2wifi.Receive(kill_player);

    if(kill_player.startsWith("G")){
        if((int)tag["life_chip"] > 0){
            Serial.println("Duct Kill!");

            has2wifi.Send(kill_player, "life_chip", "-1");
            has2wifi.Send((String)(const char*)my["device_name"], "taken_chip", "+1");
            has2wifi.Send((String)(const char*)my["device_name"], "exp", "+130");

            cool_time_neo_bool = false;
            pixels_line.lightColor(purple);
            pixels_round.lightColor(purple);
            delay(500);
            pixels_line.clear();
            pixels_round.clear();
            delay(500);
            pixels_line.lightColor(purple);
            pixels_round.lightColor(purple);
            delay(500);
            pixels_line.clear();
            pixels_round.clear();
            delay(500);
            pixels_round.lightColor(red);
            cool_time_neo_bool = true;
        }
    }
}
#line 1 "c:\\Users\\Fuzzy line Studio\\Desktop\\YuBin\\Programing Code\\Done_code\\duct\\game_state.ino"
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
}

/**
 * @brief DB gamestate가 activate 일 때 한번 동작하는 코드
 */
void ActivateRunOnce()
{
    game_state = activate;

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
#line 1 "c:\\Users\\Fuzzy line Studio\\Desktop\\YuBin\\Programing Code\\Done_code\\duct\\sensor.ino"
#include "duct.h"

//****************************************** Initialize ******************************************
void SensorInit()
{ 
  // Relay Init
  pinMode(RELAY_PIN,OUTPUT);

  // Push Button Init
  pinMode(SW_PIN,INPUT_PULLUP);

  // EMCHECK_PIN Init
  pinMode(EMCHECK_PIN,INPUT_PULLUP);

  // Emergency Init - interrupt
  pinMode(EMNERGENCY_CHK_PIN, INPUT_PULLUP);

    
  // Neopixel init
  pixels_line.begin();
  pixels_round.begin();
  pixels_switch.begin();


  // Rfid init
  RfidInit();

  // DFRobot MP3 
  MySerial2.begin(9600, SERIAL_8N1, SERIAL2_RX_PIN, SERIAL2_TX_PIN);
  Mp3Init();
}

//********************************************* Rfid *********************************************
/**
 * @brief RFID(=PN532) 세팅
 */
void RfidInit()
{
  nfc.begin();                                                                //nfc 함수 시작
  if (!(nfc.getFirmwareVersion())) {
    Serial.print("!!!RFID 연결실패!!!");
    if(!send_rfid_error){
      has2wifi.Send((String)(const char*)my["device_name"], "device_state", "PN532");
      send_rfid_error = true;
    }
  }
  else{ 
    nfc.SAMConfig();                                                          // configure board to read RFID tags
    Serial.println("RFID 연결성공");
  }
}

/**
 * @brief RFID 태그 인식
 */
void RfidLoop()
{                                                                  
  if(!rfid_tag){ 
      rfid_tag = true;
      rfid_timer_id = rfid_timer.setTimeout(2000, RfidTagTimerFunc);
  }
  else {return ;}
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };                                  // Buffer to store the returned UID
  uint8_t uidLength;                                                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  uint8_t data[32]; 
  char user_data[5];
  byte pn532_packetbuffer11[64];
  pn532_packetbuffer11[0] = 0x00;
  if(nfc.sendCommandCheckAck(pn532_packetbuffer11, 1)){                     //rfid 통신 가능한 상태인지 확인
    if(nfc.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A)){          //rfid에 tag 찍혔는지 확인용 //데이터 들어오면 uid정보 가져오기
      if(nfc.ntag2xx_ReadPage(7, data))                                     //ntag 데이터에 접근해서 불러와서 data행열에 저장 
        CardChecking(data);
    }       
  }
}

/**
 * @brief RFID에 태그된 NFC의 데이터에 따른 코드 동작
 * 
 * @param rfidData 태그된 NFC의 데이터  
 */
void CardChecking(uint8_t rfidData[32]) //어떤 카드가 들어왔는지 확인용
{ 
  String tagUser = "";
  for(int i = 0; i < 4; i++){    //GxPx 데이터만 배열에서 추출해서 string으로 저장
    tagUser += (char)rfidData[i];
  }
  Serial.println("tag_user_data : " + tagUser);


  if(game_state == activate){
    // 1. 태그한 플레이어의 역할과 생명칩갯수, 최대생명칩갯수 등 읽어오기
    has2wifi.Receive(tagUser);
    // 2. 술래인지, 플레이어인지 구분
    if((String)(const char*)tag["role"] == "player"){
      DuctTag(tagUser);
    }
    else if((String)(const char*)tag["role"] == "tagger"){
     if(digitalRead(EMCHECK_PIN)){
        DuctKill();
     }
    }
  }
  else if(game_state == setting){
    Serial.println("staff");
    digitalWrite(RELAY_PIN, HIGH);
    delay(5000);
    digitalWrite(RELAY_PIN, LOW);
  }
}

//********************************************* Mp3 **********************************************
/**
 * @brief DF Player Mini 세팅
 */
void Mp3Init(){
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  myDFPlayer.setTimeOut(1000); //Set serial communictaion time out 1000 ms

  if (!myDFPlayer.begin(MySerial2)) { //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    has2wifi.Send((String)(const char*)my["device_name"], "device_state", "MP3");
    send_mp3_err = true;
  }
  else{
    Serial.println(F("DFPlayer Mini online."));
  }
  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
  //----Set volume----
  myDFPlayer.volume(30);  //Set volume value (0~30).
  //----Set different EQ----
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  //myDFPlayer.enableDAC();  //Enable On-chip DAC
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
}

/**
 * @brief DF Player Mini MP3 플레이
 * 
 * @param folder_number 큰 폴더 이름 [01, 02 등]
 * @param file_number   폴더 내 MP3 파일이름 [0001 ~, 0002 ~ 등]
 */
void Mp3PlayLargeFolder(uint8_t folder_number, uint16_t file_number){
  
  static uint8_t play_error_count = 0;    // MP3 파일이 처음 실행되면 

  if (myDFPlayer.available()) {
    myDFPlayer.playLargeFolder(folder_number,file_number);
    play_error_count = 0;
  }
  else{
      if(play_error_count < 3){
            myDFPlayer.playLargeFolder(folder_number,file_number);
            play_error_count++;
            Serial.print("에러횟수 :"); Serial.println(play_error_count);
      }
      else{
          if(!(send_mp3_err)){
              send_mp3_err = true;
              has2wifi.Send((String)(const char*)my["device_name"], "device_state", "MP3");
          } 
      }       
  }
}

void Mp3Check()
{
  if(mp3_open){ mp3_open = false; Mp3PlayLargeFolder(1,2); }
  if(mp3_cool){ mp3_cool = false; CooltimeMp3(); }
}

void CooltimeMp3()
{
    int cooltime_min = (cooltime - current_time) / 60;
    int cooltime_sec = (cooltime - current_time) % 60;
    Serial.print("min : ");Serial.println(cooltime_min);
    Serial.print("sec : ");Serial.println(cooltime_sec);
    if(cooltime_min > 0){
        Mp3PlayLargeFolder(2,cooltime_min);
        delay(1100);
        Mp3PlayLargeFolder(1,4);
        delay(500);
    }
    if(cooltime_min <= 0){
        Mp3PlayLargeFolder(3,cooltime_sec);
        delay(1500);
        Mp3PlayLargeFolder(1,5);
        delay(500);
    }
}

//******************************************* Switch ********************************************
/**
 * @brief 비상탈출 스위치를 누르면 인터럽트로 동작하는 함수
 */
void EmegencyPush()
{
  //Todo main으로 보내는 코드 추가
  if(!digitalRead(EMNERGENCY_CHK_PIN) && ((String)(const char*)my["device_state"] != "emergency")){
    has2wifi.Send((String)(const char*)my["device_name"], "device_state", "emergency");
  }
}
#line 1 "c:\\Users\\Fuzzy line Studio\\Desktop\\YuBin\\Programing Code\\Done_code\\duct\\timer.ino"
#include "duct.h"
void TimerInit()
{
    wifi_timer_id = wifi_timer.setInterval(2000, WifiTimerFunc);
}

void TimerRun()
{
    cooltime_timer.run();
    duct_close_timer.run();
    wifi_timer.run();
    rfid_timer.run();
}

void CooltimeTimerFunc()
{
    if(current_time >= cooltime){
        pixels_line.lightColor(line_yellow);
        pixels_round.lightColor(yellow);
        pixels_switch.lightColor(yellow);

        has2wifi.Send((String)(const char*)my["device_name"], "device_state", "activate");
        current_time = 0;
        duct_available = true;
        cooltime_timer.deleteTimer(cooltime_timer_id);
    }
    else{
        current_time++;
        if(cool_time_neo_bool){
            pixels_line.lightColor(line_red, NUMPIXELS_LINE * (cooltime - current_time) / cooltime);
        }
    }
}

void RfidTagTimerFunc()
{
    rfid_tag = false;
}

void WifiTimerFunc()
{
  has2wifi.Loop(DataChange);
}
