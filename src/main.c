#include <pebble.h>
#include "pin_window.h"

static Window *s_main_window;
static TextLayer *s_time_layer;
Layer *window_layer;
char succ_message[] = "Calibration successful!";
char fail_message[] = "Cannot calibrate right now.";

static void window_load(Window *window) {
  // Get information about the Window
  window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // If calibration is successful
  s_time_layer = text_layer_create(
      GRect(0, PBL_IF_RECT_ELSE(48, 22), bounds.size.w, 100));
  text_layer_set_text(s_time_layer, succ_message);
  
  // If calibration is unsucessful
  //s_time_layer = text_layer_create(
  //    GRect(0, PBL_IF_RECT_ELSE(38, 22), bounds.size.w, 100));
  //text_layer_set_text(s_time_layer, fail_message);

  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
}

static void window_unload(Window *window) {
  //menu_layer_destroy(window_layer);
}

static void pin_complete_callback(PIN pin, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Blood sugar was %d %d %d", pin.digits[0], pin.digits[1], pin.digits[2]);
  pin_window_pop((PinWindow*)context, true);
  
  //on exit, load main window
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
      .load = window_load,
      .unload = window_unload,
  });
  window_stack_push(s_main_window, true);
}

static void init() {
  PinWindow *pin_window = pin_window_create((PinWindowCallbacks) {
          .pin_complete = pin_complete_callback
        });
  pin_window_push(pin_window, true);
}

static void deinit() {
  window_destroy(s_main_window);
}
  
int main() {
  init();
  app_event_loop();
  deinit();
}
