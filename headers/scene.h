#ifndef _SCENE
#define _MARATON

#define SPHERE_SLICES 142
#define SPHERE_STACKS 216

void draw_scene();
void set_scene();

/* Koristi se za iscrtavanje segmenata trake. */
enum
{
    OFFSET_PREV = -1,
    OFFSET_CURR,
    OFFSET_NEXT
};

#endif
