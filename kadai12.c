/* kadai12.c */
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

#define imageWidth 8
#define imageHeight 8

float diffuse[] = {0.8, 0.8, 0.8, 1.0};  /* 拡散反射成分の反射強度 */
float specular[] = {1.0, 1.0, 1.0, 1.0}; /* 鏡面反射成分の反射強度 */
float ambient[] = {0.2, 0.2, 0.2, 1.0};  /* 環境光成分の反射強度 */
float shininess = 128.0;
float light[] = {1.5, 1.5, 0.0, 0.0};    /* 光源の位置と種類 (平行光源) */
float emission[] = {1.0, 1.0, 0.0, 1.0}; /* 放射光の属性 */

unsigned char image1[imageHeight][imageWidth][4]; /* 画像データ */

unsigned char texture1[imageHeight][imageWidth] = /* texture1の定義 */
{
    {255, 255, 255, 255, 0, 0, 0, 0},
    {255, 255, 255, 255, 0, 0, 0, 0},
    {255, 255, 255, 255, 0, 0, 0, 0},
    {255, 255, 255, 255, 0, 0, 0, 0},
    {0, 0, 0, 0, 255, 255, 255, 255},
    {0, 0, 0, 0, 255, 255, 255, 255},
    {0, 0, 0, 0, 255, 255, 255, 255},
    {0, 0, 0, 0, 255, 255, 255, 255}
};

int headRotating = 0;    /* 頭の回転中フラグ */
float headAngle = 0.0;   /* 頭の回転角度 */

void makeImage1(void) /* texture1 をimage1に登録 */
{
    int i, j;
    for (i = 0; i < imageHeight; i++)
    {
        for (j = 0; j < imageWidth; j++)
        {
            image1[i][j][0] = (unsigned char)texture1[i][j];
            image1[i][j][1] = (unsigned char)texture1[i][j];
            image1[i][j][2] = (unsigned char)texture1[i][j];
            image1[i][j][3] = (unsigned char)100;
        }
    }
}

void drawTriangularPrism(float base, float height, float depth)
{
    glBegin(GL_TRIANGLES);
    // 前面
    glVertex3f(-base / 2, 0.0, depth / 2);
    glVertex3f(base / 2, 0.0, depth / 2);
    glVertex3f(0.0, height, depth / 2);

    // 背面
    glVertex3f(-base / 2, 0.0, -depth / 2);
    glVertex3f(base / 2, 0.0, -depth / 2);
    glVertex3f(0.0, height, -depth / 2);
    glEnd();

    glBegin(GL_QUADS);
    // 側面1
    glVertex3f(-base / 2, 0.0, depth / 2);
    glVertex3f(-base / 2, 0.0, -depth / 2);
    glVertex3f(0.0, height, -depth / 2);
    glVertex3f(0.0, height, depth / 2);

    // 側面2
    glVertex3f(base / 2, 0.0, depth / 2);
    glVertex3f(base / 2, 0.0, -depth / 2);
    glVertex3f(0.0, height, -depth / 2);
    glVertex3f(0.0, height, depth / 2);

    // 底面
    glVertex3f(-base / 2, 0.0, depth / 2);
    glVertex3f(base / 2, 0.0, depth / 2);
    glVertex3f(base / 2, 0.0, -depth / 2);
    glVertex3f(-base / 2, 0.0, -depth / 2);
    glEnd();
}

void drawCylinderY(float radius, float height)
{
    GLUquadricObj *quadric = gluNewQuadric();
    glPushMatrix();
    if (height < 0)
    {
        glRotatef(180.0, 1.0, 0.0, 0.0); // マイナスの高さの場合、下向きに描画
        height = -height;
    }
    glRotatef(-90.0, 1.0, 0.0, 0.0); // 円柱をY軸に沿わせる
    gluCylinder(quadric, radius, radius, height, 32, 32);
    // 底面のキャップ
    gluDisk(quadric, 0.0, radius, 32, 1);
    // 上面のキャップ
    glTranslatef(0.0, 0.0, height);
    gluDisk(quadric, 0.0, radius, 32, 1);
    glPopMatrix();
    gluDeleteQuadric(quadric);
}

