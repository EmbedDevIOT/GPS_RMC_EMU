#include "Config.h"

#include "FileConfig.h"
#include "WF.h"
#include "HTTP.h"

//=========================== GLOBAL VARIABLES =========================
char buf[32] = {0}; // buff for send message
// String rmc_msg = "$GPRMC,144526.539,A,5231.772,N,01324.117,E,777.4,084.4,120424,000.0,W*7C";
String rmc_msg = "";
uint8_t sec_cnt = 0;
uint8_t data = 0;
uint8_t month = 0;
uint16_t year = 0;
//======================================================================

//================================ OBJECTs =============================
MicroDS3231 RTC;
TaskHandle_t TaskCore_0;
TaskHandle_t TaskCore_1;
//=======================================================================

//============================== STRUCTURES =============================
DateTime Clock;
GlobalConfig CFG;
Flag STATE;
//=======================================================================

//======================    FUNCTION PROTOTYPS     ======================
void HandlerCore0(void *pvParameters);
void HandlerCore1(void *pvParameters);
//=======================================================================

//=======================================================================
// Pinned to Core 0. Network Stack Handler
void HandlerCore0(void *pvParameters)
{
    Serial.print("Task0 running on core ");
    Serial.println(xPortGetCoreID());
    for (;;)
    {
        HandleClient();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

// Pinned to Core 1.
void HandlerCore1(void *pvParameters)
{
    Serial.print("Task1 running on core ");
    Serial.println(xPortGetCoreID());
    for (;;)
    {
        Clock = RTC.getTime();
        DebugInfo();

        Build_and_SendNMEA();

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
//=======================================================================

//=======================       S E T U P       =========================
void setup()
{
    CFG.fw = "0.5";
    CFG.fwdate = "17.04.2024";

    Serial.begin(UARTSpeed);
    Serial2.begin(CFG.gps_speed);

    SystemInit();
    // RTC INIT
    RTC.begin();
    // Low battery / RTC battery crash / Set time compilations
    if (RTC.lostPower())
    {
        RTC.setTime(COMPILE_TIME);
    }
    Clock = RTC.getTime();
    Serial.println(F("RTC...Done"));

    EEP_Read();
    byte errSPIFFS = SPIFFS.begin(true);

    LoadConfig(); // Load configuration from config.json files

    ShowLoadJSONConfig();

    WIFIinit();
    delay(1000);

    HTTPinit(); // HTTP server initialisation
    delay(100);

    for (uint8_t i = 0; i < 5; i++)
    {
        Build_and_SendNMEA();
        delay(1000);
    }

    xTaskCreatePinnedToCore(
        HandlerCore0,
        "TaskCore_0",
        10000,
        NULL,
        1,
        &TaskCore_0,
        0);
    delay(500);

    xTaskCreatePinnedToCore(
        HandlerCore1,
        "TaskCore_1",
        2048,
        NULL,
        1,
        &TaskCore_1,
        1);
    delay(500);
}
//=======================================================================

//=======================        L O O P        =========================
void loop() {}
//=======================================================================
