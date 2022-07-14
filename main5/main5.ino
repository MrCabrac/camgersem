#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

#include "variables_use.h"
#include "iot/wifi-use.h"
#include "screen_use.h"
#include "keyboard_use.h"
#include "serial_use.h"

TaskHandle_t Core0Task;
TaskHandle_t Core1Task;

unsigned long lastMillis = 0;

void setup()
{
    initSerial();
    pinMode(buzzer, OUTPUT);
    initScreen();

    // Set up Core 0 task handler
    xTaskCreatePinnedToCore(
        codeForCore0Task,
        "Core 0 task",
        10000,
        NULL,
        1,
        &Core0Task,
        0);

    // Set up Core 1 task handler
    xTaskCreatePinnedToCore(
        codeForCore1Task,
        "Core 1 task",
        10000,
        NULL,
        1,
        &Core1Task,
        1);
}

void loop()
{
    vTaskDelete(nullptr);
}

void codeForCore0Task(void *parameter)
{
    for (;;)
    {
        setupWifi();
        // Serial.println("\t\t\t\t\t\t\t Core 0");
        getLocalClock();
        delay(100);
        // Enviar dato de sensores a la nube
        if (millis() - last_send > 5000 && enable_wifi && WiFi.status() == WL_CONNECTED)
        {
            // last_send = millis();
            Serial.println("Enviar datos");
            sendLocalIP();
        }
    }
}

void codeForCore1Task(void *parameter)
{
    for (;;)
    {
        showStatusBar();
        showHomeMenu();

        // Teclado
        key = keypad.getKey();

        if (key)
        {
            manageMenuKeyboardSelection(key);
        }
        soundSignal();
        refreshWindow();
        // printWifiStrength();
        printLocalTime();

        leerSerial();
        delayMicroseconds(1);
    }
}
