#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

#define channelHumedad 0
#define channelTemperatura 2

int medidaSeco = 0;
int medidaMojado = 0;

void setup() {
  // Iniciar comunicación serial para mostrar los resultados
  Serial.begin(9600);

  // Iniciar el sensor ADS1115
  ads.begin();
  ads.setGain(GAIN_ONE);
}

void loop() {
  // Llamar a las funciones para obtener los valores de humedad y temperatura
  int humidityValueMojado = readHumedadMojado();
  int temperature = readTemperatura();

  // Mostrar el valor de la humedad leída en estado mojado
  Serial.print("Humedad (mojado): ");
  Serial.print(humidityValueMojado);
  Serial.println("%");

  // Mostrar el valor de la temperatura
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  // Esperar 5 segundos antes de la próxima lectura
  delay(5000);
}

// Función para leer el valor de humedad en estado mojado
int readHumedadMojado() {
  medidaMojado = ads.readADC_SingleEnded(channelHumedad);
  
  // Mapear el valor leído para obtener la humedad en estado mojado
  int humidityValueMojado = map(medidaMojado, 14000, 31000, 100, 0);

  // Mostrar el valor crudo de humedad
  Serial.print("Medida mojado (Humedad): ");
  Serial.println(medidaMojado);

  return humidityValueMojado;
}

// Función para leer el valor de temperatura
int readTemperatura() {
  int16_t adc0 = ads.readADC_SingleEnded(channelTemperatura);
  float voltage = (adc0 * 4.096) / 32767;

  // Mostrar el valor de voltaje leído
  Serial.print("Voltage: ");
  Serial.print(voltage, 4); // Mostrar 4 decimales para mayor precisión
  Serial.println(" V");

  // Convertir el valor analógico a temperatura usando la fórmula
  int temperature = ((voltage - 0.79) / 0.035);
  return temperature;
}
