#pragma once
#include <GL/glut.h>
class modellemesinifi
{

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
            cubePositionZ += 0.01f;

            if (cubePositionZ > 0.0f) // K�p ekran�n �n�ne geldi�inde geriye g�nderiyoruz
                cubePositionZ = -3.0f;
        }

        glutPostRedisplay();
        glutTimerFunc(16, update, 0); // Her 16 ms'de bir update fonksiyonunu �a��rarak animasyonu g�ncelliyoruz
    }

    int main(int argc, char** argv)
    {
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
        glutInitWindowSize(800, 600);
        glutCreateWindow("3D K�p");

        glEnable(GL_DEPTH_TEST);

        glutDisplayFunc(display);
        glutReshapeFunc(reshape);
        glutMouseFunc(mouse);
        glutMotionFunc(mouseMotion);
        glutKeyboardFunc(keyboard); // Klavye i�lemleri i�in keyboard fonksiyonunu ekledik

        glutTimerFunc(0, update, 0); // �lk olarak 0 ms sonra update fonksiyonunu �a��rarak animasyonu ba�lat�yoruz

        glutMainLoop();

        return 0;
    }

};

