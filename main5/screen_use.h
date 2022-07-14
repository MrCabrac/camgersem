#include "icons/sensor_icon.h"
#include "icons/actuator_icon.h"
#include "icons/wifi_icon.h"
#include "icons/sd_icon.h"
#include "icons/settings_icon.h"
#include "icons/graphic_icon.h"
#include "images/ueia_logo.h"
#include "graphic_use.h"

#define USE_DMA

#include <TJpg_Decoder.h>

#ifdef USE_DMA
uint16_t dmaBuffer1[16 * 16]; // Toggle buffer for 16*16 MCU block, 512bytes
uint16_t dmaBuffer2[16 * 16]; // Toggle buffer for 16*16 MCU block, 512bytes
uint16_t *dmaBufferPtr = dmaBuffer1;
bool dmaBufferSel = 0;
#endif

/**
 * @brief
 * Invertir los colores del display
 */
void invertDisplay()
{
}

void showWindowTitle(String window_title)
{
    // Limpiar barra de estado para el título
    tft.fillRect(60, 0, 180, 16, TFT_WHITE);
    // Escribir el título en la barra de estado
    tft.setTextColor(TFT_BLUE);
    tft.drawCentreString(window_title, 160, 0, 2);
}

/**
 * @brief
 * Mostrar la barra de estado en la parte superior de la pantalla
 */
void showStatusBar()
{
    if (screen_state == 1 && statusBar_state == 0) // Si la pantalla ya se inicializó
    {
        Serial.println("================\nshowStatusBar\n================");
        tft.fillRect(0, 0, 320, 16, statusBar_bg_color);
        statusBar_state = 1; // Cambiar el estado de la barra de estado
    }
}

void showHomeMenu()
{
    if (statusBar_state == 1 && menu_state == 0 && window_state == 0)
    {
        Serial.println("================\nshowHomeMenu\n================");
        tft.fillRect(0, 16, 320, 224, TFT_BLACK);
        showWindowTitle("Menu");
        tft.fillRect(5, 22, 100, 103, statusBar_bg_color);
        tft.fillRect(110, 22, 100, 103, statusBar_bg_color);
        tft.fillRect(215, 22, 100, 103, statusBar_bg_color);
        tft.fillRect(5, 131, 100, 103, statusBar_bg_color);
        tft.fillRect(110, 131, 100, 103, statusBar_bg_color);
        tft.fillRect(215, 131, 100, 103, statusBar_bg_color);

        tft.drawXBitmap(25, 35, sensor_icon, sensor_icon_Width, sensor_icon_Height, TFT_BLACK, TFT_WHITE);
        tft.drawXBitmap(130, 35, actuator_icon, actuator_icon_Width, actuator_icon_Height, TFT_BLACK, TFT_WHITE);
        tft.drawXBitmap(235, 35, wifi_icon, wifi_icon_Width, wifi_icon_Height, TFT_BLACK, TFT_WHITE);
        tft.drawXBitmap(25, 144, sd_icon, sd_icon_Width, sd_icon_Height, TFT_BLACK, TFT_WHITE);
        tft.drawXBitmap(130, 144, settings_icon, settings_icon_Width, settings_icon_Height, TFT_BLACK, TFT_WHITE);
        tft.drawXBitmap(235, 144, graphic_icon, graphic_icon_Width, graphic_icon_Height, TFT_BLACK, TFT_WHITE);
        tft.setTextColor(TFT_BLACK);
        tft.drawCentreString(menu_options[0], 55, 100, 2);
        tft.drawCentreString(menu_options[1], 160, 100, 2);
        tft.drawCentreString(menu_options[2], 265, 100, 2);
        tft.drawCentreString(menu_options[3], 55, 210, 2);
        tft.drawCentreString(menu_options[4], 160, 210, 2);
        tft.drawCentreString(menu_options[5], 265, 210, 2);
        menu_state = 1;
    }
}

