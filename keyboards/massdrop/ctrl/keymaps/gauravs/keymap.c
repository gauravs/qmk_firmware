#include QMK_KEYBOARD_H

enum ctrl_keycodes {
    L_BRI = SAFE_RANGE, //LED Brightness Increase
    L_BRD,              //LED Brightness Decrease
    L_PTN,              //LED Pattern Select Next
    L_PTP,              //LED Pattern Select Previous
    L_PSI,              //LED Pattern Speed Increase
    L_PSD,              //LED Pattern Speed Decrease
    L_T_MD,             //LED Toggle Mode
    L_T_ONF,            //LED Toggle On / Off
    L_ON,               //LED On
    L_OFF,              //LED Off
    L_T_BR,             //LED Toggle Breath Effect
    L_T_PTD,            //LED Toggle Scrolling Pattern Direction
    U_T_AUTO,           //USB Extra Port Toggle Auto Detect / Always Active
    U_T_AGCR,           //USB Toggle Automatic GCR control
    DBG_TOG,            //DEBUG Toggle On / Off
    DBG_MTRX,           //DEBUG Toggle Matrix Prints
    DBG_KBD,            //DEBUG Toggle Keyboard Prints
    DBG_MOU,            //DEBUG Toggle Mouse Prints
    MD_BOOT,            //Restart into bootloader after hold timeout
};

#define TG_NKRO MAGIC_TOGGLE_NKRO //Toggle 6KRO / NKRO mode

#define SECONDS *1000
#define MINUTES *(60 SECONDS)

#define COLOR_RAINBOW   1
#define COLOR_GRADIENT  2
#define COLOR_GREEN     5
#define COLOR_RED       4

#define LED_ANIMATION_ID_NORMAL     COLOR_GRADIENT
#define LED_ANIMATION_ID_LOWER      COLOR_RED
#define LED_ANIMATION_ID_OFF        10
#define LED_ANIMATION_ID_DEFAULT    LED_ANIMATION_ID_NORMAL

#define LED_BRIGHTNESS_MIN       30
#define LED_GCR_DEFAULT          LED_BRIGHTNESS_MIN
#define LED_MODE_DEFAULT         LED_MODE_KEYS_ONLY
#define LED_MODE_LOWER           LED_MODE_NORMAL
#define DFU_DURATION             1 SECONDS

#define HOME

#ifdef HOME
#define LED_SLEEP_ENABLED
#define LED_SLEEP_DEFAULT  false
#define LED_SLEEP_DURATION 5 MINUTES
#else
#undef LED_MODE_DEFAULT
#define LED_MODE_DEFAULT LED_MODE_NON_KEYS_ONLY
#endif

enum {
  BASE = 0,
  LOWER,
  RAISE,
};
keymap_config_t keymap_config;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT(
        KC_ESC,     KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,                KC_PSCR, KC_NO,   TG(1),   \
        KC_GRV,     KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,     KC_BSPC,   KC_NO,   KC_HOME, KC_PGUP, \
        KC_TAB,     KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,    KC_BSLS,   KC_DEL,  KC_END,  KC_PGDN, \
        KC_ESC,     KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT, \
        KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,                                 KC_UP, \
        KC_LCTL,    KC_LGUI, KC_LALT,                  KC_SPC,                              KC_RALT, MO(1)  , KC_NO,   KC_RCTL,               KC_LEFT, KC_DOWN, KC_RGHT \
    ),
    [LOWER] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______,  _______, _______, _______, _______, _______,               KC_MUTE, _______, _______, \
        _______, _______, _______, _______, _______, _______, _______, _______,  _______, _______, _______, _______, _______,    MD_BOOT,   KC_MPLY, KC_MSTP, KC_VOLU, \
        L_T_BR,  L_PSD,   L_BRI,   L_PSI,   _______, _______, _______, KC_HOME,  KC_UP,   KC_END,  _______, _______, _______,    _______,   KC_MPRV, KC_MNXT, KC_VOLD, \
        _______, L_PTP,   L_BRD,   L_PTN,   _______, _______, _______, KC_LEFT,  KC_DOWN, KC_RGHT, _______, _______, KC_MS_BTN1, \
        _______, L_T_MD,  L_T_ONF, _______, _______, MD_BOOT, TG_NKRO, _______,  _______, _______, _______, _______,                                 _______, \
        _______, _______, _______,                  _______,                              _______, _______, _______, _______,               _______, _______, _______ \
    ),
    /*
    [X] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,               _______, _______, _______, \
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, \
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, \
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
        _______, _______, _______, _______, _______, _______, TG_NKRO, _______, _______, _______, _______, _______,                                 _______, \
        _______, _______, _______,                  _______,                             _______, _______, _______, _______,               _______, _______, _______ \
    ),
    */
};

const uint16_t PROGMEM fn_actions[] = {

};

//Tap Dance Definitions
// qk_tap_dance_action_t tap_dance_actions[] = {
//   [TD_WSS] = ACTION_TAP_DANCE_DOUBLE(KC_GRV, LGUI(KC_ESC)),
// };

