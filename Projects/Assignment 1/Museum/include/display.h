#ifndef DISPLAY_H
#define DISPLAY_H

// FPS details  
#define FPS            60
#define FPS_SEC        1000
#define TIMER_DELAY    FPS_SEC / FPS
#define QUATER_SEC     250
#define TITLE_SPACE    100
#define THREADS_BOX_COLL      4  // Windows: 1
#define THREADS_BOX_BOX_COLL  2  // Windows: 1


// globals --change

void moveBack(float angle);
void moveForward(float angle);
void lookRotation(float angle, bool _view);

// top camera
void topBottomForward(void);
void topBottomBack(void);
void topBottomLeft(void);
void topBottomRight(void);
void getView(bool _view);
void zoom(double _zoom);

void spacePressed(bool _state);
// void ballBounce(int value);
void boxCube(int value);
void collBox(int value);

char *getPath(char *path);
void loadTexture(void);	
void display(void);


#endif // DISPLAY_H