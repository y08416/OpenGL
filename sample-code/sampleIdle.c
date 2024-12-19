/* sampleIdle.c */
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

float height = 8.0;		/* 高さ */
float y_max  = 8.0;		/* 初期位置 */
float coeff  = 0.5;		/* 弾性係数 */
float gravt  = -9.81; 		/* 重力加速度 */
float dt   = 0.05;		/* 時間の刻み */
float vz   = 0.0;		/* 初速度 */
float eps  = 0.001;
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
    glutSolidSphere(1.0, 20, 20);
  glPopMatrix();

  glutSwapBuffers();
}

void idle(void)
{
  if (flag == 1 ) {
    height = height + vz*dt;
    vz     = vz + gravt*dt;
    if( hFlag == 0 && height < 0.0 ) {
      vz = -coeff*vz;
      hFlag = 1;
    } else if ( hFlag == 1 && vz < 0.0 ) {
      hFlag = 0;
    }
    if( fabs(vz) <  eps  ) {
      height = 0;
      flag = 0;
    }
  }
    
  glutPostRedisplay();      /* ディスプレイコールバック関数(display)の実行 */
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
  glutIdleFunc(idle);
  glutMainLoop();
  return 0;
}
