#include <Arduino.h>

#define LED 13
#define RX_E 2
#define TX_E 3

//Declaracion de variables
int nivelGas = 99;

//Declaración de funciones
void ajustarNivel();
void handleLed();
void handleSerial();
void initPins();
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
  ajustarNivel();
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

  //Inicializar pines para configuración como receptor
  digitalWrite(TX_E, LOW); //Tx deshabilitado
  digitalWrite(RX_E, LOW); //Rx habilitado
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