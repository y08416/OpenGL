/* kadai15.c */
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>  // スコア表示用

#define imageWidth 8
#define imageHeight 8

/* 既存の定義と変数 */
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

/* ジャンプに関する変数 */
int jumping = 0;              /* ジャンプ中フラグ */
float robotYPosition = 0.0;   /* ロボットの垂直位置 */
float jumpHeight = 2.0;       /* ジャンプの最大高さ */
float jumpDuration = 1.0;     /* ジャンプの総時間（秒） */
float jumpTime = 0.0;         /* ジャンプ開始からの経過時間 */

/* 移動に関する変数 */
float robot_x = 0.0;          /* ロボットのx位置 */
float robot_z = 0.0;          /* ロボットのz位置 */
float target_x = 0.0;         /* 移動先のx位置 */
float target_z = 0.0;         /* 移動先のz位置 */
int moving = 0;               /* 移動中フラグ */
float robot_angle = 0.0;      /* ロボットの回転角度 */
float param = 0.0;            /* 手足の動きを制御する変数 */

/* シューティングゲーム用の新しい変数 */
#define MAX_BULLETS 100
#define MAX_ENEMIES 10

typedef struct {
    float x, y, z;
    float dx, dy, dz;  // 弾の進行方向
    int active;
} Bullet;

typedef struct {
    float x, y, z;
    int active;
} Enemy;

Bullet bullets[MAX_BULLETS];
Enemy enemies[MAX_ENEMIES];
int score = 0;

/* シールドに関する変数 */
int shieldActive = 0;
float shieldDuration = 5.0;   /* シールドの持続時間（秒） */
float shieldCooldown = 10.0;  /* シールドのクールダウン時間（秒） */
float shieldTime = 0.0;       /* シールドがアクティブな経過時間 */
float shieldCooldownTime = 10.0; /* シールドが最初から使用可能になるように設定 */

/* ジェットパックと飛行に関する変数 */
int flying = 0;               /* 飛行中フラグ */
float flySpeed = 0.1;         /* 上昇・下降速度 */
float maxFlyHeight = 10.0;    /* 最大飛行高度 */

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
    glPushMatrix();
    glTranslatef(robot_x, robotYPosition, robot_z); // ロボットの位置とジャンプ・飛行を適用
    glRotatef(robot_angle, 0.0, 1.0, 0.0); // ロボットの回転を適用

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
    glMaterialfv(GL_FRONT, GL_DIFFUSE, headColor); // 肌色
    drawTriangularPrism(0.15, 0.3, 0.15);
    glPopMatrix();

    // 右耳
    glPushMatrix();
    glTranslatef(0.65, 0.25, 0.0);
    glRotatef(30.0, 0.0, 0.0, 1.0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, headColor); // 肌色
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
    glTranslatef(-0.75, 2.0, 0.0); // 肩の位置に合わせる
    glRotatef(sin(param) * 30.0, 1.0, 0.0, 0.0); // 手を振る動き
    float armColor[] = {0.8, 0.8, 0.8, 1.0}; // 薄いグレー（衣服の袖）
    glMaterialfv(GL_FRONT, GL_DIFFUSE, armColor);
    drawCylinderY(0.15, -1.0); // 下方向に描画
    glPopMatrix();

    // 右腕
    glPushMatrix();
    glTranslatef(0.75, 2.0, 0.0);
    glRotatef(-sin(param) * 30.0, 1.0, 0.0, 0.0); // 手を振る動き（逆方向）
    glMaterialfv(GL_FRONT, GL_DIFFUSE, armColor);
    drawCylinderY(0.15, -1.0);
    glPopMatrix();

    // 左脚
    glPushMatrix();
    glTranslatef(-0.375, 0.0, 0.0); // 胴体の幅に合わせて調整
    glRotatef(-sin(param) * 30.0, 1.0, 0.0, 0.0); // 脚を振る動き
    float legColor[] = {0.3, 0.3, 0.3, 1.0}; // 濃いグレー（ズボン）
    glMaterialfv(GL_FRONT, GL_DIFFUSE, legColor);
    drawCylinderY(0.15, -1.0); // 下方向に描画
    glPopMatrix();

    // 右脚
    glPushMatrix();
    glTranslatef(0.375, 0.0, 0.0);
    glRotatef(sin(param) * 30.0, 1.0, 0.0, 0.0); // 脚を振る動き（逆方向）
    glMaterialfv(GL_FRONT, GL_DIFFUSE, legColor);
    drawCylinderY(0.15, -1.0);
    glPopMatrix();

    // ジェットパックを描画
    glPushMatrix();
    glTranslatef(0.0, 1.5, -0.75); // 背中に配置
    float jetpackColor[] = {0.5, 0.5, 0.5, 1.0}; // グレー
    glMaterialfv(GL_FRONT, GL_DIFFUSE, jetpackColor);
    glutSolidCube(0.5);
    glPopMatrix();

    // シールドを描画
    if (shieldActive)
    {
        glPushMatrix();
        float shieldColor[] = {0.0, 0.5, 1.0, 0.3}; // 水色、半透明
        glMaterialfv(GL_FRONT, GL_DIFFUSE, shieldColor);
        glMaterialfv(GL_FRONT, GL_SPECULAR, shieldColor);
        glMaterialf(GL_FRONT, GL_SHININESS, 128.0);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glutSolidSphere(1.5, 32, 32);

        glDisable(GL_BLEND);
        glPopMatrix();
    }

    glPopMatrix();
}

