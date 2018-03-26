#ifndef _MARATON
#define _MARATON

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include <GL/freeglut.h>

#include "timer.h"
#include "state.h"
#include "scene.h"
#include "track.h"
#include "game.h"

#define M_PI  3.14159265359
#define M_SQRT_2 1.41421356237

extern GLfloat avatar_h;
extern GLfloat field_w, field_h;

#define UNUSED(arg) ((void)arg)
#define UNUSED_2(arg1, arg2) (UNUSED(arg1), UNUSED(arg2))
#define UNUSED_3(arg1, arg2, arg3) (UNUSED(arg1), UNUSED(arg2), UNUSED(arg3))

#endif
