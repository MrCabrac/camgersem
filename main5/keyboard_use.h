#include "Keypad.h"

const byte ROWS = 4; // four rows
const byte COLS = 3; // three columns

char keys[ROWS][COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};

// For ESP32 Microcontroller
byte rowPins[ROWS] = {25, 13, 12, 27};
byte colPins[COLS] = {26, 33, 14};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

/**
 * @brief
 * Una función para controlar la seleccion de elementos
 * del menu con el teclado matricial
 *
 */
void manageMenuKeyboardSelection(char key)
{
    digitalWrite(buzzer, HIGH);
    buzzerStart = millis();
    Serial.println("Tecla presionada: " + (String)key + " | Anterior: " + (String)okey);
    Serial.println("Menu state: " + (String)menu_state);
    if (menu_state == 1)
    {
        if (okey != key) // Pre-seleccion
        {
            switch (okey)
            {
            case '1':
                tft.fillRect(5, 22, 100, 103, TFT_WHITE);
                tft.setTextColor(TFT_BLACK);
                tft.drawXBitmap(25, 35, sensor_icon, sensor_icon_Width, sensor_icon_Height, TFT_BLACK, TFT_WHITE);
                tft.drawCentreString(menu_options[0], 55, 100, 2);
                break;
            case '2':
                tft.fillRect(110, 22, 100, 103, TFT_WHITE);
                tft.setTextColor(TFT_BLACK);
                tft.drawXBitmap(130, 35, actuator_icon, actuator_icon_Width, actuator_icon_Height, TFT_BLACK, TFT_WHITE);
                tft.drawCentreString(menu_options[1], 160, 100, 2);
                break;
            case '3':
                tft.fillRect(215, 22, 100, 103, TFT_WHITE);
                tft.setTextColor(TFT_BLACK);
                tft.drawXBitmap(235, 35, wifi_icon, wifi_icon_Width, wifi_icon_Height, TFT_BLACK, TFT_WHITE);
                tft.drawCentreString(menu_options[2], 265, 100, 2);
                break;
            case '4':
                tft.fillRect(5, 131, 100, 103, TFT_WHITE);
                tft.setTextColor(TFT_BLACK);
                tft.drawXBitmap(25, 144, sd_icon, sd_icon_Width, sd_icon_Height, TFT_BLACK, TFT_WHITE);
                tft.drawCentreString(menu_options[3], 55, 210, 2);
                break;
            case '5':
                tft.fillRect(110, 131, 100, 103, TFT_WHITE);
                tft.setTextColor(TFT_BLACK);
                tft.drawXBitmap(130, 144, settings_icon, settings_icon_Width, settings_icon_Height, TFT_BLACK, TFT_WHITE);
                tft.drawCentreString(menu_options[4], 160, 210, 2);
                break;
            case '6':
                tft.fillRect(215, 131, 100, 103, TFT_WHITE);
                tft.setTextColor(TFT_BLACK);
                tft.drawXBitmap(235, 144, graphic_icon, graphic_icon_Width, graphic_icon_Height, TFT_BLACK, TFT_WHITE);
                tft.drawCentreString(menu_options[5], 265, 210, 2);
                break;
            default:
                break;
            }
            switch (key)
            {
            case '1':
                tft.fillRect(5, 22, 100, 103, TFT_BLACK);
                tft.drawRect(5, 22, 100, 103, TFT_WHITE);
                tft.setTextColor(TFT_WHITE);
                tft.drawXBitmap(25, 35, sensor_icon, sensor_icon_Width, sensor_icon_Height, TFT_WHITE, TFT_BLACK);
                tft.drawCentreString(menu_options[0], 55, 100, 2);
                break;
            case '2':
                tft.fillRect(110, 22, 100, 103, TFT_BLACK);
                tft.drawRect(110, 22, 100, 103, TFT_WHITE);
                tft.setTextColor(TFT_WHITE);
                tft.drawXBitmap(130, 35, actuator_icon, actuator_icon_Width, actuator_icon_Height, TFT_WHITE, TFT_BLACK);
                tft.drawCentreString(menu_options[1], 160, 100, 2);
                break;
            case '3':
                tft.fillRect(215, 22, 100, 103, TFT_BLACK);
                tft.drawRect(215, 22, 100, 103, TFT_WHITE);
                tft.setTextColor(TFT_WHITE);
                tft.drawXBitmap(235, 35, wifi_icon, wifi_icon_Width, wifi_icon_Height, TFT_WHITE, TFT_BLACK);
                tft.drawCentreString(menu_options[2], 265, 100, 2);
                break;
            case '4':
                tft.fillRect(5, 131, 100, 103, TFT_BLACK);
                tft.drawRect(5, 131, 100, 103, TFT_WHITE);
                tft.setTextColor(TFT_WHITE);
                tft.drawXBitmap(25, 144, sd_icon, sd_icon_Width, sd_icon_Height, TFT_WHITE, TFT_BLACK);
                tft.drawCentreString(menu_options[3], 55, 210, 2);
                break;
            case '5':
                tft.fillRect(110, 131, 100, 103, TFT_BLACK);
                tft.drawRect(110, 131, 100, 103, TFT_WHITE);
                tft.setTextColor(TFT_WHITE);
                tft.drawXBitmap(130, 144, settings_icon, settings_icon_Width, sd_icon_Height, TFT_WHITE, TFT_BLACK);
                tft.drawCentreString(menu_options[4], 160, 210, 2);
                break;
            case '6':
                tft.fillRect(215, 131, 100, 103, TFT_BLACK);
                tft.drawRect(215, 131, 100, 103, TFT_WHITE);
                tft.setTextColor(TFT_WHITE);
                tft.drawXBitmap(235, 144, graphic_icon, graphic_icon_Width, graphic_icon_Height, TFT_WHITE, TFT_BLACK);
                tft.drawCentreString(menu_options[5], 265, 210, 2);
                break;
            default:
                break;
            }
        }
        else // Selección cuando presiona dos veces la tecla
        {
            if (key != '.' && key != '#' && key != '*' && window_state == 0)
            {
                int int_key = key - '0';
                window_state = int_key;
                showWindow();
            }
        }
    }

    else if (menu_state == 0 && window_state != 0)
    {
        if (key == '*')
        {
            // Volver al menu principal
            Serial.println("Volver al menu principal");
            int int_key = 0;
            window_state = int_key;
            restartVariables();
            showHomeMenu();
        }
    }

    okey = key;
}

/**
 * @brief
 * Para controlar el encendido y el apagado del buzzer
 */
void soundSignal()
{
    if (digitalRead(buzzer) == 1)
    {
        if (millis() - buzzerStart > 60)
        {
            digitalWrite(buzzer, LOW);
            Serial.println("\t\t\t\t\tsoundSignal low");
        }
    }
}