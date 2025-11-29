/**
 * @file lv_conf.h
 * Minimal configuration for ESP32
 */

#ifndef LV_CONF_H
#define LV_CONF_H

/* Set to 1 to enable content */
#define LV_CONF_SKIP 1

/*==================
 * COLOR SETTINGS
 *==================*/
#define LV_COLOR_DEPTH 16

/*==================
 * MEMORY SETTINGS
 *==================*/
#define LV_MEM_SIZE (64 * 1024U)

/*==================
 * HAL SETTINGS
 *==================*/
#define LV_TICK_CUSTOM 1
#if LV_TICK_CUSTOM
#define LV_TICK_CUSTOM_INCLUDE "Arduino.h"
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (millis())
#endif

/*==================
 * DISPLAY SETTINGS
 *==================*/
#define LV_HOR_RES_MAX 320
#define LV_VER_RES_MAX 240

/*==================
 * FONT SETTINGS
 *==================*/
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_DEFAULT &lv_font_montserrat_14

/*==================
 * WIDGET SETTINGS
 *==================*/
#define LV_USE_ARC 1
#define LV_USE_BAR 1
#define LV_USE_BTN 1
#define LV_USE_BUTTONMATRIX 1
#define LV_USE_CHECKBOX 1
#define LV_USE_LABEL 1
#define LV_USE_SLIDER 1
#define LV_USE_SWITCH 1

/*==================
 * THEME SETTINGS
 *==================*/
#define LV_USE_THEME_DEFAULT 1

/*==================
 * LAYOUT SETTINGS
 *==================*/
#define LV_USE_FLEX 1
#define LV_USE_GRID 1

/* Fix for problematic macro - completely disable it */
#define LV_EXPORT_CONST_INT(int_value)

#endif /* LV_CONF_H */
