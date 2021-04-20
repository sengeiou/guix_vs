/*******************************************************************************/
/*  This file is auto-generated by Azure RTOS GUIX Studio. Do not edit this    */
/*  file by hand. Modifications to this file should only be made by running    */
/*  the Azure RTOS GUIX Studio application and re-generating the application   */
/*  resource file(s). For more information please refer to the Azure RTOS GUIX */
/*  Studio User Guide, or visit our web site at azure.com/rtos                 */
/*                                                                             */
/*  GUIX Studio Revision 6.0.2.0                                               */
/*  Date (dd.mm.yyyy): 18. 9.2020   Time (hh:mm): 11:15                        */
/*******************************************************************************/


#ifndef _GUIX_SMART_WATCH_MAIN_SCREEN_RESOURCES_H_
#define _GUIX_SMART_WATCH_MAIN_SCREEN_RESOURCES_H_

#include "gx_api.h"

#if !defined(GX_UTF8_SUPPORT)
#error "The symbol GX_UTF8_SUPPORT must be defined to support the Studio project settings".
#endif

/* Display and theme definitions                                               */

#define MAIN_SCREEN 0
#define MAIN_SCREEN_COLOR_FORMAT GX_COLOR_FORMAT_565RGB
#define MAIN_SCREEN_X_RESOLUTION 360
#define MAIN_SCREEN_Y_RESOLUTION 360
#define MAIN_SCREEN_THEME_DEFAULT 0
#define MAIN_SCREEN_THEME_TABLE_SIZE 1

/* Language definitions                                                        */

#define LANGUAGE_ENGLISH 0
#define LANGUAGE_CHINESE 1
#define MAIN_SCREEN_LANGUAGE_TABLE_SIZE 2

/* Color ID definitions                                                        */

#define GX_COLOR_ID_GRAY 29
#define GX_COLOR_ID_WHITE 30
#define GX_COLOR_ID_BLUE 31
#define GX_COLOR_ID_GREEN 32
#define GX_COLOR_ID_LIGHT_GRAY 33
#define GX_COLOR_ID_BLACK 34
#define GX_COLOR_ID_DARK_GRAY 35
#define GX_COLOR_ID_SILVERY_GRAY 36
#define GX_COLOR_ID_MILK_WHITE 37
#define GX_COLOR_ID_CURSOR 38
#define GX_COLOR_ID_HEALTHY_ICON 39
#define GX_COLOR_ID_HINT_TEXT 40
#define GX_COLOR_ID_BUTTON 41
#define GX_COLOR_ID_NAME 42
#define MAIN_SCREEN_COLOR_TABLE_SIZE 43

/* Font ID definitions                                                         */

#define GX_FONT_ID_NORMAL 4
#define GX_FONT_ID_BIG_NUMBER_70 5
#define GX_FONT_ID_BIG_NUMBER_50 6
#define GX_FONT_ID_MIDDLE 7
#define GX_FONT_ID_MIDDLE_NUMBER_40 8
#define GX_FONT_ID_SMALL 9
#define GX_FONT_ID_MIDDLE_30 10
#define GX_FONT_ID_NEWFONT 11
#define MAIN_SCREEN_FONT_TABLE_SIZE 12

/* Pixelmap ID definitions                                                     */

