//programa que crea un nodo de la red CAN BUS donde se generan mensajes de sensores boton y potenciometro
//se toma como id del mensaje de activacion del pulsador 0x100
//se toma como id del mensaje del potenciometro 0x101

#include <SPI.h> // libreria para comunicacion SPI entre Arduino y modulo MCP2515
#include <mcp2515.h> // libreria para manejo de controlador CAN -requiere instalacion en IDE Arduino

struct can_frame canMsg; // creamos una estructura de mensaje CAN
MCP2515 mcp2515(10); // Definimos pin CS de conexion al modulo CAN

const int buttonPin = 7; // pin de conexion boton
const int potPin = A0;   // pin de conexion potenciometro

bool lastButtonState = HIGH;
bool buttonPressed = false;
bool toggleState = false; // almacena el estado ON/OFF tipo llave

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS); // definimos velocidad de comunicacion del bus
  mcp2515.setNormalMode(); // establecemos modo de funcionamiento del dispositivo - hay otras opciones como listenonly, debug, etc
}

void loop() {
  // Lectura del botón
  bool currentButtonState = digitalRead(buttonPin);

  // Si se detecta flanco de subida (se soltó el botón) creo mensaje
  if (lastButtonState == LOW && currentButtonState == HIGH) {
    toggleState = !toggleState;  // alternar estado
    canMsg.can_id  = 0x100; //defino ide mensaje boton
    canMsg.can_dlc = 1; //can_dlc indica al bus CAN numerode bytes del mensaje
    canMsg.data[0] = toggleState; //guardo dato util
    mcp2515.sendMessage(&canMsg); //envio dato al bus
  }
  lastButtonState = currentButtonState;

  // Creo mensaje del Potenciómetro
  int potValue = analogRead(potPin); //lectura analogica
  canMsg.can_id  = 0x101;
  canMsg.can_dlc = 2;
  // El dato del potenciometro es de 10 bit, pero can solo me deja enviar informacion en tamaño byte,
  // entonces lo separo y lo envio en dos byte independientes, luego donde lo reciba lo tengo que rearmar
  canMsg.data[0] = (potValue >> 8) & 0xFF;
  canMsg.data[1] = potValue & 0xFF;
  mcp2515.sendMessage(&canMsg);
  delay(100);
}
