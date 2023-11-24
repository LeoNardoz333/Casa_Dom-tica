//Ventiladores
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
#define izquierda1 12
#define derecha1 13
float temperatura;
//Wifi
bool activado;

void setup() {
  pinMode(izquierda1,OUTPUT);
  pinMode(derecha1,OUTPUT);
  Serial.begin(115200);
  sensors.begin();
}

void loop() {
    imprimir();
    Ventiladores();
}

void imprimir()
{
    /*if (temperatura != DEVICE_DISCONNECTED_C) {
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.println(" °C");
  } else {
    Serial.println("Error al leer la temperatura.");
  }*/
    sensors.requestTemperatures();
    temperatura = sensors.getTempCByIndex(0);
    Serial.print("Temperatura: ");
    Serial.println(temperatura);
}

void Condiciones()
{
  if(activado==true)
  {
    Ventiladores();
  }
}

void Ventiladores()
{
    if(temperatura>28)
    {
        digitalWrite(izquierda1, HIGH);
        digitalWrite(derecha1, LOW);
    }
    else
    {
        digitalWrite(izquierda1, LOW);
        digitalWrite(derecha1, LOW);
    }
}