void drawEnemy(Enemy *enemy)
{
    if (!enemy->active) return;
    glPushMatrix();
    glTranslatef(enemy->x, enemy->y, enemy->z);
    float enemyColor[] = {0.8, 0.2, 0.2, 1.0}; // 赤色
    glMaterialfv(GL_FRONT, GL_DIFFUSE, enemyColor);
    glutSolidSphere(0.5, 20, 20);
    glPopMatrix();
}

void drawBullet(Bullet *bullet)
{
    if (!bullet->active) return;
    glPushMatrix();
    glTranslatef(bullet->x, bullet->y, bullet->z);
    float bulletColor[] = {1.0, 1.0, 0.0, 1.0}; // 黄色
    glMaterialfv(GL_FRONT, GL_DIFFUSE, bulletColor);
    glutSolidSphere(0.1, 10, 10);
    glPopMatrix();
}

void updateBullets()
{
    int i, j;
    for (i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i].active)
        {
            bullets[i].x += bullets[i].dx;
            // bullets[i].y += bullets[i].dy; // Y方向の移動はなし
            bullets[i].z += bullets[i].dz;
            // 敵との衝突判定（2D）
            for (j = 0; j < MAX_ENEMIES; j++)
            {
                if (enemies[j].active)
                {
                    float dx = bullets[i].x - enemies[j].x;
                    float dz = bullets[i].z - enemies[j].z;
                    float distance = sqrt(dx * dx + dz * dz);
                    if (distance < (0.1 + 0.5)) // 弾と敵の半径の合計
                    {
                        bullets[i].active = 0;
                        enemies[j].active = 0;
                        score += 10;
                        printf("スコア: %d\n", score);

                        if (score >= 100)
                        {
                            printf("おめでとうございます！スコアが100に達しました。\n");
                            exit(0);
                        }

                        break;
                    }
                }
            }
            // 弾が一定距離を超えたら非アクティブにする
            if (bullets[i].x < -50.0 || bullets[i].x > 50.0 ||
                bullets[i].z < -50.0 || bullets[i].z > 50.0)
            {
                bullets[i].active = 0;
            }
        }
    }
}

