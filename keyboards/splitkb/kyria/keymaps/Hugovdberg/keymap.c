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
    _QWERTY,
    _GAMING,
    _COLEMAK_DH,
    _NAV,
    _SYM,
    _FUNCTION,
    _ADJUST,
    _MEDIA,
    _MOUSE,
    _NUM
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
#define FULL_MAKS DF(_FULL_MAKS)
#define QWERTY    DF(_QWERTY)
#define COLEMAK   DF(_COLEMAK_DH)
#define DVORAK    DF(_GAMING)

#define SYM       MO(_SYM)
#define NAV       MO(_NAV)
#define FKEYS     MO(_FUNCTION)
#define ADJUST    MO(_ADJUST)
#define MEDIA     MO(_MEDIA)
#define MOUSE     MO(_MOUSE)

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

#define GUI_H     MT(MOD_LGUI, KC_H)    // Left GUI when held, H when tapped
#define ALT_I     MT(MOD_LALT, KC_I)    // Left Alt when held, I when tapped
#define CTRL_E    MT(MOD_LCTL, KC_E)    // Left Ctrl when held, E when tapped
#define SHFT_A    MT(MOD_LSFT, KC_A)    // Left Shift when held, A when tapped

#define SHFT_S    MT(MOD_LSFT, KC_S)    // Right Shift when held, S when tapped
#define CTRL_T    MT(MOD_LCTL, KC_T)    // Right Ctrl when held, T when tapped
#define ALT_N     MT(MOD_LALT, KC_N)    // Right Alt when held, N when tapped
#define GUI_R     MT(MOD_LGUI, KC_R)    // Right GUI when held, R when tapped

#define APP_MENU  MT(MOD_RALT, KC_APP)  // Right Alt when held, Menu when tapped

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


