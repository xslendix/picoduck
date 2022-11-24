#include "lua_api.h"

#include "config.h"

bool hold = false;

#ifdef NO_INITIAL_DELAY
bool first_key = false;
#else
bool first_key = true;
#endif

#define FIRST_KEY_CHECK { \
  if (first_key) { \
    delay(TYPING_DELAY); \
    first_key = false; \
  } \
  while (hold) delay(1); \
}

int l_wait(lua_State *L) {
  while (hold) delay(1);
  delay(luaL_checknumber(L, 1));
  return 0;
}

// KEYBOARD

int l_type(lua_State *L) {
  FIRST_KEY_CHECK;
  Keyboard.print(luaL_checkstring(L, 1));
  return 0;
}

int l_typeln(lua_State *L) {
  FIRST_KEY_CHECK;
  Keyboard.println(luaL_checkstring(L, 1));
  return 0;
}

int l_press(lua_State *L) {
  FIRST_KEY_CHECK;
  int key = luaL_checkint(L, 1);

  if (key < 0)
    return 0;
  
  Keyboard.press(key); 
  return 0;
}

int l_release(lua_State *L) {
  FIRST_KEY_CHECK;
  int n = lua_gettop(L);
  if (n == 0) {
    Keyboard.releaseAll();
    return 0;
  }
  
  int key = luaL_checkint(L, 1);

  if (key < 0)
    return 0;
  
  Keyboard.release(key); 
  return 0;
}

int l_write(lua_State *L) {
  FIRST_KEY_CHECK;
  Keyboard.write(luaL_checkstring(L, 1)[0]);
  return 0;
}

int l_set_speed(lua_State *L) {
  double speed = luaL_checknumber(L, 1);
  if (speed <= 0) {
    Keyboard.m_delay = 0;
    return 0;
  }

  Keyboard.m_delay = speed / 1000;
  return 0;  
}

void l_load_keys(lua_State *L) {
  luaL_dostring(L, 
  "keys = {\n"
  "  ctrl = 0x80,\n"
  "  shift = 0x81,\n"
  "  alt = 0x82,\n"
  "  gui = 0x83,\n"
  "  menu = 0xed,\n"
  "\n"
  "  left_ctrl = 0x80,\n"
  "  left_shift = 0x81,\n"
  "  left_alt = 0x82,\n"
  "  left_gui = 0x83,\n"
  "  right_ctrl = 0x84,\n"
  "  right_shift = 0x85,\n"
  "  right_alt = 0x86,\n"
  "  right_gui = 0x87,\n"
  "  \n"
  "  up_arrow = 0xda,\n"
  "  down_arrow = 0xd9,\n"
  "  left_arrow = 0xd8,\n"
  "  right_arrow = 0xd7,\n"
  "  up = 0xda,\n"
  "  down = 0xd9,\n"
  "  left = 0xd8,\n"
  "  right = 0xd7,\n"
  "\n"
  "  backspace = 0xb2,\n"
  "  tab = 0xb3,\n"
  "  return = 0xb0,\n"
  "  enter = 0xb0,\n"
  "  esc = 0xb1,\n"
  "  escape = 0xb1,\n"
  "  insert = 0xd1,\n"
  "  delete = 0xd4,\n"
  "  page_up = 0xd3,\n"
  "  page_down = 0xd6,\n"
  "  home = 0xd2,\n"
  "  end = 0xd5,\n"
  "  caps_lock = 0xc1,\n"
  "\n"
  "  f1 = 0xc2,\n"
  "  f2 = 0xc3,\n"
  "  f3 = 0xc4,\n"
  "  f4 = 0xc5,\n"
  "  f5 = 0xc6,\n"
  "  f6 = 0xc7,\n"
  "  f7 = 0xc8,\n"
  "  f8 = 0xc9,\n"
  "  f9 = 0xca,\n"
  "  f10 = 0xcb,\n"
  "  f11 = 0xcc,\n"
  "  f12 = 0xcd,\n"
  "  f13 = 0xf0,\n"
  "  f14 = 0xf1,\n"
  "  f15 = 0xf2,\n"
  "  f16 = 0xf3,\n"
  "  f17 = 0xf4,\n"
  "  f18 = 0xf5,\n"
  "  f19 = 0xf6,\n"
  "  f20 = 0xf7,\n"
  "  f21 = 0xf8,\n"
  "  f22 = 0xf9,\n"
  "  f23 = 0xfa,\n"
  "  f24 = 0xfb,\n"
  "}\n"
  "");
}

// MOUSE

int l_mouse_move(lua_State *L) {
  FIRST_KEY_CHECK;
  int n = lua_gettop(L);

  int x, y, wheel=0;
  x = luaL_checknumber(L, 1);
  y = luaL_checknumber(L, 2);
  if (n >= 3)
    wheel = luaL_checknumber(L, 3);

  Mouse.move(x, y, wheel);
  return 0;
}

int l_mouse_click(lua_State *L) {
  FIRST_KEY_CHECK;
  int n = lua_gettop(L);

  uint8_t button = MOUSE_LEFT;
  if (n >= 1)
    button = luaL_checknumber(L, 1);
  
  Mouse.click(button);
  return 0;
}

