#ifndef _Config_H
#define _Config_H

#include <Arduino.h>

// #include <ElegantOTA.h>
#include "HardwareSerial.h"
#include <WiFi.h>
#include <WebServer.h>
#include "SPIFFS.h"
#include <microDS3231.h>
#include <ArduinoJson.h>

#define UARTSpeed 115200

#define WiFi_

#define WiFiTimeON 10
#define Client 0
#define AccessPoint 1
#define WorkNET

#define debug(x) Serial.println(x)
// #define DEBUG
// #define I2C_SCAN

#define DISABLE 0
#define ENABLE 1

#define ON 1
#define OFF 0

// GPIO PINs
#define BTN_USR 36     // User Button 
#define LED_ST 14      // User Led

#define TX_PIN 17  // UART_TX
#define RX_PIN 16  // UART_RX

//=======================================================================
extern MicroDS3231 RTC;
extern DateTime Clock;
//=======================================================================

//========================== ENUMERATION ================================
enum Clicks
{
  ONE = 1,
  TWO,
  THREE,
};
//=======================================================================

//=========================== GLOBAL CONFIG =============================
struct GlobalConfig
{
  uint16_t sn = 0;
  uint16_t gps_speed = 9600;
  String fw = ""; 
  String fwdate = "";
  String chipID = "";
  String MacAdr = "";

  String NTPServer = "pool.ntp.org";
  String APSSID = "0120Time";
  String APPAS = "retra0120zxc";
  
  String Ssid = "MkT";           // SSID Wifi network
  String Password = "QFCxfXMA3"; // Passwords WiFi network

  int TimeZone = 0;

  char time[7] = "";
  char date[9] = "";

  byte IP1 = 192;
  byte IP2 = 168;
  byte IP3 = 1;
  byte IP4 = 1;
  byte GW1 = 192;
  byte GW2 = 168;
  byte GW3 = 1;
  byte GW4 = 1;
  byte MK1 = 255;
  byte MK2 = 255;
  byte MK3 = 255;
  byte MK4 = 0;

  byte WiFiMode = AccessPoint; // Режим работы WiFi
};
extern GlobalConfig CFG;
//=======================================================================

//=======================================================================
struct HardwareConfig
{
  uint8_t ERRORcnt = 0;
};
extern HardwareConfig HCONF;
//=======================================================================

//=======================================================================
struct Flag
{
  bool StaticUPD : 1;
  uint8_t cnt_Supd = 0;
  bool DynamicUPD : 1;
  bool DUPDBlock : 1;
  bool IDLE : 1;
  bool LedUser : 1;
  bool SaveFlash : 1;
  bool Debug : 1;
  bool WiFiEnable : 1;
};
extern Flag STATE;
//============================================================================

//============================================================================
void UserPresetInit(void);
void SystemInit(void);     //  System Initialisation (variables and structure)
void ShowInfoDevice(void); //  Show information or this Device
void GetChipID(void);
String GetMacAdr();
void DebugInfo(void);
void SystemFactoryReset(void);
void ShowFlashSave(void);
void getTimeChar(char* array);
void getDateChar(char* array);
void Build_and_SendRMC(void);
void Build_and_SendGGA(void);
void Build_and_SendNMEA(void);
uint8_t nmea_get_checksum(const char *sentence);
// void GPSFStart(void);
//============================================================================
#endif // _Config_H