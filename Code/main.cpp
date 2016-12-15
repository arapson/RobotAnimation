// Avery Rapson and Jacob London
// October 26, 2015


/* ------------- */
/*   Project 2   */
/* ------------- */


/*
 Objective of Program:
 + In OpenGl, create an animated 3D hierarchical object.You must restrict the transformations applied to the object so that its behavior is a reasonable facsimile of its real world behavior; i.e. real elbows can't rotate 360 degrees.
 + Smooth, filled and colored 3D polygons must be used.
 + The figure must contain at least eight separate objects. These eight objects must define at least 4 parent/child relationships
 + A least one positional light must be used (diffuse or ambient).
 + You must define several new GLUT callback functions : GlutIdleFunction, GlutVisibilityFunction. The idle function defines the sequence of animations that the object goes through. The visibilty function defines when the object is visible.
 + You must add a menu to the Glut window environment for your application. Minimally the menu will allow the user to stop/start the animation and quit the application. You must write a function to handle menu selections.
 
 Controls:
 
 + To quit the program: Hit q or "esc"
 + To open screen menu: Right click
 + Screen menu options are as follows:
 + Start - starts animation
 + Stop - stops animation
 + Zoomin - zooms in on the robot
 + Zoomout - zooms out on the robot
 + Speedup - speeds up the animation
 + Slowdown - slows down the animation
 + Reset - resets the animation and zoom to original settings
 + Quit - quits the program
 
 */


#include <math.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>

float trackLocation = 5;

// globals for leg animation
float LeftLegTopX = 0,  LeftLegTopY = 0, LeftLegTopZ = 0;
float LeftLegBottomX = 0,  LeftLegBottomY = 0, LeftLegBottomZ = 0;
float RightLegTopX = 0,  RightLegTopY = 0, RightLegTopZ = 0;
float RightLegBottomX = 0,  RightLegBottomY = 0, RighLegBottomZ = 0;
float RLeftLegTopX = 0,  RLeftLegTopY = 0, RLeftLegTopZ = 0;
float RLeftLegBottomX = 0,  RLeftLegBottomY = 0, RLeftLegBottomZ = 0;
float RRightLegTopX = 0,  RRightLegTopY = 0, RRightLegTopZ = 0;
float RRightLegBottomX = 0,  RRightLegBottomY = 0, RRightLegBottomZ = 0;

// globals for arm animation
float RLeftArmTopX = 0,  RLeftArmTopY = 0, RLeftArmTopZ = 0;
float RLeftArmBottomX = 0,  RLeftArmBottomY = 0, RLeftArmBottomZ = 0;
float RRightArmTopX = 0,  RRightArmTopY = 0, RRightArmTopZ = 0;
float RRightArmBottomX = 0,  RRightArmBottomY = 0, RRightArmBottomZ = 0;

//variables for body animation
float bodyX = 0, bodyY = 0, bodyZ = 0;
float eBrowX = 0, eBrowY = 0;

//side balls animation
float rightballX = 0,  rightballY = 0, rightballZ = 0;
float leftballX = 0,  leftballY = 0, leftballZ = 0;

//booleans for animation logic
bool rightArmReset = true;
bool leftArmReset = true;
bool bodyReset = true;
bool rightLegReset = true;
bool leftLegReset = true;
bool eyeReset = true;


void robot_body();
void robot_legs();
void robot_arms();


// Animation speed globals
float multValue = 1.8, timesValue = 1.48, eyeValue = 1, boostValue = 1, earSpeed = 1;


GLboolean moving = false;

/*
 * Function used to register mouse motion.
 * Pre: correct capture and initialization of variables from mouse();
 * Post: glutPostRedisplay() is called and mouse input is shown on monitor.
 */
void motion(int x, int y);

/*
 * Function used to capture mouse input
 * Pre: user uses mouse, callback registered in main.
 * Post: mouse angle variables are intialized for motion().
 */
void mouse(int button, int state, int x, int y);