void set_sleep_state(bool keep_sleeping) {
#ifdef LED_SLEEP_ENABLED
  static bool is_sleeping;
  static bool init_complete;
  static uint32_t sleep_timer;

  if (!init_complete) {
    is_sleeping = LED_SLEEP_DEFAULT;

    if (is_sleeping) {
      sleep_timer = timer_read32();
    }

    init_complete = true;
    return;
  }

  if (keep_sleeping == false) {
    sleep_timer = timer_read32();

    if (is_sleeping) {
      I2C3733_Control_Set(1);
      is_sleeping = false;
    }

    return;
  }

  if (timer_elapsed32(sleep_timer) < LED_SLEEP_DURATION) {
    return;
  }

  // sleep
  is_sleeping = true;
  I2C3733_Control_Set(0);
#endif
}

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
  led_lighting_mode = LED_MODE_DEFAULT;
  gcr_desired       = LED_GCR_DEFAULT;
  led_animation_id  = LED_ANIMATION_ID_DEFAULT;
};


// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {
  set_sleep_state(true);
};

#define MODS_SHIFT  (keyboard_report->mods & MOD_BIT(KC_LSHIFT) || keyboard_report->mods & MOD_BIT(KC_RSHIFT))
#define MODS_CTRL  (keyboard_report->mods & MOD_BIT(KC_LCTL) || keyboard_report->mods & MOD_BIT(KC_RCTRL))
#define MODS_ALT  (keyboard_report->mods & MOD_BIT(KC_LALT) || keyboard_report->mods & MOD_BIT(KC_RALT))

uint8_t last_led_animation_id = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint32_t key_timer;

    set_sleep_state(false);

    switch (keycode) {
        case L_BRI:
            if (record->event.pressed) {
                if (LED_GCR_STEP > LED_GCR_MAX - gcr_desired) gcr_desired = LED_GCR_MAX;
                else gcr_desired += LED_GCR_STEP;
                if (led_animation_breathing) gcr_breathe = gcr_desired;
            }
            return false;
        case L_BRD:
            if (record->event.pressed) {
                if (LED_GCR_STEP > gcr_desired) gcr_desired = 0;
                else gcr_desired -= LED_GCR_STEP;
                if (led_animation_breathing) gcr_breathe = gcr_desired;
            }
            return false;
        case L_PTN:
            if (record->event.pressed) {
                if (led_animation_id == led_setups_count - 1) led_animation_id = 0;
                else led_animation_id++;

                last_led_animation_id = led_animation_id;
            }
            return false;
        case L_PTP:
            if (record->event.pressed) {
                if (led_animation_id == 0) led_animation_id = led_setups_count - 1;
                else led_animation_id--;

                last_led_animation_id = led_animation_id;
            }
            return false;
        case L_PSI:
            if (record->event.pressed) {
                led_animation_speed += ANIMATION_SPEED_STEP;
            }
            return false;
        case L_PSD:
            if (record->event.pressed) {
                led_animation_speed -= ANIMATION_SPEED_STEP;
                if (led_animation_speed < 0) led_animation_speed = 0;
            }
            return false;
        case L_T_MD:
            if (record->event.pressed) {
                led_lighting_mode++;
                if (led_lighting_mode > LED_MODE_MAX_INDEX) led_lighting_mode = LED_MODE_NORMAL;
            }
            return false;
        case L_T_ONF:
            if (record->event.pressed) {
                led_enabled = !led_enabled;
                I2C3733_Control_Set(led_enabled);
            }
            return false;
        case L_ON:
            if (record->event.pressed) {
                led_enabled = 1;
                I2C3733_Control_Set(led_enabled);
            }
            return false;
        case L_OFF:
            if (record->event.pressed) {
                led_enabled = 0;
                I2C3733_Control_Set(led_enabled);
            }
            return false;
        case L_T_BR:
            if (record->event.pressed) {
                led_animation_breathing = !led_animation_breathing;
                if (led_animation_breathing) {
                    gcr_breathe = gcr_desired;
                    led_animation_breathe_cur = BREATHE_MIN_STEP;
                    breathe_dir = 1;
                }
            }
            return false;
        case L_T_PTD:
            if (record->event.pressed) {
                led_animation_direction = !led_animation_direction;
            }
            return false;
        case U_T_AUTO:
            if (record->event.pressed && MODS_SHIFT && MODS_CTRL) {
                TOGGLE_FLAG_AND_PRINT(usb_extra_manual, "USB extra port manual mode");
            }
            return false;
        case U_T_AGCR:
            if (record->event.pressed && MODS_SHIFT && MODS_CTRL) {
                TOGGLE_FLAG_AND_PRINT(usb_gcr_auto, "USB GCR auto mode");
            }
            return false;
        case DBG_TOG:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_enable, "Debug mode");
            }
            return false;
        case DBG_MTRX:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_matrix, "Debug matrix");
            }
            return false;
        case DBG_KBD:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_keyboard, "Debug keyboard");
            }
            return false;
        case DBG_MOU:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_mouse, "Debug mouse");
            }
            return false;
        case MD_BOOT:
            if (record->event.pressed) {
                key_timer = timer_read32();
            } else {
                if (timer_elapsed32(key_timer) >= DFU_DURATION) {
                    reset_keyboard();
                }
            }
            return false;
        default:
            return true; //Process all other keycodes normally
    }
}

