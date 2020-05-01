#pragma once

//These Key Codes Are Compatible With OpenGL And Actually Taken From glfw3.h One Can Use Them Interchangably With OpenGL KeyCodes..

/* The unknown key */
#define ERMINE_KEY_UNKNOWN            -1

/* Printable keys */
#define ERMINE_KEY_SPACE              32
#define ERMINE_KEY_APOSTROPHE         39  /* ' */
#define ERMINE_KEY_COMMA              44  /* , */
#define ERMINE_KEY_MINUS              45  /* - */
#define ERMINE_KEY_PERIOD             46  /* . */
#define ERMINE_KEY_SLASH              47  /* / */
#define ERMINE_KEY_0                  48
#define ERMINE_KEY_1                  49
#define ERMINE_KEY_2                  50
#define ERMINE_KEY_3                  51
#define ERMINE_KEY_4                  52
#define ERMINE_KEY_5                  53
#define ERMINE_KEY_6                  54
#define ERMINE_KEY_7                  55
#define ERMINE_KEY_8                  56
#define ERMINE_KEY_9                  57
#define ERMINE_KEY_SEMICOLON          59  /* ; */
#define ERMINE_KEY_EQUAL              61  /* = */
#define ERMINE_KEY_A                  65
#define ERMINE_KEY_B                  66
#define ERMINE_KEY_C                  67
#define ERMINE_KEY_D                  68
#define ERMINE_KEY_E                  69
#define ERMINE_KEY_F                  70
#define ERMINE_KEY_G                  71
#define ERMINE_KEY_H                  72
#define ERMINE_KEY_I                  73
#define ERMINE_KEY_J                  74
#define ERMINE_KEY_K                  75
#define ERMINE_KEY_L                  76
#define ERMINE_KEY_M                  77
#define ERMINE_KEY_N                  78
#define ERMINE_KEY_O                  79
#define ERMINE_KEY_P                  80
#define ERMINE_KEY_Q                  81
#define ERMINE_KEY_R                  82
#define ERMINE_KEY_S                  83
#define ERMINE_KEY_T                  84
#define ERMINE_KEY_U                  85
#define ERMINE_KEY_V                  86
#define ERMINE_KEY_W                  87
#define ERMINE_KEY_X                  88
#define ERMINE_KEY_Y                  89
#define ERMINE_KEY_Z                  90
#define ERMINE_KEY_LEFT_BRACKET       91  /* [ */
#define ERMINE_KEY_BACKSLASH          92  /* \ */
#define ERMINE_KEY_RIGHT_BRACKET      93  /* ] */
#define ERMINE_KEY_GRAVE_ACCENT       96  /* ` */
#define ERMINE_KEY_WORLD_1            161 /* non-US #1 */
#define ERMINE_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define ERMINE_KEY_ESCAPE             256
#define ERMINE_KEY_ENTER              257
#define ERMINE_KEY_TAB                258
#define ERMINE_KEY_BACKSPACE          259
#define ERMINE_KEY_INSERT             260
#define ERMINE_KEY_DELETE             261
#define ERMINE_KEY_RIGHT              262
#define ERMINE_KEY_LEFT               263
#define ERMINE_KEY_DOWN               264
#define ERMINE_KEY_UP                 265
#define ERMINE_KEY_PAGE_UP            266
#define ERMINE_KEY_PAGE_DOWN          267
#define ERMINE_KEY_HOME               268
#define ERMINE_KEY_END                269
#define ERMINE_KEY_CAPS_LOCK          280
#define ERMINE_KEY_SCROLL_LOCK        281
#define ERMINE_KEY_NUM_LOCK           282
#define ERMINE_KEY_PRINT_SCREEN       283
#define ERMINE_KEY_PAUSE              284
#define ERMINE_KEY_F1                 290
#define ERMINE_KEY_F2                 291
#define ERMINE_KEY_F3                 292
#define ERMINE_KEY_F4                 293
#define ERMINE_KEY_F5                 294
#define ERMINE_KEY_F6                 295
#define ERMINE_KEY_F7                 296
#define ERMINE_KEY_F8                 297
#define ERMINE_KEY_F9                 298
#define ERMINE_KEY_F10                299
#define ERMINE_KEY_F11                300
#define ERMINE_KEY_F12                301
#define ERMINE_KEY_F13                302
#define ERMINE_KEY_F14                303
#define ERMINE_KEY_F15                304
#define ERMINE_KEY_F16                305
#define ERMINE_KEY_F17                306
#define ERMINE_KEY_F18                307
#define ERMINE_KEY_F19                308
#define ERMINE_KEY_F20                309
#define ERMINE_KEY_F21                310
#define ERMINE_KEY_F22                311
#define ERMINE_KEY_F23                312
#define ERMINE_KEY_F24                313
#define ERMINE_KEY_F25                314
#define ERMINE_KEY_KP_0               320
#define ERMINE_KEY_KP_1               321
#define ERMINE_KEY_KP_2               322
#define ERMINE_KEY_KP_3               323
#define ERMINE_KEY_KP_4               324
#define ERMINE_KEY_KP_5               325
#define ERMINE_KEY_KP_6               326
#define ERMINE_KEY_KP_7               327
#define ERMINE_KEY_KP_8               328
#define ERMINE_KEY_KP_9               329
#define ERMINE_KEY_KP_DECIMAL         330
#define ERMINE_KEY_KP_DIVIDE          331
#define ERMINE_KEY_KP_MULTIPLY        332
#define ERMINE_KEY_KP_SUBTRACT        333
#define ERMINE_KEY_KP_ADD             334
#define ERMINE_KEY_KP_ENTER           335
#define ERMINE_KEY_KP_EQUAL           336
#define ERMINE_KEY_LEFT_SHIFT         340
#define ERMINE_KEY_LEFT_CONTROL       341
#define ERMINE_KEY_LEFT_ALT           342
#define ERMINE_KEY_LEFT_SUPER         343
#define ERMINE_KEY_RIGHT_SHIFT        344
#define ERMINE_KEY_RIGHT_CONTROL      345
#define ERMINE_KEY_RIGHT_ALT          346
#define ERMINE_KEY_RIGHT_SUPER        347
#define ERMINE_KEY_MENU               348

#define ERMINE_KEY_LAST               GLFW_KEY_MENU


//Mouse Keys
#define ERMINE_MOUSE_BUTTON_1         0
#define ERMINE_MOUSE_BUTTON_2         1
#define ERMINE_MOUSE_BUTTON_3         2
#define ERMINE_MOUSE_BUTTON_4         3
#define ERMINE_MOUSE_BUTTON_5         4
#define ERMINE_MOUSE_BUTTON_6         5
#define ERMINE_MOUSE_BUTTON_7         6
#define ERMINE_MOUSE_BUTTON_8         7
#define ERMINE_MOUSE_BUTTON_LAST      ERMINE_MOUSE_BUTTON_8
#define ERMINE_MOUSE_BUTTON_LEFT      ERMINE_MOUSE_BUTTON_1
#define ERMINE_MOUSE_BUTTON_RIGHT     ERMINE_MOUSE_BUTTON_2
#define ERMINE_MOUSE_BUTTON_MIDDLE    ERMINE_MOUSE_BUTTON_3

#define ERMINE_RELEASE                0
#define ERMINE_PRESS                  1
#define ERMINE_REPEAT                 2