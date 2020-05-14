//#include <stdlib.h>
////#include <GL/glew.h>
////#include <GLFW/glfw3.h>
//#define GL_SILENCE_DEPRECATION
//#include <OpenGL/gl.h>
//#include <OpenGl/glu.h>
//#include <GLUT/glut.h>
//#include <GLFW/glfw3.h>
//#include <unistdio.h>
//
//
//GLfloat rotationvar =0;
//void displaytri() {
//    glClear(GL_COLOR_BUFFER_BIT);
//    glTranslatef(0.3,0.3,0.3);
//    glRotatef(180, 0, 0, 1);
//    glBegin(GL_POLYGON);
//    {
//        glVertex3f(0, 1, 0);
//        glVertex3f(1, 1, 0);
//        glVertex3f(0, 0, 0);
//    }
//    glEnd();
//    glFlush();
//}
//
//
//void displayV() {
//    glClear(GL_COLOR_BUFFER_BIT);
//    float rotated;
//    rotated = 180+rotationvar;
//    rotated = (float)((int)rotated % 360);
//    float x=0;
//    float y=0;
//    glPushMatrix();
//    glTranslatef(-x, -y, 0.0);
//    glRotatef(rotated, 0.0, 0.0, 1.0);
//    glTranslatef(x, y, 0.0);
//    glColor3f(0.0, 1.0, 1.0);
//    glBegin(GL_LINE_STRIP);
//    {
//       glVertex2f(1, - 0.5);
//       glVertex2f(0, 0);
//       glVertex2f(1, 0.5);
//    }
//    glEnd();
//    glPopMatrix();
//  glFlush();
//}
//
//void mouse(int button, int state, int x, int y)
//{
//    switch (button)
//    {
//        case GLUT_LEFT_BUTTON:
//        if (state == GLUT_DOWN)
//        {
//            rotationvar+=5;
//            glutPostRedisplay();
//        }
//    }
//}
//
//int main(int argc, char** argv) {
//  glutInit(&argc, argv);
//  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
//  glutInitWindowPosition(80, 80);
//  glutInitWindowSize(1000, 500);
//  glutCreateWindow("A Simple Triangle");
////  glutDisplayFunc(displayV);
//  glutDisplayFunc(displaytri);
//  glutMouseFunc(mouse);
//  glutMainLoop();
//}

