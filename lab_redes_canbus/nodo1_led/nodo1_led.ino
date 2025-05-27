//programa que crea un nodo de la red CAN BUS donde se acciona en funcion de un mensaje sobre un led
//se toma como ide de mensaje de activacion 0x100

#include <SPI.h>  // libreria para comunicacion SPI entre Arduino y modulo MCP2515
#include <mcp2515.h> // libreria para manejo de controlador CAN -requiere instalacion en IDE Arduino

struct can_frame canMsg;  // creamos una estructura de mensaje CAN
MCP2515 mcp2515(10);      // Definimos pin CS de conexion al modulo CAN

const int ledPin = 13;  // pin de conexion led //5 en arduino

void setup() {
  pinMode(ledPin, OUTPUT);
  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS);  // definimos velocidad de comunicacion del bus
  mcp2515.setNormalMode();          // establecemos modo de funcionamiento del dispositivo
}

void loop() {
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) //chequeo si llego un mensaje y esta listo para ser leido
    {
    if (canMsg.can_id == 0x100) // Si coincide id del mensaje activo/desactivo el led
    { 
      digitalWrite(ledPin, canMsg.data[0] ? HIGH : LOW);
    }
  }
}