static GLfloat lAngle = 0.0, rAngle = 0.0;   /* in degrees */ //Angle variables for mouse input.
static GLfloat lAngle2 = 0.0, rAngle2 = 0.0;   /* in degrees */ //Angle variables for mouse input.
static int lMoving = 0, rMoving = 0,lStartx, lStarty, rStartx, rStarty; //Additional variables for mouse input.


//set up material colors

GLfloat field_diffuse[4] = { 0.5, 1.0, 0.6, 1.0}; //slightly green running surface

GLfloat trunk_diffuse[4] = { 0.5, 0.7, 1.0, 1.0}; //a blue jersey

GLfloat body_diffuse[4] = {0.98, 0.7,0.8, 1.0}; //pink skin tones

GLfloat robot_color[4] = {.6, .7,  .9, 1.0}; //blue-grey robot color

GLfloat black_color[4] = {0, 0, 0, 1}; //just the color black

GLfloat yellow_color[4] ={1,1,0,1}; //yellow color

GLfloat red_color[4] = {1,0,0,1}; //red color

GLfloat robot_shoes[4] = {.1, .1  , .9, 0}; //robot shoes blue color


GLUquadric * cyl; //Cylinder object

/*
 * Init Function
 * Pre: Called from main.
 * Post: Window coordinates are set, background color set to blue, lighting initialized, depth_test enabled.
 */
void myInit(){
    
    glClearColor(.3, .5, .1, 0 );
    glColor3ub(255, 255, 255);
    glOrtho(0, 0, 0, 0, -1, 1);
    
    // cylinder declarations
    cyl = gluNewQuadric();
    gluQuadricNormals(cyl, GLU_SMOOTH);
    
    // lighting code
    GLfloat light_position[] = { 0.0, .9, .9, 0.0 };
    GLfloat light_ambient[] = {1, 1, .85};
    glShadeModel(GL_SMOOTH);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position); //posistion
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient); //red light - -might want to change
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    
}





/*
 * Method called to write text to screen.
 * Pre: RGB color specifiers, (x,y) location of starting point, string to be displayed.
 * Post: Text is displayed to screen with proper color in proper location
 */
void write(int R, int G, int B, int rX, int rY, std::string str){
    glColor3ub(R, G, B);
    glRasterPos2d(rX, rY);
    
    void * font = GLUT_BITMAP_TIMES_ROMAN_10;
    for(std::string::iterator i = str.begin(); i!= str.end(); ++i){
        char c = *i;
        glutBitmapCharacter(font, c);
    }
    
}


// Keyboard function to end program with keys 'esc' and 'q'
void processNormalKeys(unsigned char key, int x, int y)
{
    switch ( key )
    {
        case 'q': // q to quit the program
            
            exit (0);
            break;
        case 27:
            exit(0);
            break;
            
        default:
            break;
    }
    
}


/*
 * Function used to capture mouse input
 * Pre: user uses mouse, callback registered in main.
 * Post: mouse angle variables are intialized for motion().
 */
void mouse(int button, int state, int x, int y) {
    /* Move the scene with the left mouse button. */
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            lMoving = 1;
            lStartx = x;
            lStarty = y;
        }
        if (state == GLUT_UP) {
            lMoving = 0;
        }
    }
    
}


/*
 * Function used to register mouse motion.
 * Pre: correct capture and initialization of variables from mouse();
 * Post: glutPostRedisplay() is called and mouse input is shown on monitor.
 */
void motion(int x, int y)
{
    if (lMoving) {
        lAngle = lAngle + (x - lStartx);
        lAngle2 = lAngle2 + (y - lStarty);
        lStartx = x;
        lStarty = y;
        glutPostRedisplay();
    }
    else if (rMoving) {
        rAngle = rAngle + (x - rStartx);
        rAngle2 = lAngle2 + (y - rStarty);
        rStartx = x;
        rStarty = y;
        glutPostRedisplay();
    }
}


