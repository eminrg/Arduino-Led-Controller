/*
  Reconnecting MQTT example - non-blocking

  This sketch demonstrates how to keep the client connected
  using a non-blocking reconnect function. If the client loses
  its connection, it attempts to reconnect every 5 seconds
  without blocking the main loop.

*/

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include "FastLED.h"
#define NUM_LEDS 300
CRGB leds[NUM_LEDS];
#define PIN 6
#define PIN2 7
#define SwPIN 5
byte Sparkle_State = 0;
boolean publish_state[11] = {true, true, true, true, true, true, true, true, true, true, true};
byte rosu = 0;
byte verde = 0;
byte albastru = 0;
int senseMode = 0;
// vars for running lights
byte runred = 0xff;
byte rungreen = 0xff;
byte runblue = 0x00;
int runWaveDelay = 20;
// end of vars running lights
//cylon parameters
int CylonSpeedDelay;
int CylonReturnDelay;
int CylonEyeSize;
String message;
String message1;
// Update these with values suitable for your hardware/network.  --- 202 ip for due
//byte mac[]    = {  0xDE, 0xED, 0xBA, 0x11, 0x1E, 0xBC };
//IPAddress ip(192, 168, 1, 178);
//byte mac[]    = {  0xDE, 0xED, 0xBA, 0x0E, 0x0E, 0xED };
IPAddress ip(192, 168, 1, 202);
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xAE, 0x0E, 0xED };
//IPAddress ip(192, 168, 1, 200);
IPAddress server(192, 168, 1, 68);

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    messageTemp += (char)payload[i];
  }

  if (String(topic) == "masa/leduri/control/cylon") {
    //  (byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay)
    //CylonBounce(0xff, 0, 0, 4, 10, 50);
    Serial.print(messageTemp);
    if (messageTemp == "default")
    {
     runred = 0xff;
    rungreen = 0x00;
    runblue = 0x00;
    CylonEyeSize = 4;
    CylonSpeedDelay = 10;
    CylonReturnDelay = 50;

    }
    else {
      String hex = getValue(messageTemp, ':', 0);
      CylonEyeSize  = getValue(messageTemp, ':', 1).toInt();
       CylonSpeedDelay  = getValue(messageTemp, ':', 2).toInt();
        CylonReturnDelay  = getValue(messageTemp, ':', 3).toInt();
      runred = getColorCustom(hex, "r");
      rungreen = getColorCustom(hex, "g");
      runblue = getColorCustom(hex, "b");

    }

    senseMode = 6;
    publish_state[senseMode] = true;
    message = messageTemp;
  }

  if (String(topic) == "masa/leduri/control/running") {

    Serial.print(messageTemp);
    if (messageTemp == "default")
    {
      runred = 0xff;
      rungreen = 0xff;
      runblue = 0x00;
      runWaveDelay = 20;

    }
    else {
      String hex = getValue(messageTemp, ':', 0);
      runWaveDelay = getValue(messageTemp, ':', 1).toInt();
      runred = getColorCustom(hex, "r");
      rungreen = getColorCustom(hex, "g");
      runblue = getColorCustom(hex, "b");

    }

    senseMode = 5;
    publish_state[senseMode] = true;
    message = messageTemp;
  }
   if (String(topic) == "masa/leduri/control/colorwipe") {

    Serial.print(messageTemp);
    if (messageTemp == "default")
    {
      runred = 0x00;
      rungreen = 0xff;
      runblue = 0x00;
      runWaveDelay = 50;

    }
    else {
      String hex = getValue(messageTemp, ':', 0);
      runWaveDelay = getValue(messageTemp, ':', 1).toInt();
      runred = getColorCustom(hex, "r");
      rungreen = getColorCustom(hex, "g");
      runblue = getColorCustom(hex, "b");

    }

    senseMode = 8;
    publish_state[senseMode] = true;
    message = messageTemp;
  }
 

  if (String(topic) == "masa/leduri") {
    Serial.print("mesaj venit pe masa/leduri \n");
    Serial.print(messageTemp);
    if (messageTemp == "rainbow") {
      senseMode = 1;
      publish_state[senseMode] = true;
    }
    if (messageTemp == "rgbloop") {
      senseMode = 2;
      publish_state[senseMode] = true;
    }

    if (messageTemp == "off") {
      senseMode = 0;
      publish_state[senseMode] = true;
    }
    if (messageTemp == "kitt") {
      senseMode = 4;
      publish_state[senseMode] = true;
    }
    if (messageTemp == "running") {
      senseMode = 5;
      publish_state[senseMode] = true;
    }
    if (messageTemp == "cylon")
    {
      senseMode = 6;
      publish_state[senseMode] = true;
      //  (byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay)
    //CylonBounce(0xff, 0, 0, 4, 10, 50);
    runred = 0xff;
    rungreen = 0x00;
    runblue = 0x00;
    CylonEyeSize = 4;
    CylonSpeedDelay = 10;
    CylonReturnDelay = 50;
    }
    if (messageTemp == "sparkle"){
      senseMode = 7;
      publish_state[senseMode] = true;
    }
    if (messageTemp == "colorwipe"){
      senseMode = 8;
      publish_state[senseMode] = true;
    }
    if (messageTemp == "twinkle"){
      senseMode = 9;
      publish_state[senseMode] = true;
    }
    if (messageTemp == "fade") {
      senseMode = 10;
      publish_state[senseMode] = true;
    }
    if (messageTemp == "steag"){
      senseMode = 11;
      publish_state[senseMode] = true;
    }


    message = messageTemp;
  }
  if (String(topic) == "masa/leduri/culoare") {
    Serial.print("mesaj venit pe masa/leduri/culoare \n");
    Serial.print(messageTemp);

    if (messageTemp.startsWith("r") || messageTemp.startsWith("g") || messageTemp.startsWith("b") )
    {
      senseMode = 3;
      message = messageTemp;
      publish_state[senseMode] = true;
      /*    rosu = 0;
          albastru = 0;
          verde = 0;*/
      if (messageTemp.startsWith("r"))
      {
        int number = strtol( &messageTemp[1], NULL, 10);
        rosu = (byte) number;
      }
      if (messageTemp.startsWith("g"))
      {
        int number = strtol( &messageTemp[1], NULL, 10);
        verde = (byte) number;
      }
      if (messageTemp.startsWith("b"))
      {
        int number = strtol( &messageTemp[1], NULL, 10);
        albastru = (byte) number;
      }

    }
    if (messageTemp.startsWith("#"))
    {
      senseMode = 3;
      publish_state[senseMode] = true;
      message = messageTemp;
      long number = strtol( &messageTemp[1], NULL, 16);
      //use getcolor custom
      // Split them up into r, g, b values
      long r = number >> 16;
      long g = number >> 8 & 0xFF;
      long b = number & 0xFF;
      rosu = (byte) r;
      verde = (byte) g;
      albastru = (byte) b;

    }


    else {
      if (messageTemp == "rosu") {
        senseMode = 3;
        rosu = 255;
        verde = 0;
        albastru = 0;
        message = messageTemp;
      }
      if (messageTemp == "verde") {
        senseMode = 3;
        rosu = 0;
        verde = 255;
        albastru = 0;
        message = messageTemp;
      }
      if (messageTemp == "albastru") {
        senseMode = 3;
        rosu = 0;
        verde = 0;
        albastru = 250;
        message = messageTemp;
      }
    }

  }

}


