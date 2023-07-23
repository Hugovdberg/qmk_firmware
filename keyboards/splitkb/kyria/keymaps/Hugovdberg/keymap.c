/* Copyright 2019 Thomas Baart <thomas@splitkb.com>
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

enum layers {
    _FULL_MAKS = 0,
    _GAMING,
    _IKZEI,
    _NAV,
    _SYM,
    _FUNCTION,
    _ADJUST,
    _MEDIA,
    _MOUSE,
    _NUM,
    _SYM2,
    _NUMPAD,
    _FUNCTION2,
    _NAV2,
    _BUTTON
};

enum custom_keycodes {
    DEF_LYR = SAFE_RANGE,
};

enum tap_dance {
    _TD_QUOTE = 0,
    _TD_GRAVE,
    _TD_TILDE,
    _TD_CIRCUMFLEX,
};

typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_SINGLE_TAP, // Send two single taps
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD
} td_state_t;

// enum custom_keycodes {
//     APOSTROPHE = SAFE_RANGE,
// };


// Aliases for readability
#define IKZEI     DF(_IKZEI)
#define GAMING    DF(_GAMING)

#define SYM       MO(_SYM)
#define NAV       MO(_NAV)
#define FKEYS     MO(_FUNCTION)
#define ADJUST    MO(_ADJUST)
#define MEDIA     MO(_MEDIA)
#define MOUSE     MO(_MOUSE)

#define IKZEI_B   TO(_IKZEI)
#define SYM2      TO(_SYM2)
#define NUM2      TO(_NUMPAD)
#define FKEYS2    TO(_FUNCTION2)
#define MEDIA2    TO(_NAV2)
#define BUTTON    OSL(_BUTTON)

#define CTL_ESC   MT(MOD_LCTL, KC_ESC)
#define CTL_QUOT  MT(MOD_RCTL, KC_QUOTE)
#define CTL_MINS  MT(MOD_RCTL, KC_MINUS)
#define ALT_ENT   MT(MOD_LALT, KC_ENT)
// Note: LAlt/Enter (ALT_ENT) is not the same thing as the keyboard shortcut Alt+Enter.
// The notation `mod/tap` denotes a key that activates the modifier `mod` when held down, and
// produces the key `tap` when tapped (i.e. pressed and released).

#define KC_COPY     LCTL(KC_C)
#define KC_CUT      LCTL(KC_X)
#define KC_PSTE     LCTL(KC_V)
#define KC_UNDO     LCTL(KC_Z)
#define KC_REDO     LCTL(LSFT(KC_Z))
#define KC_REDO_Y   LCTL(KC_Y)

#define ALT_E     MT(MOD_LALT, KC_E)    // Left Alt when held, E when tapped
#define GUI_I     MT(MOD_LGUI, KC_I)    // Left GUI when held, I when tapped
#define GUI_D     MT(MOD_LGUI, KC_D)    // Left GUI when held, D when tapped

#define GUI_H     MT(MOD_LGUI, KC_H)    // Left GUI when held, H when tapped
#define ALT_I     MT(MOD_LALT, KC_I)    // Left Alt when held, I when tapped
#define CTRL_E    MT(MOD_LCTL, KC_E)    // Left Ctrl when held, E when tapped
#define SHFT_A    MT(MOD_LSFT, KC_A)    // Left Shift when held, A when tapped

#define SHFT_S    MT(MOD_LSFT, KC_S)    // Right Shift when held, S when tapped
#define CTRL_T    MT(MOD_LCTL, KC_T)    // Right Ctrl when held, T when tapped
#define ALT_N     MT(MOD_LALT, KC_N)    // Right Alt when held, N when tapped
#define GUI_R     MT(MOD_LGUI, KC_R)    // Right GUI when held, R when tapped

#define APP_MENU  MT(MOD_RALT, KC_APP)  // Right Alt when held, Menu when tapped

#define OS_LSFT   OSM(MOD_LSFT)
#define OS_LCTL   OSM(MOD_LCTL)
#define OS_LALT   OSM(MOD_LALT)
#define OS_LGUI   OSM(MOD_LGUI)
#define OS_RSFT   OSM(MOD_RSFT)
#define OS_RCTL   OSM(MOD_RCTL)
#define OS_RALT   OSM(MOD_RALT)
#define OS_RGUI   OSM(MOD_RGUI)

#define TD_QUOTE  TD(_TD_QUOTE)
#define TD_GRAVE  TD(_TD_GRAVE)
#define TD_TILDE  TD(_TD_TILDE)
#define TD_CIRC   TD(_TD_CIRCUMFLEX)

#define ESC_MED   LT(_MEDIA, KC_ESC)
#define SPC_NAV   LT(_NAV, KC_SPC)
#define TAB_MOUSE LT(_MOUSE, KC_TAB)
#define ENT_SYM   LT(_SYM, KC_ENT)
#define BSPC_NUM  LT(_NUM, KC_BSPC)
#define DEL_FUNC  LT(_FUNCTION, KC_DEL)


td_state_t cur_dance_step(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted) return TD_DOUBLE_SINGLE_TAP;
        else if (state->pressed) return TD_DOUBLE_HOLD;
        else return TD_DOUBLE_TAP;
    }

    if (state->count == 3) {
        if (state->interrupted || !state->pressed) return TD_TRIPLE_TAP;
        else return TD_TRIPLE_HOLD;
    } else return TD_UNKNOWN;
}

void td_deadkey_finished(tap_dance_state_t *state, void *user_data, uint16_t keycode) {
    switch (cur_dance_step(state)) {
        case TD_SINGLE_TAP:
            tap_code16(keycode);
            tap_code16(KC_SPC);
            break;

        case TD_SINGLE_HOLD:
            tap_code16(keycode);
            break;

        case TD_DOUBLE_TAP:
        case TD_DOUBLE_SINGLE_TAP:
            tap_code16(keycode);
            tap_code16(keycode);
            break;

        default:
            break;
    }
}
#define TD_DEADKEY(func_name, keycode) void func_name(tap_dance_state_t *state, void *user_data) {return td_deadkey_finished(state, user_data, keycode);}

TD_DEADKEY(td_quote_finished, KC_QUOTE);
TD_DEADKEY(td_grave_finished, KC_GRAVE);
TD_DEADKEY(td_tilde_finished, KC_TILDE);
TD_DEADKEY(td_circumflex_finished, KC_CIRCUMFLEX);

tap_dance_action_t tap_dance_actions[] = {
    [_TD_QUOTE]  = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_quote_finished, NULL),
    [_TD_GRAVE]  = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_grave_finished, NULL),
    [_TD_TILDE]  = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_tilde_finished, NULL),
    [_TD_CIRCUMFLEX]  = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_circumflex_finished, NULL),
};


// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base layer: "Ik zei"
 *
 * ,-------------------------------------------.                              ,--------------------------------------------.
 * |   Esc  |   ;  |   h  |   '  |   .  |   ,  |                              |   b  |   v  |   l   |   g  |   j  |  BSPC  |
 * |--------+------+------+------+------+------|                              |------+------+-------+------+------+--------|
 * |   Alt  |   a  |   s  |   e  |   i  |   x  |                              |   p  |   d  |   n   |   t  |   r  | Enter  |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+-------+------+------+--------|
 * |   Gui  |   u  |   f  |   y  |   o  |   q  |      |CapsLk|  |      |      |   z  |   k  |   w   |   m  |   c  |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+-------+----------------------'
 *                        | VOL/ |      |      |      |      |  |      |      |      |      |PGUPDN/|
 *                        |ADJUST| BUTN |  REP | SPC  | LSFT |  | RCTL |  L2  | AREP |      |  MENU |
 *                        `----------------------------------'  `-----------------------------------'
 */
    [_IKZEI] = LAYOUT(
    KC_ESCAPE, KC_SCLN,  KC_H  ,TD_QUOTE, KC_DOT , KC_COMM,                                      KC_B  ,  KC_V  ,  KC_L  ,  KC_G  ,  KC_J  , KC_BSPC,
      OS_LALT,  KC_A  ,  KC_S  ,  KC_E  ,  KC_I ,   KC_X  ,                                      KC_P  ,  KC_D  ,  KC_N  ,  KC_T  ,  KC_R  , KC_ENT,
      OS_LGUI,  KC_U  ,  KC_F  ,  KC_Y  ,  KC_O  ,  KC_Q  , XXXXXXX, KC_CAPS, XXXXXXX, XXXXXXX,  KC_Z  ,  KC_K  ,  KC_W  ,  KC_M  ,  KC_C  , XXXXXXX,
                                 ADJUST , BUTTON , QK_REP , KC_SPC , OS_LSFT, OS_RCTL,  SYM2  , QK_AREP, KC_APP ,  HOME
    ),

