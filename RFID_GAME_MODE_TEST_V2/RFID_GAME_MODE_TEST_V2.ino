#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>



#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


static const uint8_t NUM_MENU_ITEMS = 6;
const char* menu_items[] = {
  "Day lo ",
  "Day hi ",
  "Nite lo",
  "Nite hi ",
  "Heat ON",
  "Heat OFF"
};

const char* second_menu_items[] = {
  "Prima ",
  "A doua ",
  "A treia",
};


enum PinAssignments {
  encoderPinA = 2,   // right
  encoderPinB = 3,   // left
  selectButton = 4
};

volatile unsigned int encoderPos = 0;  // a counter for the dial
unsigned int lastReportedPos = 1;   // change management
static boolean rotating = false;    // debounce management

// interrupt service routine vars
boolean A_set = false;
boolean B_set = false;
boolean final_stare_buton ;

void setup() {
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  pinMode(selectButton, INPUT);

  // turn on pullup resistors
  digitalWrite(encoderPinA, HIGH);
  digitalWrite(encoderPinB, HIGH);


  attachInterrupt(digitalPinToInterrupt(encoderPinA), doEncoderA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPinB), doEncoderB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(selectButton), doSelect, CHANGE );


  Serial.begin(115200);


  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);

}

void loop() {

  rotating = true;




    Serial.print("BUTON: ");
    Serial.println(digitalRead(selectButton));
    Serial.println();
    Serial.println();







  if (lastReportedPos != encoderPos) {
    encoderPos = (encoderPos > NUM_MENU_ITEMS - 1 ) ? 0 : encoderPos;
    lastReportedPos = encoderPos;
    uint8_t tempPos = encoderPos;




    display.clearDisplay();


    Serial.print("Semnal: ");
    Serial.println(final_stare_buton);

    for ( uint8_t i = 0; i < NUM_MENU_ITEMS; i++ ) {
      if (final_stare_buton) {
        display.setCursor(0, i  * 16);
        display.println(menu_items[tempPos++]);
      } else {
        display.setCursor(0, i  * 16);
        display.println(second_menu_items[tempPos++]);
      }

    }


    display.display();

  }




}


void doEncoderA() {
  // debounce
  if ( rotating ) delay (1);  // wait a little until the bouncing is done

  // Test transition, did things really change?
  if ( digitalRead(encoderPinA) != A_set ) { // debounce once more
    A_set = !A_set;

    // adjust counter + if A leads B
    if ( A_set && !B_set )
      encoderPos += 1;

    rotating = false;  // no more debouncing until loop() hits again
  }
}

// Interrupt on B changing state, same as A above
void doEncoderB() {
  if ( rotating ) delay (1);
  if ( digitalRead(encoderPinB) != B_set ) {
    B_set = !B_set;
    //  adjust counter - 1 if B leads A
    if ( B_set && !A_set )
      encoderPos -= 1;

    rotating = false;
  }
}


void doSelect() {
  if (digitalRead(selectButton) == 0) {
    final_stare_buton = 0;
  } else {
    final_stare_buton = 1;
  }
}
