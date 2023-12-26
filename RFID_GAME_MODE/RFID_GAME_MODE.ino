#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET     -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#include <MFRC522.h>
#define RST_PIN         9
#define SS_PIN          10
MFRC522 mfrc522(SS_PIN, RST_PIN);
String readid;
char *punctaj[] = { // sunt stocate toate uid-urile de la carduri
  "fa45f22e", "ec69d973",
  "8425ce73", "69edbd99"
};

bool primul = true ;
bool doi = true;
bool trei = true;
bool patru = true;


#define rgb_rosu 5
#define rgb_verde 7
#define rgb_albastru 6




#define CLK 2 // alb
#define DT 3 //albastru
#define buton 4 //verde sau sw

byte counter = 0;
bool currentStateCLK;
bool lastStateCLK;


#define timp_trecut 5000
unsigned long time_now  = 0;
bool timp = false;
bool meniu_scroll = false;



long int scor_r;
long int scor_a;
long int scor_v;
long int scor_g;

void setup() {

  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(buton, INPUT_PULLUP);




  pinMode(rgb_rosu, OUTPUT);
  pinMode(rgb_verde, OUTPUT);
  pinMode(rgb_albastru, OUTPUT);




  Serial.begin(9600);


  SPI.begin();
  mfrc522.PCD_Init();


  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  delay(2000);

  display.clearDisplay();

  lastStateCLK = digitalRead(CLK);
  attachInterrupt(digitalPinToInterrupt(2), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), updateEncoder, CHANGE);

  counter = 1;

}






void getID()
{
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return false;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return false;
  }
  readid = "";
  for ( uint8_t i = 0; i < 4; i++) {
    readid.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  readid.toLowerCase();
  mfrc522.PICC_HaltA();

  for (byte h = 0; h < 5; h++)
    if (readid == punctaj[h]) { //daca imi gaseste card-ul atunci se activeaza meniu-ul

      meniu_scroll = true;
      display.clearDisplay();
      display.display();
    }
}


