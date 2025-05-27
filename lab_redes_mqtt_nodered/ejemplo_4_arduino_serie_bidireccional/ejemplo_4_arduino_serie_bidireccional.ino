//Definimos pines de conexion
#define LED 13
#define PULSADOR 5 //pulsador NA

bool estadoBotonAnterior = HIGH;

void setup() {
  Serial.begin(9600);   //Habilitamos conexion serie
  pinMode(LED, OUTPUT);
  pinMode(PULSADOR, INPUT_PULLUP);  // Pulsador con resistencia pull-up
}

void loop() {
  // Detectar cambios en el estado del pulsador
  bool estadoBoton = digitalRead(PULSADOR);

  if (estadoBoton != estadoBotonAnterior) 
  {
    estadoBotonAnterior = estadoBoton;
    // Envio mensaje solo si hay cambio
    if (estadoBoton == LOW) {
      Serial.println("1");  // Botón presionado
    } 
    else {
      Serial.println("0");  // Botón libre
    }
  }

  // Leer comandos desde Node-RED
  if (Serial.available() > 0) {
    String comando = Serial.readStringUntil('\n');
    comando.trim();  // elimina los espacios en blanco al inicio y al final de una cadena de texto
    if (comando == "LED_ON") {
      digitalWrite(LED, HIGH); //enciendo led
    } else if (comando == "LED_OFF") {
      digitalWrite(LED, LOW); //apago led
    }
  }

  delay(50);  // Antirrebote simple
}
