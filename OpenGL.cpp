#include <stdio.h>
#include <time.h>
#include <windows.h>//Get screen size
#include <glut.h>
#include <string>

//Snake directions values
#define UP 1
#define Down 2
#define LEFT 3
#define RIGHT 4

// Status Variables
GLint   lvl = 1;
GLint   points = 0;
GLint   size = 0;
GLbyte  gameOver = true;
GLbyte  EnableLight = true;

// Snake Variables
GLint   bodyPos[2][50] = { {} };
GLint   _x = 5;
GLint   _z = 10;
GLint   _oldX[2] = {};
GLint   _oldZ[2] = {};
GLbyte  direction = 0;

// Food Variables
GLint   _bx = 0;
GLint   _bz = 0;

// Screen variables
GLint   _w = 800;
GLint   _h = 550;
GLint   _Giw = 0;
GLint   _Gih = 0;
GLint   _Gfw = 150;
GLint   _Gfh = 150;

//Variables for the Camera Angle
static GLfloat view_rotx = 45.0F;
static GLfloat view_roty = 0.0F;
static GLfloat view_rotz = 0.0F;

static GLfloat headRotation = 90.0F;
static GLfloat zoom = -300.0f;

/// <summary>
/// This method is used to configure the initial light
/// </summary>
void initLight()
{
    //Add ambient light
    GLfloat ambientColor[] = { 0.3f, 0.4f, 0.8f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
    //Add positioned light
    GLfloat lightColor0[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat lightPos0[] = { 4.0f, 0.0f, 8.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    //Add directed light
    GLfloat lightColor1[] = { 0.5f, 0.2f, 0.2f, 1.0f };
    //Coming from the direction (-1, 0.5, 0.5)
    GLfloat lightPos1[] = { -1.0f, 0.5f, 0.5f, 0.0f };
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
}

/// <summary>
/// initialize the first configurations
/// </summary>
/// <param name="">void</param>
void Initialize(void)
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.4f, 0.8f, 1.0f, 1.0f); //Change the background to sky blue
    if (EnableLight) {
        glEnable(GL_COLOR_MATERIAL); //Enable color
        glEnable(GL_LIGHTING); //Enable lighting
        glEnable(GL_LIGHT0); //Enable light #0
        glEnable(GL_LIGHT1); //Enable light #1
        glEnable(GL_NORMALIZE); //Automatically normalize normals
    }
}

/// <summary>
/// Resize the window configurations
/// </summary>
/// <param name="w">int value</param>
/// <param name="h">int value</param>
void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1, 800.0);
}

/// <summary>
/// ManipulateViewAngle will rotate the object according to the Angles
/// </summary>
void ManipulateViewAngle() {
    glRotatef(view_rotx, 1.0, 0.0, 0.0);//Rotate Arround X axis
    glRotatef(view_roty, 0.0, 1.0, 0.0);//Rotate Arround Y axis
    glRotatef(view_rotz, 0.0, 0.0, 1.0);//Rotate Arround Z axis
}

/// <summary>
/// Reset Function will reset the snake size and location...
/// </summary>
void Reset() {
    _x = 5;
    _z = 10;
    direction = 0;
    lvl = 1;
    points = 0;
    size = 0;
    gameOver = false;
    view_rotx = 45.0F;
    view_roty = 0.0F;
    view_rotz = 0.0F;
    headRotation = 90.0F;
}

/// <summary>
/// Display a welcome screen method
/// </summary>
/// <param name="message">string value</param>
void Write(std::string message) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, 500.0, 0.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glColor3f(1.0, 0.0, 0.0);
    glRasterPos2i(100, 100);
    for (int i = 0; i < message.size(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, message[i]);
    }
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

/// <summary>
/// WelcomeScreen used to call another method used Write...
/// </summary>
void WelcomeScreen() { 
    //calling Write welcome message
    Write("Welcome To Snake Game Press 's' to Start 'e' to end ");
}


/// <summary>
/// This method is used to draw a food "circle"
/// </summary>
void DrawFood()
{
    //Draw the Sphere representing the Food for the snake
    glPushMatrix();
    ManipulateViewAngle();
    glTranslatef(_bx, -10.0, _bz);
    glColor3f(1, 1, 0);
    glScalef(0.5, 0.5, 0.5);
    glutSolidSphere(7, 20, 60);
    glPopMatrix();
}