void loop() {

  time_now = millis();
  bool btnState = digitalRead(buton); //starea butonului


  if (timp) {//daca s-a activat butonu imi declanseaza pauza de timp

    while (millis() < time_now + timp_trecut) {//aici se schimba si culoarea in caz ca a capturat obiectivul o echipa sau mia multe

      if ((scor_r >= scor_a) && (scor_r >= scor_v) && (scor_r >= scor_g)) {
        digitalWrite(rgb_rosu, LOW);
        digitalWrite(rgb_verde, HIGH);
        digitalWrite(rgb_albastru, HIGH);
      }

      delay(300);

      if ((scor_a >= scor_r) && (scor_a >= scor_v) && (scor_a >= scor_g)) {
        digitalWrite(rgb_rosu, HIGH);
        digitalWrite(rgb_verde, HIGH);
        digitalWrite(rgb_albastru, LOW);
      }

      delay(300);

      if ((scor_v >= scor_a) && (scor_v >= scor_r) && (scor_v >= scor_g)) {
        digitalWrite(rgb_rosu, HIGH);
        digitalWrite(rgb_verde, LOW);
        digitalWrite(rgb_albastru, HIGH);
      }

      delay(300);

      if ((scor_g >= scor_a) && (scor_g >= scor_v) && (scor_g >= scor_r)) {
        digitalWrite(rgb_rosu, LOW);
        digitalWrite(rgb_verde, LOW );
        digitalWrite(rgb_albastru, HIGH);
      }

      delay(300);

    }
    timp = false;//cand s-a facut timpu imi da sa pot adauga scor din nou
  } else {






    getID();




    digitalWrite(rgb_rosu, LOW);
    digitalWrite(rgb_verde, LOW);
    digitalWrite(rgb_albastru, LOW); //atunci cand este posibil capturat sa fie lumina alba

    if (btnState == LOW && meniu_scroll == true) {// atunci cand apas butonu ce se intampla




      if (primul)
        if (readid == punctaj[0])  {//cand am apasat butnonul si se afla pe pozitia 1 sa imi schimbe valoarea din scor_a
          if (counter == 1)
            scor_r = scor_r + 200;

          if (counter == 2)
            scor_a = scor_a + 200;

          if (counter == 3)
            scor_v = scor_v + 200;

          if (counter == 4)
            scor_g = scor_g + 200;
          primul = false;


                timp = true; // dupa ce am apasat butonu sa imi activeze sa asteptam timpu si in acelasi timp sa imi dispara ca o echipa este selectata

        }

      if (doi)
        if (readid == punctaj[1]) {
          if (counter == 1)
            scor_r = scor_r + 250;

          if (counter == 2)
            scor_a = scor_a + 250;

          if (counter == 3)
            scor_v = scor_v + 250;

          if (counter == 4)
            scor_g = scor_g + 250;

          doi = false;

                timp = true; // dupa ce am apasat butonu sa imi activeze sa asteptam timpu si in acelasi timp sa imi dispara ca o echipa este selectata

        }

      if (trei)
        if (readid == punctaj[2]) {
          if (counter == 1)
            scor_r = scor_r + 50;

          if (counter == 2)
            scor_a = scor_a + 50;

          if (counter == 3)
            scor_v = scor_v + 50;

          if (counter == 4)
            scor_g = scor_g + 50;
          trei = false;

                timp = true; // dupa ce am apasat butonu sa imi activeze sa asteptam timpu si in acelasi timp sa imi dispara ca o echipa este selectata

        }

      if (patru)
        if (readid == punctaj[3]) {

          if (counter == 1)
            scor_r = scor_r + 250;

          if (counter == 2)
            scor_a = scor_a + 250;

          if (counter == 3)
            scor_v = scor_v + 250;

          if (counter == 4)
            scor_g = scor_g + 250;

          patru = false;
          
                timp = true; // dupa ce am apasat butonu sa imi activeze sa asteptam timpu si in acelasi timp sa imi dispara ca o echipa este selectata

        }

      display.clearDisplay();
      counter = 0;
      meniu_scroll = false;

    }



    display.setTextSize(1);
    if (counter == 1 && meniu_scroll == true ) { //ce se intampla cand este pe fiecare pozitie
      display.setTextColor(BLACK, WHITE);
      display.setCursor(0, 0);
      display.print(F("Red:"));
      display.println(scor_r);
    } else {
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.print(F("Red:"));
      display.println(scor_r);
    }


    if (counter == 2 && meniu_scroll == true ) {
      display.setTextColor(BLACK, WHITE);
      display.setCursor(0, 8);
      display.print(F("Blue:"));
      display.print(scor_a);
    } else {
      display.setTextColor(WHITE);
      display.setCursor(0, 8);
      display.print(F("Blue:"));
      display.println(scor_a);
    }


    if (counter == 3 && meniu_scroll == true ) {
      display.setTextColor(BLACK, WHITE);
      display.setCursor(0, 16);
      display.print(F("Green:"));
      display.println(scor_v);
    } else {
      display.setTextColor(WHITE);
      display.setCursor(0, 16);
      display.print(F("Green:"));
      display.println(scor_v);
    }



    if (counter == 4 && meniu_scroll == true ) {
      display.setTextColor(BLACK, WHITE);
      display.setCursor(0, 24);
      display.print(F("Yellow:"));
      display.println(scor_g);
    } else {
      display.setTextColor(WHITE);
      display.setCursor(0, 24);
      display.print(F("Yellow:"));
      display.println(scor_g);
    }




    display.display();// sa imi afiseze cu toate modificarile de la scor

  }


}



void updateEncoder() {// sa imi afle tot timpu pozitia la care se afla atunci cand rotesc potentiometrul
  currentStateCLK = digitalRead(CLK);

  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1) {

    if (digitalRead(DT) != currentStateCLK) {// sa imi urce sau coboare pozitia
      counter --;
    } else {
      counter ++;
    }

    if (counter < 1)//atunci cand imi atinge pozitia maxima sau minima din meniu sa se duca la celelalt capat
      counter = 4;
    if (counter > 4)
      counter = 1 ;

    display.clearDisplay();//sa imi dea refresh la ecran ca sa iti faca update la pozitie

  }

  lastStateCLK = currentStateCLK;
}
