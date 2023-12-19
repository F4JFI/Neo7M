#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <TimeLib.h>
#include <Timezone.h>

// Configuration des broches pour le module GPS Neo-7
#define GPS_TX D6
#define GPS_RX D7

// Création d'un objet GPS
TinyGPSPlus gps;

// Configuration de la communication série avec le module GPS
SoftwareSerial gpsSerial(GPS_RX, GPS_TX);

// Configuration du fuseau horaire pour Paris (fuseau horaire +1, prenant en compte l'heure d'été)
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120}; // Central European Summer Time
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};  // Central European Time
Timezone tz(CEST, CET);

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600);

  Serial.println("Attente du signal GPS...");
}

void loop() {
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      if (gps.time.isUpdated() && gps.date.isUpdated()) {
        // Récupération de l'heure et de la date de Paris (fuseau horaire +1, prenant en compte l'heure d'été)
        tmElements_t utcTime;
        utcTime.Year = gps.date.year() - 1970;
        utcTime.Month = gps.date.month();
        utcTime.Day = gps.date.day();
        utcTime.Hour = gps.time.hour();
        utcTime.Minute = gps.time.minute();
        utcTime.Second = gps.time.second();

        time_t utcTimestamp = makeTime(utcTime);
        time_t localTimestamp = tz.toLocal(utcTimestamp);

        tmElements_t localTime;
        breakTime(localTimestamp, localTime);

        Serial.print("Heure et date de Paris: ");
        Serial.print(localTime.Year + 1970);
        Serial.print("-");
        Serial.print(localTime.Month);
        Serial.print("-");
        Serial.print(localTime.Day);
        Serial.print(" ");
        Serial.print(localTime.Hour);
        Serial.print(":");
        Serial.print(localTime.Minute);
        Serial.print(":");
        Serial.println(localTime.Second);
      }
    }
  }
}
