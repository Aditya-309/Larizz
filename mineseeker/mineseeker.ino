#define ENA 11
#define IN1 9
#define IN2 10

#define ENB 5
#define IN3 6
#define IN4 12

#define trigPin 7
#define echoPin 8

#define metalSensorPin A0
#define buzzerPin 2  // Buzzer connected to digital pin 2

const int metalThreshold = 100; // Adjust based on Serial Monitor readings

void setup() {
  Serial.begin(9600);

  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(metalSensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);  // Setup buzzer pin

  moveForward();
  Serial.println("Cart started moving...");
}

void loop() {
  int metalValue = analogRead(metalSensorPin);
  Serial.print("Metal Sensor: "); Serial.println(metalValue);

  // Metal detection
  if (metalValue < metalThreshold) {
    Serial.println("Metal detected! Stopping for 3 seconds...");
    stopMotors();
    digitalWrite(buzzerPin, HIGH);  // Buzzer ON
    delay(3000);
    digitalWrite(buzzerPin, LOW);   // Buzzer OFF
    moveForward();
    return;
  }

  // Obstacle detection and reroute
  if (getDistance() < 15 && getDistance() != -1) {
    Serial.println("Obstacle detected! Attempting reroute...");
    reroute();
    return;
  }

  moveForward();
  delay(100);
}

// Movement functions
void moveForward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, 200); analogWrite(ENB, 200);
}

void moveBackward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, 200); analogWrite(ENB, 200);
}

void turnLeft() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 200); analogWrite(ENB, 0);
}

void turnRight() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, 0); analogWrite(ENB, 200);
}

void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0); analogWrite(ENB, 0);
}

// Improved obstacle handling
void reroute() {
  stopMotors();
  delay(300);

  // Try left
  turnLeft(); delay(600); stopMotors(); delay(300);
  if (getDistance() > 15 || getDistance() == -1) {
    Serial.println("Path clear on left.");
    moveForward(); return;
  }

  // Try right
  turnRight(); delay(1200); stopMotors(); delay(300);
  if (getDistance() > 15 || getDistance() == -1) {
    Serial.println("Path clear on right.");
    moveForward(); return;
  }

  // If both blocked
  Serial.println("All directions blocked. Going backward...");
  moveBackward(); delay(800); stopMotors(); delay(200);

  // Try forward again
  if (getDistance() > 15 || getDistance() == -1) {
    Serial.println("Path clear after reversing.");
    moveForward(); return;
  }

  Serial.println("Still blocked. Waiting...");
  stopMotors(); delay(1000);
}

long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);
  if (duration == 0) return -1; // timeout or no echo
  return duration * 0.034 / 2;
}
