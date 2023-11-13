#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

#define channelHumedad 0
#define channelTemperatura 2

int medidaSeco = 0;
int medidaMojado = 0;

void setup() {
  Serial.begin(9600);
  ads.begin();
  ads.setGain(GAIN_ONE);
}

void loop() {

  // Leer y almacenar la medida con el sensor mojado (humedad)
  medidaMojado = ads.readADC_SingleEnded(channelHumedad);
  
  // Mapear el valor leído para obtener el valor de humedad mojado
  int humidityValueMojado = map(medidaMojado, 14000, 31000, 100, 0);
  
  // Imprimir el valor leído en mojado
  Serial.print("Medida mojado (Humedad): ");
  Serial.println(medidaMojado);
  Serial.print("Humedad (mojado): ");
  Serial.print(humidityValueMojado);
  Serial.println("%");
  
int16_t adc0 = ads.readADC_SingleEnded(channelTemperatura);
  float voltage = (adc0 * 4.096) / 32767;

Serial.print("Voltage: ");
Serial.print(voltage, 4); // 4 decimales para mayor precisión
Serial.println(" V");


  //Convertir el valor analogico a temperatura usando la formula
  int temperature = ((voltage - 0.79) / 0.035);

  // Imprimir el valor de la temperatura al Seraail monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  
  delay(5000);
}