/**
 * @brief
 * Ventana de Sensores seleccionada en el menu
 */
void sensorsWindow()
{
    if (otemperatura_interior != temperatura_interior)
    {
        tft.drawString("Temperatura [°C]", 10, 25, 2);
        tft.drawRoundRect(5, 20, 110, 90, 5, TFT_BLACK);
        int xpos = 25;
        otemperatura_interior = temperatura_interior;
        tft.setTextColor(TFT_BLACK, windowBackgrounds[window_state - 1]);
        if (temperatura_interior < 10)
            xpos += tft.drawChar('0', xpos, 50, 7); // Add hours leading zero for 24 hr clock
        tft.drawNumber(temperatura_interior, xpos, 50, 7);
    }
    if (ohumedad_interior != humedad_interior)
    {
        tft.drawString("Humedad [%]", 10, 120, 2);
        tft.drawRoundRect(5, 115, 110, 90, 5, TFT_BLUE);
        int xpos = 25;
        ohumedad_interior = humedad_interior;
        tft.setTextColor(TFT_BLACK, windowBackgrounds[window_state - 1]);
        if (humedad_interior < 10)
            xpos += tft.drawChar('0', xpos, 142, 7); // Add hours leading zero for 24 hr clock
        tft.drawNumber(humedad_interior, xpos, 142, 7);
    }

    if (oluminosidad_interior != luminosidad_interior)
    {
        tft.drawString("Iluminacion [lx]", 125, 25, 2);
        tft.drawRoundRect(120, 20, 195, 90, 5, TFT_BLACK);
        int xpos = 130;
        oluminosidad_interior = luminosidad_interior;
        tft.setTextColor(TFT_BLACK, windowBackgrounds[window_state - 1]);
        if (luminosidad_interior < 10)
        {
            xpos += tft.drawChar('0', xpos, 50, 7); // Add hours leading zero for 24 hr clock
            xpos += tft.drawChar('0', xpos, 50, 7); // Add hours leading zero for 24 hr clock
            xpos += tft.drawChar('0', xpos, 50, 7); // Add hours leading zero for 24 hr clock
            xpos += tft.drawChar('0', xpos, 50, 7); // Add hours leading zero for 24 hr clock
        }
        else if (luminosidad_interior < 100 && luminosidad_interior > 9)
        {
            xpos += tft.drawChar('0', xpos, 50, 7); // Add hours leading zero for 24 hr clock
            xpos += tft.drawChar('0', xpos, 50, 7); // Add hours leading zero for 24 hr clock
            xpos += tft.drawChar('0', xpos, 50, 7); // Add hours leading zero for 24 hr clock
        }
        else if (luminosidad_interior < 999 && luminosidad_interior > 99)
        {
            xpos += tft.drawChar('0', xpos, 50, 7); // Add hours leading zero for 24 hr clock
            xpos += tft.drawChar('0', xpos, 50, 7); // Add hours leading zero for 24 hr clock
        }
        else if (luminosidad_interior < 9999 && luminosidad_interior > 999)
        {
            xpos += tft.drawChar('0', xpos, 50, 7); // Add hours leading zero for 24 hr clock
        }
        tft.drawNumber(luminosidad_interior, xpos, 50, 7);
    }

    tft.drawString("Set-point Temp:", 120, 115, 2);
    tft.drawString((String)sp1 + "     ", 220, 115, 2);
    tft.drawString("Set-point Hum:", 120, 132, 2);
    tft.drawString((String)sp2 + "     ", 220, 132, 2);
    tft.drawString("Set-point Ilum:", 120, 147, 2);
    tft.drawString((String)sp3 + "     ", 220, 147, 2);

    if (sps == 1)
    {
        tft.drawString("Configurar set-point de ", 120, 170, 2);
        tft.drawString("Humedad", 120, 187, 2);
    }
    else if (sps == 3)
    {
        tft.drawString("Configurar set-point de ", 120, 170, 2);
        tft.drawString("Temperatura", 120, 187, 2);
    }
    else if (sps == 4)
    {
        tft.drawString("Configurar set-point de  ", 120, 170, 2);
        tft.drawString("Iluminacion", 120, 187, 2);
    }
    else
    {
        tft.drawString("                          ", 120, 170, 2);
        tft.drawString("                          ", 120, 187, 2);
    }

    tft.drawString("[1] Abrir graficas    ", 3, 220, 2);

    if (key == '1')
    {
        // Ir a la gráfica
        int int_key = 6;
        window_state = int_key;
    }
}

