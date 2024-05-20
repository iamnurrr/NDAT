#include <iostream>
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

//M�N� OYUN KOMUTLARI

// Oyun alan�n�n boyutlar�
const int screenWidth = 800;
const int screenHeight = 600;

// Sepetin boyutlar� ve konumu
const int basketWidth = 100;
const int basketHeight = 20;
int basketX = screenWidth / 2 - basketWidth / 2;
const int basketY = 50;

// Elmalar�n ve ��genlerin boyutlar� ve h�zlar�
const int appleSize = 40;
float appleSpeed = 4.0f;
const int triangleSize = 40;
float triangleSpeed = 5.0f;
float triangleScale = 1.0f;


// Elmalar�n ve ��genlerin konumlar�
vector<pair<int, int>> apples;    // Ye�il elmalar�n konumlar�
vector<pair<int, int>> triangles; // K�rm�z� ��genlerin konumlar�

// Puanlar, canlar ve oyun durumu
int score = 0;
int lives = 3;
bool gameover = false;

// Klavye kontrol� i�in kullan�lan fonksiyon
void specialKeys(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
        basketX -= 15;
        break;
    case GLUT_KEY_RIGHT:
        basketX += 15;
        break;
    }

    glutPostRedisplay();
}

// Elmalar�n ve ��genlerin hareketi ve �arp��ma kontrol�
void minigameupdate(int value) {
    // Elmalar�n hareketi
    int i = 0;
    while (i < apples.size()) {
        apples[i].second -= appleSpeed;

        // Elma sepete de�di mi kontrol�
        if (apples[i].second <= basketY && apples[i].first >= basketX &&
            apples[i].first <= basketX + basketWidth) {
            score += 25; // Puan� g�ncelle
            apples.erase(apples.begin() + i); // Elmay� kald�r
            continue;
        }

        // Elma ekran�n d���na ��kt� m� kontrol�
        if (apples[i].second < 0) {
            apples.erase(apples.begin() + i);
            continue; // Sonraki iterasyona atla
        }

        i++;
    }

    // Yeni bir elma ekle
    if (rand() % 100 < 2) {
        int x = rand() % (screenWidth - appleSize);
        apples.push_back(make_pair(x, screenHeight));
    }

    // ��genlerin hareketi
    i = 0;
    while (i < triangles.size()) {
        triangles[i].second -= triangleSpeed;

        // ��gen sepete de�di mi kontrol�
        if (triangles[i].second <= basketY && triangles[i].first >= basketX &&
            triangles[i].first <= basketX + basketWidth) {
            score -= 50; // Puan� azalt
            lives--;
            triangles.erase(triangles.begin() + i); // ��geni kald�r
            continue;
        }

        // ��gen ekran�n d���na ��kt� m� kontrol�
        if (triangles[i].second < 0) {
            triangles.erase(triangles.begin() + i);
            continue; // Sonraki iterasyona atla
        }

        i++;
    }

    // Yeni bir ��gen ekle
    if (rand() % 100 < 2) {
        int x = rand() % (screenWidth - triangleSize);
        triangles.push_back(make_pair(x, screenHeight));
    }

    // Oyun durumunu kontrol et
    if (lives == 0) {
        gameover = true;
    }

    // Seviyeye ba�l� olarak h�z�, boyutu ve canlar� g�ncelle
    if (score >= 100 && score < 200) {
        triangleSpeed = 6.0f;
    }
    else if (score >= 200 && score < 300) {
        triangleSpeed = 7.0f;
        appleSpeed = 6.0f;
    }
    else if (score >= 300 && score < 400) {
        triangleSpeed = 8.0f;
        appleSpeed = 7.0f;
    }
    else if (score >= 400 && score < 500) {
        triangleScale = 2.0f;
        appleSpeed = 9.0f;
        triangleSpeed = 9.0f;
    }
    else if (score > 500) {
        triangleScale = 2.50f;
        appleSpeed = 11.0f;
        triangleSpeed = 12.0f;

    }

    // Oyun devam ediyorsa tekrar g�ncelle
    if (!gameover) {
        glutTimerFunc(25, minigameupdate, 0);
    }

    glutPostRedisplay();
}

// Oyun alan�n� ve nesneleri �izen fonksiyon
void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Sepeti �iz
    glColor3f(1.0f, 1.0f, 1.0f);
    glRectf(basketX, basketY, basketX + basketWidth, basketY + basketHeight);

    // Elmalar� �iz
    glColor3f(0.0f, 1.0f, 0.0f);
    for (int i = 0; i < apples.size(); i++) {
        glRectf(apples[i].first, apples[i].second, apples[i].first + appleSize, apples[i].second + appleSize * 2);
    }

    // ��genleri �iz
    glColor3f(1.0f, 0.0f, 0.0f);
    for (int i = 0; i < triangles.size(); i++) {
        glBegin(GL_TRIANGLES);
        glVertex2f(triangles[i].first, triangles[i].second + triangleSize * triangleScale);
        glVertex2f(triangles[i].first + triangleSize * triangleScale, triangles[i].second + triangleSize * triangleScale);
        glVertex2f(triangles[i].first + triangleSize * triangleScale / 2, triangles[i].second);
        glEnd();
    }

    // Puan� ve canlar� ekrana yazd�r
    glColor3f(1.0f, 1.0f, 1.0f);
    string scoreStr = "Score: " + to_string(score) + " Lives: " + to_string(lives);
    glRasterPos2f(10, 10);
    for (int i = 0; i < scoreStr.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, scoreStr[i]);
    }

    // Oyun bitti mi kontrol�
    if (gameover) {
        glColor3f(1.0f, 0.0f, 0.0f);
        string gameOverStr = "Game Over! Press 'R' to restart.";
        glRasterPos2f(screenWidth / 2 - 150, screenHeight / 2);
        for (int i = 0; i < gameOverStr.length(); i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, gameOverStr[i]);
        }
    }

    glutSwapBuffers();
}

