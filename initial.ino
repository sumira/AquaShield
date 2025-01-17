const int flowSensorPin = 2;
volatile int pulseCount = 0;
float flowRate = 0.0;
float totalLiters = 0.0;
unsigned long lastMeasurementTime = 0;
const int measurementInterval = 1000;
const float leakThreshold = 30.0;
const unsigned long maxDuration = 5000;
unsigned long leakStartTime = 0;
bool leakDetected = false;

void IRAM_ATTR countPulse() {
  pulseCount++;
}

void setup() {
  Serial.begin(115200);
  pinMode(flowSensorPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(flowSensorPin), countPulse, RISING);
}

void loop() {
  if (millis() - lastMeasurementTime >= measurementInterval) {
    lastMeasurementTime = millis();
    flowRate = (pulseCount * 60.0) / 7.5;
    totalLiters += (flowRate / 60.0);
    pulseCount = 0;

    Serial.print("Flow Rate: ");
    Serial.print(flowRate);
    Serial.println(" L/min");

    Serial.print("Total Water Used: ");
    Serial.print(totalLiters);
    Serial.println(" L");

    if (flowRate > leakThreshold) {
      if (!leakDetected) {
        leakStartTime = millis();
        leakDetected = true;
      } else if (millis() - leakStartTime >= maxDuration) {
        Serial.println("Leak Detected! Immediate Action Required.");
      }
    } else {
      leakDetected = false;
    }
  }
  delay(100);
}