void drawRobot()
{
    // 頭部
    glPushMatrix();
    glTranslatef(0.0, 2.5, 0.0);
    glRotatef(headAngle, 0.0, 1.0, 0.0); // 頭の回転

    // 頭（立方体）
    float headColor[] = {0.9, 0.8, 0.7, 1.0}; // 肌色
    glMaterialfv(GL_FRONT, GL_DIFFUSE, headColor);
    glutSolidCube(1.0);

    // 左目
    glPushMatrix();
    glTranslatef(-0.25, 0.25, 0.5);
    float eyeColor[] = {0.0, 0.0, 0.0, 1.0}; // 黒
    glMaterialfv(GL_FRONT, GL_DIFFUSE, eyeColor);
    glutSolidSphere(0.1, 10, 10);
    glPopMatrix();

    // 右目
    glPushMatrix();
    glTranslatef(0.25, 0.25, 0.5);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, eyeColor);
    glutSolidSphere(0.1, 10, 10);
    glPopMatrix();

    // 口
    glPushMatrix();
    glTranslatef(0.0, -0.1, 0.55);
    float mouthColor[] = {1.0, 0.6, 0.6, 1.0}; // 薄い赤
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mouthColor);
    glutSolidCube(0.1);
    glPopMatrix();

    // 左耳
    glPushMatrix();
    glTranslatef(-0.65, 0.25, 0.0);
    glRotatef(-30.0, 0.0, 0.0, 1.0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, headColor);
    drawTriangularPrism(0.15, 0.3, 0.15);
    glPopMatrix();

    // 右耳
    glPushMatrix();
    glTranslatef(0.65, 0.25, 0.0);
    glRotatef(30.0, 0.0, 0.0, 1.0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, headColor);
    drawTriangularPrism(0.15, 0.3, 0.15);
    glPopMatrix();

    glPopMatrix();

    // 胴体
    glPushMatrix();
    glTranslatef(0.0, 1.0, 0.0);
    glScalef(1.5, 2.0, 1.0);
    float bodyColor[] = {0.4, 0.6, 0.8, 1.0}; // 明るい青（シャツ）
    glMaterialfv(GL_FRONT, GL_DIFFUSE, bodyColor);
    glutSolidCube(1.0);
    glPopMatrix();

    // 左腕
    glPushMatrix();
    glTranslatef(-0.75, 2.0, 0.0);
    float armColor[] = {0.8, 0.8, 0.8, 1.0}; // 薄いグレー（衣服の袖）
    glMaterialfv(GL_FRONT, GL_DIFFUSE, armColor);
    drawCylinderY(0.15, -1.0);
    glPopMatrix();

    // 右腕
    glPushMatrix();
    glTranslatef(0.75, 2.0, 0.0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, armColor);
    drawCylinderY(0.15, -1.0);
    glPopMatrix();

    // 左脚
    glPushMatrix();
    glTranslatef(-0.375, 0.0, 0.0);
    float legColor[] = {0.3, 0.3, 0.3, 1.0}; // 濃いグレー（ズボン）
    glMaterialfv(GL_FRONT, GL_DIFFUSE, legColor);
    drawCylinderY(0.15, -1.0);
    glPopMatrix();

    // 右脚
    glPushMatrix();
    glTranslatef(0.375, 0.0, 0.0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, legColor);
    drawCylinderY(0.15, -1.0);
    glPopMatrix();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /* zバッファを初期化 */

    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    glEnable(GL_LIGHTING);

    // ロボットを描画
    drawRobot();

    glDisable(GL_LIGHTING);

    // 床面のテクスチャ
    glEnable(GL_TEXTURE_2D); /* テクスチャを有効にする */
    glNormal3f(0.0, 1.0, 0.0);
    glBegin(GL_QUADS);
    /* テクスチャ座標とポリゴン座標を手動で設定 */
    glTexCoord2f(0.0, 0.0); glVertex3f(-5.0, -2.0, -5.0);
    glTexCoord2f(0.0, 5.0); glVertex3f(-5.0, -2.0, 5.0);
    glTexCoord2f(5.0, 5.0); glVertex3f(5.0, -2.0, 5.0);
    glTexCoord2f(5.0, 0.0); glVertex3f(5.0, -2.0, -5.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glutSwapBuffers();
}

void idle(void)
{
    if (headRotating)
    {
        headAngle += 2.0; // 回転角度を増加
        if (headAngle >= 360.0)
        {
            headAngle = 0.0;
            headRotating = 0; // 回転終了
        }
        glutPostRedisplay(); // 再描画
    }
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'h':
    case 'H':
        if (!headRotating)
            headRotating = 1; // 回転開始
        break;
    default:
        break;
    }
}

void initTexture1() /* テクスチャの初期設定 */
{
    makeImage1();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); /* テクスチャを1バイトずつ読み込み */
    /* テクスチャの繰り返し方の設定 */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); /* テクスチャを繰り返す */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    /* テクスチャの拡大・縮小の設定 */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); /* 最近傍の画素を元に拡大 */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, image1); /* テクスチャの割り当て */
}

void initLighting() /* 光源の属性の設定 */
{
    float diffuse_light[] = {0.5, 0.5, 0.5, 1.0};  /* 光源の拡散反射成分の強度 */
    float specular_light[] = {0.5, 0.5, 0.5, 1.0}; /* 光源の鏡面反射成分の強度 */
    float ambient_light[] = {0.5, 0.5, 0.5, 1.0};  /* 光源の環境光成分の強度 */

    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);   /* 拡散反射成分の強度 */
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light); /* 鏡面反射成分の強度 */
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);   /* 環境光成分の強度 */
}

void init(char *winname)
{
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(500, 500);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); /* zバッファ利用の宣言 */
    glutCreateWindow(winname);

    glClearColor(0.2, 0.4, 0.4, 1.0);

    glMatrixMode(GL_PROJECTION); /* 投影法に関する座標変換を開始 */
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 50.0); // 上下開き角を調整
    glMatrixMode(GL_MODELVIEW);           /* 視点の移動やモデルの移動など投影法以外の座標変換を開始 */
    gluLookAt(3.0, 4.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glShadeModel(GL_FLAT);   /* シェーディングの種類をフラットシェーディングに設定 */
    glEnable(GL_LIGHT0);     /* LIGHT0の光源を有効にする */
    glEnable(GL_DEPTH_TEST); /* 隠面処理を有効にする */

    initLighting();          /* 光源の設定を初期化 */
    initTexture1();          /* テクスチャの設定を初期化 */
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    init(argv[0]);
    glutDisplayFunc(display);
    glutIdleFunc(idle);          /* アイドル関数を登録 */
    glutKeyboardFunc(keyboard);  /* キーボード関数を登録 */
    glutMainLoop();
    return 0;
}