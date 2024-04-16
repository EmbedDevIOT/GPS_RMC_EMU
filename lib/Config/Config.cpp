#include "Config.h"
//==========================================================================

//=======================================================================
void UserPresetInit()
{
  // strcat(UserText.runtext, "РЭТРА - КАЩЕНКО - ИЮЛЬСКИЕ ДНИ");
  // strcat(UserText.carname, "Вагон ");
}

/************************ System Initialisation **********************/
void SystemInit(void)
{
  STATE.IDLE = true;
  STATE.StaticUPD = true;
  STATE.DynamicUPD = true;
  STATE.DUPDBlock = false;
  STATE.SaveFlash = false;
  STATE.Debug = true;
  STATE.WiFiEnable = true;

  GetChipID();
}
/*******************************************************************************************************/

/***************************** Function Show information or Device *************************************/
void ShowInfoDevice(void)
{
  Serial.println(F("Starting..."));
  Serial.println(F("Emulator_RMC_GPS_0120.002"));
  Serial.print(F("SN:"));
  Serial.println(CFG.sn);
  Serial.print(F("fw_date:"));
  Serial.println(CFG.fwdate);
  Serial.println(CFG.fw);
  Serial.println(CFG.chipID);
  Serial.println(F("by EmbedDev"));
  Serial.println();
}
/*******************************************************************************************************/
/*******************************************************************************************************/
void GetChipID()
{
  uint32_t chipId = 0;

  for (int i = 0; i < 17; i = i + 8)
  {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  CFG.chipID = chipId;
}
/*******************************************************************************************************/
/*******************************************************************************************************/
String GetMacAdr()
{
  CFG.MacAdr = WiFi.macAddress(); //
  Serial.print(F("MAC:"));        // временно
  Serial.println(CFG.MacAdr);     // временно
  return WiFi.macAddress();
}
/*******************************************************************************************************/

/*******************************************************************************************************/
// Debug information
void DebugInfo()
{
#ifndef DEBUG
  if (STATE.Debug)
  {
    char message[37];

    Serial.println(F("!!!!!!!!!!!!!!  DEBUG INFO  !!!!!!!!!!!!!!!!!!"));

    sprintf(message, "StaticUPD: %0d CNT: %0d", STATE.StaticUPD, STATE.cnt_Supd);
    Serial.println(message);
    sprintf(message, "RTC Time: %02d:%02d:%02d", Clock.hour, Clock.minute, Clock.second);
    Serial.println(message);
    sprintf(message, "RTC Date: %4d.%02d.%02d", Clock.year, Clock.month, Clock.date);
    Serial.println(message);
    Serial.printf("SN:");
    Serial.println(CFG.sn);
    Serial.println(F("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
    Serial.println();
  }
#endif
}

//=======================================================================

/*******************************************************************************************************/
void ShowFlashSave()
{
  bool state = false;

  for (uint8_t i = 0; i <= 1; i++)
  {
    digitalWrite(LED_ST, ON);
    vTaskDelay(400 / portTICK_PERIOD_MS);
    digitalWrite(LED_ST, OFF);
    vTaskDelay(400 / portTICK_PERIOD_MS);
  }
}
/***************************************************** ****************************/
void SystemFactoryReset()
{
  CFG.TimeZone = 3;
  CFG.WiFiMode = AccessPoint;
  CFG.APSSID = "0120-0";
  CFG.APPAS = "retra0120zxc";
  CFG.IP1 = 192;
  CFG.IP2 = 168;
  CFG.IP3 = 1;
  CFG.IP4 = 1;
  CFG.GW1 = 192;
  CFG.GW2 = 168;
  CFG.GW3 = 1;
  CFG.GW4 = 1;
  CFG.MK1 = 255;
  CFG.MK2 = 255;
  CFG.MK3 = 255;
  CFG.MK4 = 0;
  CFG.gps_speed = 9600;
}

/***************************************************************************************/
void getTimeChar(char *array)
{
  DateTime now = RTC.getTime();

  array[0] = now.hour / 10 + '0';
  array[1] = now.hour % 10 + '0';
  array[2] = now.minute / 10 + '0';
  array[3] = now.minute % 10 + '0';
  array[4] = now.second / 10 + '0';
  array[5] = now.second % 10 + '0';
  array[6] = '\0';
}
/**********************************************************************************/

void getDateChar(char *array)
{
  DateTime now = RTC.getTime();

  int8_t bytes[4];
  byte amount;
  uint16_t year;
  year = now.year;

  for (byte i = 0; i < 4; i++)
  {                       //>
    bytes[i] = year % 10; // записываем остаток в буфер
    year /= 10;           // "сдвигаем" число
    if (year == 0)
    {             // если число закончилось
      amount = i; // запомнили, сколько знаков
      break;
    }
  } // массив bytes хранит цифры числа data в обратном порядке!

  array[0] = now.date / 10 + '0';
  array[1] = now.date % 10 + '0';
  array[2] = now.month / 10 + '0';
  array[3] = now.month % 10 + '0';
  array[4] = (char)bytes[1] + '0';
  array[5] = (char)bytes[0] + '0';
  array[6] = '\0';
}
/*****************************************************************************************/

/*****************************************************************************************/
void SendXMLUserData(char *msg)
{

  getTimeChar(CFG.time);
  getDateChar(CFG.date);

  unsigned int crc;

  char buf_crc[256] = "";
  char xml[256] = "";


  itoa(CFG.TimeZone, buf_crc + strlen(buf_crc), DEC);
  strcat(buf_crc, "</gmt>\r\n");
  strcat(buf_crc, "<time>");
  strcat(buf_crc, CFG.time);
  strcat(buf_crc, "</time>\r\n");
  strcat(buf_crc, "<date>");
  strcat(buf_crc, CFG.date);
  strcat(buf_crc, "</date>\r\n");
  strcat(buf_crc, "<lat></lat>\r\n");
  strcat(buf_crc, "<lon></lon>\r\n");
  strcat(buf_crc, "<speed></speed>\r\n");


  strcat(buf_crc, "<auxtext1>");
  strcat(buf_crc, msg);
  strcat(buf_crc, "</auxtext1>\r\n");

  // crc = CRC16_mb(buf_crc, strlen(buf_crc));

  strcat(xml, "<gps_data>\r\n");
  strcat(xml, buf_crc);
  strcat(xml, "<gps_crc>");

  char crc_temp[5] = {0};

  sprintf(crc_temp, "%04X", crc);
  strcat(xml, crc_temp);
  strcat(xml, "</gps_crc>\r\n");
  strcat(xml, "</gps_data>");

  Serial2.println(xml);
  Serial2.println();
}
/*****************************************************************************************/

/*****************************************************************************************/
void SendXMLDataD()
{
  getTimeChar(CFG.time);
  getDateChar(CFG.date);

  unsigned int crc;

  char buf_crc[256] = "";
  char xml[256] = "";

  if (CFG.TimeZone == 0)
  {
    strcat(buf_crc, "<gmt>");
  }
  else if (CFG.TimeZone < 0)
  {
    strcat(buf_crc, "<gmt>-");
  }
  else
    strcat(buf_crc, "<gmt>+");

  itoa(CFG.TimeZone, buf_crc + strlen(buf_crc), DEC);
  strcat(buf_crc, "</gmt>\r\n");
  strcat(buf_crc, "<time>");
  strcat(buf_crc, CFG.time);
  strcat(buf_crc, "</time>\r\n");
  strcat(buf_crc, "<date>");
  strcat(buf_crc, CFG.date);
  strcat(buf_crc, "</date>\r\n");
  strcat(buf_crc, "<lat></lat>\r\n");
  strcat(buf_crc, "<lon></lon>\r\n");
  strcat(buf_crc, "<speed></speed>\r\n");


  // crc = CRC16_mb(buf_crc, strlen(buf_crc));

  strcat(xml, "<gps_data>\r\n");
  strcat(xml, buf_crc);
  strcat(xml, "<gps_crc>");
  char crc_temp[5] = {0};
  sprintf(crc_temp, "%04X", crc);
  strcat(xml, crc_temp);
  // itoa(crc, xml + strlen(xml), HEX);
  strcat(xml, "</gps_crc>\r\n");
  strcat(xml, "</gps_data>");

  Serial2.println(xml);
  Serial2.println();
}
//=========================================================================

//======================= CRC Check summ calculators  =====================
unsigned int CRC16_mb(char *buf, int len)
{
  unsigned int crc = 0xFFFF;
  for (int pos = 0; pos < len; pos++)
  {
    crc ^= (unsigned int)buf[pos]; // XOR byte into least sig. byte of crc

    for (int i = 8; i != 0; i--)
    { // Loop over each bit
      if ((crc & 0x0001) != 0)
      {            // If the LSB is set
        crc >>= 1; // Shift right and XOR 0xA001
        crc ^= 0xA001;
        // crc ^= 0x8005;
      }
      else         // Else LSB is not set
        crc >>= 1; // Just shift right
    }
  }
  return crc;
}
//=========================================================================