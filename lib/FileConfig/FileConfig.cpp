#include "FileConfig.h"

AT24Cxx eep(0x57, 32);

//////////////////////////////////////////////
// Loading settings from config.json file   //
//////////////////////////////////////////////
void LoadConfig()
{
  // Serial.println("Loading Configuration from config.json");
  String jsonConfig = "";

  File configFile = SPIFFS.open("/config.json", "r"); // Открываем файл для чтения
  jsonConfig = configFile.readString();               // загружаем файл конфигурации из EEPROM в глобальную переменную JsonObject
  configFile.close();

  StaticJsonDocument<512> doc; //  Резервируем памяь для json обекта
  // Дессиарилизация - преобразование строки в родной объкт JavaScript (или преобразование последовательной формы в параллельную)
  deserializeJson(doc, jsonConfig); //  вызовите парсер JSON через экземпляр jsonBuffer

  char TempBuf[15];


  CFG.fw = doc["firmware"].as<String>();

  CFG.IP1 = doc["ip1"];
  CFG.IP2 = doc["ip2"];
  CFG.IP3 = doc["ip3"];
  CFG.IP4 = doc["ip4"];

  CFG.APPAS = doc["pass"].as<String>();
  CFG.sn = doc["sn"];

  CFG.APSSID = doc["ssid"].as<String>();

}

void ShowLoadJSONConfig()
{
  char msg[32] = {0}; // buff for send message

  Serial.println(F("##############  System Configuration  ###############"));
  Serial.println("---------------------- SYSTEM ------------------------");
  sprintf(msg, "####  DATA: %0002d-%02d-%02d", Clock.year, Clock.month, Clock.date);
  Serial.println(F(msg));
  sprintf(msg, "####  TIME: %02d:%02d:%02d", Clock.hour, Clock.minute, Clock.second);
  Serial.println(F(msg));
  Serial.printf("####  WiFI NAME:");
  Serial.print(CFG.APSSID);
  Serial.println();
  Serial.printf("####  WiFI PASS:");
  Serial.print(CFG.APPAS);
  Serial.println();
  sprintf(msg, "####  IP: %00d.%00d.%00d.%00d", CFG.IP1, CFG.IP2, CFG.IP3, CFG.IP4);
  Serial.println(F(msg));
  Serial.printf("####  SN: %d", CFG.sn);
  Serial.printf(" FW:");
  Serial.print(CFG.fw);
  Serial.println();
  Serial.println("-------------------- SYSTEM END-----------------------");
  Serial.println(F("######################################################"));
}

//////////////////////////////////////////////
// Save configuration in config.json file   //
//////////////////////////////////////////////
void SaveConfig()
{
  String jsonConfig = "";
  StaticJsonDocument<512> doc;

  // doc["date"] = String(Clock.year) + "-" + ((Clock.month < 10) ? "0" : "") + String(Clock.month) + "-" + ((Clock.date < 10) ? "0" : "") + String(Clock.date);
  doc["firmware"] = CFG.fw;
  doc["ip1"] = CFG.IP1;
  doc["ip2"] = CFG.IP2;
  doc["ip3"] = CFG.IP3;
  doc["ip4"] = CFG.IP4;
  doc["pass"] = CFG.APPAS;
  doc["sn"]   = CFG.sn;
  doc["ssid"] = CFG.APSSID;
  // doc["time"] = ((Clock.hour < 10) ? "0" : "") + String(Clock.hour) + ":" + ((Clock.minute < 10) ? "0" : "") + String(Clock.minute);

  // serializeJson(doc, Serial);
  // Serial.println();

  // serializeJsonPretty(doc, Serial);
  // Serial.println();

  File configFile = SPIFFS.open("/config.json", "w");
  serializeJson(doc, configFile); // Writing json string to file
  configFile.close();

  Serial.println("Configuration SAVE");
}

void TestDeserializJSON()
{
  String jsonConfig = "";

  File configFile = SPIFFS.open("/config.json", "r"); // Открываем файл для чтения
  jsonConfig = configFile.readString();               // загружаем файл конфигурации из EEPROM в глобальную переменную JsonObject
  configFile.close();

  StaticJsonDocument<512> doc;
  deserializeJson(doc, jsonConfig); //  вызовите парсер JSON через экземпляр jsonBuffer

  serializeJsonPretty(doc, Serial);
  Serial.println();

  Serial.println("JSON testing comleted");
}

// void EEP_Write()
// {
//   eep.write(0, CFG.sn);
// }

// // Reading data from EEPROM
// void EEP_Read()
// {
//   CFG.sn = eep.read(0);
// }