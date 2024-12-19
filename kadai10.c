/* kadai10.c */
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

#define imageWidth 8
#define imageHeight 8

float param = 0.0;
int flag = 0;

float diffuse[] = {0.8, 0.8, 0.8, 1.0};
float specular[] = {1.0, 1.0, 1.0, 1.0};
float ambient[] = {0.2, 0.2, 0.2, 1.0};
float shininess = 128.0;
float light[] = {1.5, 1.5, 0.0, 0.0};
float emission[] = {1.0, 1.0, 0.0, 1.0};

unsigned char image1[imageHeight][imageWidth][4];

unsigned char texture1[imageHeight][imageWidth] = {
  {255, 255, 255, 255,   0,   0,   0,   0},
  {255, 255, 255, 255,   0,   0,   0,   0},
  {255, 255, 255, 255,   0,   0,   0,   0},
  {255, 255, 255, 255,   0,   0,   0,   0},
  {  0,   0,   0,   0, 255, 255, 255, 255},
  {  0,   0,   0,   0, 255, 255, 255, 255},
  {  0,   0,   0,   0, 255, 255, 255, 255},
  {  0,   0,   0,   0, 255, 255, 255, 255}
};

void makeImage1(void) {
  int i,j;
  for (i = 0; i < imageHeight; i++) {
    for (j = 0; j < imageWidth; j++) {
      image1[i][j][0] = (unsigned char) texture1[i][j];
      image1[i][j][1] = (unsigned char) texture1[i][j];
      image1[i][j][2] = (unsigned char) texture1[i][j];
      image1[i][j][3] = (unsigned char) 100;
    }
  }
}

void drawTriangularPrism(float base, float height, float depth) {
  glBegin(GL_TRIANGLES);
    // Front face
    glVertex3f(-base / 2, 0.0, depth / 2);
    glVertex3f(base / 2, 0.0, depth / 2);
    glVertex3f(0.0, height, depth / 2);
    
    // Back face
    glVertex3f(-base / 2, 0.0, -depth / 2);
    glVertex3f(base / 2, 0.0, -depth / 2);
    glVertex3f(0.0, height, -depth / 2);
  glEnd();

  glBegin(GL_QUADS);
    // Side faces
    glVertex3f(-base / 2, 0.0, depth / 2);
    glVertex3f(-base / 2, 0.0, -depth / 2);
    glVertex3f(0.0, height, -depth / 2);
    glVertex3f(0.0, height, depth / 2);

    glVertex3f(base / 2, 0.0, depth / 2);
    glVertex3f(base / 2, 0.0, -depth / 2);
    glVertex3f(0.0, height, -depth / 2);
    glVertex3f(0.0, height, depth / 2);

    // Bottom face
    glVertex3f(-base / 2, 0.0, depth / 2);
    glVertex3f(base / 2, 0.0, depth / 2);
    glVertex3f(base / 2, 0.0, -depth / 2);
    glVertex3f(-base / 2, 0.0, -depth / 2);
  glEnd();
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
  glMaterialf(GL_FRONT, GL_SHININESS, shininess);
  glEnable(GL_LIGHTING);

  glPushMatrix();
  
  // y軸周りにparamの角度で回転
  glRotatef(param, 0.0, 1.0, 0.0);
  
  // 頭（立方体）
  glTranslatef(0.0, 1.0, 0.0);
  glutSolidCube(2.0);

  // 左目
  glPushMatrix();
  glTranslatef(-0.5, 0.5, 1.0);
  glutSolidSphere(0.2, 10, 10);
  glPopMatrix();

  // 右目
  glPushMatrix();
  glTranslatef(0.5, 0.5, 1.0);
  glutSolidSphere(0.2, 10, 10);
  glPopMatrix();

  // 口
  glPushMatrix();
  glTranslatef(0.0, 0.0, 1.1);
  glutSolidCube(0.2);
  glPopMatrix();

  // 左耳
  glPushMatrix();
  glTranslatef(-1.3, 0.5, 0.0);
  glRotatef(-30.0, 0.0, 0.0, 1.0); // 左耳の回転
  drawTriangularPrism(0.3, 0.6, 0.3); // 三角柱のサイズ設定
  glPopMatrix();

  // 右耳
  glPushMatrix();
  glTranslatef(1.3, 0.5, 0.0);
  glRotatef(30.0, 0.0, 0.0, 1.0); // 右耳の回転
  drawTriangularPrism(0.3, 0.6, 0.3); // 三角柱のサイズ設定
  glPopMatrix();

  glPopMatrix();

  glDisable(GL_LIGHTING);

  glEnable(GL_TEXTURE_2D);
  glNormal3f(0.0, 1.0, 0.0);
  glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-5.5, -1.5, -5.5);
    glTexCoord2f(0.0, 20.0); glVertex3f(-5.5, -1.5, 5.5);
    glTexCoord2f(20.0, 20.0); glVertex3f(5.5, -1.5, 5.5);
    glTexCoord2f(20.0, 0.0); glVertex3f(5.5, -1.5, -5.5);
  glEnd();

  glDisable(GL_TEXTURE_2D);

  glutSwapBuffers();
}

void idle(void)
{
  if(flag == 1){
    param = fmod(param + 2.0, 360.0);
  }
  glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
  if(state == GLUT_DOWN){
    switch(button){
    case GLUT_LEFT_BUTTON:
      flag = 1;
      break;
    case GLUT_RIGHT_BUTTON:
      flag = 0;
      break;
    }
  }
}

void keyboard(unsigned char key, int x, int y)
{
  static float col[4][4] = {{1.0, 0.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 1.0}, {0.0, 0.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}};
  static int index = 0;
  static int smooth = 0;
  switch (key) {
  case 'q':
    exit(0);
  case 'r':
    param += 30.0;
    break;
  case 'c':
    index++;
    index %= 4;
    glMaterialfv(GL_FRONT, GL_DIFFUSE, col[index]);
    break;
  case 's':
    if (smooth)
      glShadeModel(GL_SMOOTH);
    else
      glShadeModel(GL_FLAT);
    smooth = !smooth;
    break;
  default:
    break;
  }
}

void initTexture1()
{
  makeImage1();
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1);
}

void initLighting()
{
  float diffuse_light[] = {0.5, 0.5, 0.5, 1.0};
  float specular_light[] = {0.5, 0.5, 0.5, 1.0};
  float ambient_light[] = {0.5, 0.5, 0.5, 1.0};

  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
}

void init(char *winname)
{
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(500, 500);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutCreateWindow(winname);

  glClearColor(0.2, 0.4, 0.4, 1.0);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, 1.0, 1.0, 50.0);  // fovy を 45 度に設定
  glMatrixMode(GL_MODELVIEW);
  gluLookAt(3.0, 4.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

  glShadeModel(GL_FLAT);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);

  initLighting();
  initTexture1();
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  init(argv[0]);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutIdleFunc(idle);
  glutMainLoop();
  return 0;
}