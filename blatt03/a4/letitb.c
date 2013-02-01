#include "makros.h"

#ifdef WRITEINC
    #undef LIB
    #define LIB write in c
#endif
#define WHEN1 TIME
#define WHEN2 DARKNESS
#define ACTION1 speaking WW
#define ACTION2 ACTION1

#include "strophe.h"
#include "refrain.h"

#undef WHEN1
#define WHEN1 HEARTED
#undef WHEN2
#define WHEN2 PARTED
#undef ACTION1
#define ACTION1 there will be an answer
#undef ACTION2
#define ACTION2 ACTION1 

#include "strophe.h"
#include "refrain.h"

#undef WHEN1
#define WHEN1 CLOUDY 
#undef WHEN2
#define WHEN2 WAKE
#undef ACTION1
#define ACTION1 shine until tomorrow
#undef ACTION2
#define ACTION2 speaking WW 

#include "strophe.h"
#include "refrain.h"
