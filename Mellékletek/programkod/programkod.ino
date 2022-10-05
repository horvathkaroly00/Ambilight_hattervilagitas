#include "FastLED.h" //FastLed könyvtár használata

#define LEDEK_SZAMA 96 //Összesen mennyi led van 
#define DATA_PIN 6 // Az Arduino melyik data pinjét használjuk
#define FENYERO 170 //Fényerő megadása
#define serialRate 115200 // Baud rate beállítása

// Kommunikációs trigger értékek megadása
uint8_t prefix[] = {'A', 'd', 'a'}, hi, lo, chk, i;

// Led tömb meghatározása
CRGB leds[LEDEK_SZAMA];

void setup() {
  // Ledek meghívása
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, LEDEK_SZAMA);

  // Fényerő beállítása
  FastLED.setBrightness(FENYERO);
  
  // R G B színek tesztelése bekapcsoláskor
  LEDS.showColor(CRGB(255, 0, 0));
  delay(500);
  LEDS.showColor(CRGB(0, 255, 0));
  delay(500);
  LEDS.showColor(CRGB(0, 0, 255));
  delay(500);
  LEDS.showColor(CRGB(0, 0, 0));
  
  //Serial kapcsolat megnyitása
  Serial.begin(serialRate);
  // Komminikációs trigger string elküldése
  Serial.print("Ada\n");
}

void loop() { 
  // Trigger értékre való várakozás
  for(i = 0; i < sizeof prefix; ++i) {
    waitLoop: while (!Serial.available()) ;;
    // Trigger érték következő byte-jának olvasása
    if(prefix[i] == Serial.read()) continue;
    i = 0;
    goto waitLoop;
  }
  
  // Serialból való olvasás
  while (!Serial.available()) ;;
  hi=Serial.read();
  while (!Serial.available()) ;;
  lo=Serial.read();
  while (!Serial.available()) ;;
  chk=Serial.read();
  
  // Checksum ellenőrzés
  if (chk != (hi ^ lo ^ 0x55)) {
    i=0;
    goto waitLoop;
  }
  
  memset(leds, 0, LEDEK_SZAMA * sizeof(struct CRGB));
  // Kommunikáció értékeinek feldolgozása, ledek beállítása
  for (uint8_t i = 0; i < LEDEK_SZAMA; i++) {
    byte r, g, b;    
    while(!Serial.available());
    r = Serial.read();
    while(!Serial.available());
    g = Serial.read();
    while(!Serial.available());
    b = Serial.read();
    leds[i].r = r;
    leds[i].g = g;
    leds[i].b = b;
  }
  
  // Ledek beállításainak frissítése
  FastLED.show();
}
