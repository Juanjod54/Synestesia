#include "rgb_light_configuration.h"

void setup() {
  Serial.begin(9600);
  while (!Serial) delay(500);
  
  RGBLightConfiguration * configuration = load_rgb_light_configuration();
  RGB_LIGHT ** lights = get_lights(configuration);

  RGB * DO = get_color(configuration, lights[0], "DO");
  RGB * re = get_color(configuration, lights[0], "RE");
  RGB * mi = get_color(configuration, lights[0], "MI");
  RGB * fa = get_color(configuration, lights[0], "FA");
  RGB * sol = get_color(configuration, lights[0], "SOL");
  RGB * la = get_color(configuration, lights[0], "LA");
  RGB * si = get_color(configuration, lights[0], "SI");

  Serial.print("Color DO: ");
  Serial.print(get_red_color(DO));
  Serial.print(", ");
  Serial.print(get_green_color(DO));
  Serial.print(", ");
  Serial.print(get_blue_color(DO));
  Serial.println("");

  Serial.print("Color RE: ");
  Serial.print(get_red_color(re));
  Serial.print(", ");
  Serial.print(get_green_color(re));
  Serial.print(", ");
  Serial.print(get_blue_color(re));
  Serial.println("");
  
  Serial.print("Color MI: ");
  Serial.print(get_red_color(mi));
  Serial.print(", ");
  Serial.print(get_green_color(mi));
  Serial.print(", ");
  Serial.print(get_blue_color(mi));
  Serial.println("");
}

void loop() {
    
}
