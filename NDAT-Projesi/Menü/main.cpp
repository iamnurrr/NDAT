#include <iostream>
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

//MÝNÝ OYUN KOMUTLARI

// Oyun alanýnýn boyutlarý
const int screenWidth = 800;
const int screenHeight = 600;

// Sepetin boyutlarý ve konumu
const int basketWidth = 100;
const int basketHeight = 20;
int basketX = screenWidth / 2 - basketWidth / 2;
const int basketY = 50;

// Elmalarýn ve üçgenlerin boyutlarý ve hýzlarý
const int appleSize = 40;
float appleSpeed = 4.0f;
const int triangleSize = 40;
float triangleSpeed = 5.0f;
float triangleScale = 1.0f;


// Elmalarýn ve üçgenlerin konumlarý
vector<pair<int, int>> apples;    // Yeþil elmalarýn konumlarý
vector<pair<int, int>> triangles; // Kýrmýzý üçgenlerin konumlarý

// Puanlar, canlar ve oyun durumu
int score = 0;
int lives = 3;
bool gameover = false;

// Klavye kontrolü için kullanýlan fonksiyon
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

// Elmalarýn ve üçgenlerin hareketi ve çarpýþma kontrolü
void minigameupdate(int value) {
    // Elmalarýn hareketi
    int i = 0;
    while (i < apples.size()) {
        apples[i].second -= appleSpeed;

        // Elma sepete deðdi mi kontrolü
        if (apples[i].second <= basketY && apples[i].first >= basketX &&
            apples[i].first <= basketX + basketWidth) {
            score += 25; // Puaný güncelle
            apples.erase(apples.begin() + i); // Elmayý kaldýr
            continue;
        }

        // Elma ekranýn dýþýna çýktý mý kontrolü
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

    // Üçgenlerin hareketi
    i = 0;
    while (i < triangles.size()) {
        triangles[i].second -= triangleSpeed;

        // Üçgen sepete deðdi mi kontrolü
        if (triangles[i].second <= basketY && triangles[i].first >= basketX &&
            triangles[i].first <= basketX + basketWidth) {
            score -= 50; // Puaný azalt
            lives--;
            triangles.erase(triangles.begin() + i); // Üçgeni kaldýr
            continue;
        }

        // Üçgen ekranýn dýþýna çýktý mý kontrolü
        if (triangles[i].second < 0) {
            triangles.erase(triangles.begin() + i);
            continue; // Sonraki iterasyona atla
        }

        i++;
    }

    // Yeni bir üçgen ekle
    if (rand() % 100 < 2) {
        int x = rand() % (screenWidth - triangleSize);
        triangles.push_back(make_pair(x, screenHeight));
    }

    // Oyun durumunu kontrol et
    if (lives == 0) {
        gameover = true;
    }

    // Seviyeye baðlý olarak hýzý, boyutu ve canlarý güncelle
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

    // Oyun devam ediyorsa tekrar güncelle
    if (!gameover) {
        glutTimerFunc(25, minigameupdate, 0);
    }

    glutPostRedisplay();
}

// Oyun alanýný ve nesneleri çizen fonksiyon
void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Sepeti çiz
    glColor3f(1.0f, 1.0f, 1.0f);
    glRectf(basketX, basketY, basketX + basketWidth, basketY + basketHeight);

    // Elmalarý çiz
    glColor3f(0.0f, 1.0f, 0.0f);
    for (int i = 0; i < apples.size(); i++) {
        glRectf(apples[i].first, apples[i].second, apples[i].first + appleSize, apples[i].second + appleSize * 2);
    }

    // Üçgenleri çiz
    glColor3f(1.0f, 0.0f, 0.0f);
    for (int i = 0; i < triangles.size(); i++) {
        glBegin(GL_TRIANGLES);
        glVertex2f(triangles[i].first, triangles[i].second + triangleSize * triangleScale);
        glVertex2f(triangles[i].first + triangleSize * triangleScale, triangles[i].second + triangleSize * triangleScale);
        glVertex2f(triangles[i].first + triangleSize * triangleScale / 2, triangles[i].second);
        glEnd();
    }

    // Puaný ve canlarý ekrana yazdýr
    glColor3f(1.0f, 1.0f, 1.0f);
    string scoreStr = "Score: " + to_string(score) + " Lives: " + to_string(lives);
    glRasterPos2f(10, 10);
    for (int i = 0; i < scoreStr.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, scoreStr[i]);
    }

    // Oyun bitti mi kontrolü
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

// Klavye kontrolü için kullanýlan fonksiyon
void minigamekeyboard(unsigned char key, int x, int y) {
    if (gameover && key == 'r') { // 'R' tuþuna basýldýðýnda oyunu yeniden baþlat
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
float cubePositionZ = -3.0f; // Küpün baþlangýçta ne kadar uzaklýkta olduðunu belirlemek için kullanacaðýz

bool animate = false; // Animasyonun açýk veya kapalý olduðunu belirlemek için kullanacaðýz

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(3.0, 3.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glRotatef(rotationX, 1.0, 0.0, 0.0);
    glRotatef(rotationY, 0.0, 1.0, 0.0);

    glTranslatef(0.0f, 0.0f, cubePositionZ); // Küpün uzaklýðýný güncelleyerek hareket etmesini saðlýyoruz

    glBegin(GL_QUADS);
    // Küpün yüzlerini çizdik, renkleri deðiþtirilmedi
    glColor3f(1.0, 0.0, 0.0); // Kýrmýzý renk
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);

    glColor3f(0.0, 1.0, 0.0); // Yeþil renk
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);

    glColor3f(0.0, 0.0, 1.0); // Mavi renk
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);

    glColor3f(1.0, 1.0, 0.0); // Sarý renk
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
        animate = !animate; // Animasyon durumunu tersine çeviriyoruz (açýksa kapatýyor, kapalýysa açýyor)
        glutPostRedisplay();
    }
}

void update(int value)
{
    if (animate)
    {
        // Küpün Z ekseninde hareket etmesini saðlýyoruz
        cubePositionZ += 0.05f;

        if (cubePositionZ > 0.0f) // Küp ekranýn önüne geldiðinde geriye gönderiyoruz
            cubePositionZ = -3.0f;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // Her 16 ms'de bir update fonksiyonunu çaðýrarak animasyonu güncelliyoruz
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
        glutCreateWindow("MÝNÝ_OYUN");
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
        glutCreateWindow("3D_ANÝMASYONLU_KÜP");

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
