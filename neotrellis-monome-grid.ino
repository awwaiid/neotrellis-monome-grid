#include "Adafruit_NeoTrellis.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>

// CONFIG: Declare how many rows/columns you have
#define Y_DIM 8 //number of rows of key
#define X_DIM 8 //number of columns of keys

// CONFIG: Default brightness
#define BRIGHTNESS 10

// CONFIG: Fill in the i2c layout of your neotrellis
// Create a matrix of trellis panels
Adafruit_NeoTrellis trellis_array[Y_DIM/4][X_DIM/4] = {
  { Adafruit_NeoTrellis(0x2E), Adafruit_NeoTrellis(0x2F) },
  { Adafruit_NeoTrellis(0x30), Adafruit_NeoTrellis(0x32) }
};

// Pass this matrix to the multitrellis object
Adafruit_MultiTrellis trellis((Adafruit_NeoTrellis *)trellis_array, Y_DIM/4, X_DIM/4);

// CONFIG: Wifi connection
char ssid[] = "wifi2.42t.org"; // your network SSID (name)
char pass[] = "";              // your network password

// CONFIG: OSC Configuration, so we can listen and send OSC messages to a particular computer
const IPAddress outIp(10,0,0,184);   // remote IP of your computer
const unsigned int outPort = 44100;   // remote port to receive OSC
const unsigned int localPort = 8889; // local port to listen for OSC packets (actually not used for sending)

// Define a callback for key presses
TrellisCallback blink(keyEvent evt) {

  // Rising == push, Falling == release
  if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
    int x = evt.bit.NUM % 8;
    int y = evt.bit.NUM / 8;

    Serial.print("Key: ");
    Serial.print(evt.bit.NUM);
    Serial.print(" x: ");
    Serial.print(x);
    Serial.print(" y: ");
    Serial.print(y);
    Serial.println("");

    send_grid_key(x, y, 1);

  } else if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING) {

    int x = evt.bit.NUM % 8;
    int y = evt.bit.NUM / 8;
    send_grid_key(x, y, 0);
  }

  return 0;
}

void setup_neotrellis() {
  if(!trellis.begin()) {
    Serial.println("Failed to begin trellis!");
    while(1);
  } else {
    Serial.println("Trellis started!");
  }

  set_brightness(BRIGHTNESS);

  // Fancy random initialization display
  for(int n = 0; n < 10; n++) {
    for(int i=0; i < Y_DIM * X_DIM; i++) {
      trellis.setPixelColor(i, random(0x1000000));
    }
    trellis.show();
    delay(50);
  }

  // Hook up callbacks for every button
  for(int y=0; y<Y_DIM; y++){
    for(int x=0; x<X_DIM; x++){
      // activate rising and falling edges on all keys
      trellis.activateKey(x, y, SEESAW_KEYPAD_EDGE_RISING, true);
      trellis.activateKey(x, y, SEESAW_KEYPAD_EDGE_FALLING, true);
      trellis.registerCallback(x, y, blink);
      trellis.setPixelColor(x, y, 0x000000); // Addressed with x,y
      trellis.show(); // Show all LEDs
    }
    delay(5);
  }

  // draw_board();
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  setup_osc_udp();
  setup_neotrellis();
}

void loop() {
  trellis.read(); // Read from NeoTrellis and invoke callbacks

  // ... does this block or keep going?
  process_osd_receipt();

  // Kinda arbitrary delay, I don't know if it is
  // (a) needed
  // (b) a good idea
  // ... But it was in whatever example I copied
  // delay(20);
}

