#include <WiFi.h>
#include <HTTPClient.h>
// #include <ArduinoJson.h>
#include <ArduinoJson.h>

DynamicJsonDocument doc(2048);
String serverName = "https://us-central1-camara-germinacion-semillas.cloudfunctions.net/camgersem/v1";

void setupWifi()
{
    if (enable_wifi && WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Starting wifi");

        WiFi.mode(WIFI_STA);
        // WiFi.setSleep(false); // May help with disconnect? Seems to have been removed from WiFi
        WiFi.begin(ssid, password);
        Serial.println("Connecting to WiFi");
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(100);
        }
        Serial.print("Connected to WiFi network with IP Address: ");
        Serial.println(WiFi.localIP());
        localIp = WiFi.localIP().toString();

        configTime(-18000, 0, ntp_primary, ntp_secondary);
        Serial.println("Waiting on time sync...");
        // while (time(nullptr) < 1510644967 && enable_wifi)
        // {
        //     delay(10);
        // }
        // Serial.println("Time syncronized");
    }
}

void getLocalClock()
{
    // Serial.println("getLocalTime\n================");
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
        return;
    }
    // Serial.println("Time obtained!");
    // Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    // Serial.print("Day of week: ");
    // Serial.println(&timeinfo, "%A");
    // Serial.print("Month: ");
    // Serial.println(&timeinfo, "%B");
    // Serial.print("Day of Month: ");
    // Serial.println(&timeinfo, "%d");
    // Serial.print("Year: ");
    // Serial.println(&timeinfo, "%Y");
    // Serial.print("Hour: ");
    // Serial.println(&timeinfo, "%H");
    // Serial.print("Hour (12 hour format): ");
    // Serial.println(&timeinfo, "%I");
    // Serial.print("Minute: ");
    // Serial.println(&timeinfo, "%M");
    // Serial.print("Second: ");
    // Serial.println(&timeinfo, "%S");

    hh = timeinfo.tm_hour;
    mm = timeinfo.tm_min;
    ss = timeinfo.tm_sec;
}

void sendLocalIP()
{
    HTTPClient http;
    // String serverPath = serverName + "/querier";
    // String serverPath = serverName + "/sensors";
    String serverPath = serverName + "/sensors_data";

    Serial.println("URL: " + serverPath);

    http.begin(serverPath.c_str());

    http.addHeader("Content-Type", "application/json");

    doc["name"] = "camgersem";
    doc["local_ip"] = localIp;
    doc["temperatura"] = temperatura_interior;
    doc["humedad"] = humedad_interior;
    doc["iluminacion"] = luminosidad_interior;
    doc["sp_temperatura"] = sp1;
    doc["sp_humedad"] = sp2;
    doc["sp_iluminacion"] = sp3;
    doc["ec_temperatura"] = actuadores[4];
    doc["ec_humedad"] = actuadores[1];
    doc["ec_iluminacion"] = actuadores[0];

    String httpRequestData = "";
    serializeJson(doc, httpRequestData);
    Serial.println(httpRequestData);

    // httpRequestData = "{\"name\":\"camgersem\",\"local_ip\":\""+localIp+"\",\"temperatura\":"+ (String)temperatura_interior+",\"humedad\":"+ (String)humedad_interior+",\"iluminacion\":"+(String)luminosidad_interior+"}";
    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode > 0)
    {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
    }
    else
    {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
    last_send = millis();
}