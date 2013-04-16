#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#include "utils.h"


#define MY_UUID { 0xE5, 0xAB, 0x90, 0x03, 0x23, 0x6B, 0x44, 0x54, 0x9F, 0x43, 0xBD, 0x97, 0x6E, 0xFA, 0x7A, 0x83 }
PBL_APP_INFO(MY_UUID,
             "24hrs", "Julien Grenier",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_WATCH_FACE);

Window window;
Layer display_layer;
Layer hour_layer;
Layer minute_layer;


int32_t current_hour = -1;
int32_t current_minute = -1;


#define CIRCLE_RADIUS 4
#define BIG_CIRCLE_RADIUS 6
#define PADDING 2
#define NUMBER_OF_HOURS 24
#define NUMBER_OF_MINUTES 60
#define HOUR_CLOCK_FACE_RADIUS 50 //144/2 - (CIRCLE_RADIUS + PADDING)
#define MINUTE_CLOCK_FACE_RADIUS 68 //144/2 - (CIRCLE_RADIUS + PADDING)




void draw_cell(GContext* ctx, GPoint center, bool filled, int radius) {
  if (filled) {
    graphics_context_set_fill_color(ctx, GColorWhite);
    graphics_fill_circle(ctx, center, radius);
  }else{
    graphics_context_set_stroke_color(ctx, GColorWhite);
    graphics_draw_circle(ctx, center, radius);
  }
}
void draw_minutes(GContext* ctx){
  PblTm t;
  get_time(&t);

  GPoint window_center = grect_center_point(&window.layer.frame);
  for(int32_t i=0; i < NUMBER_OF_MINUTES; i++){
    if (i == t.tm_min || (i % 5) == 0){
      int32_t hex_angle = get_nth_angle(i, NUMBER_OF_MINUTES, true); 
      GPoint minute_center = polar_to_cartesian(MINUTE_CLOCK_FACE_RADIUS, hex_angle, window_center); 
      draw_cell(ctx, minute_center, i == t.tm_min,(i % 5)==0?CIRCLE_RADIUS-1:CIRCLE_RADIUS-2);
    }
  }
  //current_minute = t.tm_min / 5;
}
void draw_hours(GContext* ctx) {
  PblTm t;
  get_time(&t);
  GPoint window_center = grect_center_point(&window.layer.frame);

  current_hour = t.tm_hour; 
  for(int32_t i=0; i < NUMBER_OF_HOURS; i++){
    int32_t hex_angle = get_nth_angle(i, NUMBER_OF_HOURS, true); 

    GPoint hour_center = polar_to_cartesian(HOUR_CLOCK_FACE_RADIUS, hex_angle, window_center); 
    draw_cell(ctx, hour_center, current_hour == i, CIRCLE_RADIUS);
  }

}
void display_hour_layer_update_callback(Layer *me, GContext* ctx) {
  (void)me;
  draw_hours(ctx);
}

void display_minute_layer_update_callback(Layer *me, GContext* ctx) {
  (void)me;
  draw_minutes(ctx);
}

void handle_minute(AppContextRef ctx, PebbleTickEvent *t) {
  (void)t;
  (void)ctx; // TODO: Pass tick event/PblTime rather than make layer use `get_time()`?
  PblTm time;
  get_time(&time);
  layer_mark_dirty(&minute_layer);

  if (current_hour != time.tm_hour ){
    layer_mark_dirty(&hour_layer);
  }
}

void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Window Name");
  window_stack_push(&window, true /* Animated */);
  window_set_background_color(&window, GColorBlack);

  // Init the layer for the display
  layer_init(&display_layer, window.layer.frame);
//  display_layer.update_proc = &display_layer_update_callback;
  hour_layer.update_proc = &display_hour_layer_update_callback;
  minute_layer.update_proc = &display_minute_layer_update_callback;
  layer_add_child(&window.layer, &display_layer);
  layer_add_child(&display_layer, &minute_layer);
  layer_add_child(&display_layer, &hour_layer);
  graphics_context_set_fill_color(ctx, GColorWhite);
  GPoint window_center = grect_center_point(&window.layer.frame);
  graphics_fill_circle(ctx, window_center, CIRCLE_RADIUS);
  draw_hours(ctx);
  draw_minutes(ctx);
//  layer_mark_dirty(&display_layer);
}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .tick_info = {
      .tick_handler = &handle_minute,
      .tick_units = MINUTE_UNIT
    }
  };
  app_event_loop(params, &handlers);
}
