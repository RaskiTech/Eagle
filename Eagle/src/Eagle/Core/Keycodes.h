#pragma once

#ifdef EAGLE_PLATFORM_WINDOWS
// Same as GLFW uses. From glfw.h
#define EGL_KEY_SPACE              32
#define EGL_KEY_APOSTROPHE         39  /* ' */
#define EGL_KEY_COMMA              44  /* , */
#define EGL_KEY_MINUS              45  /* - */
#define EGL_KEY_PERIOD             46  /* . */
#define EGL_KEY_SLASH              47  /* / */
#define EGL_KEY_0                  48
#define EGL_KEY_1                  49
#define EGL_KEY_2                  50
#define EGL_KEY_3                  51
#define EGL_KEY_4                  52
#define EGL_KEY_5                  53
#define EGL_KEY_6                  54
#define EGL_KEY_7                  55
#define EGL_KEY_8                  56
#define EGL_KEY_9                  57
#define EGL_KEY_SEMICOLON          59  /* ; */
#define EGL_KEY_EQUAL              61  /* = */
#define EGL_KEY_A                  65
#define EGL_KEY_B                  66
#define EGL_KEY_C                  67
#define EGL_KEY_D                  68
#define EGL_KEY_E                  69
#define EGL_KEY_F                  70
#define EGL_KEY_G                  71
#define EGL_KEY_H                  72
#define EGL_KEY_I                  73
#define EGL_KEY_J                  74
#define EGL_KEY_K                  75
#define EGL_KEY_L                  76
#define EGL_KEY_M                  77
#define EGL_KEY_N                  78
#define EGL_KEY_O                  79
#define EGL_KEY_P                  80
#define EGL_KEY_Q                  81
#define EGL_KEY_R                  82
#define EGL_KEY_S                  83
#define EGL_KEY_T                  84
#define EGL_KEY_U                  85
#define EGL_KEY_V                  86
#define EGL_KEY_W                  87
#define EGL_KEY_X                  88
#define EGL_KEY_Y                  89
#define EGL_KEY_Z                  90
#define EGL_KEY_LEFT_BRACKET       91  /* [ */
#define EGL_KEY_BACKSLASH          92  /* \ */
#define EGL_KEY_RIGHT_BRACKET      93  /* ] */
#define EGL_KEY_GRAVE_ACCENT       96  /* ` */
#define EGL_KEY_WORLD_1            161 /* non-US #1 */
#define EGL_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define EGL_KEY_ESCAPE             256
#define EGL_KEY_ENTER              257
#define EGL_KEY_TAB                258
#define EGL_KEY_BACKSPACE          259
#define EGL_KEY_INSERT             260
#define EGL_KEY_DELETE             261
#define EGL_KEY_RIGHT              262
#define EGL_KEY_LEFT               263
#define EGL_KEY_DOWN               264
#define EGL_KEY_UP                 265
#define EGL_KEY_PAGE_UP            266
#define EGL_KEY_PAGE_DOWN          267
#define EGL_KEY_HOME               268
#define EGL_KEY_END                269
#define EGL_KEY_CAPS_LOCK          280
#define EGL_KEY_SCROLL_LOCK        281
#define EGL_KEY_NUM_LOCK           282
#define EGL_KEY_PRINT_SCREEN       283
#define EGL_KEY_PAUSE              284
#define EGL_KEY_F1                 290
#define EGL_KEY_F2                 291
#define EGL_KEY_F3                 292
#define EGL_KEY_F4                 293
#define EGL_KEY_F5                 294
#define EGL_KEY_F6                 295
#define EGL_KEY_F7                 296
#define EGL_KEY_F8                 297
#define EGL_KEY_F9                 298
#define EGL_KEY_F10                299
#define EGL_KEY_F11                300
#define EGL_KEY_F12                301
#define EGL_KEY_F13                302
#define EGL_KEY_F14                303
#define EGL_KEY_F15                304
#define EGL_KEY_F16                305
#define EGL_KEY_F17                306
#define EGL_KEY_F18                307
#define EGL_KEY_F19                308
#define EGL_KEY_F20                309
#define EGL_KEY_F21                310
#define EGL_KEY_F22                311
#define EGL_KEY_F23                312
#define EGL_KEY_F24                313
#define EGL_KEY_F25                314
#define EGL_KEY_KP_0               320
#define EGL_KEY_KP_1               321
#define EGL_KEY_KP_2               322
#define EGL_KEY_KP_3               323
#define EGL_KEY_KP_4               324
#define EGL_KEY_KP_5               325
#define EGL_KEY_KP_6               326
#define EGL_KEY_KP_7               327
#define EGL_KEY_KP_8               328
#define EGL_KEY_KP_9               329
#define EGL_KEY_KP_DECIMAL         330
#define EGL_KEY_KP_DIVIDE          331
#define EGL_KEY_KP_MULTIPLY        332
#define EGL_KEY_KP_SUBTRACT        333
#define EGL_KEY_KP_ADD             334
#define EGL_KEY_KP_ENTER           335
#define EGL_KEY_KP_EQUAL           336
#define EGL_KEY_LEFT_SHIFT         340
#define EGL_KEY_LEFT_CONTROL       341
#define EGL_KEY_LEFT_ALT           342
#define EGL_KEY_LEFT_SUPER         343
#define EGL_KEY_LEFT_WINDOWS       EGL_KEY_LEFT_SUPER
#define EGL_KEY_LEFT_CMD		   EGL_KEY_LEFT_SUPER
#define EGL_KEY_RIGHT_SHIFT        344
#define EGL_KEY_RIGHT_CONTROL      345
#define EGL_KEY_RIGHT_ALT          346
#define EGL_KEY_RIGHT_SUPER        347
#define EGL_KEY_RIGHT_WINDOWS      EGL_KEY_RIGHT_SUPER
#define EGL_KEY_RIGHT_CMD		   EGL_KEY_RIGHT_SUPER
#define EGL_KEY_MENU               348

//#define EGL_KEY_LAST               EGL_KEY_MENU

//#define EGL_MOD_SHIFT           0x0001
//#define EGL_MOD_CONTROL         0x0002
//#define EGL_MOD_ALT             0x0004
//#define EGL_MOD_SUPER           0x0008

#define EGL_MOUSE_BUTTON_1         0
#define EGL_MOUSE_BUTTON_2         1
#define EGL_MOUSE_BUTTON_3         2
#define EGL_MOUSE_BUTTON_4         3
#define EGL_MOUSE_BUTTON_5         4
#define EGL_MOUSE_BUTTON_6         5
#define EGL_MOUSE_BUTTON_7         6
#define EGL_MOUSE_BUTTON_8         7
#define EGL_MOUSE_BUTTON_LAST      EGL_MOUSE_BUTTON_8
#define EGL_MOUSE_BUTTON_LEFT      EGL_MOUSE_BUTTON_1
#define EGL_MOUSE_BUTTON_RIGHT     EGL_MOUSE_BUTTON_2
#define EGL_MOUSE_BUTTON_MIDDLE    EGL_MOUSE_BUTTON_3

#endif