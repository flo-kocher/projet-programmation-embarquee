//Fichier BRARD Thibault
// Tâches : 
//    - Moteur
//    - Merge
//    - Montage final
//    - Debug du main

//1s entre chaque affichage dans le Serial monitor
#define PERIODE_AFFICHAGE       1000000  

//1024 valeur => 511 
#define MILIEU_POTENTIOMETRE    511 

//gère la pwm
#define CONTROLH_PIN_1          9

//controle le sens horaire
#define CONTROLH_PIN_2          13

//controle le sens anti-horaire
#define CONTROLH_PIN_7          12 

//pin analogique du potentiomètre
#define POTENTIOMETRE_PIN       A0   

//pin analogique de l'axe X de l'accelerometre
#define X_PIN                   A1

//manuel 0, auto 1, bluetooth 2
#define MODE                    1 

static unsigned long ulmicroseconds = 0;
static unsigned long ulPrecMicroseconds = 0; 

//met le sens horaire du moteur
void set_horaire()
{
  digitalWrite(CONTROLH_PIN_7, LOW);
  digitalWrite(CONTROLH_PIN_2, HIGH);
}

//met le sens anti-horaire du moteur
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

  //stocke la valeur de l'axe X temporairement
  int temp_moteur = 0;

  //stocke la valeur qui sera convertit pour la vitesse du moteur
  int value = 0;
  char symb = '+';

  //valeur de l'axe sur X
  int angleX = 0;

  if(MODE == 0)
  {
    // Lecture de la valeur du potentiomètre 
    value = analogRead(POTENTIOMETRE_PIN);  
  }
  else if (MODE == 1) 
  {
    //lecture de la valeur de l'accelerometre
    temp_moteur = analogRead(X_PIN);

    //conversion en valeur d'un potentiometre
    value = map(temp_moteur, 210, 310, 0, 1023);

    //conversion en valeur d'un angle
    angleX = map(temp_moteur, 210, 310, 0, 360);

    //sécurité pour le moteur car l'accelerometre 
    //peut avoir des valeurs fluctuantes en fonction du bon branchement de VCC
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
    symb = '-';
    motorSpeed--;
    set_anti_horaire();
  }

  //variable contenant la vitesse du moteur 
  //12000 car c'est la vitesse max du moteur dans le datasheet
  // vitesse calculée faute d'avoir eu le montage pour la capturer
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
    Serial.print(symb);
    Serial.print(vitesse);
    Serial.println(" tr/min");
    Serial.print("acc : X = ");
    Serial.println(temp_moteur);

    ulPrecMicroseconds = ulmicroseconds;
  }

  // On envoie la vitesse du moteur vers le pont en H 
  analogWrite(CONTROLH_PIN_1, motorSpeed);

}
