#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

float param = 0.0;
float epsilon = 1.0e-4;
int flag = 0;

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glPushMatrix();                         /* 現在の変換行列を保存 */
  glRotatef(param, 0.0, 1.0, 0.0);        /* y軸を中心にparam度回転 */
  glBegin(GL_QUADS);
    glVertex3f(-0.5, -0.5, 0.0);
    glVertex3f(0.5, -0.5, 0.0);
    glVertex3f(0.5, 0.5, 0.0);
    glVertex3f(-0.5, 0.5, 0.0);
  glEnd();
  glPopMatrix();                          /* 9行目の変換行列に戻す */
  glutSwapBuffers();
}

void idle(void)
{
  if(flag == 1){
    param = fmod(param + 2.0, 360.0);  /* 回転角を2度ずつ増加し、360度を超えたらリセット */
  }
  glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
  if(state == GLUT_DOWN){                 /* マウスがクリックされたとき */
    switch(button){
    case GLUT_LEFT_BUTTON:                /* 左ボタンがクリックされた場合 */
      flag = 1;                           /* 回転を開始 */
      break;
    case GLUT_RIGHT_BUTTON:               /* 右ボタンがクリックされた場合 */
      flag = 0;                           /* 回転を停止 */
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

  switch (key) {
  case 'q':                   /* 'q'キーが押されたらプログラムを終了 */
    exit(0);
  case 'r':
    param += 30.0;
    break;
  case 'c':
    index++;
    index %= 4;
    glColor3f(r[index], g[index], b[index]);
    break;
  default:
    break;
  }
}

void init(char *winname)
{
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(500, 500);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); /* ダブルバッファを有効化 */
  glutCreateWindow(winname);

  glClearColor(0.2, 0.4, 0.4, 1.0);

  glMatrixMode(GL_PROJECTION); /* 投影変換を開始 */
  glLoadIdentity();
  gluPerspective(30.0, 1.0, 1.0, 10.0);
  glMatrixMode(GL_MODELVIEW); /* モデルビュー変換を開始 */
  gluLookAt(3.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  init(argv[0]);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);  /* マウスコールバック関数を登録 */
  glutIdleFunc(idle);
  glutMainLoop();
  return 0;
}