//display function
void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    
    //Trackball
    glPushMatrix(); //Trackball Push
    
    
    //LEFT MOUSE
    glTranslated(0, 0, -trackLocation);
    glRotatef(lAngle2, 1.0, 0.0, 0.0);
    glRotatef(lAngle, 0.0, 1.0, 0.0);
    glTranslated(0, 0, trackLocation);
    
    
    
    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, field_diffuse); //define color of next object (and all others without definition)
    //glMaterialfv(GL_BACK,GL_DIFFUSE, trunk_diffuse);
    glColor3ub(0, 0, 0);
    glTranslated(0, 0, -trackLocation);
    glRotatef(90, 0, 0, 0);
    //gluCylinder(cyl, .1, .1, 2.5, 32, 32);x    //glutSolidSphere(.2, 16, 16);
    glPopMatrix();
    
    
    glPushMatrix();
    glTranslated(0, 0, -trackLocation);
    
    //draw the object, specify light color
    glTranslated(bodyX, bodyY, bodyZ);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, robot_color);
    robot_body();
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, robot_color);
    robot_legs();
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, robot_color);
    robot_arms();
    
    glPopMatrix();
    
    
    //Trackball Pop
    glPopMatrix();
    //start = true;
    glutSwapBuffers();
}

/*
 * Function used to draw the body of the robot
 * Pre: Initialize globals
 * Post: Body is drawn */

void robot_body(){
    glPushMatrix();
    
    glPushMatrix();
    glutSolidCube(1.25);
    glPopMatrix();
    
    //neck
    glPushMatrix();
    glTranslated(0, .65, 0);
    glScaled(1.5, 1.5, 1.5);
    glutSolidSphere(.24, 16, 16);
    glPopMatrix();
    
    
    // Face
    glPushMatrix();
    glTranslated(0, 1.15, 0);
    glutSolidCube(.77);
    glPopMatrix();
    
    //hat base
    glPushMatrix();
    glTranslated(0, 1.5,0);
    glRotated(-90, 0, 0, 0);
    glutSolidCone(.38, .38, 32, 32);
    glPopMatrix();
    
    //top ball
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, red_color);
    glPushMatrix();
    glTranslated(0, 2, 0);
    glutSolidSphere(.095, 32, 32);
    glPopMatrix();
    
    
    //top hat stick
    glPushMatrix();
    glTranslated(0, 2, 0);
    glRotatef(90, 0, 0, 0);
    gluCylinder(cyl, .035, .035, .2, 32, 32);
    glPopMatrix();
    
    // Right Ball animation Push
    
    glPushMatrix();
    
    glTranslated(.525, 1.25, 0);
    glRotatef(50 + rightballX, 1, 0, 0);
    glRotatef(0 + rightballY, 0, 1, 0);
    glRotatef(0 + rightballZ, 0, 0, 1);
    glTranslated(-.525, -1.25, 0);
    
    //side right ear stick
    glPushMatrix();
    glTranslated(.525, 1.25, 0);
    glRotatef(-90, 45, -45, 0);
    gluCylinder(cyl, .035, .035, .22, 32, 32);
    glPopMatrix();
    
    
    
    //right ear ball
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, red_color);
    glPushMatrix();
    glTranslated(.715, 1.45, 0);
    glutSolidSphere(.07, 32, 32);
    glPopMatrix();
    
    
    //Right Ball animation pop
    glPopMatrix();
    
    // Right Ball animation Push
    
    //searchme
    glPushMatrix();
    
    glTranslated(.525, 1.25, 0);
    glRotatef(50 + leftballX, 1, 0, 0);
    glRotatef(0 + leftballY, 0, 1, 0);
    glRotatef(0 + leftballZ, 0, 0, 1);
    glTranslated(-.525, -1.25, 0);
    
    
    // side left ear stick
    glPushMatrix();
    glTranslated(-.525, 1.25, 0);
    glRotatef(-90, 45, 45, 0);
    gluCylinder(cyl, .035, .035, .22, 32, 32);
    glPopMatrix();
    
    //left ear ball
    glPushMatrix();
    glTranslated(-.715, 1.45, 0);
    glutSolidSphere(.07, 32, 32);
    glPopMatrix();
    
    
    //Left Ball animation pop
    glPopMatrix();
    
    
    //right ear cone
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, robot_color);
    glPushMatrix();
    glTranslated(.38, 1.25,0);
    glRotated(-90, 0, -90, 0);
    glutSolidCone(.15, .2, 32, 32);
    glPopMatrix();
    
    //left ear cone
    glPushMatrix();
    glTranslated(-.38, 1.25,0);
    glRotated(-90, 0, 90, 0);
    glutSolidCone(.15, .2, 32, 32);
    glPopMatrix();
    
    
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, red_color);
    
    //Right Eye
    glPushMatrix();
    glTranslated(.2, 1.25, .4);
    glutSolidSphere(.095, 32, 32);
    glPopMatrix();
    
    //Left Eye
    glPushMatrix();
    glTranslated(-0.2, 1.25, .4);
    glutSolidSphere(.095, 32, 32);
    glPopMatrix();
    
    
    /*Eye Brows*/
    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, yellow_color);
    
    glTranslated(0, eBrowY, 0);
    //Right Eyebrow
    glPushMatrix();
    glTranslated(.25, 1.42, .4);
    glRotated(-20, 0, 0, 1);
    glScaled(2.2, .8, 1);
    glutSolidCube(.06);
    glPopMatrix();
    
    
    //Left Eyebrow
    glPushMatrix();
    glTranslated(-.25, 1.42, .4);
    glRotated(20, 0, 0, 1);
    glScaled(2.2, .8, 1);
    glutSolidCube(.06);
    glPopMatrix();
    
    glPopMatrix();
    
    /*Mouth*/
    glPushMatrix();
    
    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, black_color);
    glTranslated(0, 1, .4);
    glScaled(5.5, 1, 1);
    glutSolidCube(.1);
    glPopMatrix();
    
    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, yellow_color);
    glTranslated(.22, 1, .41);
    glutSolidCube(.11);
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(.07, 1, .41);
    glutSolidCube(.11);
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(-.07, 1, .41);
    glutSolidCube(.11);
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(-.22, 1, .41);
    glutSolidCube(.11);
    glPopMatrix();
    
    
    glPopMatrix();
    
    
    
    
    glPopMatrix();
}


