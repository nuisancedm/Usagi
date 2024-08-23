#pragma once

// stolen from glfw3.h
/* The unknown key */
#define USG_KEY_UNKNOWN            -1

/* Printable keys */
#define USG_KEY_SPACE              32
#define USG_KEY_APOSTROPHE         39  /* ' */
#define USG_KEY_COMMA              44  /* , */
#define USG_KEY_MINUS              45  /* - */
#define USG_KEY_PERIOD             46  /* . */
#define USG_KEY_SLASH              47  /* / */
#define USG_KEY_0                  48
#define USG_KEY_1                  49
#define USG_KEY_2                  50
#define USG_KEY_3                  51
#define USG_KEY_4                  52
#define USG_KEY_5                  53
#define USG_KEY_6                  54
#define USG_KEY_7                  55
#define USG_KEY_8                  56
#define USG_KEY_9                  57
#define USG_KEY_SEMICOLON          59  /* ; */
#define USG_KEY_EQUAL              61  /* = */
#define USG_KEY_A                  65
#define USG_KEY_B                  66
#define USG_KEY_C                  67
#define USG_KEY_D                  68
#define USG_KEY_E                  69
#define USG_KEY_F                  70
#define USG_KEY_G                  71
#define USG_KEY_H                  72
#define USG_KEY_I                  73
#define USG_KEY_J                  74
#define USG_KEY_K                  75
#define USG_KEY_L                  76
#define USG_KEY_M                  77
#define USG_KEY_N                  78
#define USG_KEY_O                  79
#define USG_KEY_P                  80
#define USG_KEY_Q                  81
#define USG_KEY_R                  82
#define USG_KEY_S                  83
#define USG_KEY_T                  84
#define USG_KEY_U                  85
#define USG_KEY_V                  86
#define USG_KEY_W                  87
#define USG_KEY_X                  88
#define USG_KEY_Y                  89
#define USG_KEY_Z                  90
#define USG_KEY_LEFT_BRACKET       91  /* [ */
#define USG_KEY_BACKSLASH          92  /* \ */
#define USG_KEY_RIGHT_BRACKET      93  /* ] */
#define USG_KEY_GRAVE_ACCENT       96  /* ` */
#define USG_KEY_WORLD_1            161 /* non-US #1 */
#define USG_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define USG_KEY_ESCAPE             256
#define USG_KEY_ENTER              257
#define USG_KEY_TAB                258
#define USG_KEY_BACKSPACE          259
#define USG_KEY_INSERT             260
#define USG_KEY_DELETE             261
#define USG_KEY_RIGHT              262
#define USG_KEY_LEFT               263
#define USG_KEY_DOWN               264
#define USG_KEY_UP                 265
#define USG_KEY_PAGE_UP            266
#define USG_KEY_PAGE_DOWN          267
#define USG_KEY_HOME               268
#define USG_KEY_END                269
#define USG_KEY_CAPS_LOCK          280
#define USG_KEY_SCROLL_LOCK        281
#define USG_KEY_NUM_LOCK           282
#define USG_KEY_PRINT_SCREEN       283
#define USG_KEY_PAUSE              284
#define USG_KEY_F1                 290
#define USG_KEY_F2                 291
#define USG_KEY_F3                 292
#define USG_KEY_F4                 293
#define USG_KEY_F5                 294
#define USG_KEY_F6                 295
#define USG_KEY_F7                 296
#define USG_KEY_F8                 297
#define USG_KEY_F9                 298
#define USG_KEY_F10                299
#define USG_KEY_F11                300
#define USG_KEY_F12                301
#define USG_KEY_F13                302
#define USG_KEY_F14                303
#define USG_KEY_F15                304
#define USG_KEY_F16                305
#define USG_KEY_F17                306
#define USG_KEY_F18                307
#define USG_KEY_F19                308
#define USG_KEY_F20                309
#define USG_KEY_F21                310
#define USG_KEY_F22                311
#define USG_KEY_F23                312
#define USG_KEY_F24                313
#define USG_KEY_F25                314
#define USG_KEY_KP_0               320
#define USG_KEY_KP_1               321
#define USG_KEY_KP_2               322
#define USG_KEY_KP_3               323
#define USG_KEY_KP_4               324
#define USG_KEY_KP_5               325
#define USG_KEY_KP_6               326
#define USG_KEY_KP_7               327
#define USG_KEY_KP_8               328
#define USG_KEY_KP_9               329
#define USG_KEY_KP_DECIMAL         330
#define USG_KEY_KP_DIVIDE          331
#define USG_KEY_KP_MULTIPLY        332
#define USG_KEY_KP_SUBTRACT        333
#define USG_KEY_KP_ADD             334
#define USG_KEY_KP_ENTER           335
#define USG_KEY_KP_EQUAL           336
#define USG_KEY_LEFT_SHIFT         340
#define USG_KEY_LEFT_CONTROL       341
#define USG_KEY_LEFT_ALT           342
#define USG_KEY_LEFT_SUPER         343
#define USG_KEY_RIGHT_SHIFT        344
#define USG_KEY_RIGHT_CONTROL      345
#define USG_KEY_RIGHT_ALT          346
#define USG_KEY_RIGHT_SUPER        347
#define USG_KEY_MENU               348

#define USG_MOUSE_BUTTON_1         0
#define USG_MOUSE_BUTTON_2         1
#define USG_MOUSE_BUTTON_3         2
#define USG_MOUSE_BUTTON_4         3
#define USG_MOUSE_BUTTON_5         4
#define USG_MOUSE_BUTTON_6         5
#define USG_MOUSE_BUTTON_7         6
#define USG_MOUSE_BUTTON_8         7
#define USG_MOUSE_BUTTON_LAST      USG_MOUSE_BUTTON_8
#define USG_MOUSE_BUTTON_LEFT      USG_MOUSE_BUTTON_1
#define USG_MOUSE_BUTTON_RIGHT     USG_MOUSE_BUTTON_2
#define USG_MOUSE_BUTTON_MIDDLE    USG_MOUSE_BUTTON_3