#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <SSD1306Wire.h>
#include <WiFiUdp.h>

const char *ssid     = "***";
const char *password = "***";

const long utcOffsetInSeconds = 10800;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

SSD1306Wire display(0x3c, 5, 4); // SDA - IO5 (D1), SCL - IO4 (D2) 

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup() {
  Serial.begin(115200);
  display.init();
  display.flipScreenVertically();

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  timeClient.begin();
}

void loop() {
  timeClient.update();

  String strDay = "Day : " + String(daysOfTheWeek[timeClient.getDay()]);
  String time = String(timeClient.getHours()) + " : " + String(timeClient.getMinutes()) + " : " + String(timeClient.getSeconds());
  String date = getFormatDate();

  display.clear();
  display.drawRect(0, 0, 128, 64);
  display.setFont(ArialMT_Plain_16);
  display.drawString(2, 2, strDay);
  display.drawString(2, 22, time);
  display.drawString(2, 42, date);
  display.display();
  delay(1000);
}

String getFormatDate() {
  time_t epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime);
  int day = ptm->tm_mday;
  int month = ptm->tm_mon + 1;
  int year = ptm->tm_year + 1900;
  return String(day) + "/" + String(month) + "/" + String(year);
}