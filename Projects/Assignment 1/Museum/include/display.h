#ifndef DISPLAY_H
#define DISPLAY_H

// FPS details  
#define FPS            60
#define FPS_SEC        1000
#define TIMER_DELAY    FPS_SEC / FPS
#define QUATER_SEC     250
#define TITLE_SPACE    100


// globals --change


void moveBack(float angle);
void moveForward(float angle);
void lookRotation(float angle);


char *getPath(char *path);
void loadTexture(void);	
void display(void);


#endif // DISPLAY_H