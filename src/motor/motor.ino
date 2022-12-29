//1s entre chaque affichage dans le Serial monitor
#define PERIODE_AFFICHAGE       1000000  
//1024 valeur => 511 
#define MILIEU_POTENTIOMETRE    511 
#define CONTROLH_PIN_1          9
#define CONTROLH_PIN_2          3
#define CONTROLH_PIN_7          2
#define POTENTIOMETRE_PIN       A0              

static unsigned long ulmicroseconds = 0;
static unsigned long ulPrecMicroseconds = 0; 

void set_horaire()
{
  digitalWrite(CONTROLH_PIN_7, LOW);
  digitalWrite(CONTROLH_PIN_2, HIGH);
}

void set_anti_horaire()
{
  digitalWrite(CONTROLH_PIN_7, HIGH);
  digitalWrite(CONTROLH_PIN_2, LOW);
}

void setup()
{
  // initialize the inputs and outputs
  pinMode(CONTROLH_PIN_7, OUTPUT);
  pinMode(CONTROLH_PIN_2, OUTPUT);
  pinMode(CONTROLH_PIN_1, OUTPUT);
  pinMode(POTENTIOMETRE_PIN, INPUT);

  // pull the enable pin LOW to start
  digitalWrite(CONTROLH_PIN_1, LOW);
  Serial.begin(115200); 
}

void loop() {
  // Lecture de la valeur du potentiomètre 
  int potValue = analogRead(POTENTIOMETRE_PIN);

  // Explications motorSpeed
  // - On utilise abs() pour avoir constamment une valeur positive
  //
  // - On fait la différence de la valeur du potentiometre par MILIEU_POTENTIOMETRE
  //   pour avoir d'un côté la plage anti horaire et de l'autre la plage horaire
  //
  // - Enfin on divise par 2 pour avoir une valeur comprise entre 0 et 255 pour
  //   être dans la plage du moteur.
  int motorSpeed = (abs(potValue-MILIEU_POTENTIOMETRE))/2;

  if (potValue <= MILIEU_POTENTIOMETRE) 
  {
    //horaire (511->0)
    set_horaire();
  }
  else 
  { 
    // anti-horaire (512->1023)
    set_anti_horaire();
  }

  //affichage toutes les secondes sur le moniteur série
  ulmicroseconds = micros();
  if(ulmicroseconds-ulPrecMicroseconds >= PERIODE_AFFICHAGE)
  {
    Serial.print("Valeur du potentiomètre : ");
    Serial.println(potValue);
    Serial.print("speed : ");
    Serial.println(motorSpeed);

    ulPrecMicroseconds = ulmicroseconds;
  }

  // On envoie la vitesse du moteur vers le pont en H 
  analogWrite(CONTROLH_PIN_1, motorSpeed);

}