int l_mouse_press(lua_State *L) {
  FIRST_KEY_CHECK;
  int n = lua_gettop(L);

  uint8_t button = MOUSE_LEFT;
  if (n >= 1)
    button = luaL_checknumber(L, 1);
  
  Mouse.press(button);
  return 0;
}

int l_mouse_release(lua_State *L) {
  FIRST_KEY_CHECK;
  int n = lua_gettop(L);

  uint8_t button = MOUSE_LEFT;
  if (n >= 1)
    button = luaL_checknumber(L, 1);
  
  Mouse.release(button);
  return 0;
}

// I/O

int l_set_led(lua_State *L) {
  while (hold) delay(1);
  digitalWrite(BUILT_IN_LED_PIN, lua_toboolean(L, 1) ? HIGH : LOW);
  return 0;
}

int l_set_led_external(lua_State *L) {
  while (hold) delay(1);
  digitalWrite(EXTERNAL_LED_PIN, lua_toboolean(L, 1) ? HIGH : LOW);
  return 0;
}

int l_pin_mode(lua_State *L) {
  while (hold) delay(1);
  if (lua_gettop(L) < 2)
    return 0;

  pinMode(luaL_checknumber(L, 1), luaL_checknumber(L, 2));
  return 0;
}

int l_analog_write(lua_State *L) {
  while (hold) delay(1);
  if (lua_gettop(L) < 2)
    return 0;

  analogWrite(luaL_checknumber(L, 1), luaL_checknumber(L, 2));
  return 0;
}

int l_digital_write(lua_State *L) {
  while (hold) delay(1);
  if (lua_gettop(L) < 2)
    return 0;

  digitalWrite(luaL_checknumber(L, 1), luaL_checknumber(L, 2));
  return 0;
}

int l_digital_read(lua_State *L) {
  while (hold) delay(1);
  if (lua_gettop(L) < 1)
    return 0;

  lua_pushboolean(L, digitalRead(luaL_checknumber(L, 1)));
  return 1;
}

int l_analog_read(lua_State *L) {
  while (hold) delay(1);
  if (lua_gettop(L) < 1)
    return 0;

  lua_pushnumber(L, analogRead(luaL_checknumber(L, 1)));
  return 1;
}

void l_init(lua_State *L) {
  lua_pushcfunction(L, l_set_led);
  lua_setglobal(L, "setLed");
  lua_pushcfunction(L, l_set_led_external);
  lua_setglobal(L, "setLedExternal");
  
  lua_pushcfunction(L, l_wait);
  lua_setglobal(L, "wait");

  lua_pushcfunction(L, l_type);
  lua_setglobal(L, "type");
  lua_pushcfunction(L, l_typeln);
  lua_setglobal(L, "typeln");

  lua_pushcfunction(L, l_write);
  lua_setglobal(L, "write");

  lua_pushcfunction(L, l_press);
  lua_setglobal(L, "press");
  lua_pushcfunction(L, l_release);
  lua_setglobal(L, "release");

  lua_pushcfunction(L, l_mouse_move);
  lua_setglobal(L, "mouseMove");
  lua_pushcfunction(L, l_mouse_click);
  lua_setglobal(L, "mouseClick");
  lua_pushcfunction(L, l_mouse_press);
  lua_setglobal(L, "mousePress");
  lua_pushcfunction(L, l_mouse_release);
  lua_setglobal(L, "mouseRelease");

  lua_pushcfunction(L, l_set_speed);
  lua_setglobal(L, "setSpeed");

  // Aliases
  lua_getglobal(L, "type");
  lua_setglobal(L, "string");

  lua_getglobal(L, "wait");
  lua_setglobal(L, "sleep");
  lua_getglobal(L, "wait");
  lua_setglobal(L, "delay");

  l_load_keys(L);
  
  lua_pushcfunction(L, l_pin_mode);
  lua_setglobal(L, "pinMode");
  lua_pushcfunction(L, l_analog_write);
  lua_setglobal(L, "analogWrite");
  lua_pushcfunction(L, l_digital_write);
  lua_setglobal(L, "digitalWrite");
  lua_pushcfunction(L, l_digital_read);
  lua_setglobal(L, "digitalRead");
  lua_pushcfunction(L, l_analog_read);
  lua_setglobal(L, "analogRead");

  luaL_dostring(L, "\
INPUT = 0\n\
OUTPUT = 1\n\
INPUT_PULLUP = 2\n\
\n");
  luaL_dostring(L, "\
LOW = 0\n\
HIGH = 1\n\
\n");

  luaL_dostring(L, "function gui(char)\n\
  press(131)\n\
  write(char)\n\
  release(131)\n\
end");

  lua_getglobal(L, "gui");
  lua_setglobal(L, "win");
  lua_getglobal(L, "gui");
  lua_setglobal(L, "meta");

  luaL_dostring(L, "function alt(char)\n\
  press(130)\n\
  write(char)\n\
  release(130)\n\
end");

  luaL_dostring(L, "function ctrl(char)\n\
  press(128)\n\
  write(char)\n\
  release(128)\n\
end");

  luaL_dostring(L, "function blinkLed(ratem)\n\
  rate = rate or 1000\n\
  while true do\n\
    setLed(true)\n\
    wait(rate)\n\
    setLed(false)\n\
    wait(rate)\n\
  end\n\
end\n");
}