/// <summary>
/// Generates Random Numbers for the location of the food that the snake will eat
/// </summary>
/// <param name="high">int value</param>
/// <param name="low">int value</param>
/// <returns></returns>
int RandomNumber(int high, int low)
{
    return (rand() % (high - low)) + low;
}

/// <summary>
/// This method is used to render the text for GameStatus to display scores and levels
/// </summary>
/// <param name="lvl">int value</param>
/// <param name="points">int value</param>
void renderText(int lvl, int points) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-400, 600, -100, 40, -1, 2);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glColor3f(1.0f, 0.0f, 1.0f);
    glRasterPos2i(10, 10);
    char buf[256];
    sprintf_s(buf, "Level: %d Points: %d", lvl, points);
    for (const char* c = buf; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
    }
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

/// <summary>
/// This method is used to show levels and scores for snake
/// </summary>
void GameStatus() {
    glColor3f(0.8, 0.2, 0);
    glRasterPos2f(5, 20);
    // Print the status of the game on the screen
    glColor3f(0, 0, 0);
    glRasterPos2f(5, 10);
    renderText(lvl, points);
}

/// <summary>
///Generate the New Food that the snake will eat 
/// </summary>
void newFood() {
    time_t seconds;
    time(&seconds);
    srand((unsigned int)seconds);
    _bx = RandomNumber(_Gfw , _Giw);
    _bz = RandomNumber(_Gfh , _Gih);
}


/// <summary>
/// This method is used to draw a snake with it's properties like color , scale , translate ,etc...
/// </summary>
void DrawSnake() {
    int  i;
    //Drawing the head & the plane
    glPushMatrix();
    ManipulateViewAngle();
    //This will draw the plane that the snake will run on.
    glPushMatrix();
    glColor3f(1, 0.7, 0.2);
    glColor3f(0.0, 0.6, 0.2);
    glTranslatef(75.0, -16.0, 75.0);
    glScalef(155, 5.0, 155);
    glutSolidCube(1);
    glPopMatrix();
    //Here we will draw the Head of the snake
    glColor3f(1, 0, 0);//Color it red
    glTranslatef(_x, -10.0, _z);//Give it the location according to _x & _z
    glScalef(0.5, 0.5, 0.5);
    glutSolidSphere(10, 20, 20);//Draw the head as a sphere a litle bit bigger than the body spheres
    glRotatef(headRotation, 0.0, 1.0, 0.0);
    glColor3f(1, 0, 0);
    glTranslatef(0, 0.0, 6.0);
    glScalef(0.8, 1.0, 1.0);
    glutSolidCone(10, 10, 20, 20);
    glColor3f(1, 1, 1);
    glTranslatef(-4.0, 10.0, 0.0);
    glScalef(0.3, 0.3, 0.3);
    glutSolidSphere(10, 20, 20);
    glTranslatef(26.0, 0.0, 0.0);
    glutSolidSphere(10, 20, 20);
    glPopMatrix();

    //Drawing the body
    for (i = 0; i < size; i++) {//Loop throw the size and draw spheres representing the body
        glPushMatrix();
        ManipulateViewAngle();
        glTranslatef(bodyPos[0][i], -10.0, bodyPos[1][i]);//this will locate the spheres
        glColor3f(1, 0, 0);//Color Red
        glScalef(0.5, 0.5, 0.5);
        glutSolidSphere(7, 20, 20);
        glPopMatrix();
    }
}

/// <summary>
/// This method Will Check for Collision
/// </summary>
/// <returns>boolean value</returns>
bool collision() {
    for (int i = 0; i < size; i++) {
        int bodyX = bodyPos[0][i];
        int bodyZ = bodyPos[1][i];

        // Check if the player's position is within a bounding box around each body segment
        if (_x >= bodyX - 5 && _x <= bodyX + 5 &&
            _z >= bodyZ - 5 && _z <= bodyZ + 5) {
            return true;
        }
    }
    return false;
}