void updateEnemies()
{
    int i;
    for (i = 0; i < MAX_ENEMIES; i++)
    {
        if (enemies[i].active)
        {
            // 敵をロボットに向かって移動（XZ平面上のみ）
            float dx = robot_x - enemies[i].x;
            float dz = robot_z - enemies[i].z;
            float horizontal_distance = sqrt(dx * dx + dz * dz);

            if (horizontal_distance > 0)
            {
                enemies[i].x += 0.02 * (dx / horizontal_distance);
                enemies[i].z += 0.02 * (dz / horizontal_distance);
                // 敵はY方向には移動しない
            }

            // 垂直距離を計算
            float dy = (robotYPosition + 1.0) - enemies[i].y; // ロボットの高さを考慮
            float vertical_distance = fabs(dy);

            // ロボットとの衝突判定（水平距離と垂直距離の両方を考慮）
            if (horizontal_distance < 1.0 && vertical_distance < 1.5)
            {
                if (!shieldActive)
                {
                    // ゲームオーバー
                    printf("ゲームオーバー！最終スコア: %d\n", score);
                    exit(0);
                }
                else
                {
                    // 敵を弾き返す
                    enemies[i].x -= 0.1 * (dx / horizontal_distance);
                    enemies[i].z -= 0.1 * (dz / horizontal_distance);
                }
            }

            // 敵が一定距離を超えたらリセット
            if (enemies[i].x < -50.0 || enemies[i].x > 50.0 ||
                enemies[i].z < -50.0 || enemies[i].z > 50.0)
            {
                enemies[i].x = (rand() % 20) - 10;
                enemies[i].y = 0.0; // Y座標をリセット
                enemies[i].z = -((rand() % 20) + 5);
            }
        }
    }
}

void fireBullet()
{
    int i;
    // 非アクティブな弾を見つけて発射
    for (i = 0; i < MAX_BULLETS; i++)
    {
        if (!bullets[i].active)
        {
            float angle_rad = robot_angle * (M_PI / 180.0);
            float dir_x = sin(angle_rad);
            float dir_z = cos(angle_rad);
            float offset = 0.75;

            bullets[i].x = robot_x + dir_x * offset;
            bullets[i].y = 0.5; // 敵と同じ高さに設定
            bullets[i].z = robot_z + dir_z * offset;
            bullets[i].dx = dir_x * 0.2; // 弾の速度を設定
            bullets[i].dy = 0.0;
            bullets[i].dz = dir_z * 0.2;
            bullets[i].active = 1;
            break;
        }
    }
}

void initGame()
{
    int i;
    // 弾の初期化
    for (i = 0; i < MAX_BULLETS; i++)
    {
        bullets[i].active = 0;
    }

    // 敵の初期化（ランダムな位置に配置）
    for (i = 0; i < MAX_ENEMIES; i++)
    {
        enemies[i].x = (rand() % 20) - 10;
        enemies[i].y = 0.0;
        enemies[i].z = -((rand() % 20) + 5);
        enemies[i].active = 1;
    }
}