uint32_t layer_state_set_user(uint32_t state) {
  switch (biton32(state)) {
    case BASE:
        led_animation_id = last_led_animation_id;
        led_lighting_mode = LED_MODE_DEFAULT;
        break;
    case LOWER:
        last_led_animation_id = led_animation_id;
        led_animation_id = LED_ANIMATION_ID_LOWER;
        led_lighting_mode = LED_MODE_LOWER;
        break;
  }
  return state;
}


led_instruction_t led_instructions[] = {
    //LEDs are normally inactive, no processing is performed on them
    //Flags are used in matching criteria for an LED to be active and indicate how to color it
    //Flags can be found in tmk_core/protocol/arm_atsam/led_matrix.h (prefixed with LED_FLAG_)
    //LED IDs can be found in config_led.h in the keyboard's directory
    //Examples are below

    //All LEDs use the user's selected pattern (this is the factory default)
     { .flags = LED_FLAG_USE_ROTATE_PATTERN },

    //Specific LEDs use the user's selected pattern while all others are off
    // { .flags = LED_FLAG_MATCH_ID | LED_FLAG_USE_ROTATE_PATTERN, .id0 = 0xFFFFFFFF, .id1 = 0xAAAAAAAA, .id2 = 0x55555555, .id3 = 0x11111111 },

    //Specific LEDs use specified RGB values while all others are off
    // { .flags = LED_FLAG_MATCH_ID | LED_FLAG_USE_RGB, .id0 = 0xFF, .id1 = 0x00FF, .id2 = 0x0000FF00, .id3 = 0xFF000000, .r = 75, .g = 150, .b = 225 },

    //All LEDs use the user's selected pattern
    //On layer 1, all key LEDs (except the top row which keeps active pattern) are red while all edge LEDs are green
    //When layer 1 is active, key LEDs use red    (id0  32 -  16: 1111 1111 1111 1111 1000 0000 0000 0000 = 0xFFFF8000) (except top row 15 - 1)
    //When layer 1 is active, key LEDs use red    (id1  64 -  33: 1111 1111 1111 1111 1111 1111 1111 1111 = 0xFFFFFFFF)
    //When layer 1 is active, key LEDs use red    (id2  67 -  65: 0000 0000 0000 0000 0000 0000 0000 0111 = 0x00000007)
    //When layer 1 is active, edge LEDs use green (id2  95 -  68: 1111 1111 1111 1111 1111 1111 1111 1000 = 0xFFFFFFF8)
    //When layer 1 is active, edge LEDs use green (id3 105 -  96: 0000 0000 0000 0000 0000 0011 1111 1111 = 0x000003FF)
    // { .flags = LED_FLAG_USE_ROTATE_PATTERN },
    // { .flags = LED_FLAG_MATCH_ID | LED_FLAG_MATCH_LAYER | LED_FLAG_USE_RGB, .id0 = 0xFFFF8000, .id1 = 0xFFFFFFFF, .id2 = 0x00000007, .r = 255, .layer = 1 },
    // { .flags = LED_FLAG_MATCH_ID | LED_FLAG_MATCH_LAYER | LED_FLAG_USE_RGB, .id2 = 0xFFFFFFF8, .id3 = 0x000003FF, .g = 127, .layer = 1 },

    //All key LEDs use red while edge LEDs use the active pattern
    //All key LEDs use red     (id0  32 -   1: 1111 1111 1111 1111 1111 1111 1111 1111 = 0xFFFFFFFF)
    //All key LEDs use red     (id1  64 -  33: 1111 1111 1111 1111 1111 1111 1111 1111 = 0xFFFFFFFF)
    //All key LEDs use red     (id2  67 -  65: 0000 0000 0000 0000 0000 0000 0000 0111 = 0x00000007)
    //Edge uses active pattern (id2  95 -  68: 1111 1111 1111 1111 1111 1111 1111 1000 = 0xFFFFFFF8)
    //Edge uses active pattern (id3 105 -  96: 0000 0000 0000 0000 0000 0011 1111 1111 = 0x000003FF)
    // { .flags = LED_FLAG_MATCH_ID | LED_FLAG_USE_RGB, .id0 = 0xFFFFFFFF, .id1 = 0xFFFFFFFF, .id2 = 0x00000007, .r = 255 },
    // { .flags = LED_FLAG_MATCH_ID | LED_FLAG_USE_ROTATE_PATTERN , .id2 = 0xFFFFFFF8, .id3 = 0x000003FF },

    //end must be set to 1 to indicate end of instruction set
     { .end = 1 }
};