/**
 * @brief
 *
 */
void graphicSensorsWindow()
{
    // Serial.println("graphicSensorsWindow");
    if (!graph_visible)
    {
        // Dibujar el cuadrado en negro
        tft.fillRect(graph_box_init_x, graph_box_init_y, graph_width, graph_height, TFT_BLACK);

        firstDraw = 0;
        drawAxis();
        initBuffers();
        // tft.setTextColor(TFT_WHITE, windowBackgrounds[window_state - 1]);

        // Escalas
        tft.fillRoundRect(0, 220, 18, 18, 3, TFT_RED);
        tft.fillRoundRect(100, 220, 18, 18, 3, TFT_GREEN);
        tft.fillRoundRect(175, 220, 34, 18, 3, TFT_CYAN);
        tft.fillRoundRect(graph_box_init_x + graph_width, graph_box_init_y + graph_height, 320 - graph_box_init_x + graph_width, 240 - graph_box_init_y + graph_height, 0, TFT_DARKGREY);

        tft.setTextColor(TFT_WHITE, TFT_RED);
        tft.drawCentreString((String)((signal_1_Limits[1] - signal_1_Limits[0]) / VERTICAL_DIVIDER), 9, 221, 2);
        tft.setTextColor(TFT_WHITE, windowBackgrounds[window_state - 1]);

        tft.setTextColor(TFT_BLACK, TFT_GREEN);
        tft.drawCentreString((String)((signal_2_Limits[1] - signal_2_Limits[0]) / VERTICAL_DIVIDER), 109, 221, 2);
        tft.setTextColor(TFT_WHITE, windowBackgrounds[window_state - 1]);

        tft.setTextColor(TFT_BLACK, TFT_CYAN);
        tft.drawCentreString((String)((signal_3_Limits[1] - signal_3_Limits[0]) / VERTICAL_DIVIDER), 192, 221, 2);
        tft.setTextColor(TFT_WHITE, windowBackgrounds[window_state - 1]);

        // Label de los sensores
        tft.drawString("Temperatura", 21, 222, 2);
        tft.drawString("Humedad", 121, 222, 2);
        tft.drawString("Iluminacion", 212, 222, 2);

        tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
        tft.drawString((String)((float)periodo_muestro * (float)graph_width / (float)HORIZONTAL_DIVIDER / 1000), 282, 222, 2);

        // Columna derecha
        tft.setTextColor(TFT_WHITE, TFT_RED);
        tft.drawString("Temp.", 283, 21, 2);
        tft.setTextColor(TFT_BLACK, TFT_GREEN);
        tft.drawString("Hume.", 283, 57, 2);
        tft.setTextColor(TFT_BLACK, TFT_CYAN);
        tft.drawString("Ilumi.", 283, 93, 2);
        tft.setTextColor(TFT_BLACK, TFT_WHITE);
        tft.drawString("Time:", 283, 130, 2);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.drawString("      ", 283, 147, 2);
        graph_visible = true;
        Serial.println("First Draw: " + (String)firstDraw);
    }
}

/**
 * @brief
 * Ventana de actuadores seleccionada en el menu
 */