#include <GLUT/glut.h>
#include <stdlib.h>      // LIBRARIES
#include <stdio.h>
#include <math.h>
#define BALL_SIDE 12        //SIDE PIXES PARA D MAG LAPAW
#define BALL_SPEED 4.0      // BALL SPEED
#define PLAYER_SIDE 40      //PLAYER SIDE
#define PLAYER_SPEED 12     //PLAYER SPEED
#define ANGLE 45;       //ANGLE
int angles[4] = {30,45,-30,-45};   // ANGLE
#define RED 0           //COLORS
#define GREEN 1
#define BLUE 2
#define YELLOW 3
#define BLACK 4
#define WHITE 5
#define GRAY 6
#define PINK 7
#define PURPLE 8
#define DARKGREEN 9
GLfloat colors[10][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, // Red, Green
                        {0.0, 0.0, 1.0}, {1.0, 1.0, 0.0}, // Blue, Yellow
                        {0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, // Black, White
                        {0.5, 0.5, 0.5}, {1.0, 0.0, 1.0}, // Gray, Pink
                        {0.29, 0.0, 0.5}, {0.0, 0.39, 0.0} };
GLfloat pixel_color[3];     //for pixel comparison
typedef struct {        // STRUCTURE FOR COORDINATES
    double x;
    double y;
} coords;           // coords datatype
int GetRandRange()
{
    return(rand()%50+(-15));
}
int width, height ;     // WIDTH & HEIGHT
int cw, ch;         //current
coords ball;            // we have 1 ball having x and y values
coords ball_direction;      // ball x and y directions
coords player;          //PLAYER
coords player_direction;    //PLAYER only X direction will be used
int player_moving;      //?
coords brick;           //bricks
int numlines = 4;
int bricksperline = 10;
int brickspace = 50;
int showbrick[40];          // show brick 1 - true , 0 - false
/*---------------------------------------------------------------------------------*/
/*START OF BRICKS*/
void DrawBricks(){
int i,j;
int x1,y1,x2,y2;
    for (j=0; j<numlines; j++)
        {
           for (i=0; i<bricksperline; i++)
                {
                if (showbrick[j*bricksperline+i]){
                x1 = brick.x;
                y1 = brick.y;
                x2 = x1 + PLAYER_SIDE;
                y2 = y1 + PLAYER_SIDE;
                glColor3fv(colors[BLACK]);
                //glRectf(x1 + brickspace*i,y1,x2 + brickspace*i ,y2);
                glRectf(x1 + brickspace*i,y1 - brickspace*j,x2 + brickspace*i,y2-brickspace*j);
                }
            }
        }
}
void InitBricks()
{
    int i;
    for (i=0; i<numlines*bricksperline; i++)
      showbrick[i]=1;   // 1 as true
}
void CheckBricks()
{
    int i,j,x,y;
    for (j=0; j<numlines; j++)
    {
      for (i=0; i<bricksperline; i++)
      {
        if (showbrick[j*bricksperline+i])
        {
          y= 5 + j*(brickspace+PLAYER_SIDE);
          x= 5 +i*(brickspace+PLAYER_SIDE);
          if (ball.y>=(y-BALL_SIDE) && ball.y<(y+PLAYER_SIDE) &&
              ball.x>=(x-BALL_SIDE) && ball.x<(x+PLAYER_SIDE))
          {
            showbrick[j*bricksperline+i]= 0;
          }
        }
      }
    }
}
/*END OF BRICKS*/
/* START OF BALL*/
int reverse(int i){
    return i * -1;
}
double radian(double deg){
    return deg * (3.14159265/ 180);
}
void bounce_x(){
//    int angle = angles[rand() % 4];
  //  int angle = ANGLE;
    int angle = GetRandRange();
    float x = sqrt(2) * sin(radian(90 - angle));
    float y = sqrt(2) * sin(radian(angle));
    if(ball_direction.x > 0){
        x *= -1;
    }
    /*if(ball_direction.y > 0){
        y *= -1;
    }*/
    ball_direction.x = x;
    ball_direction.y = y;
}
void bounce_y(){
//    int angle = angles[rand() % 4];
    //int angle = ANGLE;
    int angle = GetRandRange();
    float x = sqrt(2) * sin(radian(90 - angle));
    float y = sqrt(2) * sin(radian(angle));
    /*if(ball_direction.x > 0){
        x *= -1;
    }*/
    if(ball_direction.y > 0){
        y *= -1;
    }
    ball_direction.x = x;
    ball_direction.y = y;
}
void bounce(){ //BOUNCES BALL BACK?
//    int angle = angles[rand() % 4];
   // int angle = ANGLE;
    int angle = GetRandRange();
    float x = sqrt(2) * sin(radian(90 - angle));
    float y = sqrt(2) * sin(radian(angle));
    if(ball_direction.x > 0){
        x *= -1;
    }
    if(ball_direction.y > 0){
        y *= -1;
    }
    ball_direction.x = x;
    ball_direction.y = y;
}
void updateBallPosition(){
    coords bd = ball_direction;
    ball.x += BALL_SPEED * bd.x;
    ball.y += BALL_SPEED * bd.y;
    if(ball.x < 0){
        ball.x = 0;
        //ball_direction.x = reverse(bd.x);
        bounce_x();
    }
    if(ball.x + BALL_SIDE > width){
        ball.x = width -  BALL_SIDE;
        //ball_direction.x = reverse(bd.x);
        bounce_x();
    }
    if(ball.y < 0){
        ball.y = 0;
        //ball_direction.y = reverse(bd.y);
        bounce_y();
    }
    if(ball.y + BALL_SIDE > height){
        ball.y = height - BALL_SIDE;
        //ball_direction.y = reverse(bd.y);
        bounce_y();
    }
    glReadPixels(ball.x-1,ball.y-1,1,1,GL_RGB,GL_FLOAT,pixel_color);
    //if(pixel_color[0] != colors[WHITE][0] && pixel_color[1] != colors[WHITE][1] && pixel_color[2] != colors[WHITE][2])        //{
    if(pixel_color[0] != colors[WHITE][0]){
    if(pixel_color != colors[WHITE]){
        bounce();
        //showbrick[j*bricksperline+i]= 0;
        //int s = ball.x;
        //showbrick[s] = 0;
        CheckBricks();
        glutPostRedisplay();
    }}
}
void displayBall(){
    int i;
    int x1,y1,x2,y2;
        x1 = ball.x;    //struct
        y1 = ball.y;
        x2 = x1 + BALL_SIDE;
        y2 = y1 + BALL_SIDE;
        glColor3fv(colors[RED]);
        //glColor3f(0.0,0.0,0.0);
        glRectf(x1,y1,x2,y2);
}
/* END OF BALL*/
/*START OF PLAYER*/
void movePlayer(){
    coords pd = player_direction;
    player.x += BALL_SPEED * pd.x; //used to be PLAYER_SPEED, but too fast
    if(player.x <= 0){
        player.x = 0;
        player_moving = 0;
    }
    if(player.x + PLAYER_SIDE >= width){
        player.x = width - PLAYER_SIDE;
        player_moving = 0;
    }
    if(player_moving == 0){
    player_direction.x = 0;     //para kun ga stack up sa gilid pwede pa ka balik
    }
}
void displayPlayer(){
    int x1,y1,x2,y2;
    x1 = player.x;
    y1 = player.y;
    x2 = x1 + PLAYER_SIDE;
    y2 = y1 + BALL_SIDE;
    glColor3fv(colors[BLACK]);
    glRectf(x1,y1,x2,y2);
}
/*END OF PLAYER*/
void myDisplay(){   //DISPLAY FUNC
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0,1.0,1.0);
    glRectf(0,0,width,height);
    displayBall();//display ball
    displayPlayer(); //display Player
    DrawBricks();
    glFlush();
}
void myReshape(int w,int h){    // RESHAPE FUNC
    cw = w;
    ch = h;
    glViewport((cw-width)/2,(ch-height)/2,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,width,0,height);
    glMatrixMode(GL_MODELVIEW);
}
void myIdle(){  //IDLE
    updateBallPosition();
    /*if(player_moving){
        movePlayer();
    }*/
    if(player_moving){
        movePlayer();
    }
    glutPostRedisplay();
}
void myKeyboard(unsigned char key, int x, int y){   //KEYBOARD
    coords move_direction;
    int change_direction = 1;
    if(key == 'a' || key == 'A'){
        //left
        player.x += PLAYER_SPEED * -1;
        move_direction.x = -1;
        if(player_direction.x == 1){
                change_direction = 1;
            }
    }
    else if(key == 'd' || key == 'D'){
        //right
        player.x += PLAYER_SPEED * 1;
        move_direction.x = 1;
            if(player_direction.x == -1){
                change_direction = 1;
            }
    }
    if((player.x != 0 )){
        player_moving = 1;
    if((player.x != width - PLAYER_SIDE)){
        player_moving = 1;
    }
    if(change_direction == 1){
                player_direction = move_direction;
            }
    }
    glutPostRedisplay();
}
void init(){    // INITIALIZE
    glClearColor(0.0,0.0,0.0,1.0);
    //player.x = (width/2)-(BALL_SIDE/2);
    player.x = 250;
    player.y = 0;
    player_direction.x = 0;
    player_direction.y = 0;
    player_moving = 0;
    ball.x = 250;   //set ball to center    // not yet so much centered
    ball.y = 30;
    ball_direction.x = 1;
    ball_direction.y = -1;
    InitBricks();       //INITIALIZE BRICKS SHOW
    brick.x = 5 ;
    brick.y = 450 ;
}
int main(int argc, char** argv){    //MAIN
    //srand(time(0));
    glutInit(&argc,argv);
    width = 500;
    height = 500;
    glutInitWindowSize(width,height);
    glutInitWindowPosition(100,100);
    glutCreateWindow("BRicK-OUT");
    glutDisplayFunc(myDisplay);
    glutReshapeFunc(myReshape);
    glutKeyboardFunc(myKeyboard);
    glutIdleFunc(myIdle);
    init();//initBricks();
    glutMainLoop();
}
