#include QMK_KEYBOARD_H

#include "action_layer.h"
#include "process_keycode/process_tap_dance.h"

enum charybdis_keymap_layers {
    LAYER_BASE = 0,
    LAYER_NUM_A,
    LAYER_NUM_B,
    LAYER_SYM_A,
    LAYER_SYM_B,
    LAYER_MEDIA,
    LAYER_POINTER,
};

/** \brief Automatically enable sniping-mode on the pointer layer. */
#define CHARYBDIS_AUTO_SNIPING_ON_LAYER LAYER_POINTER

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
static uint16_t auto_pointer_layer_timer = 0;

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS 1000
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD 8
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#endif     // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

enum custom_keycodes {
    TD_MY_MACRO = SAFE_RANGE,
    CK_AA,
    // other custom keycodes...
};

enum { TD_OE, TD_AE, TD_AA };

static inline void send_unicode_hex(const char *hex) {
    register_code(KC_LCTL);
    register_code(KC_LSFT);
    wait_ms(10);
    tap_code(KC_U);
    unregister_code(KC_LSFT);
    unregister_code(KC_LCTL);
    wait_ms(20);
    SEND_STRING(hex);
    wait_ms(20);
    tap_code(KC_SPC);
}

// Specific letters (lower/upper)
static inline void send_oe(bool upper) {
    send_unicode_hex(upper ? "00d8" : "00f8");
} // Ø/ø
static inline void send_ae(bool upper) {
    send_unicode_hex(upper ? "00c6" : "00e6");
} // Æ/æ
static inline void send_aa(bool upper) {
    send_unicode_hex(upper ? "00c5" : "00e5");
} // Å/å

void td_oe_finished(tap_dance_state_t *state, void *user_data) {
    send_oe(state->count >= 2);
}
void td_ae_finished(tap_dance_state_t *state, void *user_data) {
    send_ae(state->count >= 2);
}
void td_aa_finished(tap_dance_state_t *state, void *user_data) {
    send_aa(state->count >= 2);
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_OE] = ACTION_TAP_DANCE_FN(td_oe_finished),
    [TD_AE] = ACTION_TAP_DANCE_FN(td_ae_finished),
    [TD_AA] = ACTION_TAP_DANCE_FN(td_aa_finished),
};

#define HRM_A MT(MOD_LCTL, KC_A)
#define HRM_S MT(MOD_LSFT, KC_S)
#define HRM_D MT(MOD_LGUI, KC_D)
#define HRM_F LT(LAYER_NUM_A, KC_F)
#define HRM_G LT(LAYER_POINTER, KC_G)

#define HRM_SCLN MT(MOD_LCTL, KC_SCLN)
#define HRM_L MT(MOD_RSFT, KC_L)
#define HRM_K MT(MOD_RGUI, KC_K)
#define HRM_J LT(LAYER_NUM_B, KC_J)
#define HRM_H LT(LAYER_POINTER, KC_H)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮      ╭──────────────────────────────────────────────────────╮
       KC_UP,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,          KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_NO,
  // ├──────────────────────────────────────────────────────┤      ├──────────────────────────────────────────────────────┤
       KC_DOWN, HRM_A, HRM_S, HRM_D, HRM_F, HRM_G,                  HRM_H, HRM_J, HRM_K, HRM_L, HRM_SCLN, KC_NO,
  // ├──────────────────────────────────────────────────────┤      ├──────────────────────────────────────────────────────┤
       KC_DEL,   LT(LAYER_POINTER,KC_Z), KC_X, KC_C, LT(LAYER_POINTER,KC_V), LT(LAYER_POINTER,KC_B),     KC_N, KC_M, KC_COMM, KC_DOT, LT(LAYER_POINTER,KC_SLSH), KC_NO,
  // ╰──────────────────────────────────────────────────────┤      ├──────────────────────────────────────────────────────╯
                                  KC_ESC,  KC_SPC,   KC_BSPC,        KC_TAB,  KC_ENT
  //                            ╰───────────────────────────╯      ╰──────────────────╯
  ),

  [LAYER_NUM_A] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       KC_TRNS,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,       KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    TD(TD_AA),
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_PGUP, KC_HOME,     KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, TD(TD_OE), TD(TD_AE),
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_TRNS,  KC_GRV,  KC_QUOT, KC_ESC,  KC_PGDN, KC_END,      KC_EQL,  KC_MINS, KC_LBRC, KC_RBRC, KC_BSLS, KC_TRNS,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                  KC_TRNS, KC_TRNS,  KC_TRNS,    KC_TRNS, KC_TRNS
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_NUM_B] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       KC_TRNS,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,       KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    TD(TD_AA),
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_PGUP, KC_HOME,     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, TD(TD_OE), TD(TD_AE),
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_TRNS,  KC_GRV,  KC_QUOT, KC_ESC,  KC_PGDN, KC_END,      KC_EQL,  KC_MINS, KC_LBRC, KC_RBRC, KC_BSLS, KC_NO,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                  KC_TRNS, KC_TRNS,  KC_TRNS,    KC_TRNS, KC_TRNS
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_POINTER] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       KC_NO,    S(KC_1), S(KC_2), S(KC_3), S(KC_4), S(KC_5),    S(KC_6), S(KC_7), S(KC_8), S(KC_9), S(KC_0), KC_TRNS,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       DRG_TOG, KC_NO,  KC_NO,   KC_NO,   KC_NO,   S_D_MOD,     DPI_MOD, DPI_RMOD, S_D_MOD, S_D_RMOD, KC_TRNS, KC_TRNS,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_NO,    KC_TRNS, SNIPING, DRGSCRL, KC_NO, KC_NO,      DRGSCRL, KC_NO, SNIPING, DRGSCRL, KC_TRNS, QK_BOOT,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                  KC_MS_BTN2, KC_MS_BTN1, KC_MS_BTN3,        KC_MS_BTN3, KC_MS_BTN1
  //                            ╰───────────────────────────╯               ╰──────────────────╯
  ),
};
// clang-format on

