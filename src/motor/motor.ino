const int controlPin1 = 2;                // connected to pin 7 on the H-bridge
const int controlPin2 = 3;                // connected to pin 2 on the H-bridge
const int enablePin = 9;                  // connected to pin 1 on the H-bridge
const int potPin = A0;                    // connected to the potentiometer's output

int motorSpeed = 0;      // speed of the motor

void setup() {
  // initialize the inputs and outputs
  pinMode(controlPin1, OUTPUT);
  pinMode(controlPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);

  // pull the enable pin LOW to start
  digitalWrite(enablePin, LOW);
}

void loop() {

  // read the value of the pot and divide by 4 to get a value that can be
  // used for PWM
  motorSpeed = analogRead(potPin) / 4;

  // change the direction the motor spins by talking to the control pins
  // on the H-Bridge
  if (motorDirection == 1) {
    digitalWrite(controlPin1, HIGH);
    digitalWrite(controlPin2, LOW);
  } else {
    digitalWrite(controlPin1, LOW);
    digitalWrite(controlPin2, HIGH);
  }

  analogWrite(enablePin, motorSpeed);

}
