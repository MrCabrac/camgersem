int buffer1[300]; // Almacenar las variables para graficar en la pantalla

void drawAxis()
{
    // Dibujar la linea que encierra el cuadrado
    tft.drawRect(graph_box_init_x, graph_box_init_y, graph_width, graph_height, AXIS_COLOR);

    // Lineas horizontales
    for (int i = graph_box_init_y + (graph_height / VERTICAL_DIVIDER); i < graph_box_init_y + graph_height; i += graph_height / VERTICAL_DIVIDER)
    {
        tft.drawLine(graph_box_init_x, i, graph_box_init_x + 4, i, TFT_LIGHTGREY);
        tft.drawLine(graph_box_init_x + graph_width - 6, i, graph_box_init_x + graph_width - 2, i, TFT_LIGHTGREY);
        for (int j = graph_box_init_x + (graph_width / (HORIZONTAL_DIVIDER * 5)); j < graph_box_init_x + graph_width; j += (graph_width / (HORIZONTAL_DIVIDER * 5)))
        {
            tft.drawPixel(j, i, TFT_DARKGREY);
        }
    }

    // Lineas verticales
    for (int i = graph_box_init_x + (graph_width / HORIZONTAL_DIVIDER); i < graph_box_init_x + graph_width; i += graph_width / HORIZONTAL_DIVIDER)
    {
        tft.drawLine(i, graph_box_init_y, i, graph_box_init_y + 4, TFT_LIGHTGREY);
        tft.drawLine(i, graph_box_init_y + graph_height - 4, i, graph_box_init_y + graph_height - 1, TFT_LIGHTGREY);
        for (int j = graph_box_init_y + (graph_height / (VERTICAL_DIVIDER * 5)); j < graph_box_init_y + graph_height; j += (graph_height / (VERTICAL_DIVIDER * 5)))
        {
            tft.drawPixel(i, j, TFT_DARKGREY);
        }
    }
    // Serial.println("Axis ready");
}

/**
 * @brief
 * @param buffer
 * Poner todo el buffer en -1 para que no se muestre en pantalla
 */
void initBuffers()
{
    Serial.println("Init buffers");
    if (!buffersReady)
    {
        for (int i = 0; i < graph_width; ++i)
        {
            signal_Buffer_1[i] = -1; // do what you want to do here
            signal_Buffer_2[i] = -1; // do what you want to do here
            signal_Buffer_3[i] = -1; // do what you want to do here
        }
        buffersReady = true;
    }
}

void shiftLeftBuffers()
{
    for (int i = 1; i < graph_width; i++)
    {
        signal_Buffer_1[i - 1] = signal_Buffer_1[i];
        signal_Buffer_2[i - 1] = signal_Buffer_2[i];
        signal_Buffer_3[i - 1] = signal_Buffer_3[i];
    }
    signal_Buffer_1[graph_width - 1] = 0;
    signal_Buffer_2[graph_width - 1] = 0;
    signal_Buffer_3[graph_width - 1] = 0;
}

/**
 * @brief
 * Agregar una variable a los buffer
 * @param buffer_selection
 * @param data
 */
void bufferAppend(uint8_t buffer_selection, float data)
{
    // Shift left
    // shiftLeftBuffers();
    // Add value
    if (buffer_selection == 0)
    {
        signal_Buffer_1[graph_width - 1] = data;
    }
    else if (buffer_selection == 1)
    {
        signal_Buffer_2[graph_width - 1] = data;
    }
    else if (buffer_selection == 2)
    {
        signal_Buffer_3[graph_width - 1] = data;
    }
}

/**
 * @brief
 * Graficar la señal que tenga el buffer correspondiente
 */
