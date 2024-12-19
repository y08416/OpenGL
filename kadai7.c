#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

float param = 0.0;
int flag = 0;
int shadeFlag = 0; // 0:フラットシェーディング, 1:スムースシェーディング
int colorIndex = 0;

float colors[4][4] = {
  {1.0, 1.0, 1.0, 1.0}, // 白
  {1.0, 0.0, 0.0, 1.0}, // 赤
  {0.0, 1.0, 0.0, 1.0}, // 緑
  {0.0, 0.0, 1.0, 1.0}  // 青
};
float diffuse[] = {1.0, 1.0, 1.0, 1.0};  /* 初期色は白 */
float specular[] = {1.0, 1.0, 1.0, 1.0}; /* 鏡面反射の強さ */
float ambient[] = {0.2, 0.2, 0.2, 1.0};  /* 環境光の強さ */
float shininess = 128.0;

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
  glRotatef(param, 0.0, 1.0, 0.0);

  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
  glMaterialf(GL_FRONT, GL_SHININESS, shininess);

  glEnable(GL_LIGHTING);
  glutSolidSphere(1.0, 10, 10); // 半径1.0、分割数10で球を描画
  glDisable(GL_LIGHTING);

  glPopMatrix();
  glutSwapBuffers();
}

void idle(void)
{
  if (flag == 1) {
    param = fmod(param + 1.0, 360.0);  /* 回転角を0から360まで増加 */
  }
  glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
  if (state == GLUT_DOWN) {
    switch (button) {
      case GLUT_LEFT_BUTTON:
        flag = 1; // 左クリックで回転開始
        break;
      case GLUT_RIGHT_BUTTON:
        flag = 0; // 右クリックで回転停止
        break;
    }
  }
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
    case 'q':
      exit(0);
    case 's':
      if (shadeFlag == 0) {
        glShadeModel(GL_SMOOTH);
        shadeFlag = 1;
      } else {
        glShadeModel(GL_FLAT);
        shadeFlag = 0;
      }
      break;
    case 'c':
      colorIndex = (colorIndex + 1) % 4;
      diffuse[0] = colors[colorIndex][0];
      diffuse[1] = colors[colorIndex][1];
      diffuse[2] = colors[colorIndex][2];
      diffuse[3] = colors[colorIndex][3];
      break;
    default:
      break;
  }
}

void init(char *winname)
{
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(500, 500);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutCreateWindow(winname);

  glClearColor(0.2, 0.4, 0.4, 1.0);

  glEnable(GL_DEPTH_TEST); // 深度テストを有効にする
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30.0, 1.0, 1.0, 10.0);
  glMatrixMode(GL_MODELVIEW);
  gluLookAt(3.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

  glShadeModel(GL_FLAT); // 初期設定はフラットシェーディング
  glEnable(GL_LIGHT0);    // 光源を有効化
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