/*
 * Function used to draw the arms of the robot
 * Pre: Initialize globals
 * Post: Arms are drawn */
void robot_arms(){
    
    /*Left H*/
    glPushMatrix();
    
    glTranslated(-.65, .3, 0);
    glRotated(RLeftArmTopX, 1, 0, 0);
    glRotated(RLeftArmTopY, 0, 1, 0);
    glRotated(RLeftArmTopZ, 0, 0, 1);
    glTranslated(.65, -.3, 0);
    
    //top left joint
    glPushMatrix();
    glTranslatef(-.65, .3, 0);
    glutSolidSphere(.25 ,20 ,20);
    glPopMatrix();
    
    //left rear arm
    glPushMatrix();
    glTranslatef(-.67, .3, 0);
    glRotatef(60, 1, -1, 0);
    gluCylinder(cyl, .2, .1, .8, 32, 32);
    glPopMatrix();
    
    
    glPushMatrix();
    glTranslated(-1.21, -.26, .43);
    glRotated(RLeftArmBottomX, 1, 0, 0);
    glRotated(RLeftArmBottomY, 0, 1, 0);
    glRotated(RLeftArmBottomZ, 0, 0, 1);
    glTranslated(1.21, .26, -.43);
    
    
    //middle left joint
    glPushMatrix();
    glTranslatef(-1.21, -.26, .43);
    glutSolidSphere(.15 ,20 ,20);
    glPopMatrix();
    
    //left forarm
    glPushMatrix();
    glTranslatef(-1.21, -.26, .43);
    glRotatef(60, 1, 0, 0);
    glRotatef(30, 0, 1, 0);
    gluCylinder(cyl, .1, .05, .65, 32, 32);
    glPopMatrix();
    
    //left hand
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, robot_shoes);
    glPushMatrix();
    glTranslatef(-.843, -.81, .75);
    glutSolidSphere(.1 ,20 ,20);
    glPopMatrix();
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, robot_color);
    
    glPopMatrix();
    
    glPopMatrix();
    /*End Left H*/
    
    
    /*Right H*/
    
    glPushMatrix();
    
    glTranslated(-.65, .3, 0);
    glRotated(RRightArmTopX, 1, 0, 0);
    glRotated(RRightArmTopY, 0, 1, 0);
    glRotated(RRightArmTopZ, 0, 0, 1);
    glTranslated(.65, -.3, 0);
    
    
    //top right joint
    glPushMatrix();
    glTranslatef(-.65, .3, 0);
    glTranslatef(1.3, 0, 0);
    glutSolidSphere(.25 ,20 ,20);
    glPopMatrix();
    
    //right rear arm
    glPushMatrix();
    glTranslatef(.67, .3, 0);
    glRotatef(60, 1, 1, 0);
    gluCylinder(cyl, .2, .1, .8, 32, 32);
    glPopMatrix();
    
    
    glPushMatrix();
    
    
    glTranslated(1.21, -.26, .43);
    glRotated(RRightArmBottomX, 1, 0, 0);
    glRotated(RRightArmBottomY, 0, 1, 0);
    glRotated(RRightArmBottomZ, 0, 0, 1);
    glTranslated(-1.21, .26, -.43);
    
    
    //middle right joint
    glPushMatrix();
    glTranslatef(1.21, -.26, .43);
    glutSolidSphere(.15 ,20 ,20);
    glPopMatrix();
    
    
    //right forarm
    glPushMatrix();
    glTranslatef(+1.21, -.26, .43);
    glRotatef(60, 1, 0, 0);
    glRotated(-30, 0, 1, 0);
    gluCylinder(cyl, .1, .05, .65, 32, 32);
    glPopMatrix();
    
    
    
    //right hand
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, robot_shoes);
    glPushMatrix();
    glTranslatef(.843, -.81, .75);
    glutSolidSphere(.1 ,20 ,20);
    glPopMatrix();
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, robot_color);
    
    
    
    glPopMatrix();
    
    glPopMatrix();
    /*End Right H*/
    
    
    
}

