#ifndef DISPLAY_H
#define DISPLAY_H

// FPS details  
#define FPS            30   // If I change this to 60, why does why the GLU object flicker?
#define FPS_SEC        1000
#define TIMER_DELAY    FPS_SEC / FPS
#define QUATER_SEC     250
#define TITLE_SPACE    100


char *getPath(char *path);
void display(void);


#endif // DISPLAY_H