
#include <TimeLib.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

//Pinos utilizados para conexao do modulo GY-NEO6MV2
static const int RXPin = 9, TXPin = 10;

//Objeto TinyGPS++
TinyGPSPlus gps;

//Conexao serial do modulo GPS
SoftwareSerial Serial_GPS(RXPin, TXPin);

void draw()
{
  //Ajuste do horario/timezone
  GPS_Timezone_Adjust();
}

//Ajuste o timezone de acordo com a regiao
const int UTC_offset = -3;

void setup()
{
  //Baud rate Arduino
  Serial.begin(9600);
  //Baud rate Modulo GPS
  Serial_GPS.begin(9600);

  //Mostra informacoes iniciais no serial monitor
  Serial.println(F("Data, Hora, Latitude e Longitude"));
  Serial.println(F("Modulo GPS GY-NEO6MV2"));
  Serial.print(F("Biblioteca TinyGPS++ v. ")); 
  Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println();
}

void loop()
{
  //Conexao com modulo GPS
 while (Serial_GPS.available() > 0)
    if (gps.encode(Serial_GPS.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
   Serial.println(F("No GPS detected: check wiring."));
    while (true);
  }

  delay(1000);
}

void displayInfo()
{
  //Mostra informacoes no Serial Monitor
  Serial.print(F("Location: "));
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6); //latitude
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6); //longitude
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.day()); //dia
    Serial.print(F("/"));
    Serial.print(gps.date.month()); //mes
    Serial.print(F("/"));
    Serial.print(gps.date.year()); //ano
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour()); //hora
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute()); //minuto
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second()); //segundo
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }
  Serial.println();
}

void GPS_Timezone_Adjust()
{
  while (Serial_GPS.available())
  {
    if (gps.encode(Serial_GPS.read()))
    {
      int Year = gps.date.year();
      byte Month = gps.date.month();
      byte Day = gps.date.day();
      byte Hour = gps.time.hour();
      byte Minute = gps.time.minute();
      byte Second = gps.time.second();

      //Ajusta data e hora a partir dos dados do GPS
      setTime(Hour, Minute, Second, Day, Month, Year);
      //Aplica offset para ajustar data e hora
      //de acordo com a timezone
      adjustTime(UTC_offset * SECS_PER_HOUR);
    }
  }
}