void actuatorsWindow()
{
    uint8_t actuatorsNumber = 10;
    tft.setTextColor(TFT_BLACK, windowBackgrounds[window_state - 1]);
    // Imprimir una lista con los actuadores
    if (option_list_selected != ooption_list_selected)
    {
        for (uint8_t i = 0; i < actuatorsNumber; i++)
        {
            tft.drawString(actuators_list[i], 4, 21 + 16 * i, 2);
        }
        if (0 <= option_list_selected && option_list_selected < actuatorsNumber)
        {
            tft.fillRect(0, 21 + 16 * option_list_selected, 320, 16, TFT_BLACK);
            tft.setTextColor(TFT_WHITE, TFT_BLACK);
            tft.drawString(actuators_list[option_list_selected], 4, 21 + 16 * option_list_selected, 2);
        }
        if (0 <= ooption_list_selected && ooption_list_selected < actuatorsNumber)
        {
            tft.fillRect(0, 21 + 16 * ooption_list_selected, 320, 16, TFT_WHITE);
            tft.setTextColor(TFT_BLACK, TFT_WHITE);
            tft.drawString(actuators_list[ooption_list_selected], 4, 21 + 16 * ooption_list_selected, 2);
        }
        ooption_list_selected = option_list_selected;
    }

    tft.setTextColor(TFT_BLACK, windowBackgrounds[window_state - 1]);
    tft.drawString((String)actuadores[option_list_selected], 5, 226, 2);

    if (key == '8')
    {
        option_list_selected++;
        if (option_list_selected > actuatorsNumber - 1)
        {
            option_list_selected = 0;
        }
        key = '.';
        okey = '.';
    }
    if (key == '2')
    {
        option_list_selected--;
        if (option_list_selected < 0)
        {
            option_list_selected = actuatorsNumber - 1;
        }
        key = '.';
        okey = '.';
    }
}

/**
 * @brief
 * Imprimir la fuerza de la señal WiFi sobre la barra de estado
 * únicamente si está dibujada en pantalla
 */
void printWifiStrength()
{
    if (statusBar_state == 1)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            rssi = WiFi.RSSI();
            if (orssi != rssi)
            {
                orssi = rssi;
                tft.setTextColor(TFT_BLACK, statusBar_bg_color);
                tft.drawString("WiFi: " + (String)rssi, 250, 0, 2); // Draw db
                uint32_t wifiColor = TFT_WHITE;
                if (rssi >= -30)
                    wifiColor = tft.color565(126, 202, 39);
                if (rssi < -30 && rssi >= -67)
                    wifiColor = tft.color565(204, 225, 15);
                if (rssi < -67 && rssi >= -70)
                    wifiColor = tft.color565(241, 238, 28);
                if (rssi < -70 && rssi >= -80)
                    wifiColor = tft.color565(252, 221, 15);
                if (rssi < -80 && rssi >= -90)
                    wifiColor = tft.color565(245, 133, 17);
                if (rssi < -90)
                    wifiColor = tft.color565(239, 69, 34);
                tft.fillRect(310, 2, 8, 12, wifiColor); // Less TFT refresh aliasing than line above for large balls
            }
        }
        else
        {
            rssi = WiFi.RSSI();
            if (orssi != rssi)
            {
                orssi = rssi;
                tft.setTextColor(TFT_BLACK, statusBar_bg_color);
                tft.fillRect(250, 0, 70, 16, statusBar_bg_color);
                tft.drawString("WiFi: X", 250, 0, 2); // Draw db
                tft.fillRect(310, 2, 8, 12, TFT_RED); // Less TFT refresh aliasing than line above for large balls
            }
        }
    }
}

/**
 * @brief
 * Imprimir la hora en formato hh%mm%ss sobre la barra de estado
 * únicamente si esta está en dibujada en pantalla
 * Se accede a las variables 'hh' 'mm' 'ss'
 */
