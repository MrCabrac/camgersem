float sensorValues[3]; // Array para guardar el valor de los sensores

void initSerial()
{
    Serial.begin(115200);
    Serial2.begin(115200, SERIAL_8N1, 16, 17); // RX, TX
    Serial2.print("ESP32 Welcome!");
    Serial.println("================\ninitSerial\n================");
}

void interpretarSerial()
{
    if (strlen(serial_buffer) >= 4)
    {
        // Serial.println(serial_buffer);
        char *ptr = strtok(serial_buffer, ";");
        byte i = 0;
        while (ptr)
        {
            sensorValues[i] = strtod(ptr, NULL);
            ptr = strtok(NULL, ";");
            i++;
        }

        // Humedad
        if (serial_buffer[0] == 'H')
        {
            if (sensorValues[1] == 1)
            {
                humedad_interior = sensorValues[2];
            }
            else if (sensorValues[1] == 2)
            {
                // TODO: Borrar las graficas al cambiar el periodo de muestreo
                signal_2_Limits[1] = sensorValues[2];
                maxSignals[1] = signal_2_Limits[1];
                graph_visible = false;
            }
            else if (sensorValues[1] == 3)
            {
                // TODO: Borrar las graficas al cambiar el periodo de muestreo
                signal_2_Limits[0] = sensorValues[2];
                minSignals[1] = signal_2_Limits[0];
                graph_visible = false;
            }
        }
        // Temperatura
        else if (serial_buffer[0] == 'T')
        {
            if (sensorValues[1] == 1)
            {
                temperatura_interior = sensorValues[2];
            }
            else if (sensorValues[1] == 2)
            {
                // TODO: Borrar las graficas al cambiar el periodo de muestreo
                signal_1_Limits[1] = sensorValues[2];
                maxSignals[0] = signal_1_Limits[1];
                graph_visible = false;
            }
            else if (sensorValues[1] == 3)
            {
                // TODO: Borrar las graficas al cambiar el periodo de muestreo
                signal_1_Limits[0] = sensorValues[2];
                minSignals[0] = signal_1_Limits[0];
                graph_visible = false;
            }
        }
        // Iluminacion
        else if (serial_buffer[0] == 'I')
        {
            if (sensorValues[1] == 1)
                luminosidad_interior = sensorValues[2];
            else if (sensorValues[1] == 2)
            {
                // TODO: Borrar las graficas al cambiar el periodo de muestreo
                signal_3_Limits[1] = sensorValues[2];
                maxSignals[2] = signal_3_Limits[1];
                graph_visible = false;
            }
            else if (sensorValues[1] == 3)
            {
                // TODO: Borrar las graficas al cambiar el periodo de muestreo
                signal_3_Limits[0] = sensorValues[2];
                minSignals[2] = signal_3_Limits[0];
                graph_visible = false;
            }
        }
        // Muestra pantalla
        else if (serial_buffer[0] == 'P')
        {
            firstDraw = 0;
            initBuffers();
            periodo_muestro = sensorValues[2];
            // TODO: Borrar las graficas al cambiar el periodo de muestreo
        }
        // Setpoint
        else if (serial_buffer[0] == 'L')
        {
            if (sensorValues[1] == 1) // Set-point temperatura
            {
                osp1 = sp1;
                sp1 = sensorValues[2];
            }
            else if (sensorValues[1] == 2) // Set-point humedad
            {
                osp2 = sp2;
                sp2 = sensorValues[2];
            }
            else if (sensorValues[1] == 3) // Set-point iluminacion
            {
                osp3 = sp3;
                sp3 = sensorValues[2];
            }
            else if (sensorValues[1] == 4) // Set-point iluminacion
            {
                osps = sps;
                sps = sensorValues[2];
            }
        }
        // Esfuerzo de control
        else if (serial_buffer[0] == 'A')
        {
            if (sensorValues[1] == 1) // Actuador 1 | Luces
            {
                actuadores[0] = sensorValues[2];
            }
            if (sensorValues[1] == 2) // Actuador 2 | Celda Peltier
            {
                actuadores[1] = sensorValues[2];
            }
            if (sensorValues[1] == 5) // Actuador 5 | Resistencia 1
            {
                actuadores[4] = sensorValues[2];
            }
            if (sensorValues[1] == 6) // Actuador 6 | Resistencia 2
            {
                actuadores[5] = sensorValues[2];
            }
        }
    }
}

void leerSerial()
{
    while (Serial2.available() > 0)
    {
        char c = Serial2.read();
        if (c == '\n')
        {
            serial_buffer[serial_buffer_counter] = '\0';
            interpretarSerial();
            serial_buffer_counter = 0;
        }
        else
        {
            serial_buffer[serial_buffer_counter] = c;
            serial_buffer_counter++;
        }
    }

    if (buffersReady && window_state == 6 && millis() - lastDraw > periodo_muestro)
    {
        float m1 = -(float)(graph_height - 4) / (signal_1_Limits[1] - signal_1_Limits[0]);
        float m2 = -(float)(graph_height - 4) / (signal_2_Limits[1] - signal_2_Limits[0]);
        float m3 = -(float)(graph_height - 4) / (signal_3_Limits[1] - signal_3_Limits[0]);
        float b1 = graph_box_init_y + 3 + (graph_height - 4) - (m1 * signal_1_Limits[0]);
        float b2 = graph_box_init_y + 3 + (graph_height - 4) - (m2 * signal_2_Limits[0]);
        float b3 = graph_box_init_y + 3 + (graph_height - 4) - (m3 * signal_3_Limits[0]);

        clearLastDraw();
        shiftLeftBuffers();
        bufferAppend(0, m1 * temperatura_interior + b1);
        setpoint_y_pos1 = m1 * sp1 + b1;
        setpoint_y_pos2 = m2 * sp2 + b2;
        setpoint_y_pos3 = m3 * sp3 + b3;
        bufferAppend(1, m2 * humedad_interior + b2);
        bufferAppend(2, m3 * luminosidad_interior + b3);
        firstDraw += periodo_muestro;
        drawSignal();
        drawSetPoints();
        lastDraw = millis();
    }
}