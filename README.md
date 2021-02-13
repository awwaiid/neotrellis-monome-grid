# Neotrellis Monome Grid (kinda)

I got my [Neotrellis 8x8 kit](https://www.adafruit.com/product/1929) all set up and am on the 'net via wifi on [Adafruit HUZZAH32 – ESP32 Feather Board](https://www.adafruit.com/product/3405). In this project I'm going to implement the OSC controls for [Monome Grid](https://monome.org/docs/grid/). Other "arduinome" type things implement the usb-serial protocol and then use serialosc to turn into the net, but here I'm going straight for OSC. It's possible we could implement some other fake bits of serialosc so that other software doesn't know the difference -- that'd work if this is your only serialosc device. Dunno.

The plan is to control this from [Clojure's Overtone](https://overtone.github.io/) to build some [MLR inspired thing](https://vimeo.com/search?q=monome+mlr).

This is the Neotrellis/Arduino side. Outside of this repo will be the computer-side, where most likely I'll refresh my nREPL-looper experiments, making this some sort of UI for that. The Overtone side will use synths and samples and all that and kinda do the hard bits.

There are several things in the code labeled `CONFIG`, these will need to be edited to hook into your own neotrellis, wifi network, and companion computer.

References:
* Hardware used:
  * [Neotrellis 8x8 kit](https://www.adafruit.com/product/1929)
  * [Adafruit HUZZAH32 – ESP32 Feather Board](https://www.adafruit.com/product/3405)
  * I also soldered a Qwiic cable onto the Neotrellis to plug in other fun stuff
* [OSC Arduino Library](https://github.com/CNMAT/OSC)
  * [OSC Send Example](https://github.com/CNMAT/OSC/blob/master/examples/ESP8266sendMessage/ESP8266sendMessage.ino)
  * [OSC Receive Example](https://github.com/CNMAT/OSC/blob/master/examples/ESP8266ReceiveMessage/ESP8266ReceiveMessage.ino)
* [Monome Grid OSC](https://monome.org/docs/serialosc/osc/)

TODO:
* Measure where latency is worst
