
// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;

void setup_osc_udp() {
  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(localPort);
}

void send_osc_msg(OSCMessage &msg) {
  Udp.beginPacket(outIp, outPort);
  msg.send(Udp);
  Udp.endPacket();
}

void sendNoteOn(int channel, int note, int vel) {
  OSCMessage msg("/noteOn");
  msg.add(channel);
  msg.add(note);
  msg.add(vel);
  send_osc_msg(msg);
  msg.empty();
}

void sendNoteOff(int channel, int note) {
  OSCMessage msg("/noteOff");
  msg.add(channel);
  msg.add(note);
  send_osc_msg(msg);
  msg.empty();
}

/*
 *  /grid/led/set x y s
 *  set led at (x,y) to state s (0 or 1).
 */
void osd_grid_led_set(OSCMessage &msg) {
  int x = msg.getInt(0);
  int y = msg.getInt(1);
  int color = msg.getInt(2);

  Serial.print("/grid/led/set ");
  Serial.print(x);
  Serial.print(y);
  Serial.print(color);
  Serial.println("");

  set_led(x, y, color);
}


// /grid/led/all s
// set all leds to state s (0 or 1).
void osd_grid_led_all(OSCMessage &msg) {
  int color = msg.getInt(0);

  Serial.print("/grid/led/all ");
  Serial.print(color);
  Serial.println("");

  set_all_led(color);
}

void process_osd_receipt() {
  OSCMessage msg;
  OSCErrorCode error;
  int size = Udp.parsePacket();

  if (size > 0) {
    while (size--) {
      msg.fill(Udp.read());
    }
    if (!msg.hasError()) {
      Serial.println("Got osd msg");
      msg.dispatch("/grid/led/set", osd_grid_led_set);
      msg.dispatch("/grid/led/all", osd_grid_led_all);
    } else {
      error = msg.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }
}

/*
/grid/led/map x_offset y_offset s[8]

Set a quad (8×8, 64 buttons) in a single message.

Each number in the list is a bitmask of the buttons in a row, one number in the list for each row. The message will fail if the list doesn’t have 8 entries plus offsets.

taken apart:

(/grid/led/map)  <- the message/route
               (8 8)  <- the offsets
                    (1 2 4 8 16 32 64 128)  <- the bitmasks for each row

examples

/grid/led/map 0 0 4 4 4 4 8 8 8 8
/grid/led/map 0 0 254 253 125 247 239 36 191 4

Offsets must be mutliples of 8.

/grid/led/row x_offset y s[..]

Set a row in a quad in a single message.

Each number in the list is a bitmask of the buttons in a row, one number in the list for each row being updated.

examples (for 256)

/grid/led/row 0 0 255 255
/grid/led/row 8 5 255

examples (for 64)

/grid/led/row 0 0 232
/grid/led/row 0 3 129

Offsets must be mutliples of 8. Offsets for monome64 should always be zero.

/grid/led/col x y_offset s[..]

Set a column in a quad in a single message.

Each number in the list is a bitmask of the buttons in a column, one number in the list for each row being updated.

examples (for 256)

/grid/led/col 0 0 255 255 (updates quads 1 and 3)
/grid/led/col 13 8 255 (updates quad 4 due to offset.)

examples (for 64)

/grid/led/col 0 0 232
/grid/led/col 6 0 155

Offsets must be mutliples of 8. Offsets for monome64 should always be zero.

/grid/led/intensity i

variable brightness:

Valid values for ‘l’ below are in the range [0, 15].

January 2011 devices only support four intensity levels (off + 3 brightness levels). The value passed in /level/ messages will be “rounded down” to the lowest available intensity as below:

    [0, 3] - off
    [4, 7] - low intensity
    [8, 11] - medium intensity
    [12, 15] - high intensity

June 2012 devices allow the full 16 intensity levels.

/grid/led/level/set x y l
/grid/led/level/all l
/grid/led/level/map x_off y_off l[64]
/grid/led/level/row x_off y l[..]
/grid/led/level/col x y_off l[..]

*/