EthernetClient ethClient;
PubSubClient client(ethClient);

long lastReconnectAttempt = 0;

boolean reconnect() {
  if (client.connect("arduinoClient")) {
    // Once connected, publish an announcement...
    client.publish("outTopic", "hello world");
    // ... and resubscribe
    client.subscribe("masa/rainbow");
    client.subscribe("masa/rgbloop");
    client.subscribe("masa/leduri");
    client.subscribe("masa/leduri/culoare");
    client.subscribe("masa/leduri/control/running");
    client.subscribe("masa/leduri/control/cylon");
    client.subscribe("masa/leduri/control/colorwipe");
    client.subscribe("masa/rosu");

  }
  return client.connected();
}

void setup()
{
  Serial.begin(57600);
  client.setServer(server, 1883);
  client.setCallback(callback);
  pinMode(SwPIN, OUTPUT);
  digitalWrite(SwPIN, HIGH);
  FastLED.addLeds<WS2811, PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<WS2811, PIN2, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  Ethernet.begin(mac, ip);
  delay(700);
  lastReconnectAttempt = 0;
}


void loop()
{
  if (!client.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
      // Attempt to reconnect
      if (reconnect()) {
        lastReconnectAttempt = 0;
      }
    }
  } else {
    // Client connected
    message1 = message;
    switch (senseMode) {
      case 0:
        setAll(0, 0, 0);
        if (publish_state[0]) {
          publish_state[0] = false;
          client.publish("outTopic", "Lights Off");

        }
        digitalWrite(SwPIN, HIGH);
        break;
      case 1:
        // light should be off
        digitalWrite(SwPIN, LOW);
         if (publish_state[1]) {
          publish_state[1] = false;
        client.publish("outTopic", "RainbowCycle");
         }
        rainbowCycle(20);

        //  Serial.println("sugi pula inca rulez rainbow ");
        break;
      case 2:
        // light should be on
        digitalWrite(SwPIN, LOW);
         if (publish_state[2]) {
          publish_state[2] = false;
        client.publish("outTopic", "RGBLoop");
         }
        RGBLoop();
        break;
      case 3:
        digitalWrite(SwPIN, LOW);
         if (publish_state[3]) {
          publish_state[3] = false;
        client.publish("outTopic", "Picked Color");
         }
        setAll(rosu, verde, albastru);
        break;
      case 4:
        digitalWrite(SwPIN, LOW);
         if (publish_state[4]) {
          publish_state[4] = false;
        client.publish("outTopic", "NewKITT");
         }
        NewKITT(0xff, 0, 0, 8, 10, 50);

        break;
      case 5:
       if (publish_state[5]) {
          publish_state[5] = false;
        client.publish("outTopic", "RunningLights");
       }
        digitalWrite(SwPIN, LOW);
        RunningLights(runred, rungreen, runblue, runWaveDelay);

        break;
      case 6:
        if (publish_state[6]) {
          publish_state[6] = false;
          client.publish("outTopic", "Cylon");
        }
        digitalWrite(SwPIN, LOW);
        
        //CylonBounce(0xff, 0, 0, 4, 10, 50);
        CylonBounce(runred, rungreen, runblue, CylonEyeSize, CylonSpeedDelay, CylonReturnDelay);

        break;
      case 7:
       if (publish_state[7]) {
          publish_state[7] = false;
        client.publish("outTopic", "Sparkle");
       }
        digitalWrite(SwPIN, LOW);
        Sparkle(random(255), random(255), random(255), 0);
        break;
      case 8:
      if (publish_state[8]) {
          publish_state[8] = false;
        client.publish("outTopic", "ColorWipe");
        setAll(0,0,0);
      }
        digitalWrite(SwPIN, LOW);
        colorWipe(runred, rungreen, runblue, runWaveDelay);
        colorWipe(0x00, 0x00, 0x00, 50);
        break;
      case 9:
      if (publish_state[9]) {
          publish_state[9] = false;
        client.publish("outTopic", "Color Twinkle");
      }
        digitalWrite(SwPIN, LOW);
        TwinkleRandom(20, 100, false);
        break;
      case 10:
        if (publish_state[10]) {
          publish_state[10] = false;
          client.publish("outTopic", "Color Fader");
        }
        digitalWrite(SwPIN, LOW);
        FadeInOut(rosu, verde, albastru);
        break;
      case 11:
        digitalWrite(SwPIN, LOW);
        tricolor();
        break;
    }
    client.loop();
  }
}

void rainbowCycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    client.loop();
    if (message != message1) return;
    for (i = 0; i < NUM_LEDS; i++) {
      client.loop();
      if (message != message1) return;
      c = Wheel(((i * 256 / NUM_LEDS) + j) & 255);
      setPixel(i, *c, *(c + 1), *(c + 2));

    }
    showStrip();
    delay(SpeedDelay);

  }
  return ;
}

byte * Wheel(byte WheelPos) {
  static byte c[3];

  if (WheelPos < 85) {
    c[0] = WheelPos * 3;
    c[1] = 255 - WheelPos * 3;
    c[2] = 0;
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    c[0] = 255 - WheelPos * 3;
    c[1] = 0;
    c[2] = WheelPos * 3;
  } else {
    WheelPos -= 170;
    c[0] = 0;
    c[1] = WheelPos * 3;
    c[2] = 255 - WheelPos * 3;
  }

  return c;
}
// *** REPLACE TO HERE ***

void showStrip() {
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  strip.show();
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  FastLED.show();
#endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  strip.setPixelColor(Pixel, strip.Color(red, green, blue));
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  leds[Pixel].r = red;
  leds[Pixel].g = green;
  leds[Pixel].b = blue;
#endif
}



void setAll(byte red, byte green, byte blue) {
  for (int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}

byte getColorCustom(String colorcode, String color)
{
  long number = strtol( &colorcode[1], NULL, 16);

  if (color == "r")   number = number >> 16;
  if (color == "g")     number  = number >> 8 & 0xFF;
  if (color == "b")     number = number & 0xFF;
  return (byte)number;


}
/* functie parsare string */
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {
    0, -1
  };
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

/* EFEEEEEECTEEE ================================================================================================================================*/

void RGBLoop() {

  for (int j = 0; j < 3; j++ ) {
    // Fade IN

    for (int k = 0; k < 256; k++) {
      client.loop();
      if (message != message1) return;
      switch (j) {
        case 0: setAll(k, 0, 0); break;
        case 1: setAll(0, k, 0); break;
        case 2: setAll(0, 0, k); break;
      }
      showStrip();
      delay(3);

    }
    // Fade OUT
    for (int k = 255; k >= 0; k--) {
      client.loop();
      if (message != message1) return;
      switch (j) {
        case 0: setAll(k, 0, 0); break;
        case 1: setAll(0, k, 0); break;
        case 2: setAll(0, 0, k); break;
      }
      showStrip();

      delay(3);
      client.loop();
      if (message != message1) return;
    }


  }
}

/* _____________________NEW KIT BEGIN_______________________________________*/
void NewKITT(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  RightToLeft(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  client.loop();
  if (message != message1) return;
  LeftToRight(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  client.loop();
  if (message != message1) return;
  OutsideToCenter(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  client.loop();
  if (message != message1) return;
  CenterToOutside(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  client.loop();
  if (message != message1) return;
  LeftToRight(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  client.loop();
  if (message != message1) return;
  RightToLeft(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  client.loop();
  if (message != message1) return;
  OutsideToCenter(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  client.loop();
  if (message != message1) return;
  CenterToOutside(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  client.loop();
  if (message != message1) return;
}

void CenterToOutside(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for (int i = ((NUM_LEDS - EyeSize) / 2); i >= 0; i--) {
    setAll(0, 0, 0);
    client.loop();
    if (message != message1) return;
    setPixel(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++) {
      client.loop();
      if (message != message1) return;
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);
    client.loop();
    if (message != message1) return;
    setPixel(NUM_LEDS - i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++) {
      setPixel(NUM_LEDS - i - j, red, green, blue);
    }
    setPixel(NUM_LEDS - i - EyeSize - 1, red / 10, green / 10, blue / 10);

    showStrip();
    delay(SpeedDelay);
    client.loop();
    if (message != message1) return;
  }
  delay(ReturnDelay);
  client.loop();
  if (message != message1) return;
}

void OutsideToCenter(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for (int i = 0; i <= ((NUM_LEDS - EyeSize) / 2); i++) {
    setAll(0, 0, 0);
    client.loop();
    if (message != message1) return;
    setPixel(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++) {
      client.loop();
      if (message != message1) return;
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);

    setPixel(NUM_LEDS - i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++) {
      client.loop();
      if (message != message1) return;
      setPixel(NUM_LEDS - i - j, red, green, blue);
    }
    setPixel(NUM_LEDS - i - EyeSize - 1, red / 10, green / 10, blue / 10);
    client.loop();
    if (message != message1) return;
    showStrip();
    delay(SpeedDelay);
  }
  client.loop();
  if (message != message1) return;
  delay(ReturnDelay);
}

void LeftToRight(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for (int i = 0; i < NUM_LEDS - EyeSize - 2; i++) {
    setAll(0, 0, 0);
    client.loop();
    if (message != message1) return;
    setPixel(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++) {
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);
    showStrip();
    delay(SpeedDelay);
    client.loop();
    if (message != message1) return;
  }
  delay(ReturnDelay);
  client.loop();
  if (message != message1) return;
}

void RightToLeft(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for (int i = NUM_LEDS - EyeSize - 2; i > 0; i--) {
    setAll(0, 0, 0);
    client.loop();
    if (message != message1) return;
    setPixel(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++) {
      client.loop();
      if (message != message1) return;
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);
    showStrip();
    delay(SpeedDelay); client.loop();
    if (message != message1) return;

  }
  delay(ReturnDelay);
  client.loop();
  if (message != message1) return;
}

/* _____________________NEW KIT END_______________________________________*/


/* _____________________running lights start _______________________________________*/


void RunningLights(byte red, byte green, byte blue, int WaveDelay) {
  int Position = 0;

  for (int i = 0; i < NUM_LEDS * 2; i++)
  {
    client.loop();
    if (message != message1) return;
    Position++; // = 0; //Position + Rate;
    for (int i = 0; i < NUM_LEDS; i++) {
      client.loop();
      if (message != message1) return;
      // sine wave, 3 offset waves make a rainbow!
      //float level = sin(i+Position) * 127 + 128;
      //setPixel(i,level,0,0);
      //float level = sin(i+Position) * 127 + 128;
      setPixel(i, ((sin(i + Position) * 127 + 128) / 255)*red,
               ((sin(i + Position) * 127 + 128) / 255)*green,
               ((sin(i + Position) * 127 + 128) / 255)*blue);
    }

    showStrip();
    delay(WaveDelay);
    client.loop();
    if (message != message1) return;
  }
}
/* _____________________running lights end _______________________________________*/

/* _____________________Cylon Start _______________________________________*/
void CylonBounce(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {

  for (int i = 0; i < NUM_LEDS - EyeSize - 2; i++) {
    setAll(0, 0, 0);
    setPixel(i, red / 10, green / 10, blue / 10);
    client.loop();
    if (message != message1) return;
    for (int j = 1; j <= EyeSize; j++) {
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);
    showStrip();
    delay(SpeedDelay);
    client.loop();
    if (message != message1) return;
  }

  delay(ReturnDelay);

  for (int i = NUM_LEDS - EyeSize - 2; i > 0; i--) {
    setAll(0, 0, 0);
    setPixel(i, red / 10, green / 10, blue / 10);
    client.loop();
    if (message != message1) return;
    for (int j = 1; j <= EyeSize; j++) {
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);
    showStrip();
    delay(SpeedDelay);
    client.loop();
    if (message != message1) return;
  }

  delay(ReturnDelay);
}
/*cylon end =====================================================================*/
/*SPARKLE START=======================*/
void Sparkle(byte red, byte green, byte blue, int SpeedDelay) {
  int Pixel = random(NUM_LEDS);
  if (Sparkle_State == 0) {
    Sparkle_State = 1 ;
    setAll(0, 0, 0);
  }
  setPixel(Pixel, red, green, blue);
  showStrip();
  delay(SpeedDelay);
  setPixel(Pixel, 0, 0, 0);
  client.loop();
  if (message != message1) {
    setAll(0, 0, 0);
    Sparkle_State = 0;
    showStrip();
    return;
  }
}
/*end color sparkle ============================================================================*/
void colorWipe(byte red, byte green, byte blue, int SpeedDelay) {
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    setPixel(i, red, green, blue);
    showStrip();
    client.loop();
    if (message != message1) {
      setAll(0, 0, 0);
      showStrip();
      return;
    }
    delay(SpeedDelay);
  }
}


/*color wipe end ====================================================================================*/
/*color twinkle===================================================================================================*/
void TwinkleRandom(int Count, int SpeedDelay, boolean OnlyOne) {
  setAll(0, 0, 0);

  for (int i = 0; i < Count; i++) {
    setPixel(random(NUM_LEDS), random(0, 255), random(0, 255), random(0, 255));
    showStrip();
    delay(SpeedDelay);
    client.loop();
    if (message != message1) {
      setAll(0, 0, 0);
      showStrip();
      return;
    }
    if (OnlyOne) {
      setAll(0, 0, 0);
    }
  }
  client.loop();
  if (message != message1) {
    setAll(0, 0, 0);
    showStrip();
    return;
  }
  delay(SpeedDelay);
}

/*twinkle end --------------------------------------------------------------------------------------------------------------------------/-*/

/*fade in custom color ===============================================================*/
void FadeInOut(byte red, byte green, byte blue) {
  float r, g, b;

  for (int k = 0; k < 256; k = k + 1) {
    r = (k / 256.0) * red;
    g = (k / 256.0) * green;
    b = (k / 256.0) * blue;
    setAll(r, g, b);
    showStrip();
    client.loop();
    if (message != message1) {
      setAll(0, 0, 0);
      showStrip();
      return;
    }
  }

  for (int k = 255; k >= 0; k = k - 2) {
    r = (k / 256.0) * red;
    g = (k / 256.0) * green;
    b = (k / 256.0) * blue;
    setAll(r, g, b);
    showStrip();
    client.loop();
    if (message != message1) {
      setAll(0, 0, 0);
      showStrip();
      return;
    }
  }

}

void tricolor() {
  for (int  i = 0; i < 100; i++)  setPixel(i, 255, 0, 0);
  for (int  i = 100; i < 200; i++)  setPixel(i, 255, 215, 0);
  for (int  i = 200; i < 300; i++)  setPixel(i, 0, 0, 255);
  showStrip();
}