#define GX_PIXELMAP_ID_ALARM 5
#define GX_PIXELMAP_ID_ARROW_DOWN 6
#define GX_PIXELMAP_ID_ARROW_LEFT 7
#define GX_PIXELMAP_ID_ARROW_RIGHT 8
#define GX_PIXELMAP_ID_ARROW_UP 9
#define GX_PIXELMAP_ID_BATTERY 10
#define GX_PIXELMAP_ID_BLUETOOTH 11
#define GX_PIXELMAP_ID_BRUSH 12
#define GX_PIXELMAP_ID_BUTTON_AM_PM 13
#define GX_PIXELMAP_ID_BUTTON_CONTACT_PLUS 14
#define GX_PIXELMAP_ID_BUTTON_CONTACT_PLUS_ON 15
#define GX_PIXELMAP_ID_BUTTON_LEFT 16
#define GX_PIXELMAP_ID_BUTTON_MIDDLE 17
#define GX_PIXELMAP_ID_BUTTON_RIGHT 18
#define GX_PIXELMAP_ID_BUTTON_SEND 19
#define GX_PIXELMAP_ID_BUTTON_SEND_ON 20
#define GX_PIXELMAP_ID_BUTTON_START_RESET 21
#define GX_PIXELMAP_ID_CALCULATOR_MULTIPLY 22
#define GX_PIXELMAP_ID_CALCULATOR_PLUS 23
#define GX_PIXELMAP_ID_CLOCK_ON 24
#define GX_PIXELMAP_ID_CLOCK_PUSHED 25
#define GX_PIXELMAP_ID_CONTACTS_ON 26
#define GX_PIXELMAP_ID_CONTACTS_PUSHED 27
#define GX_PIXELMAP_ID_CONTACT_INFO_BG 28
#define GX_PIXELMAP_ID_DASHBOARD 29
#define GX_PIXELMAP_ID_DOT 30
#define GX_PIXELMAP_ID_EKG_WAVE_DOT 31
#define GX_PIXELMAP_ID_GAMES_ON 32
#define GX_PIXELMAP_ID_GAMES_PUSHED 33
#define GX_PIXELMAP_ID_GLOB 34
#define GX_PIXELMAP_ID_GX_CONTACT_COMMENT_OFF 35
#define GX_PIXELMAP_ID_HEALTH_HEART_LARGE 36
#define GX_PIXELMAP_ID_HEALTH_HEART_SMALL 37
#define GX_PIXELMAP_ID_HEALTH_ON 38
#define GX_PIXELMAP_ID_HEALTH_PUSHED 39
#define GX_PIXELMAP_ID_HEALTH_RUN 40
#define GX_PIXELMAP_ID_ICON_PENCIL 41
#define GX_PIXELMAP_ID_KEYBOARD_BG 42
#define GX_PIXELMAP_ID_MESSAGES_ON 43
#define GX_PIXELMAP_ID_MESSAGES_PUSHED 44
#define GX_PIXELMAP_ID_MICROPHONE 45
#define GX_PIXELMAP_ID_PAGINATION_DOT 46
#define GX_PIXELMAP_ID_PENCIL_EDIT 47
#define GX_PIXELMAP_ID_PRELOADER 48
#define GX_PIXELMAP_ID_SCROLL_WHEEL 49
#define GX_PIXELMAP_ID_SCROLL_WHEEL_SELECTED_BG 50
#define GX_PIXELMAP_ID_SEARCH 51
#define GX_PIXELMAP_ID_SETTINGS_ON 52
#define GX_PIXELMAP_ID_SETTINGS_PUSHED 53
#define GX_PIXELMAP_ID_SOUND 54
#define GX_PIXELMAP_ID_STOP_WATCH 55
#define GX_PIXELMAP_ID_SWITCH_ACTIVE 56
#define GX_PIXELMAP_ID_SWITCH_BG 57
#define GX_PIXELMAP_ID_SWITCH_DISACTIVE 58
#define GX_PIXELMAP_ID_TIME 59
#define GX_PIXELMAP_ID_UP_ARROW 60
#define GX_PIXELMAP_ID_USER_FEMALE 61
#define GX_PIXELMAP_ID_USER_MALE 62
#define GX_PIXELMAP_ID_WEATHER_CLOUDS_LARGE 63
#define GX_PIXELMAP_ID_WEATHER_CLOUD_SMALL 64
#define GX_PIXELMAP_ID_WEATHER_ON 65
#define GX_PIXELMAP_ID_WEATHER_PUSHED 66
#define GX_PIXELMAP_ID_WEATHER_SUN_LARGE 67
#define GX_PIXELMAP_ID_WEATHER_SUN_SMALL 68
#define GX_PIXELMAP_ID_WIFI 69
#define MAIN_SCREEN_PIXELMAP_TABLE_SIZE 70

/* String Ids                                                                  */

