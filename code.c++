class UltrasonicSensor {
private:
  int triggerPin;
  int echoPin;

public:
  UltrasonicSensor(int triggerPin, int echoPin) : triggerPin(triggerPin), echoPin(echoPin) {
    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);
  }

  long readDistance() {
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
    return pulseIn(echoPin, HIGH);
  }
};

class DistanceIndicator {
private:
  int bluePin;
  int redPin;
  int whitePin;
  int bluePin2;
  unsigned long previousMillis;
  const long interval;

public:
  DistanceIndicator(int bluePin, int redPin, int whitePin, int bluePin2, long interval) : bluePin(bluePin), redPin(redPin), whitePin(whitePin), bluePin2(bluePin2), interval(interval), previousMillis(0) {
    pinMode(bluePin, OUTPUT);
    pinMode(redPin, OUTPUT);
    pinMode(whitePin, OUTPUT);
    pinMode(bluePin2, OUTPUT);
  }

  void blinkLED(int pin) {
    if (millis() - previousMillis >= interval) {
      previousMillis = millis();
      digitalWrite(pin, !digitalRead(pin));
    }
  }

  void turnOffLEDs() {
    digitalWrite(bluePin, LOW);
    digitalWrite(redPin, LOW);
    digitalWrite(whitePin, LOW);
    digitalWrite(bluePin2, LOW);
  }
};

UltrasonicSensor ultrasonicSensor(19, 18);
DistanceIndicator distanceIndicator(13, 12, 14, 27, 300);

void setup() {
  Serial.begin(9600);
}

void loop() {
  long duration = ultrasonicSensor.readDistance();
  int cm = 0.01723 * duration;
  int inches = cm / 2.54;

  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.println("cm");

  const int thresholdArray[] = {10, 20, 30, 40};
  const int pinArray[] = {13, 12, 14, 27};

  distanceIndicator.turnOffLEDs();

  for (int i = 0; i < sizeof(thresholdArray) / sizeof(thresholdArray[0]); ++i) {
    if (cm <= thresholdArray[i]) {
      distanceIndicator.blinkLED(pinArray[i]);
      break;
    }
  }

  delay(100);
}