void printLocalTime()
{
    // Serial.println("printLocalTime\n================");
    if (statusBar_state == 1)
    {
        // getLocalClock();
        tft.setTextColor(TFT_BLACK, statusBar_bg_color);

        // Imprimir solo cuando hay cambios en los minutos
        if (oss != ss)
        { // Redraw hours and minutes time every minute
            // Serial.println("Redraw hours and minutes time every minute");
            String hhp;
            String mmp;
            String ssp;
            oss = ss;
            if (hh < 10)
            {
                hhp = "0" + (String)hh;
            }
            else
            {
                hhp = (String)hh;
            }
            if (mm < 10)
            {
                mmp = "0" + (String)mm;
            }
            else
            {
                mmp = (String)mm;
            }
            if (ss < 10)
            {
                ssp = "0" + (String)ss;
            }
            else
            {
                ssp = (String)ss;
            }

            tft.drawString(hhp + ":" + mmp + ":" + ssp, 2, 0, 2); // Draw time
        }
    }
}

/**
 * @brief
 * Ventana de WiFi seleccionada en el menu
 */
void wifiWindow()
{
    tft.setTextColor(TFT_WHITE, windowBackgrounds[window_state - 1]);
    if (oenable_wifi != enable_wifi)
    {
        if (enable_wifi)
        {
            tft.drawString("Estado: Habilitado    ", 3, 18, 2);
            tft.drawString("[1] Deshabilitar Wifi    ", 3, 220, 2);
            if (owifi_ssid != WiFi.SSID())
            {
                owifi_ssid = WiFi.SSID();
                tft.drawString(WiFi.SSID(), 3, 40, 4);
            }
            if (owifi_connection_status != WiFi.status())
            {
                owifi_connection_status = WiFi.status();
                if (WiFi.status() == 3)
                {
                    tft.drawString("Conectado       ", 3, 63, 2);
                    tft.drawString("IP: " + WiFi.localIP().toString(), 3, 78, 2);
                    tft.drawString("Gateway: " + WiFi.gatewayIP().toString(), 3, 93, 2);
                    tft.drawString("Mask: " + WiFi.subnetMask().toString(), 3, 108, 2);
                }
                else if (WiFi.status() == 6)
                {
                    tft.drawString("Conexion perdida", 3, 63, 2);
                }
            }
        }
        else
        {
            tft.drawString("Estado: Deshabilitado ", 3, 18, 2);
            tft.drawString("[1] Habilitar Wifi    ", 3, 220, 2);
            tft.fillRect(0, 35, 320, 123, windowBackgrounds[window_state - 1]);
        }
    }

    if (key == '1')
    {
        oenable_wifi = enable_wifi;
        enable_wifi = !enable_wifi;
        key = '.';
        okey = '.';
        if (!enable_wifi)
        { // Desactivar internet
            Serial.println("Desactivar internet\n=================");
            WiFi.disconnect();
            owifi_connection_status = 7;
        }
    }
}

// This next function will be called during decoding of the jpeg file to render each
// 16x16 or 8x8 image tile (Minimum Coding Unit) to the TFT.
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap)
{
    // Stop further decoding as image is running off bottom of screen
    if (y >= tft.height())
        return 0;

        // STM32F767 processor takes 43ms just to decode (and not draw) jpeg (-Os compile option)
        // Total time to decode and also draw to TFT:
        // SPI 54MHz=71ms, with DMA 50ms, 71-43 = 28ms spent drawing, so DMA is complete before next MCU block is ready
        // Apparent performance benefit of DMA = 71/50 = 42%, 50 - 43 = 7ms lost elsewhere
        // SPI 27MHz=95ms, with DMA 52ms. 95-43 = 52ms spent drawing, so DMA is *just* complete before next MCU block is ready!
        // Apparent performance benefit of DMA = 95/52 = 83%, 52 - 43 = 9ms lost elsewhere
#ifdef USE_DMA
    // Double buffering is used, the bitmap is copied to the buffer by pushImageDMA() the
    // bitmap can then be updated by the jpeg decoder while DMA is in progress
    if (dmaBufferSel)
        dmaBufferPtr = dmaBuffer2;
    else
        dmaBufferPtr = dmaBuffer1;
    dmaBufferSel = !dmaBufferSel; // Toggle buffer selection
    //  pushImageDMA() will clip the image block at screen boundaries before initiating DMA
    tft.pushImageDMA(x, y, w, h, bitmap, dmaBufferPtr); // Initiate DMA - blocking only if last DMA is not complete
                                                        // The DMA transfer of image block to the TFT is now in progress...
#else
    // Non-DMA blocking alternative
    tft.pushImage(x, y, w, h, bitmap); // Blocking, so only returns when image block is drawn
#endif
    // Return 1 to decode next block.
    return 1;
}