td_state_t cur_dance(tap_dance_state_t *state) {
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
    switch (cur_dance(state)) {
        case TD_SINGLE_TAP:
            tap_code16(keycode);
            tap_code16(KC_SPC);
            break;

        case TD_SINGLE_HOLD:
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
 * Base Layer: Full maks minimo
 *
 * ,-------------------------------------------.                              ,-----------------------------------------.
 * |  LAlt  |   Q  |   Y  |   O  |   U  |   '  |                              |   :  |   C  |   L  |   P  |   V  |   X  |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+------|
 * | LShift |   H  |   I  |   E  |   A  |   ,  |                              |   D  |   S  |   T  |   N  |   R  |   W  |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+------|
 * |        |   K  |   J  |   _  |   .  |   ;  |  /   |CapsLk|  |F-keys|   \  |   F  |   G  |   M  |   B  |   Z  |   =  |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+--------------------'
 *                        |Adjust| LGUI | LAlt/| Space| Nav  |  | Sym/ | Num/ | Fun/ | Menu/| Menu |
 *                        |      |      | Enter|      |      |  |  Ent | Bksp |  Del | AltGr|      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_FULL_MAKS] = LAYOUT(
     KC_LALT, KC_Q ,  KC_Y,  KC_O  ,  KC_U  , TD_QUOTE,                                           KC_COLN ,   KC_C  ,  KC_L , KC_P , KC_V , KC_X,
     KC_LSFT, GUI_H, ALT_I, CTRL_E , SHFT_A , KC_COMM ,                                             KC_D   ,  SHFT_S , CTRL_T, ALT_N, GUI_R, KC_W,
     KC_NO  , KC_K ,  KC_J, KC_UNDS, KC_DOT , KC_SCLN , KC_SLSH, KC_CAPS  ,      FKEYS , KC_BSLS ,  KC_F   ,   KC_G  ,  KC_M , KC_B , KC_Z , KC_EQL,
                             ADJUST, KC_LGUI, ESC_MED , SPC_NAV, TAB_MOUSE,     ENT_SYM, BSPC_NUM, DEL_FUNC, APP_MENU, KC_APP
    ),
/*
 * Base Layer: QWERTY
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |  Tab   |   Q  |   W  |   E  |   R  |   T  |                              |   Y  |   U  |   I  |   O  |   P  |  Bksp  |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |Ctrl/Esc|   A  |   S  |   D  |   F  |   G  |                              |   H  |   J  |   K  |   L  | ;  : |Ctrl/' "|
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   V  |   B  | [ {  |CapsLk|  |F-keys|  ] } |   N  |   M  | ,  < | . >  | /  ? | RShift |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |Adjust| LGUI | LAlt/| Space| Nav  |  | Sym  | Space| AltGr| RGUI | Menu |
 *                        |      |      | Enter|      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_QWERTY] = LAYOUT(
     KC_TAB  , KC_Q ,  KC_W   ,  KC_E  ,   KC_R ,   KC_T ,                                        KC_Y,   KC_U ,  KC_I ,   KC_O ,  KC_P , KC_BSPC,
     CTL_ESC , KC_A ,  KC_S   ,  KC_D  ,   KC_F ,   KC_G ,                                        KC_H,   KC_J ,  KC_K ,   KC_L ,KC_SCLN,CTL_QUOT,
     KC_LSFT , KC_Z ,  KC_X   ,  KC_C  ,   KC_V ,   KC_B , KC_LBRC,KC_CAPS,     FKEYS  , KC_RBRC, KC_N,   KC_M ,KC_COMM, KC_DOT ,KC_SLSH, KC_RSFT,
                                ADJUST , KC_LGUI, ALT_ENT, KC_SPC , NAV   ,     SYM    , KC_SPC ,KC_RALT, KC_RGUI, KC_APP
    ),

/*
 * Base Layer: Dvorak
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
     KC_TAB  ,KC_TAB ,  KC_Q  ,  KC_W  ,   KC_E ,   KC_R ,                                         KC_T  ,   KC_Y ,  KC_U ,   KC_I ,  KC_O , KC_P,
     CTL_ESC ,CTL_ESC,  KC_A  ,  KC_S  ,   KC_D ,   KC_F ,                                         KC_G  ,   KC_H ,  KC_J ,   KC_K ,  KC_L , KC_SCLN,
     KC_LSFT ,KC_LSFT,  KC_Z  ,  KC_X  ,   KC_C ,   KC_V , KC_LBRC,KC_CAPS,     FKEYS  , KC_RBRC,  KC_B  ,   KC_N ,  KC_M , KC_COMM, KC_DOT, KC_SLSH,
                                 ADJUST, KC_LGUI, ALT_ENT, KC_SPC , NAV   ,     ENT_SYM,BSPC_NUM,DEL_FUNC, KC_RGUI, KC_APP
    ),

/*
 * Base Layer: Colemak DH
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |  Tab   |   Q  |   W  |   F  |   P  |   B  |                              |   J  |   L  |   U  |   Y  | ;  : |  Bksp  |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |Ctrl/Esc|   A  |   R  |   S  |   T  |   G  |                              |   M  |   N  |   E  |   I  |   O  |Ctrl/' "|
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   D  |   V  | [ {  |CapsLk|  |F-keys|  ] } |   K  |   H  | ,  < | . >  | /  ? | RShift |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |Adjust| LGUI | LAlt/| Space| Nav  |  | Sym  | Space| AltGr| RGUI | Menu |
 *                        |      |      | Enter|      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_COLEMAK_DH] = LAYOUT(
     KC_TAB  , KC_Q ,  KC_W   ,  KC_F  ,   KC_P ,   KC_B ,                                        KC_J,   KC_L ,  KC_U ,   KC_Y ,KC_SCLN, KC_BSPC,
     CTL_ESC , KC_A ,  KC_R   ,  KC_S  ,   KC_T ,   KC_G ,                                        KC_M,   KC_N ,  KC_E ,   KC_I ,  KC_O , CTL_QUOT,
     KC_LSFT , KC_Z ,  KC_X   ,  KC_C  ,   KC_D ,   KC_V , KC_LBRC,KC_CAPS,     FKEYS  , KC_RBRC, KC_K,   KC_H ,KC_COMM, KC_DOT ,KC_SLSH, KC_RSFT,
                                 ADJUST, KC_LGUI, ALT_ENT, KC_SPC , NAV   ,     SYM    , KC_SPC ,KC_RALT, KC_RGUI, KC_APP
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
 * |    `   |  1   |  2   |  3   |  4   |  5   |                              |   6  |  7   |  8   |  9   |  0   |   =    |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |    ~   |  !   |  @   |  #   |  $   |  %   |                              |   ^  |  &   |  *   |  (   |  )   |   +    |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |    |   |   \  |  :   |  ;   |  -   |  [   |  {   |      |  |      |   }  |   ]  |  _   |  ,   |  .   |  /   |   ?    |
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
 * |    `   |  1   |  2   |  3   |  4   |  5   |                              |   6  |  7   |  8   |  9   |  0   |   =    |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |    ~   |  !   |  @   |  #   |  $   |  %   |                              |   ^  |  &   |  *   |  (   |  )   |   +    |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |    |   |   \  |  :   |  ;   |  -   |  [   |  {   |      |  |      |   }  |   ]  |  _   |  ,   |  .   |  /   |   ?    |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
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
 * |        |      |      |Dvorak|      |      |                              | TOG  | SAI  | HUI  | VAI  | MOD  |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |Colmak|      |      |      |      |  |      |      |      | SAD  | HUD  | VAD  | RMOD |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_ADJUST] = LAYOUT(
      _______, _______, _______, QWERTY ,  _______ , _______,                                    _______, _______, _______, _______,  _______, _______,
      _______, _______, _______, DVORAK , FULL_MAKS, _______,                                    RGB_TOG, RGB_SAI, RGB_HUI, RGB_VAI,  RGB_MOD, _______,
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
