/**
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#    include "timer.h"
#endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

enum charybdis_keymap_layers {
    LAYER_BASE = 0,
    LAYER_LOWER,
    LAYER_RAISE,
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

#define LOWER MO(LAYER_LOWER)
#define RAISE MO(LAYER_RAISE)
#define PT_Z    LT(LAYER_POINTER, KC_Z)
#define PT_SLSH LT(LAYER_POINTER, KC_SLSH)

// Default modifiers
// clang-format off
#define LL_ESC  LT(LAYER_LOWER, KC_ESC)
#define LL_QUOT LT(LAYER_LOWER, KC_QUOT)
#define RL_G    LT(LAYER_RAISE, KC_G)
#define RL_H    LT(LAYER_RAISE, KC_H)
// clang-format on

// Homerow mods
// clang-format off
#define HM_A MT(MOD_LCTL, KC_A)
#define HM_S MT(MOD_LALT, KC_S)
#define HM_D MT(MOD_LGUI, KC_D)
#define HM_F MT(MOD_LSFT, KC_F)
#define HM_J MT(MOD_LSFT, KC_J)
#define HM_K MT(MOD_LGUI, KC_K)
#define HM_L MT(MOD_LALT, KC_L)
#define HM_SCLN MT(MOD_LCTL, KC_SCLN)
// clang-format on

#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL  KC_NO  // Drag Scroll
#    define DPI_MOD  KC_NO  // DPI+
#    define DPI_RMOD KC_NO  // DPI-
#    define S_D_MOD  KC_NO  // Snip DPI+
#    define S_D_RMOD KC_NO  // Snip DPI-
#    define SNIPING  KC_NO
#endif // !POINTING_DEVICE_ENABLE

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
        KC_EQL,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,       KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_MINS,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
        KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,       KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_BSLS,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
        LL_ESC,    HM_A,    HM_S,    HM_D,    HM_F,    RL_G,       RL_H,    HM_J,    HM_K,    HM_L, HM_SCLN, LL_QUOT,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_LSFT,    PT_Z,    KC_X,    KC_C,    KC_V,    KC_B,       KC_N,    KC_M, KC_COMM,  KC_DOT, PT_SLSH, KC_RSFT,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                   KC_LGUI, KC_SPC, KC_BSPC,     KC_ENT,  KC_SPC,
                                            KC_DEL,   LOWER,      RAISE
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_LOWER] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       RGB_TOG, KC_MPLY,  KC_F10,  KC_F11,  KC_F12, _______,    _______,  KC_NUM, KC_PEQL, KC_PSLS, KC_PAST, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       RGB_MOD, KC_VOLU,   KC_F7,   KC_F8,   KC_F9, _______,    _______,   KC_P7,   KC_P8,   KC_P9, KC_PPLS, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       _______, KC_VOLD,   KC_F4,   KC_F5,   KC_F6, _______,    _______,   KC_P4,   KC_P5,   KC_P6, KC_PMNS, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
      RGB_RMOD, KC_MUTE,   KC_F1,   KC_F2,   KC_F3, _______,    _______,   KC_P1,   KC_P2,   KC_P3, KC_PENT, _______,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                  _______, _______, _______,    _______, KC_PDOT,
                                           _______, _______,      KC_P0
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_RAISE] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       _______, _______, _______, _______, _______, _______,    _______, _______, _______, _______, _______, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_EXLM, KC_LCBR, KC_QUOT,  KC_DQT, KC_RCBR, KC_QUES,    KC_SCLN, KC_LBRC, KC_RBRC, KC_LPRN, KC_RPRN, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_HASH, KC_CIRC,  KC_EQL, KC_MINS,  KC_DLR, KC_ASTR,     KC_DOT, KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
         KC_AT, KC_LABK, KC_PIPE, _______, KC_RABK, KC_SLSH,    KC_ASTR, KC_BSLS, KC_AMPR,  KC_GRV, KC_TILD, _______,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                  KC_TILD, KC_PERC, XXXXXXX,    XXXXXXX, XXXXXXX,
                                           KC_COLN, _______,    XXXXXXX
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_POINTER] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  EE_CLR,     EE_CLR, XXXXXXX, DPI_MOD, S_D_MOD, XXXXXXX, QK_BOOT,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX,DPI_RMOD,S_D_RMOD, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       XXXXXXX, KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT, XXXXXXX,    XXXXXXX, KC_RSFT, KC_RGUI, KC_RALT, KC_RCTL, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       XXXXXXX, _______, DRGSCRL, SNIPING, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, SNIPING, DRGSCRL, _______, XXXXXXX,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                  XXXXXXX, KC_BTN2, KC_BTN3,    KC_BTN3, KC_BTN2,
                                           XXXXXXX, KC_BTN1,    KC_BTN1
  //                            ╰───────────────────────────╯ ╰──────────────────╯
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
