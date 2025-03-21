#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h> 

const int ledPin = 15;

void tareaEncender(void *parameter);
void tareaApagar(void *parameter);

SemaphoreHandle_t semaforoEncender;
SemaphoreHandle_t semaforoApagar;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);

  // Creamos dos semáforos binarios
  semaforoEncender = xSemaphoreCreateBinary();
  semaforoApagar = xSemaphoreCreateBinary();

  // Creamos las tareas
  xTaskCreate(tareaEncender, "Encender LED", 1000, NULL, 1, NULL);
  xTaskCreate(tareaApagar, "Apagar LED", 1000, NULL, 1, NULL);

  // Damos el primer semáforo para iniciar el ciclo (encender primero)
  xSemaphoreGive(semaforoEncender);
}

void loop() {
  // Nada aquí
}

void tareaEncender(void *parameter) {
  for (;;) {
    xSemaphoreTake(semaforoEncender, portMAX_DELAY);

    digitalWrite(ledPin, HIGH);
    Serial.println("LED Encendido");
    vTaskDelay(pdMS_TO_TICKS(1000)); // Espera 1 segundo

    xSemaphoreGive(semaforoApagar); // Libera semáforo para tarea de apagar
  }
}

void tareaApagar(void *parameter) {
  for (;;) {
    xSemaphoreTake(semaforoApagar, portMAX_DELAY);

    digitalWrite(ledPin, LOW);
    Serial.println("LED Apagado");
    vTaskDelay(pdMS_TO_TICKS(1000)); // Espera 1 segundo

    xSemaphoreGive(semaforoEncender); // Libera semáforo para tarea de encender
  }
}
