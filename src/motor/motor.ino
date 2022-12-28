#define PERIODE_AFFICHAGE       1000000  

const int controlPin1 = 2;                // connected to pin 7 on the H-bridge
const int controlPin2 = 3;                // connected to pin 2 on the H-bridge
const int enablePin = 9;                  // connected to pin 1 on the H-bridge
const int potPin = A0;                    // connected to the potentiometer's output

static unsigned long ulmicroseconds = 0;
static unsigned long ulPrecMicroseconds = 0;

int motorSpeed = 0;      // speed of the motor
void setup() {
  // initialize the inputs and outputs
  pinMode(controlPin1, OUTPUT);
  pinMode(controlPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(A0, INPUT);

  // pull the enable pin LOW to start
  digitalWrite(enablePin, LOW);
  Serial.begin(115200); 
}

void loop() {

  // read the value of the pot and divide by 4 to get a value that can be
  // used for PWM
  int potValue = analogRead(potPin);
  // change the direction the motor spins by talking to the control pins
  // on the H-Bridge
  //en avant
  if (potValue <= 511) {
    motorSpeed = (abs(potValue-511))/2;
    digitalWrite(controlPin1, HIGH);
    digitalWrite(controlPin2, LOW);
  } else { //en arrière
    motorSpeed = (potValue - 511)/2;
    digitalWrite(controlPin1, LOW);
    digitalWrite(controlPin2, HIGH);
  }

  ulmicroseconds = micros();
  if(ulmicroseconds-ulPrecMicroseconds >= PERIODE_AFFICHAGE)
  {
    Serial.print("Valeur du potentiomètre : ");
    Serial.println(potValue);
    Serial.print("speed : ");
    Serial.println(motorSpeed);

    ulPrecMicroseconds = ulmicroseconds;
  }
  analogWrite(enablePin, motorSpeed);

}
