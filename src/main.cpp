#include <Arduino.h>

#define LED 13
#define RX_E 2
#define TX_E 3

#define LED_LEVEL0 4
#define LED_LEVEL1 5
#define LED_LEVEL2 8
#define SENSOR_GAS A1

//Declaracion de variables
int nivelGas = 99;

//Declaración de funciones
void ajustarNivel();
void handleLed();
void handleSerial();
void initPins();
void leerNivel();
void mostrarNivelLeds();
void returnGasVal();

void setup()
{
  //Inicializar Serial
  Serial.begin(4800); //Realmente son 9600

  //Esperar a Inicializacion de Serial
  while (!Serial)
  {
  };

  //Inicializar pines
  initPins();
}

void loop()
{
  //Manejar led de actividad
  handleLed();

  //Manejar comunicacion serial
  handleSerial();

  //Ajustar nivel de gas (DEMO)
  //ajustarNivel();

  //Mostrar nivel gas en leds
  mostrarNivelLeds();

  //Leer valor de sensor
  leerNivel();
}

/** Definicion de funciones **/
void ajustarNivel()
{
  static unsigned long timeRef;

  if (millis() - timeRef > 8000)
  {
    nivelGas--;
    if (nivelGas == 0)
    {
      nivelGas = 99;
    }
    timeRef = millis();
  }
}

void handleLed()
{
  static unsigned long timeRef;

  if (millis() - timeRef > 1000)
  {
    digitalWrite(LED, !digitalRead(LED));
    timeRef = millis();
  }
}

void handleSerial()
{
  if (Serial.available())
  {
    //Dato disponible
    //Leer byte entrante
    char mChar = (char)Serial.read();

    //Evaluar caracter
    if (mChar == '2')
    {
      delay(10);
      returnGasVal();
    }
  }
}

void initPins()
{
  //Declarar pines como salida
  pinMode(LED, OUTPUT);
  pinMode(RX_E, OUTPUT);
  pinMode(TX_E, OUTPUT);

  pinMode(LED_LEVEL0, OUTPUT);
  pinMode(LED_LEVEL1, OUTPUT);
  pinMode(LED_LEVEL2, OUTPUT);

  //Inicializar pines para configuración como receptor
  digitalWrite(TX_E, LOW); //Tx deshabilitado
  digitalWrite(RX_E, LOW); //Rx habilitado
}

void leerNivel()
{
  static unsigned long timeRef;

  if (millis() - timeRef > 250)
  {
    //Obtener valor del adc
    float valorAdc = analogRead(SENSOR_GAS);

    //Convertir a voltaje
    float voltaje = (valorAdc * 5.0) / 1023.0;

    //Convertir a nivel de gas
    nivelGas = (voltaje * 100) / 3.3;

    //Imprimir valores
    /*Serial.println("Valores -> ");
    Serial.println(String(valorAdc));
    Serial.println(String(voltaje));
    Serial.println(String(nivelGas));
    Serial.println("");*/

    //Obtener referencia de tiempo
    timeRef = millis();
  }
}

void returnGasVal()
{
  //Convertir valor int de gas a String
  String gasString = String(nivelGas);

  //Convertirse en transmisor
  digitalWrite(RX_E, HIGH); //Rx deshabilitado
  digitalWrite(TX_E, HIGH); //Tx habilitado

  //Enviar valor por puerto serial
  Serial.print(gasString);
  delay(10);

  //Regresar a ser receptor
  digitalWrite(TX_E, LOW); //Tx deshabilitado
  digitalWrite(RX_E, LOW); //Rx habilitado
}

//Mostrar nivel de gas en leds
void mostrarNivelLeds()
{
  if (nivelGas == 0)
  {
    digitalWrite(LED_LEVEL0, HIGH);
    digitalWrite(LED_LEVEL1, HIGH);
    digitalWrite(LED_LEVEL2, HIGH);
  }
  else if (nivelGas <= 33)
  {
    digitalWrite(LED_LEVEL0, LOW);
    digitalWrite(LED_LEVEL1, HIGH);
    digitalWrite(LED_LEVEL2, HIGH);
  }
  else if (nivelGas <= 66)
  {
    digitalWrite(LED_LEVEL0, LOW);
    digitalWrite(LED_LEVEL1, LOW);
    digitalWrite(LED_LEVEL2, HIGH);
  }
  else
  {
    digitalWrite(LED_LEVEL0, LOW);
    digitalWrite(LED_LEVEL1, LOW);
    digitalWrite(LED_LEVEL2, LOW);
  }
}