void showInitialLogo()
{
#ifdef USE_DMA
    tft.initDMA(); // To use SPI DMA you must call initDMA() to setup the DMA engine
#endif

    // The jpeg image can be scaled down by a factor of 1, 2, 4, or 8
    TJpgDec.setJpgScale(1);

    // The colour byte order can be swapped by the decoder
    // using TJpgDec.setSwapBytes(true); or by the TFT_eSPI library:
    tft.setSwapBytes(true);

    // The decoder must be given the exact name of the rendering function above
    TJpgDec.setCallback(tft_output);

    uint16_t w = 0, h = 0;
    TJpgDec.getJpgSize(&w, &h, u_eia_logo, sizeof(u_eia_logo));
    Serial.print("Width = ");
    Serial.print(w);
    Serial.print(", height = ");
    Serial.println(h);

    tft.startWrite();

    // Draw the image, top left at 0,0 - DMA request is handled in the call-back tft_output() in this sketch
    TJpgDec.drawJpg(60, 0, u_eia_logo, sizeof(u_eia_logo));

    // Must use endWrite to release the TFT chip select and release the SPI channel
    tft.endWrite();
    // Universidad EIA
    tft.setTextColor(TFT_BLUE, TFT_WHITE);
    tft.drawCentreString("UNIVERSIDAD EIA", 160, 185, 4);
    // Camara germinación de semillas
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.drawCentreString("Camara germinacion de semillas", 160, 210, 2);
}

void refreshWindow()
{
    // Serial.println("================\nrefreshWindow "+(String)window_state+"\n================");
    switch (window_state)
    {
    case 1:
        // Pantalla de Sensores
        sensorsWindow();
        break;
    case 2:
        // Pantalla de Actuadores
        actuatorsWindow();
        break;
    case 3:
        // Pantalla de Wifi
        wifiWindow();
        break;
    case 6:
        // Pantalla de Graficas
        graphicSensorsWindow();
        break;

    default:
        break;
    }
}

/**
 * @brief
 * Mostrar la ventana de la opción seleccionada en el menu principal
 *
 */
void showWindow()
{
    key = '.';
    okey = '.';
    menu_state = 0; // El menu ya no se muestra
    showWindowTitle(menu_options[window_state - 1]);
    // Limpiar el área de la ventana
    tft.fillRect(0, 16, 320, 224, windowBackgrounds[window_state - 1]);
    refreshWindow();
}

/**
 * @brief
 * Inicializar la pantalla
 */
void initScreen()
{
    // Initialise the TFT screen
    tft.init();

    // Set the rotation before we calibrate
    tft.setRotation(1);

    tft.fillScreen(random(0xFFFF));
    delay(1000);

    // Clear the screen
    tft.fillScreen(TFT_WHITE);
    delay(500);

    showInitialLogo();
    // tft.drawXBitmap(5, 22, sensor_icon, sensor_icon_Width, sensor_icon_Height, TFT_WHITE, TFT_RED);
    delay(3000);
    screen_state = 1; // Cambiar el estado de la pantalla a inicializada
    showStatusBar();
    showHomeMenu();
}