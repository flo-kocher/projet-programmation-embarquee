//1s entre chaque affichage dans le Serial monitor
#define PERIODE_AFFICHAGE       1000000  
//1024 valeur => 511 
#define MILIEU_POTENTIOMETRE    511 
#define CONTROLH_PIN_1          9
#define CONTROLH_PIN_2          13
#define CONTROLH_PIN_7          12
#define POTENTIOMETRE_PIN       A0   

#define X_PIN                   A3

#define MODE                    0 //manuel 0, auto 1, bluetooth 2

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
  pinMode(X_PIN, INPUT);

  // pull the enable pin LOW to start
  digitalWrite(CONTROLH_PIN_1, LOW);
  Serial.begin(115200); 
}

void loop() {

  int value = 0;

  if(MODE == 0)
  {
    // Lecture de la valeur du potentiomètre 
    value = analogRead(POTENTIOMETRE_PIN);  
  }
  else if (MODE == 1) 
  {
    value = map(analogRead(X_PIN), 210, 310, 0, 1023);

    //sécurité pour le moteur
    if(value > 1023)
    {
      value = 1023;
    }
    else if (value < 0)
    {
      value = 0;
    }
  }

  // Explications motorSpeed
  // - On utilise abs() pour avoir constamment une valeur positive
  //
  // - On fait la différence de la valeur du potentiometre par MILIEU_POTENTIOMETRE
  //   pour avoir d'un côté la plage anti horaire et de l'autre la plage horaire
  //
  // - Enfin on divise par 2 pour avoir une valeur comprise entre 0 et 255 pour
  //   être dans la plage du moteur.
  int motorSpeed = (abs(value-MILIEU_POTENTIOMETRE))/2;

  if (value <= MILIEU_POTENTIOMETRE) 
  {
    //horaire (511->0)
    set_horaire();
  }
  else 
  { 
    // anti-horaire (512->1023)
    motorSpeed--;
    set_anti_horaire();
  }
  int vitesse = map(motorSpeed, 0, 255, 0, 12000);

  //affichage toutes les secondes sur le moniteur série
  ulmicroseconds = micros();
  if(ulmicroseconds-ulPrecMicroseconds >= PERIODE_AFFICHAGE)
  {
    if(MODE == 0)
      Serial.print("Valeur du potentiomètre : ");
    else if (MODE == 1)
      Serial.print("Valeur de l'accéléromètre : ");
    Serial.println(value);
    Serial.print("speed : ");
    Serial.println(vitesse);

    ulPrecMicroseconds = ulmicroseconds;
  }

  // On envoie la vitesse du moteur vers le pont en H 
  analogWrite(CONTROLH_PIN_1, motorSpeed);

}
