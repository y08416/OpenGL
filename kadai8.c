/* kadai8.c */
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

float param = 0.0;
int flag = 0;

float diffuse[] = {0.8, 0.8, 0.8, 0.0};
float specular[] = {1.0, 1.0, 1.0, 1.0};
float ambient[] = {0.2, 0.2, 0.2, 1.0};
float shininess = 128.0;
float light_pos[] = {1.5, 1.5, 0.0, 1.0}; /* スポットライトの位置 */
float light_dir[] = {-1.5, -1.5, 0.0};    /* スポットライトの方向 */
float em_light[] = {1.5, 1.5, 0.0, 1.0};
float emission[] = {1.0, 1.0, 0.0, 1.0};
float spot_cutoff = 30.0;                 /* 初期のスポットライトの広がり角度 */
float spot_exponent = 10.0;               /* スポットライトの減衰率 */

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
  glMaterialf(GL_FRONT, GL_SHININESS, shininess);

  glEnable(GL_LIGHTING);
  glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_dir);
  glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, spot_cutoff);
  glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, spot_exponent);

  glPushMatrix();
  glRotatef(param, 0.0, 1.0, 0.0);
  glutSolidSphere(1.0, 30, 30); /* 分割数を増加 */
  glPopMatrix();

  glPushMatrix();
  glTranslatef(em_light[0], em_light[1], em_light[2]);
  glPushAttrib(GL_LIGHTING_BIT);
  glMaterialfv(GL_FRONT, GL_EMISSION, emission);
  glutSolidSphere(0.1, 10, 10);
  glPopAttrib();
  glPopMatrix();

  glDisable(GL_LIGHTING);
  glutSwapBuffers();
}

void idle(void)
{
  if(flag == 1){
    param = fmod(param+2.0, 360.0);
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
  static float r[] = {1.0, 1.0, 0.0, 0.0};
  static float g[] = {1.0, 0.0, 1.0, 0.0};
  static float b[] = {1.0, 0.0, 0.0, 1.0};
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
    diffuse[0] = r[index];
    diffuse[1] = g[index];
    diffuse[2] = b[index];
    break;
  case 's':
    if(smooth)
      glShadeModel(GL_SMOOTH);
    else
      glShadeModel(GL_FLAT);
    smooth = !smooth;
    break;
  case '1': /* スポットライトの広がり角度を20度に設定 */
    spot_cutoff = 20.0;
    break;
  case '2': /* スポットライトの広がり角度を90度に設定 */
    spot_cutoff = 90.0;
    break;
  default:
    break;
  }
}

void initLighting()
{
  float diffuse_light[]  = {0.5, 0.5, 0.5, 1.0};
  float specular_light[] = {0.5, 0.5, 0.5, 1.0};
  float ambient_light[]  = {0.5, 0.5, 0.5, 1.0};
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
  gluPerspective(30.0, 1.0, 1.0, 10.0);
  glMatrixMode(GL_MODELVIEW);
  gluLookAt(3.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

  glShadeModel(GL_FLAT);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
  
  initLighting();
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