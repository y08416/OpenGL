/* sampleTimer.c */
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

float radius = 1.0;
float height = 10.0;		/* 高さ */
float y_max  = 10.0;		/* 初期位置 */
float coeff  = 0.9;		/* 弾性係数 */
float gravt  = -9.81; 		/* 重力加速度 */
float dt   = 0.1;		/* 時間の刻み */
float vz   = 0.0;		/* 初速度 */
float vzt  = 0.0;
float eps  = 0.01;
int flag  = 1;
int hFlag = 0; 

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glPushMatrix();
    glBegin(GL_QUADS);
      glColor3f( 0.5, 0.5, 0.5 );
      glVertex3f(-3.5, 0.0, -3.5 );
      glVertex3f( 3.5, 0.0, -3.5 );
      glVertex3f( 3.5, 0.0,  3.5 );
      glVertex3f(-3.5, 0.0,  3.5 );
    glEnd();
  glPopMatrix(); 
  
  glColor3f( 0.0, 0.0, 1.0 );
  glPushMatrix();
    glTranslatef(0.0, height, 0.0);
    glutSolidSphere(radius, 20, 20);
  glPopMatrix();

  glutSwapBuffers();
}

void timer(int val)
{
  float v_diff;

  if (flag == 1 ) {
    vzt    = vzt + gravt*dt;
    height = height + vzt*dt;
    if( height < 0.0 ) {
      height = 0.0;
      vzt = -coeff*vzt;
    } 
    v_diff = fabs( vz - vzt );
    if( height<eps && fabs(v_diff) <  eps  ) {
      height = 0;
      flag = 0;
      }
    vz = vzt;
  }
    
  glutPostRedisplay();      /* ディスプレイコールバック関数(display)の実行 */

  glutTimerFunc(50 , timer , 1); /* Timer を再セットする */

}


void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case 'q':                   /* qが入力されたら，プログラムを終了する */
    exit(0);
  case 'r':                   /* rが入力されたら，球を初期位置にもどす */
    height = y_max;
    flag = 1;
    break;
  case 's':                   /* sが入力されたら，球を停止 */
    flag = !flag;
    break;
  default:
    break;
  }
}

void init(char *winname)
{
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(500, 500);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); /* ダブルバッファの宣言 */
  glutCreateWindow(winname);

  glClearColor(0.2, 0.4, 0.4, 1.0);

  glMatrixMode(GL_PROJECTION); /* 投影法に関する座標変換を開始 */
  glLoadIdentity();
  gluPerspective( 45.0, 1.0, 1.0, 50.0 );
  glMatrixMode(GL_MODELVIEW); /* 視点の移動やモデルの移動など投影法
                                 以外の座標変換を開始 */
  gluLookAt( 3.0, 8.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  init(argv[0]);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard); /* キーボード入力のコールバック関数keyboardの指定 */

  glutTimerFunc(50 , timer , 1); /* Timer をセットする */

  glutMainLoop();
  return 0;
}