/*
 * Function used to draw the legs of the robot
 * Pre: Initialize globals
 * Post: Legs are drawn */
void robot_legs(){
    
    
    //left leg Matrix push
    glPushMatrix();
    glTranslatef(0, 0.05, 0);
    
    glRotatef(0+RLeftLegTopX, 1, 0, 0);
    glRotated(0+RLeftLegTopY, 0 ,1, 0);
    glRotated(0+RLeftLegTopZ, 0, 0, 1);
    
    
    //leg base top left
    glPushMatrix();
    glScalef(1.2, 1, 1.);
    glTranslatef(-.27+LeftLegTopX, -.6+LeftLegTopY, 0+LeftLegTopZ);
    glRotatef(90, 1, 0, 0);
    gluCylinder(cyl, .25, .1, .7, 32, 32);
    glPopMatrix();
    
    //Left leg joint Matrix  Push
    glPushMatrix();
    glTranslatef(-.31, -1.4, 0);
    glRotatef(0 + RLeftLegBottomX, 1, 0, 0);
    glRotatef(0 + RLeftLegBottomY, 0, 1, 0);
    glRotatef(0 + RLeftLegBottomZ, 0, 0, 1);
    glTranslatef(.31, 1.4, 0);
    
    //leg base bottom left
    glPushMatrix();
    glTranslatef(-.3, -1.6, 0);
    glRotatef(90, 1, 0, 0);
    gluCylinder(cyl, .12, .17, .55, 32, 32);
    glPopMatrix();
    
    //left joint
    glPushMatrix();
    glTranslatef(-.31, -1.4, 0);
    glutSolidSphere(.25 ,20 ,20);
    glPopMatrix();
    
    
    //left shoe
    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, robot_shoes);
    glScalef(1., .7, 1.5);
    glTranslatef(-.3, -3.1, .05);
    glutSolidCube(.35);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, robot_color);
    glPopMatrix();
    
    //left leg joint pop
    glPopMatrix();
    //Left leg pop
    glPopMatrix();
    
    
    //RIGHT LEG PUSH
    //shift leg up to avoid clipping
    glTranslatef(0,.05,0);
    
    
    glPushMatrix();
    glRotatef(0+RRightLegTopX, 1, 0, 0);
    glRotated(0+RRightLegTopY, 0 ,1, 0);
    glRotated(0+RRightLegTopZ, 0, 0, 1);
    
    
    
    //leg base top right
    glPushMatrix();
    glScalef(1.2, 1, 1);
    glTranslatef(-.27+LeftLegTopX, -.6+LeftLegTopY, 0+LeftLegTopZ);
    glRotatef(90, 1, 0, 0);
    glTranslatef(.54+RightLegTopX, 0+RightLegTopY, 0+RightLegTopZ);
    gluCylinder(cyl, .25, .1, .7, 32, 32);
    glPopMatrix();
    
    
    // right leg joint matrix Push
    glPushMatrix();
    glTranslatef(-.31, -1.4, 0);
    glRotatef(0+RRightLegBottomX, 1, 0, 0);
    glRotated(0+RRightLegBottomY, 0 ,1, 0);
    glRotated(0+RRightLegBottomZ, 0, 0, 1);
    glTranslatef(.31, 1.4, 0);
    
    // leg base bottom right
    glPushMatrix();
    glTranslatef(-.3, -1.6, 0);
    glRotatef(90, 1, 0, 0);
    glTranslatef(.64, 0, 0);
    gluCylinder(cyl, .12, .17, .55, 32, 32);
    glPopMatrix();
    
    //right joint
    glPushMatrix();
    glTranslatef(-.31, -1.4, 0);
    glTranslatef(.65, 0, 0);
    glutSolidSphere(.25 ,20 ,20);
    glPopMatrix();
    
    //right shoe
    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, robot_shoes);
    glScalef(1., .7, 1.5);
    glTranslatef(-.3, -3.1, .05);
    glTranslatef(.64, 0,0);
    glutSolidCube(.35);
    glPopMatrix();
    
    //right leg joint pop
    glPopMatrix();
    //right leg pop
    glPopMatrix();
}


