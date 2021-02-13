/* General helpers for the neotrellis grid */

// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
uint32_t wheel(byte wheel_pos) {
  if(wheel_pos < 85) {
    return seesaw_NeoPixel::Color(wheel_pos * 3, 255 - wheel_pos * 3, 0);
  } else if(wheel_pos < 170) {
    wheel_pos -= 85;
    return seesaw_NeoPixel::Color(255 - wheel_pos * 3, 0, wheel_pos * 3);
  } else {
    wheel_pos -= 170;
    return seesaw_NeoPixel::Color(0, wheel_pos * 3, 255 - wheel_pos * 3);
  }
  return 0;
}

void set_brightness(int level) {
	for (int x = 0; x < X_DIM / 4; x++) {
		for (int y = 0; y < Y_DIM / 4; y++) {
		  trellis_array[y][x].pixels.setBrightness(level);
		}
	}
}

void set_led(int x, int y, int color) {
  trellis.setPixelColor(x, y, color);
  trellis.show();
}

void set_all_led(int color) {
  for(int x = 0; x < X_DIM; x++) {
    for(int y = 0; y < Y_DIM; y++) {
      trellis.setPixelColor(x, y, color);
    }
  }
  trellis.show();
}
