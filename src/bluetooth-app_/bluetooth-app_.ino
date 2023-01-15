#include <SoftwareSerial.h>
#define rxPin 10
#define txPin 11
//on inclut la bibliothèque SoftwareSerial qui permet la communication entre les pin digital de la carte arduino

//ne pas oublier de mettre à 9600 baud pour voir le résultat sur le Serial Monitor
SoftwareSerial mySerial(rxPin, txPin); //on assigne les pin
char trameRecue = "";//chaine qui correspond à la trame que l'application nous envoie avec les données du roll et du pitch
char nbRpm = "+10";//chaine test qui correspond aux nombres de tours par minute

void setup() {
  // On ouvre le port pour les communications:
  Serial.begin(9600);
  while (!Serial) {
    ; // on attend qu'il se connecte. nécessaire pour les ports usb natifs seulement.
  }
  // on assigne le port pour mySerial également.
  mySerial.begin(9600);
}

void loop() {
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  if (Serial.available()) {//on en a besoin pour que le tout fonctionne
    mySerial.write(Serial.read());

    //tout les 200ms on recoit les valeurs pitch et roll du smartphone
    trameRecue = Serial.read();//on lit le contenu pour voir la trame envoyé en bluetooth
    Serial.print(trameRecue);//on la print pour voir le contenu et on voit que ce qui est renovyé c'est à dire ("valeur du pitch" "point virgule" "valeur du roll")
    Serial.print("\n");
    Serial.write(nbRpm); //on envoie les tours par minutes du moteur pour pouvoir l'afficher sur l'appli;
    Serial.print("\n");
  }
}