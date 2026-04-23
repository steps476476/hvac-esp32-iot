#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22

#define BUTTON_PIN 18
#define RELAY_PIN 2
#define BUZZER_PIN 15
#define POT_PIN 34

DHT dht(DHTPIN, DHTTYPE);

volatile bool emergency = false;
float temperature = 0;
int adcValue = 0;

void IRAM_ATTR buttonISR() {
  static unsigned long lastInterrupt = 0;
  if (millis() - lastInterrupt > 200) {
    emergency = true;
    lastInterrupt = millis();
  }
}

void sensor_task(void *pv) {
  while (1) {
    temperature = dht.readTemperature();
    adcValue = analogRead(POT_PIN);

    Serial.print("Temp: ");
    Serial.print(temperature);
    Serial.print(" | ADC: ");
    Serial.println(adcValue);

    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void control_task(void *pv) {
  while (1) {

    if (emergency) {
      digitalWrite(RELAY_PIN, LOW);
      digitalWrite(BUZZER_PIN, HIGH);
      Serial.println("EMERGENCY STOP");
    }

    else if (temperature > 30) {
      digitalWrite(RELAY_PIN, HIGH);
      digitalWrite(BUZZER_PIN, LOW);
      Serial.println("Cooling ON");
    }

    else {
      digitalWrite(RELAY_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);
      Serial.println("System Idle");
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void safety_task(void *pv) {
  while (1) {
    if (temperature > 40) {
      digitalWrite(RELAY_PIN, LOW);
      digitalWrite(BUZZER_PIN, HIGH);
      Serial.println("OVERHEAT SHUTDOWN!");
    }

    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  dht.begin();

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, FALLING);

  xTaskCreate(sensor_task, "Sensor", 2048, NULL, 1, NULL);
  xTaskCreate(control_task, "Control", 2048, NULL, 1, NULL);
  xTaskCreate(safety_task, "Safety", 2048, NULL, 2, NULL);
}

void loop() {
  
}