#ifdef POINTING_DEVICE_ENABLE
#    ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (abs(mouse_report.x) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD || abs(mouse_report.y) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD) {
        if (auto_pointer_layer_timer == 0) {
            layer_on(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
            rgb_matrix_mode_noeeprom(RGB_MATRIX_NONE);
            rgb_matrix_sethsv_noeeprom(HSV_GREEN);
#        endif // RGB_MATRIX_ENABLE
        }
        auto_pointer_layer_timer = timer_read();
    }
    return mouse_report;
}

void matrix_scan_user(void) {
    if (auto_pointer_layer_timer != 0 && TIMER_DIFF_16(timer_read(), auto_pointer_layer_timer) >= CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS) {
        auto_pointer_layer_timer = 0;
        layer_off(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
        rgb_matrix_mode_noeeprom(RGB_MATRIX_DEFAULT_MODE);
#        endif // RGB_MATRIX_ENABLE
    }
}
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#    ifdef CHARYBDIS_AUTO_SNIPING_ON_LAYER
layer_state_t layer_state_set_user(layer_state_t state) {
    charybdis_set_pointer_sniping_enabled(layer_state_cmp(state, CHARYBDIS_AUTO_SNIPING_ON_LAYER));
    return state;
}
#    endif // CHARYBDIS_AUTO_SNIPING_ON_LAYER
#endif     // POINTING_DEVICE_ENABLE

#ifdef RGB_MATRIX_ENABLE
// Forward-declare this helper function since it is defined in rgb_matrix.c.
void rgb_matrix_update_pwm_buffers(void);
#endif

// bool process_record_user(uint16_t keycode, keyrecord_t *record) {
//     switch (keycode) {
//         case TD_MY_MACRO:
//         case CK_AA:
//             if (record->event.pressed) {
//                 // when keycode QMKBEST is pressed
//                 SEND_STRING("QMK is the best thing ever!");
//             } else {
//                 // when keycode QMKBEST is released
//             }
//             return false;

//     }
//     return true;
// };

// bool process_record_user(uint16_t keycode, keyrecord_t *record) {
//     switch (keycode) {
//         case TD_MY_MACRO:
//             if (record->event.pressed) {
//                 // register_code(KC_LCTL);
//                 // wait_ms(500);
//                 // register_code(KC_LSFT);
//                 // wait_ms(500);
//                 // tap_code(KC_U);
//                 // wait_ms(500);
//                 // unregister_code(KC_LSFT);
//                 // wait_ms(500);
//                 // unregister_code(KC_LCTL);
//                 wait_ms(500);
//                 SEND_STRING("f8");
//                 wait_ms(500);
//                 // tap_code(KC_ENTER);
//                 // wait_ms(500);
//             }
//             return false;
//     }
//     return true;
// }