/*
 * Function used to animate robot
 * Pre: Initialize globals, callback registered in main, corresponding logic in draw functions
 * Post: Robot is animated in a loop */
void Animate(void) {
    
    
    
    //EYEBROWS
    if (eyeReset && eBrowX < .05) {
        eBrowX += .0018 * eyeValue;
        eBrowY += .001 * eyeValue;
    }
    else if(eyeReset && eBrowX >= .05 ){
        eyeReset = !eyeReset;
        eBrowX += -.0018*eyeValue;
        eBrowY += -.001 * eyeValue;
    }
    else if(!eyeReset && eBrowX > 0)
        
    {
        eBrowX += -.0018*eyeValue;
        eBrowY += -.001 * eyeValue;
        
    }
    else if(!eyeReset && eBrowX <= 0){
        eyeReset = !eyeReset;
        eBrowX += .0018*eyeValue;
        eBrowY += .001 * eyeValue;
    }
    
    
    //RIGHT ARM
    
    if (rightArmReset && RRightArmTopX > -150) {
        RRightArmTopX += -2 * multValue;
        RRightArmBottomZ+= .66 * multValue;
    }
    else if(rightArmReset && RRightArmTopX <= -150 ){
        rightArmReset = !rightArmReset;
        RRightArmTopX += 2*multValue;
        RRightArmBottomZ += -.66*multValue;
    }
    else if(!rightArmReset && RRightArmTopX < 0)
        
    {
        RRightArmTopX += 2*multValue;
        RRightArmBottomZ += -.66*multValue;
    }
    else if(!rightArmReset && RRightArmTopX >= 0){
        rightArmReset = !rightArmReset;
        RRightArmTopX += -2*multValue;
        RRightArmBottomZ += .66*multValue;
    }
    
    
    //LEFT ARM
    
    if (leftArmReset && RLeftArmTopX > -150) {
        RLeftArmTopX += -2 * multValue;
        RLeftArmBottomZ+= -.66 * multValue;
    }
    else if(leftArmReset && RLeftArmTopX <= -150 ){
        leftArmReset = !leftArmReset;
        RLeftArmTopX += 2*multValue;
        RLeftArmBottomX += .66*multValue;
    }
    else if(!leftArmReset && RLeftArmTopX < 0)
        
    {
        RLeftArmTopX += 2*multValue;
        RLeftArmBottomZ += .66*multValue;
    }
    else if(!leftArmReset && RLeftArmTopX >= 0){
        leftArmReset = !leftArmReset;
        RLeftArmTopX += -2*multValue;
        RLeftArmBottomX += -.66*multValue;
    }
    
    //right leg animation
    if (rightLegReset && RRightLegTopX < 30) {
        RRightLegTopX += .5 *timesValue;
        RRightLegBottomX += 1.5*timesValue;
    }
    else if(rightLegReset && RRightLegTopX >= 30 ){
        rightLegReset= !rightLegReset;
        RRightLegTopX += -.5 *timesValue;
        RRightLegBottomX += -1.5*timesValue;
    }
    else if(!rightLegReset && RRightLegTopX > 0){
        RRightLegTopX += -.5 *timesValue;
        RRightLegBottomX += -1.5*timesValue;
    }
    else if(!rightLegReset && RRightLegTopX <= 0){
        rightLegReset = !rightLegReset;
        RRightLegTopX += .5 *timesValue;
        RRightLegBottomX += 1.5*timesValue;
        
    }
    
    // left leg animation
    
    if (leftLegReset && RLeftLegTopX < 30) {
        RLeftLegTopX += .5 *timesValue;
        RLeftLegBottomX += 1.5 *timesValue;
    }
    else if(leftLegReset && RLeftLegTopX >= 30 ){
        leftLegReset= !leftLegReset;
        RLeftLegTopX += -.5 *timesValue;
        RLeftLegBottomX += -1.5 *timesValue;
        
    }
    else if(!leftLegReset && RLeftLegTopX > 0){
        RLeftLegTopX += -.5 *timesValue;
        RLeftLegBottomX += -1.5 *timesValue;
    }
    else if(!leftLegReset && RLeftLegTopX <= 0){
        leftLegReset = !leftLegReset;
        RLeftLegTopX += .5 *timesValue;
        RLeftLegBottomX += 1.5 *timesValue;
    }
    
    
    //WHOLE BODY TRANSLATION
    if (bodyReset && bodyY < 2) {
        bodyY += .05 * boostValue;
    }
    else if(bodyReset && bodyY >= 2 ){
        bodyReset = !bodyReset;
        bodyY -= .05 *boostValue;
    }
    else if(!bodyReset && bodyY > 0)
        
    {
        bodyY -= .05 *boostValue;
    }
    else if(!bodyReset && bodyY <= 0){
        bodyReset = !bodyReset;
        bodyY += .05 * boostValue;
    }
    
    
    // animates left and right balls on head
    rightballX += 8 * earSpeed;
    leftballX -= 8 * earSpeed;
    glutPostRedisplay();
}