/*
 * Base Layer: GAMING
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |  Tab   | ' "  | , <  | . >  |   P  |   Y  |                              |   F  |   G  |   C  |   R  |   L  |  Bksp  |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |Ctrl/Esc|   A  |   O  |   E  |   U  |   I  |                              |   D  |   H  |   T  |   N  |   S  |Ctrl/- _|
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | LShift | ; :  |   Q  |   J  |   K  |   X  | [ {  |CapsLk|  |F-keys|  ] } |   B  |   M  |   W  |   V  |   Z  | RShift |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |Adjust| LGUI | LAlt/| Space| Nav  |  | Sym  | Space| AltGr| RGUI | Menu |
 *                        |      |      | Enter|      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_GAMING] = LAYOUT(
     KC_ESC  ,KC_TAB ,  KC_Q  ,  KC_W  ,   KC_E ,   KC_R ,                                         KC_T  ,   KC_Y ,  KC_U ,   KC_I ,  KC_O , KC_P,
     CTL_ESC ,KC_LCTL,  KC_A  ,  KC_S  ,   KC_D ,   KC_F ,                                         KC_G  ,   KC_H ,  KC_J ,   KC_K ,  KC_L , KC_SCLN,
     KC_LSFT ,KC_LSFT,  KC_Z  ,  KC_X  ,   KC_C ,   KC_V , KC_LBRC,KC_CAPS,     FKEYS  , KC_RBRC,  KC_B  ,   KC_N ,  KC_M , KC_COMM, KC_DOT, KC_SLSH,
                                 ADJUST, KC_LGUI, ALT_ENT, KC_SPC , NAV   ,     ENT_SYM,BSPC_NUM,DEL_FUNC, KC_RGUI, KC_APP
    ),

/*
 * Symbol layer
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |      |      |      |                              |      |  _   |  <   |  >   |  =   |  BSPC  |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |   ^  |  &   |  @   |  $   |      |                              |      |  /   |  (   |  )   |  \   | Enter  |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |   ~  |  !   |  ?   |      |      |      |      |  |      |      |      |  {   |  [   |  ]   |  }   |  NAV2  |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |ikzei |      |  |      | NUM2 |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_SYM2] = LAYOUT(
      _______, ________, _______, _______, _______, _______,                                     _______, KC_UNDS,  KC_LT ,  KC_RT , KC_EQL , KC_BSPC,
      _______, TD_CIRC , KC_AMPR,  KC_AT , KC_DLR , _______,                                     _______, KC_SLSH, KC_LPRN, KC_RPRN, KC_BSLS, KC_ENT ,
      _______, TD_TILDE, KC_EXLM, KC_QUES, _______, _______, _______, _______, _______, _______, _______, KC_LCBR, KC_LBRC, KC_RBRC, KC_RCBR,   NAV2 ,
                                  _______, _______, _______, IKZEI_B, _______, _______,   NUM2 , _______, _______, _______
    ),

/*
 * Numpad layer
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |      |      |      |                              |NumLck|  7   |  8   |  9   |  -   |   /    |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |                              |      |  4   |  5   |  6   |  +   |   *    |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |      |  |      |      |      |  1   |  2   |  3   | Enter|   =    |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |ikzei | LSFT |  |      |   0  |   .  |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_NUMPAD] = LAYOUT(
      _______, ________, _______, _______, _______, _______,                                     KC_NUM ,  KC_P7 ,  KC_P8 ,  KC_P9 , KC_PMNS, KC_PSLS,
      _______, KC_MPRV , KC_MSTP, KC_MPLY, KC_MNXT, _______,                                     _______,  KC_P4 ,  KC_P5 ,  KC_P6 , KC_PPLS, KC_PAST,
      _______, KC_HOME , KC_PGDN, KC_PGUP, KC_END , _______, _______, _______, _______, _______, _______,  KC_P1 ,  KC_P2 ,  KC_P3 , KC_PENT, KC_PEQL,
                                  _______, _______, KC_MUTE, IKZEI_B, OS_LSFT, _______,  KC_P0 , KC_PDOT, _______, _______
    ),
/*
 * Nav Layer: Media, navigation
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |      |      |      |                              | PgUp | Home |   ↑  | End  | VolUp| Delete |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  GUI |  Alt | Ctrl | Shift|      |                              | PgDn |  ←   |   ↓  |   →  | VolDn| Insert |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |ScLck |  |      |      | Pause|M Prev|M Play|M Next|VolMut| PrtSc  |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_NAV] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                     KC_REDO, KC_PSTE, KC_COPY, KC_CUT , KC_UNDO,  KC_REDO_Y,
      _______, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, _______,                                     KC_CAPS, KC_LEFT, KC_DOWN,  KC_UP , KC_RGHT,  KC_NO ,
      _______, _______, _______, _______, _______, _______, _______, KC_SCRL, _______, _______, KC_INS , KC_HOME, KC_PGDN, KC_PGUP, KC_END , KC_PSCR,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),

/*
 * Nav Layer: Mouse
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |      |      |      |                              | PgUp | Home |   ↑  | End  | VolUp| Delete |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  GUI |  Alt | Ctrl | Shift|      |                              | PgDn |  ←   |   ↓  |   →  | VolDn| Insert |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |ScLck |  |      |      | Pause|M Prev|M Play|M Next|VolMut| PrtSc  |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_MOUSE] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                        KC_REDO,   KC_PSTE   ,   KC_COPY   ,  KC_CUT   ,   KC_UNDO    ,KC_REDO_Y,
      _______, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, _______,                                         KC_NO , KC_MS_LEFT  , KC_MS_DOWN  , KC_MS_UP  , KC_MS_RIGHT  ,  KC_NO  ,
      _______, _______, _______, _______, _______, _______, _______, _______,    _______, _______,  KC_NO ,KC_MS_WH_LEFT,KC_MS_WH_DOWN,KC_MS_WH_UP,KC_MS_WH_RIGHT,  KC_NO  ,
                                 _______, _______, _______, _______, _______,    KC_BTN3, KC_BTN1, KC_BTN2,   _______   ,    _______
    ),

/*
 * Media Layer
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |      |      |      |                              | RGB tog | Home |   ↑  | End  | VolUp| Delete |
 * |--------+------+------+------+------+------|                              |---------+------+------+------+------+--------|
 * |        |  GUI |  Alt | Ctrl | Shift|      |                              |   PgDn  |  ←   |   ↓  |   →  | VolDn| Insert |
 * |--------+------+------+------+------+------+-------------.  ,-------------+---------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |ScLck |  |      |      |  Pause  |M Prev|M Play|M Next|VolMut| PrtSc  |
 * `----------------------+------+------+------+------+------|  |------+------+---------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_MEDIA] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                     KC_REDO, KC_PSTE, KC_COPY, KC_CUT , KC_UNDO,KC_REDO_Y,
      _______, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, _______,                                      KC_NO , KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT,  KC_NO  ,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  KC_NO , KC_HOME, KC_PGDN, KC_PGUP, KC_END ,  KC_NO  ,
                                 _______, _______, _______, _______, _______, KC_MSTP, KC_MPLY, KC_MUTE, _______, _______
    ),

/*
 * Num Layer: Numbers
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  [   |  7   |  8   |  9   |  ]   |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  ;   |  4   |  5   |  6   |  =   |                              |      | LSFT | CTRL |  ALT |  GUI |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |  `   |  4   |  2   |  3   |  \   |  {   |      |  |      |   }  |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_NUM] = LAYOUT(
      KC_NO , KC_LBRC,  KC_7  ,  KC_8  ,  KC_9  , KC_RBRC,                                     _______, _______, _______, _______, _______, _______,
      KC_NO , KC_SCLN,  KC_4  ,  KC_5  ,  KC_6  , KC_EQL ,                                     _______, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI, _______,
      KC_NO ,TD_GRAVE,  KC_1  ,  KC_2  ,  KC_3  , KC_NUBS, KC_LCBR, _______, _______, KC_RCBR, _______, _______, _______, _______, _______, _______,
                                _______, _______, KC_DOT ,  KC_0  , KC_MINS, _______, _______, _______, _______, _______
    ),


/*
 * Sym Layer: Numbers and symbols
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  {   |  &   |  *   |  (   |  }   |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  :   |  $   |  %   |  ^   |  +   |                              |      | Shift| Ctrl |  Alt |  GUI |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |  ~   |  !   |  @   |  #   |  |   |  {   |      |  |      |   }  |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |  (   |  )   |  _   |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_SYM] = LAYOUT(
      KC_NO , KC_LCBR, KC_AMPR, KC_ASTR, KC_LPRN, KC_RCBR,                                     _______, _______, _______, _______, _______, _______,
      KC_NO , KC_COLN, KC_DLR , KC_PERC, TD_CIRC, KC_PLUS,                                     _______, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI, _______,
      KC_NO ,TD_TILDE, KC_EXLM,  KC_AT , KC_HASH, KC_PIPE, KC_LCBR, _______, _______, KC_RCBR, _______, _______, _______, _______, _______, _______,
                                _______, _______, KC_LPRN, KC_RPRN, KC_UNDS, _______, _______, _______, _______, _______
    ),

/*
 * Function Layer: Function keys
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  F9  | F10  | F11  | F12  |      |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  F5  |  F6  |  F7  |  F8  |      |                              |      | Shift| Ctrl |  Alt |  GUI |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |  F1  |  F2  |  F3  |  F4  |      |      |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_FUNCTION] = LAYOUT(
      _______,  KC_F12 ,  KC_F7 ,  KC_F8 ,  KC_F9 , KC_PSCR ,                                     _______, _______, _______, _______, _______, _______,
      _______,  KC_F11 ,  KC_F4 ,  KC_F5 ,  KC_F6 , KC_SCRL ,                                     _______, KC_RSFT, KC_RCTL, KC_LALT, KC_RGUI, _______,
      _______,  KC_F10 ,  KC_F1 ,  KC_F2 ,  KC_F3 , KC_PAUSE, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                  _______, _______, KC_APP  , KC_SPC , KC_TAB , _______, _______, _______, _______, _______
    ),

/*
 * Adjust Layer: Default layer settings, RGB
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |QWERTY|      |      |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      |      |GAMING|      |      |                              | TOG  | SAI  | HUI  | VAI  | MOD  |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |Colmak|      |      |      |      |  |      |      |      | SAD  | HUD  | VAD  | RMOD |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_ADJUST] = LAYOUT(
      _______, _______, _______, QWERTY ,   IKZEI  , _______,                                    _______, _______, _______, _______,  _______, _______,
      _______, _______, _______, GAMING , FULL_MAKS, _______,                                    RGB_TOG, RGB_SAI, RGB_HUI, RGB_VAI,  RGB_MOD, _______,
      _______, _______, _______, COLEMAK,  _______ , _______,_______, _______, _______, _______, _______, RGB_SAD, RGB_HUD, RGB_VAD, RGB_RMOD, _______,
                                 _______,  _______ , _______,_______, _______, _______, _______, _______, _______, _______
    ),

// /*
//  * Layer template
//  *
//  * ,-------------------------------------------.                              ,-------------------------------------------.
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
//  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        `----------------------------------'  `----------------------------------'
//  */
//     [_LAYERINDEX] = LAYOUT(
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
//                                  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
//     ),
};

