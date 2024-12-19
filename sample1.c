


/* sample1.c */
#include <GL/glut.h>               /* glutヘッダファイルのインクルード */



void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);    /* ウィンドウの背景を指定された色で塗りつぶす */
  glFlush();                       /* OpenGLコマンドを強制的に実行（描画実行） */
}

void init(char *winname)
{
  glutInitWindowPosition(0, 0);    /* ウィンドウの左上の位置を(0,0)とする */
  glutInitWindowSize(500, 500);    /* ウィンドウのサイズを500×500ドットとする */
  glutInitDisplayMode(GLUT_RGBA);  /* 色の指定にRGBAモードを用いる */
  glutCreateWindow(winname);       /* winnameで指定された名前でウィンドウを開く */

  glClearColor(0.0, 0.0, 0.0, 1.0);/* ウィンドウの背景色の指定 */
                                   /* R(赤),G(緑),B(青),A(透明度)の順で指定*/
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);           /* glutの初期化 */
  init(argv[0]);
  glutDisplayFunc(display);        /* ディスプレイコールバック関数の指定 */
  glutMainLoop();                  /* イベント待ちの無限ループへ入る */
  return 0;                        /* ループが終わったら0を返して終了 */
}