#define GX_STRING_ID_DATE 1
#define GX_STRING_ID_TIME_AM 2
#define GX_STRING_ID_ZERO 3
#define GX_STRING_ID_ABOUT_GUIX 4
#define GX_STRING_ID_COMPANY 5
#define GX_STRING_ID_COMPANY_NAME 6
#define GX_STRING_ID_COMPANY_CONTACT 7
#define GX_STRING_ID_COMPANY_WEBSITE 8
#define GX_STRING_ID_STOPWATCH 9
#define GX_STRING_ID_DOUBLE_ZERO 10
#define GX_STRING_ID_TIME_PM 11
#define GX_STRING_ID_EDIT_ALARM 12
#define GX_STRING_ID_SEPARATOR 13
#define GX_STRING_ID_MOBILE 14
#define GX_STRING_ID_OFFICE 15
#define GX_STRING_ID_EMAIL 16
#define GX_STRING_ID_ADDRESS 17
#define GX_STRING_ID_SAVE 18
#define GX_STRING_ID_BEIJING 19
#define GX_STRING_ID_FIRST_NAME 20
#define GX_STRING_ID_STATIC 21
#define GX_STRING_ID_NEW_YORK 22
#define GX_STRING_ID_STRING_3 23
#define GX_STRING_ID_CHARACTER_O 24
#define GX_STRING_ID_STRING_5 25
#define GX_STRING_ID_STRING_6 26
#define GX_STRING_ID_STRING_7 27
#define GX_STRING_ID_STRING_8 28
#define GX_STRING_ID_MONDAY 29
#define GX_STRING_ID_THUESDAY 30
#define GX_STRING_ID_WEDNESDAY 31
#define GX_STRING_ID_THURSDAY 32
#define GX_STRING_ID_FRIDAY 33
#define GX_STRING_ID_SATURDAY 34
#define GX_STRING_ID_SUNDAY 35
#define GX_STRING_ID_SAN_FRANCISCO 36
#define GX_STRING_ID_STRING_10 37
#define GX_STRING_ID_LOS_ANGELES 38
#define GX_STRING_ID_STRING_12 39
#define GX_STRING_ID_STRING_13 40
#define GX_STRING_ID_STRING_14 41
#define GX_STRING_ID_STRING_15 42
#define GX_STRING_ID_STRING_16 43
#define GX_STRING_ID_STRING_19 44
#define GX_STRING_ID_STRING_20 45
#define GX_STRING_ID_STRING_21 46
#define GX_STRING_ID_STRING_22 47
#define GX_STRING_ID_REPLAY 48
#define GX_STRING_ID_STRING_60 49
#define GX_STRING_ID_STRING_29 50
#define GX_STRING_ID_STRING_9 51
#define GX_STRING_ID_ENGLISH 52
#define GX_STRING_ID_SPANISH 53
#define GX_STRING_ID_CHINESE 54
#define GX_STRING_ID_JAPANESE 55
#define GX_STRING_ID_SOUNDS 56
#define GX_STRING_ID_WIFI 57
#define GX_STRING_ID_BLUETOOTH 58
#define GX_STRING_ID_NOTIFICATIONS 59
#define GX_STRING_ID_THEMES 60
#define GX_STRING_ID_VOICE_CONTROL 61
#define GX_STRING_ID_START 62
#define GX_STRING_ID_RESET 63
#define GX_STRING_ID_STOP 64
#define GX_STRING_ID_SENT 65
#define GX_STRING_ID_YESTERDAY 66
#define GX_STRING_ID_STRING_1 67
#define GX_STRING_ID_CHICAGO 68
#define GX_STRING_ID_LONDON 69
#define GX_STRING_ID_MOSCOW 70
#define GX_STRING_ID_STRING_2 71
#define GX_STRING_ID_STRING_4 72
#define GX_STRING_ID_STRING_11 73
#define GX_STRING_ID_STRING_17 74
#define GX_STRING_ID_STRING_18 75
#define GX_STRING_ID_STRING_23 76
#define GX_STRING_ID_STRING_24 77
#define GX_STRING_ID_STRING_25 78
#define GX_STRING_ID_STRING_26 79
#define GX_STRING_ID_STRING_27 80
#define GX_STRING_ID_STRING_28 81
#define GX_STRING_ID_STRING_30 82
#define GX_STRING_ID_STRING_31 83
#define MAIN_SCREEN_STRING_TABLE_SIZE 84

#endif                                       /* sentry                         */