/// <summary>
/// this function will move snake according to directions
/// taken from keyboard hit and check collision and create new 
/// food and glutTimerFunc to change speed in limited status
/// </summary>
/// <param name="value">is int</param>
void Run(int value) {
    int i;
    _oldX[1] = _x;
    _oldZ[1] = _z;
    switch (direction) {
    case RIGHT:
        headRotation = 90;
        _x += 6;
        if (_x > _Gfw - 2) _x = _Giw - 1;//This will check if the snake is going into the border so it will appear on the other side
        break;
    case LEFT:
        headRotation = -90;
        _x -= 6;
        if (_x < 0) _x = _Gfw - 2;//This will check if the snake is going into the border so it will appear on the other side
        break;
    case UP:
        headRotation = 0;
        _z += 6;
        if (_z > _Gfh - 2) _z = _Gih - 1;//This will check if the snake is going into the border so it will appear on the other side
        break;
    case Down:
        headRotation = 180;
        _z -= 6;
        if (_z < 2) _z = _Gfh - 2;//This will check if the snake is going into the border so it will appear on the other side
        break;
    }
    //Checks for Collisoin if yes Game Over
    if (collision()) {
        WelcomeScreen();
        gameOver = true;
    }
    //Checks if the snake ate the food (check the X and Y)
    // If yes it will increase the points & the size of the snake & create a new food
    if ((_x == _bx && _z == _bz) ||
        ((_x >= _bx) && (_x <= _bx + 4) && (_z >= _bz) && (_z <= _bz + 4)) ||
        ((_x <= _bx) && (_x >= _bx - 4) && (_z <= _bz) && (_z >= _bz - 4)) ||
        ((_x <= _bx) && (_x >= _bx - 4) && (_z >= _bz) && (_z <= _bz + 4)) ||
        ((_x >= _bx) && (_x <= _bx + 4) && (_z <= _bz) && (_z >= _bz - 4))) {
        points++;
        if (points < 100) size++;
        if (points % 5 == 0 && lvl < 15) lvl++;
        newFood();
    }

    for (i = 0; i < size; i++) {// Save the positions of the body parts
        _oldX[0] = _oldX[1];
        _oldZ[0] = _oldZ[1];
        _oldX[1] = bodyPos[0][i];
        _oldZ[1] = bodyPos[1][i];
        bodyPos[0][i] = _oldX[0];
        bodyPos[1][i] = _oldZ[0];
    }
    
    if (lvl == 1) {//Conditions for increase the speed dynamically
        glutTimerFunc(140, Run, 0);
    }
    if(lvl>=2 && lvl<3) {
        glutTimerFunc(115, Run, 1);
    }
    if(lvl>=3 &&lvl<4) {
        glutTimerFunc(80, Run, 2);
    }
    if (lvl >= 4) {
        glutTimerFunc(60, Run, 3);

    }
    //Set the Timer
}


void Display(void) {//Draw Function
    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (EnableLight) initLight();

    glTranslatef(-60.0, 40.0,zoom);
    //Check if the value of the Flag "Game Over is not True it will continue the game"
    if (!gameOver) {
        GameStatus();
        DrawFood();
        DrawSnake();
    }
    else
     WelcomeScreen();
    // Updates the screen
    glutPostRedisplay();
    glutSwapBuffers();
}


/// <summary>
/// This method for directions left & right & up & down...
/// </summary>
/// <param name="key">is char</param>
/// <param name="x">is int</param>
/// <param name="y">is int</param>
void Special(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_RIGHT:
        if (direction != LEFT)
            direction = RIGHT;
        break;
    case GLUT_KEY_LEFT:
        if (direction != RIGHT)
            direction = LEFT;
        break;
    case GLUT_KEY_UP:
        if (direction != UP)
            direction = Down;
        break;
    case GLUT_KEY_DOWN:
        if (direction != Down)
            direction = UP;
        break;
    }
}

/// <summary>
/// This method is used to take an input directions
/// </summary>
/// <param name="key">is char s for start a game if 's' or 'S'</param>
/// <param name="x">is int value</param>
/// <param name="y">is int value</param>
void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 'S':
    case 's':
        Reset();
        glutPostRedisplay();
        break;
    case 'e'://E or e to Exit
    case 'E':
        exit(0);
        break;
    default:
        break;
    }
}


int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
int screenHeight = glutGet(GLUT_SCREEN_HEIGHT); 
int windowWidth = 800; // Desired window width
int windowHeight = 600;


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(_w, _h);
    int windowPosX = (screenWidth - windowWidth) / 2;
    int windowPosY = (screenHeight - windowHeight) / 2;
    // Set the window position
    glutInitWindowPosition(windowPosX, windowPosY);
    glutCreateWindow("Snake Game");
    Initialize();
    glutSpecialFunc(Special);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(Display);
    glutReshapeFunc(resize);
    newFood();
    Run(0);
    glutMainLoop();
}
