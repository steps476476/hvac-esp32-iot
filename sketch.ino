#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22

#define RELAY 2
#define BUZZER 15
#define BUTTON 18
#define POT 34

DHT dht(DHTPIN, DHTTYPE);

volatile bool emergency = false;

float temperature = 0;
int adcValue = 0;

void IRAM_ATTR emergencyISR() {
  static unsigned long lastInterrupt = 0;
  if (millis() - lastInterrupt > 200) { // debounce
    emergency = true;
    lastInterrupt = millis();
  }
}

void sensorTask(void *pv) {
  while (1) {
    temperature = dht.readTemperature();
    adcValue = analogRead(POT);

    Serial.print("Temp: ");
    Serial.print(temperature);
    Serial.print(" | ADC: ");
    Serial.println(adcValue);

    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void controlTask(void *pv) {
  while (1) {

    if (emergency) {
      digitalWrite(RELAY, LOW);
      digitalWrite(BUZZER, HIGH);
      Serial.println("EMERGENCY STOP!");
    }

    else if (temperature > 30) {
      digitalWrite(RELAY, HIGH);
      digitalWrite(BUZZER, LOW);
      Serial.println("Cooling ON");
    }

    else {
      digitalWrite(RELAY, LOW);
      digitalWrite(BUZZER, LOW);
      Serial.println("System Idle");
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void safetyTask(void *pv) {
  while (1) {

    if (temperature > 40) {
      digitalWrite(RELAY, LOW);
      digitalWrite(BUZZER, HIGH);
      Serial.println("OVERHEAT SHUTDOWN!");
    }

    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  dht.begin();

  attachInterrupt(digitalPinToInterrupt(BUTTON), emergencyISR, FALLING);


  xTaskCreate(sensorTask, "Sensor", 2048, NULL, 1, NULL);
  xTaskCreate(controlTask, "Control", 2048, NULL, 1, NULL);
  xTaskCreate(safetyTask, "Safety", 2048, NULL, 2, NULL);
}

void loop() {
}