void display(void)
{
    int i;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /* zバッファを初期化 */

    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    glEnable(GL_LIGHTING);

    // ロボットを描画
    drawRobot();

    // 敵を描画
    for (i = 0; i < MAX_ENEMIES; i++)
    {
        drawEnemy(&enemies[i]);
    }

    // 弾を描画
    for (i = 0; i < MAX_BULLETS; i++)
    {
        drawBullet(&bullets[i]);
    }

    glDisable(GL_LIGHTING);

    // 床面のテクスチャ
    glEnable(GL_TEXTURE_2D); /* テクスチャを有効にする */
    glNormal3f(0.0, 1.0, 0.0);
    glBegin(GL_QUADS);
    /* テクスチャ座標とポリゴン座標を手動で設定 */
    glTexCoord2f(0.0, 0.0); glVertex3f(-20.0, -2.0, -50.0);
    glTexCoord2f(0.0, 10.0); glVertex3f(-20.0, -2.0, 10.0);
    glTexCoord2f(10.0, 10.0); glVertex3f(20.0, -2.0, 10.0);
    glTexCoord2f(10.0, 0.0); glVertex3f(20.0, -2.0, -50.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glutSwapBuffers();
}

void idle(void)
{
    if (moving)
    {
        float dx = target_x - robot_x;
        float dz = target_z - robot_z;
        float distance = sqrt(dx * dx + dz * dz);

        if (distance > 0.1)
        {
            // ロボットをターゲットに向かって移動
            robot_x += 0.05 * (dx / distance);
            robot_z += 0.05 * (dz / distance);
            robot_angle = atan2(dx, dz) * (180.0 / M_PI); // ロボットの向きを設定
            param += 0.1; // 手足の動きを更新
        }
        else
        {
            // ターゲットに到達したら移動を停止
            moving = 0;
            param = 0.0; // 手足の動きをリセット
        }
    }

    if (headRotating)
    {
        headAngle += 2.0; // 回転角度を増加
        if (headAngle >= 360.0)
        {
            headAngle = 0.0;
            headRotating = 0; // 回転終了
        }
    }

    // シールドのタイマーを更新
    if (shieldActive)
    {
        shieldTime += 0.016; // 約60FPSを想定
        if (shieldTime >= shieldDuration)
        {
            shieldActive = 0;
            shieldTime = 0.0;
            shieldCooldownTime = 0.0; // クールダウン開始
        }
    }
    else
    {
        if (shieldCooldownTime < shieldCooldown)
        {
            shieldCooldownTime += 0.016;
        }
    }

    // 飛行中の高さを更新
    if (flying)
    {
        if (robotYPosition < maxFlyHeight)
        {
            robotYPosition += flySpeed;
        }
    }
    else
    {
        if (robotYPosition > 0.0)
        {
            robotYPosition -= flySpeed;
            if (robotYPosition < 0.0)
                robotYPosition = 0.0;
        }
    }

    updateBullets();
    updateEnemies();

    glutPostRedisplay(); // 再描画
}

void jumpTimer(int value)
{
    if (jumping)
    {
        // 経過時間を更新
        jumpTime += 0.016; // 約60FPSを想定

        // ジャンプの垂直位置を計算（放物線運動をモデル化）
        float t = jumpTime;
        float T = jumpDuration;

        if (t > T) t = T;

        // 放物線の式を使用して高さを計算
        robotYPosition = -4.0 * jumpHeight / (T * T) * (t - T / 2.0) * (t - T / 2.0) + jumpHeight;

        if (jumpTime >= jumpDuration)
        {
            // ジャンプ終了
            jumping = 0;
            robotYPosition = 0.0;
        }
        else
        {
            glutTimerFunc(16, jumpTimer, 0); // 16ms後に再度タイマーを設定
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
    case 'j':
    case 'J':
        if (!jumping && robotYPosition == 0.0 && !flying)
        {
            jumping = 1;
            jumpTime = 0.0;
            glutTimerFunc(0, jumpTimer, 0); // タイマー関数を開始
        }
        break;
    case 's':
    case 'S':
        if (!shieldActive && shieldCooldownTime >= shieldCooldown)
        {
            shieldActive = 1;
            shieldTime = 0.0;
        }
        break;
    case 'f':
    case 'F':
        flying = !flying; // 飛行状態をトグル
        break;
    case ' ':
        fireBullet();
        break;
    default:
        break;
    }
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        // ウィンドウの中央を基準にマウスクリック位置をターゲット位置に変換
        int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
        int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
        target_x = (x - windowWidth / 2) * 0.05;
        target_z = -(windowHeight / 2 - y) * 0.05; // y座標を反転
        moving = 1; // 移動フラグをオンにする
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
    glutInitWindowSize(800, 600);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); /* zバッファ利用の宣言 */
    glutCreateWindow(winname);

    glClearColor(0.2, 0.4, 0.4, 1.0);

    glMatrixMode(GL_PROJECTION); /* 投影法に関する座標変換を開始 */
    glLoadIdentity();
    gluPerspective(45.0, 800.0/600.0, 1.0, 100.0); // 上下開き角を調整
    glMatrixMode(GL_MODELVIEW);           /* 視点の移動やモデルの移動など投影法以外の座標変換を開始 */
    gluLookAt(0.0, 5.0, 15.0, 0.0, 0.0, -5.0, 0.0, 1.0, 0.0);

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
    initGame(); // ゲーム要素の初期化
    glutDisplayFunc(display);
    glutIdleFunc(idle);          /* アイドル関数を登録 */
    glutKeyboardFunc(keyboard);  /* キーボード関数を登録 */
    glutMouseFunc(mouse);        /* マウス関数を登録 */
    glutMainLoop();
    return 0;
}