/*
 * Function to create pop up menu on right click of mouse
 * Pre: Initialize globals, callback registered in main, corresponding logic in draw functions
 * Post: Menu is ready for use*/
void menuSelect(int value) {
    
    switch (value) {
            
        case 1: moving = GL_TRUE; //start the animation
            
            glutIdleFunc(Animate); break;
            
        case 2: moving = GL_FALSE; //stop the animation
            
            glutIdleFunc(NULL); break;
        case 3:
            if (trackLocation <= 6 && trackLocation >2) {
                trackLocation --;
            }
            else if (trackLocation > 2) {
                trackLocation -= 2;
            }
            glutPostRedisplay();
            
            break;
            
        case 4:
            if(trackLocation <= 6 && trackLocation < 21){
                trackLocation ++;
            }
            else if(trackLocation < 21){
                trackLocation += 2;
            }
            glutPostRedisplay();
            
            break;
            
        case 5:
            multValue *= 1.5; timesValue *= 1.5; eyeValue *= 1.5; boostValue *= 1.5, earSpeed *= 1.5;
            break;
            
        case 6:
            multValue /= 1.5; timesValue /= 1.5; eyeValue /= 1.5; boostValue /= 1.5, earSpeed /= 1.5;
            break;
        case 7:
            multValue = 1.8, timesValue = 1.48, eyeValue = 1, boostValue = 1;
            trackLocation = 5;
            
            LeftLegTopX = 0,  LeftLegTopY = 0, LeftLegTopZ = 0;
            LeftLegBottomX = 0,  LeftLegBottomY = 0, LeftLegBottomZ = 0;
            RightLegTopX = 0,  RightLegTopY = 0, RightLegTopZ = 0;
            RightLegBottomX = 0,  RightLegBottomY = 0, RighLegBottomZ = 0;
            
            RLeftLegTopX = 0,  RLeftLegTopY = 0, RLeftLegTopZ = 0;
            RLeftLegBottomX = 0,  RLeftLegBottomY = 0, RLeftLegBottomZ = 0;
            RRightLegTopX = 0,  RRightLegTopY = 0, RRightLegTopZ = 0;
            RRightLegBottomX = 0,  RRightLegBottomY = 0, RRightLegBottomZ = 0;
            
            
            RLeftArmTopX = 0,  RLeftArmTopY = 0, RLeftArmTopZ = 0;
            RLeftArmBottomX = 0,  RLeftArmBottomY = 0, RLeftArmBottomZ = 0;
            RRightArmTopX = 0,  RRightArmTopY = 0, RRightArmTopZ = 0;
            RRightArmBottomX = 0,  RRightArmBottomY = 0, RRightArmBottomZ = 0;
            
            bodyX = 0, bodyY = 0, bodyZ = 0;
            eBrowX = 0, eBrowY = 0;
            rightballX = 0,  rightballY = 0, rightballZ = 0;
            leftballX = 0,  leftballY = 0, leftballZ = 0;
            earSpeed = 1;
            rightArmReset = true;
            leftArmReset = true;
            bodyReset = true;
            rightLegReset = true;
            leftLegReset = true;
            eyeReset = true;
            
            break;
            
        case 8: exit(0); //quit application
            break;
            
        default:
            break;
    }
    
}

