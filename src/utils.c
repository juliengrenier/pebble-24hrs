#include "pebble_os.h"
#include "utils.h"

int32_t get_nth_angle(int32_t nth, int32_t number_of_section, bool clock_angle){
  int32_t hex_angle = nth * (0xffff / number_of_section);
  if (clock_angle) {
    int32_t transpose_angle = 0x10000 - ((0.75 * 0xffff) + hex_angle); 
    return transpose_angle;
  }
  return hex_angle;
}

GPoint polar_to_cartesian(int32_t radius, int32_t angle, GPoint origin) {
  int32_t x = radius * cos_lookup(angle) / TRIG_MAX_RATIO;
  int32_t y = radius * sin_lookup(angle) / TRIG_MAX_RATIO;
  x = origin.x + x;
  y = origin.y - y;
  return GPoint(x, y);
}