// Klavye kontrol� i�in kullan�lan fonksiyon
void minigamekeyboard(unsigned char key, int x, int y) {
    if (gameover && key == 'r') { // 'R' tu�una bas�ld���nda oyunu yeniden ba�lat
        score = 0;
        lives = 3;
        gameover = false;
        apples.clear();
        triangles.clear();
        triangleScale = 1.0f;

        appleSpeed = 4.0f;
        triangleSpeed = 5.0f;
        triangleScale = 1.0f;

        glutTimerFunc(25, minigameupdate, 0);
    }
}


//**************************************************************************************************************
//3D MODELLEME KOMUTLARI 
int lastMouseX, lastMouseY;
float rotationX = 0.0f;
float rotationY = 0.0f;
float cubePositionZ = -3.0f; // K�p�n ba�lang��ta ne kadar uzakl�kta oldu�unu belirlemek i�in kullanaca��z

bool animate = false; // Animasyonun a��k veya kapal� oldu�unu belirlemek i�in kullanaca��z

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(3.0, 3.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glRotatef(rotationX, 1.0, 0.0, 0.0);
    glRotatef(rotationY, 0.0, 1.0, 0.0);

    glTranslatef(0.0f, 0.0f, cubePositionZ); // K�p�n uzakl���n� g�ncelleyerek hareket etmesini sa�l�yoruz

    glBegin(GL_QUADS);
    // K�p�n y�zlerini �izdik, renkleri de�i�tirilmedi
    glColor3f(1.0, 0.0, 0.0); // K�rm�z� renk
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);

    glColor3f(0.0, 1.0, 0.0); // Ye�il renk
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);

    glColor3f(0.0, 0.0, 1.0); // Mavi renk
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);

    glColor3f(1.0, 1.0, 0.0); // Sar� renk
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(-0.5, -0.5, -0.5);

    glColor3f(1.0, 0.0, 1.0); // Mor renk
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(-0.5, -0.5, -0.5);

    glColor3f(0.0, 1.0, 1.0); // Cyan renk
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, -0.5, -0.5);

    glEnd();

    glFlush();
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / (double)height, 1.0, 10.0);
}

void mouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
    {
        lastMouseX = x;
        lastMouseY = y;
    }
}

void mouseMotion(int x, int y)
{
    rotationX += (y - lastMouseY);
    rotationY += (x - lastMouseX);

    lastMouseX = x;
    lastMouseY = y;

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 'a' || key == 'A')
    {
        animate = !animate; // Animasyon durumunu tersine �eviriyoruz (a��ksa kapat�yor, kapal�ysa a��yor)
        glutPostRedisplay();
    }
}

void update(int value)
{
    if (animate)
    {
        // K�p�n Z ekseninde hareket etmesini sa�l�yoruz
        cubePositionZ += 0.05f;

        if (cubePositionZ > 0.0f) // K�p ekran�n �n�ne geldi�inde geriye g�nderiyoruz
            cubePositionZ = -3.0f;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // Her 16 ms'de bir update fonksiyonunu �a��rarak animasyonu g�ncelliyoruz
}

//****************************************************************************************************************

int main(int argc, char** argv) {
    int secim;

    std::cout << "Menu\n";
    std::cout << "1- MINI OYUN\n";
    std::cout << "2- 3D MODELLEME\n";
    std::cout << "Seciminizi yapin: ";
    std::cin >> secim;

    switch (secim) {
    case 1:
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
        glutInitWindowSize(screenWidth, screenHeight);
        glutCreateWindow("M�N�_OYUN");
        glutDisplayFunc(drawScene);
        glutSpecialFunc(specialKeys);
        glutKeyboardFunc(minigamekeyboard);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        gluOrtho2D(0, screenWidth, 0, screenHeight);

        srand(time(0));

        glutTimerFunc(25, minigameupdate, 0);

        glutMainLoop();
        break;
    case 2:
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
        glutInitWindowSize(800, 600);
        glutCreateWindow("3D_AN�MASYONLU_K�P");

        glEnable(GL_DEPTH_TEST);

        glutDisplayFunc(display);
        glutReshapeFunc(reshape);
        glutMouseFunc(mouse);
        glutMotionFunc(mouseMotion);
        glutKeyboardFunc(keyboard);

        glutTimerFunc(0, update, 0);

        glutMainLoop();
        break;
    default:
        std::cout << "Hatali secim!\n";
        break;
    }

    return 0;
}
