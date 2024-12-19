#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

float param = 0.0;
int rotate_flag = 0; // 0：静止、1：回転
int color_index = 0;
float colors[4][3] = {
    {1.0, 1.0, 1.0}, // 白
    {1.0, 0.0, 0.0}, // 赤
    {0.0, 1.0, 0.0}, // 緑
    {0.0, 0.0, 1.0}  // 青
};

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glPushMatrix();
  glRotatef(param, 0.0, 1.0, 0.0); // y軸中心に回転
  glBegin(GL_QUADS);
    glColor3f(colors[color_index][0], colors[color_index][1], colors[color_index][2]);
    glVertex3f(-0.5, -0.5, 0.0);
    glVertex3f(0.5, -0.5, 0.0);
    glVertex3f(0.5, 0.5, 0.0);
    glVertex3f(-0.5, 0.5, 0.0);
  glEnd();
  glPopMatrix();
  glutSwapBuffers();
}

void idle(void)
{
  if (rotate_flag) {
    param = fmod(param + 1.0, 360.0); // 回転角を増加
    glutPostRedisplay();
  }
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case 'q': // プログラム終了
    exit(0);
    break;
  case 'r': // 30度ずつ回転
    param = fmod(param + 30.0, 360.0);
    glutPostRedisplay();
    break;
  case 'c': // 色を循環
    color_index = (color_index + 1) % 4;
    glutPostRedisplay();
    break;
  default:
    break;
  }
}

void init(char *winname)
{
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(500, 500);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

  glutCreateWindow(winname);

  glClearColor(0.2, 0.4, 0.4, 1.0);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30.0, 1.0, 1.0, 10.0);
  glMatrixMode(GL_MODELVIEW);
  gluLookAt(3.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  init(argv[0]);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutIdleFunc(idle);
  glutMainLoop();
  return 0;
}