void drawSignal()
{
    // Serial.println("Pixel read: " + (String)tft.readPixel(0, 0));
    uint16_t pixelReaded = 0;
    for (uint16_t i = 2; i < graph_width; i++)
    {
        if (signal_Buffer_1[i] > graph_box_init_y && signal_Buffer_1[i] < graph_box_init_y + graph_height)
        {
            tft.drawPixel(i + graph_box_init_x, signal_Buffer_1[i], TFT_RED);
        }
        if (signal_Buffer_2[i] > graph_box_init_y && signal_Buffer_2[i] < graph_box_init_y + graph_height)
        {
            tft.drawPixel(i + graph_box_init_x, signal_Buffer_2[i], TFT_GREEN);
        }
        if (signal_Buffer_3[i] > graph_box_init_y && signal_Buffer_3[i] < graph_box_init_y + graph_height)
        {
            tft.drawPixel(i + graph_box_init_x, signal_Buffer_3[i], TFT_CYAN);
        }
    }
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString((String)temperatura_interior, 283, 37, 2);
    tft.drawString((String)humedad_interior, 283, 73, 2);
    tft.drawString((String)luminosidad_interior, 283, 109, 2);

    tft.setTextColor(TFT_WHITE, TFT_RED);
    tft.drawString((String)maxSignals[0], graph_box_init_x + 2, graph_box_init_y + 1, 2);
    tft.drawString((String)minSignals[0], graph_box_init_x + 2, graph_box_init_y + graph_height - 17, 2);
    tft.setTextColor(TFT_BLACK, TFT_GREEN);
    tft.drawString((String)maxSignals[1], graph_box_init_x + 30, graph_box_init_y + 1, 2);
    tft.drawString((String)minSignals[1], graph_box_init_x + 30, graph_box_init_y + graph_height - 17, 2);
    tft.setTextColor(TFT_BLACK, TFT_CYAN);
    tft.drawString((String)maxSignals[2], graph_box_init_x + 60, graph_box_init_y + 1, 2);
    tft.drawString((String)minSignals[2], graph_box_init_x + 60, graph_box_init_y + graph_height - 17, 2);

    // Tiempo
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString((String)(firstDraw / 1000), 283, 147, 2);

    drawAxis();
}

/**
 * @brief
 * Graficar los puntos de setpoint que son configurador por medio del encoder rotativo
 */
void drawSetPoints()
{
    //          x1,y1
    // x0,y0
    //          x2,22
    if (osp1 != sp1)
    {
        osp1 = sp1;
        // Remover el viejo setpoint de la pantalla
        tft.fillRect(graph_box_init_x + graph_width - triangle_width - 2, graph_box_init_y + 1, triangle_width + 1, graph_box_init_y + graph_height - 2, windowBackgrounds[5]);
    }
    if (osp2 != sp2)
    {
        osp2 = sp2;
        tft.fillRect(graph_box_init_x + graph_width - triangle_width - 2, graph_box_init_y + 1, triangle_width + 1, graph_box_init_y + graph_height - 2, windowBackgrounds[5]);
    }
    if (osp3 != sp3)
    {
        osp3 = sp3;
        tft.fillRect(graph_box_init_x + graph_width - triangle_width - 2, graph_box_init_y + 1, triangle_width + 1, graph_box_init_y + graph_height - 2, windowBackgrounds[5]);
    }
    tft.fillTriangle(graph_box_init_x + graph_width - triangle_width - 2, setpoint_y_pos1, graph_box_init_x + graph_width - 2, setpoint_y_pos1 - 4, graph_box_init_x + graph_width - 2, setpoint_y_pos1 + 4, TFT_RED);   // Temperatura
    tft.fillTriangle(graph_box_init_x + graph_width - triangle_width - 2, setpoint_y_pos2, graph_box_init_x + graph_width - 2, setpoint_y_pos2 - 4, graph_box_init_x + graph_width - 2, setpoint_y_pos2 + 4, TFT_GREEN); // Humedad
    tft.fillTriangle(graph_box_init_x + graph_width - triangle_width - 2, setpoint_y_pos3, graph_box_init_x + graph_width - 2, setpoint_y_pos3 - 4, graph_box_init_x + graph_width - 2, setpoint_y_pos3 + 4, TFT_CYAN);  // Iluminacion

    // Set-point seleccionado
    tft.setTextColor(TFT_BLACK, TFT_ORANGE);
    if (sps == 1)
    {
        tft.drawString("Hume", 283, 170, 2);
    }
    else if (sps == 3)
    {
        tft.drawString("Temp", 283, 170, 2);
    }
    else if (sps == 4)
    {
        tft.drawString("Lumi ", 283, 170, 2);
    }
    else
    {
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.drawString("       ", 283, 170, 2);
    }
}

void clearLastDraw()
{
    for (uint16_t i = 0; i < graph_width; i++)
    {
        if (signal_Buffer_1[i] > graph_box_init_y && signal_Buffer_1[i] < graph_box_init_y + graph_height)
        {
            tft.drawPixel(i + graph_box_init_x + 2, signal_Buffer_1[i], windowBackgrounds[window_state - 1]);
        }
        if (signal_Buffer_2[i] > graph_box_init_y && signal_Buffer_2[i] < graph_box_init_y + graph_height)
        {
            tft.drawPixel(i + graph_box_init_x + 2, signal_Buffer_2[i], windowBackgrounds[window_state - 1]);
        }
        if (signal_Buffer_3[i] > graph_box_init_y && signal_Buffer_3[i] < graph_box_init_y + graph_height)
        {
            tft.drawPixel(i + graph_box_init_x + 2, signal_Buffer_3[i], windowBackgrounds[window_state - 1]);
        }
    }
}