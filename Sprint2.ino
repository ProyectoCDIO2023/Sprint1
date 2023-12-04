#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

#define channelHumedad 0
#define channelTemperatura 4
#define channelValue 1     // Canal de lectura del ADC
#define Offset 0.60        // Offset para el cálculo del pH (ajuste específico según el sensor)
#define samplingInterval 20 // Intervalo de tiempo entre muestras de pH (en milisegundos)
#define printInterval 800  // Intervalo de tiempo para imprimir los resultados (en milisegundos)
#define ArrayLength 50     // Longitud del array de muestras de pH

float pHArray[ArrayLength]; // almacena las muestras 
int pHArrayIndex; // Índice actual en el array de muestras de pH

int medidaSeco = 0;
int medidaMojado = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Inicializando...");

  ads.begin();
  ads.setGain(GAIN_ONE);
}

float calculatepH(int adcValue) {
  float voltage = adcValue * 0.0001875; // La resolución es 0.1875 mV por bit para GAIN_TWOTHIRDS
  float pHValue = 3.5 * voltage + Offset;
  return pHValue;
}

void loop() {
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float voltage, pHValue;

  // Llamar a las funciones para obtener los valores de humedad y temperatura
  int humidityValueMojado = readHumedadMojado();
  int temperature = readTemperatura();

  // Realizar muestreo a intervalos regulares
  if (millis() - samplingTime > samplingInterval) {
    // Realizar una lectura del canal del ADS1115
    int16_t adcValue;
    adcValue = ads.readADC_SingleEnded(channelValue);

    // Calcular el valor de pH 
    pHValue = calculatepH(adcValue);
    voltage = adcValue * 0.0001875; // Convertir a voltaje

    // Almacenar la lectura en el array
    pHArray[pHArrayIndex++] = pHValue;
    if (pHArrayIndex == ArrayLength) pHArrayIndex = 0;

    // Reiniciar el tiempo de muestreo
    samplingTime = millis();
  }

  // Imprimir los resultados a intervalos regulares
  if (millis() - printTime > printInterval) {
    Serial.print("Voltage: ");
    Serial.print(voltage, 2);
    Serial.print("    pH value: ");
    Serial.println(pHValue, 2);
    Serial.print("Humedad (mojado): ");
    Serial.print(humidityValueMojado);
    Serial.println("%");
    Serial.print("Temperatura: ");
    Serial.print(temperature);
    Serial.println(" °C");

    printTime = millis();
  }

  // Esperar 5 segundos antes de la próxima lectura
  delay(5000);
}

// Función para leer el valor de humedad en estado mojado
int readHumedadMojado() {
  medidaMojado = ads.readADC_SingleEnded(channelHumedad);
  int humidityValueMojado = map(medidaMojado, 17000, 31000, 100, 0);
  return humidityValueMojado;
}

// Función para leer el valor de temperatura
int readTemperatura() {
  int16_t adcValue = ads.readADC_SingleEnded(channelTemperatura);
  float voltage = (adcValue * 4.096) / 32767;
  int temperature = ((voltage - 0.79) / 0.035);
  return temperature;
}