uint16_t get_alt_repeat_key_keycode_user(uint16_t keycode, uint8_t mods) {
    if ((mods & MOD_MASK_CTRL)) {  // Was Ctrl held?
        switch (keycode) {
            case KC_Y: return C(KC_Z);  // Ctrl + Y reverses to Ctrl + Z.
            case KC_Z: return C(KC_Y);  // Ctrl + Z reverses to Ctrl + Y.
        }
    }

    return KC_TRNS;  // Defer to default definitions.
}

/* The default OLED and rotary encoder code can be found at the bottom of qmk_firmware/keyboards/splitkb/kyria/rev1/rev1.c
 * These default settings can be overriden by your own settings in your keymap.c
 * For your convenience, here's a copy of those settings so that you can uncomment them if you wish to apply your own modifications.
 * DO NOT edit the rev1.c file; instead override the weakly defined default functions by your own.
 */


#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_180; }

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        // QMK Logo and version information
        // clang-format off
        static const char PROGMEM qmk_logo[] = {
            0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
            0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
            0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0};
        // clang-format on

        oled_write_P(qmk_logo, false);
        oled_write_P(PSTR("Kyria rev3.1\n\n"), false);

        // Host Keyboard Layer Status
        oled_write_P(PSTR("Layer: "), false);
        switch (get_highest_layer(layer_state|default_layer_state)) {
            case _FULL_MAKS:
                oled_write_P(PSTR("Full maks\n"), false);
                break;
            case _QWERTY:
                oled_write_P(PSTR("QWERTY\n"), false);
                break;
            case _GAMING:
                oled_write_P(PSTR("Gaming\n"), false);
                break;
            case _COLEMAK_DH:
                oled_write_P(PSTR("Colemak-DH\n"), false);
                break;
            case _IKZEI:
                oled_write_P(PSTR("ikzei\n"), false);
                break;
            case _NAV:
                oled_write_P(PSTR("Nav\n"), false);
                break;
            case _SYM:
                oled_write_P(PSTR("Sym\n"), false);
                break;
            case _FUNCTION:
                oled_write_P(PSTR("Function\n"), false);
                break;
            case _ADJUST:
                oled_write_P(PSTR("Adjust\n"), false);
                break;
            case _MEDIA:
                oled_write_P(PSTR("Media\n"), false);
                break;
            case _MOUSE:
                oled_write_P(PSTR("Mouse\n"), false);
                break;
            case _NUM:
                oled_write_P(PSTR("Num\n"), false);
                break;
            case _SYM2:
                oled_write_P(PSTR("Symbols\n"), false);
                break;
            case _NUMPAD:
                oled_write_P(PSTR("Numpad\n"), false);
                break;
            case _FUNCTION2:
                oled_write_P(PSTR("F-keys\n"), false);
                break;
            default:
                oled_write_P(PSTR("Undefined\n"), false);
        }

        // Write host Keyboard LED Status to OLEDs
        led_t led_usb_state = host_keyboard_led_state();
        oled_write_P(led_usb_state.num_lock    ? PSTR("NUMLCK ") : PSTR("       "), false);
        oled_write_P(led_usb_state.caps_lock   ? PSTR("CAPLCK ") : PSTR("       "), false);
        oled_write_P(led_usb_state.scroll_lock ? PSTR("SCRLCK ") : PSTR("       "), false);
    } else {
        // clang-format off
        static const char PROGMEM kyria_logo[] = {
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,192,224,240,112,120, 56, 60, 28, 30, 14, 14, 14,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7, 14, 14, 14, 30, 28, 60, 56,120,112,240,224,192,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,192,224,240,124, 62, 31, 15,  7,  3,  1,128,192,224,240,120, 56, 60, 28, 30, 14, 14,  7,  7,135,231,127, 31,255,255, 31,127,231,135,  7,  7, 14, 14, 30, 28, 60, 56,120,240,224,192,128,  1,  3,  7, 15, 31, 62,124,240,224,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,240,252,255, 31,  7,  1,  0,  0,192,240,252,254,255,247,243,177,176, 48, 48, 48, 48, 48, 48, 48,120,254,135,  1,  0,  0,255,255,  0,  0,  1,135,254,120, 48, 48, 48, 48, 48, 48, 48,176,177,243,247,255,254,252,240,192,  0,  0,  1,  7, 31,255,252,240,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,255,255,255,  0,  0,  0,  0,  0,254,255,255,  1,  1,  7, 30,120,225,129,131,131,134,134,140,140,152,152,177,183,254,248,224,255,255,224,248,254,183,177,152,152,140,140,134,134,131,131,129,225,120, 30,  7,  1,  1,255,255,254,  0,  0,  0,  0,  0,255,255,255,  0,  0,  0,  0,255,255,  0,  0,192,192, 48, 48,  0,  0,240,240,  0,  0,  0,  0,  0,  0,240,240,  0,  0,240,240,192,192, 48, 48, 48, 48,192,192,  0,  0, 48, 48,243,243,  0,  0,  0,  0,  0,  0, 48, 48, 48, 48, 48, 48,192,192,  0,  0,  0,  0,  0,
            0,  0,  0,255,255,255,  0,  0,  0,  0,  0,127,255,255,128,128,224,120, 30,135,129,193,193, 97, 97, 49, 49, 25, 25,141,237,127, 31,  7,255,255,  7, 31,127,237,141, 25, 25, 49, 49, 97, 97,193,193,129,135, 30,120,224,128,128,255,255,127,  0,  0,  0,  0,  0,255,255,255,  0,  0,  0,  0, 63, 63,  3,  3, 12, 12, 48, 48,  0,  0,  0,  0, 51, 51, 51, 51, 51, 51, 15, 15,  0,  0, 63, 63,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 48, 48, 63, 63, 48, 48,  0,  0, 12, 12, 51, 51, 51, 51, 51, 51, 63, 63,  0,  0,  0,  0,  0,
            0,  0,  0,  0, 15, 63,255,248,224,128,  0,  0,  3, 15, 63,127,255,239,207,141, 13, 12, 12, 12, 12, 12, 12, 12, 30,127,225,128,  0,  0,255,255,  0,  0,128,225,127, 30, 12, 12, 12, 12, 12, 12, 12, 13,141,207,239,255,127, 63, 15,  3,  0,  0,128,224,248,255, 63, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  3,  7, 15, 62,124,248,240,224,192,128,  1,  3,  7, 15, 30, 28, 60, 56,120,112,112,224,224,225,231,254,248,255,255,248,254,231,225,224,224,112,112,120, 56, 60, 28, 30, 15,  7,  3,  1,128,192,224,240,248,124, 62, 15,  7,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  3,  7, 15, 14, 30, 28, 60, 56,120,112,112,112,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,112,112,112,120, 56, 60, 28, 30, 14, 15,  7,  3,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
        };
        // clang-format on
        oled_write_raw_P(kyria_logo, sizeof(kyria_logo));
    }
    return false;
}
#endif

#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {

    if (index == 0) {
        // Volume control
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    } else if (index == 1) {
        // Page up/Page down
        if (clockwise) {
            tap_code(KC_PGDN);
        } else {
            tap_code(KC_PGUP);
        }
    }
    return false;
}
#endif