/*
 * Function to prevent animations from drawing off-screen
 * Pre:  callback registered in main
 * Post: animations will only be drawn when visible */
void Visible(int state) {
    
    if (state == GLUT_VISIBLE) {
        
        if (moving) glutIdleFunc(Animate); } //if visible and moving then animate it
    
    else { if (moving) glutIdleFunc(NULL); } } //if invisible and moving then stop animation


/* Function responsible for handling window reshapes
 * Pre: Callback registered in main.
 * Post: Window is reshaped effectively.
 */
void reshape(int w, int h)
{
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if(h == 0)
    {
        h = 1;
    }
    float ratio = 1.0* w / h;
    
    glViewport(0, 0, w, h);
    // Use the Projection Matrix
    glMatrixMode(GL_PROJECTION);
    // Reset Matrix
    glLoadIdentity();
    // Set the viewport to be the entire window
    
    // Set the correct perspective.
    gluPerspective(90,ratio,0.1f,100.0f);
    // Get Back to the Modelview
    
    glMatrixMode(GL_MODELVIEW);
    
    // recall identity
    glLoadIdentity();
}

/* Main Method
 * Pre: Libraries are added to project.
 * Post: Program initialized, glutMainLoop() called.
 */
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Project 3 Rapson and London");
    //Menu Building
    glutCreateMenu(menuSelect);
    glutAddMenuEntry("Start", 1); //start animation
    glutAddMenuEntry("Stop", 2); //stop animation
    glutAddMenuEntry("Zoom In", 3);//Zoom in
    glutAddMenuEntry("Zoom Out", 4); //Zoom out
    glutAddMenuEntry("Speed Up", 5); //Speeds simulation up
    glutAddMenuEntry("Slow Down", 6); //Slows simulation down
    glutAddMenuEntry("Reset", 7); //Restores default values
    glutAddMenuEntry("Quit", 8); //quit
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    glutIdleFunc(Animate);
    glutVisibilityFunc(Visible);
    myInit();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(processNormalKeys);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    
    
    glutMainLoop();
}