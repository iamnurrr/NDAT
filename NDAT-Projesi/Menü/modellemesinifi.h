#pragma once
#include <GL/glut.h>
class modellemesinifi
{

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
            cubePositionZ += 0.01f;

            if (cubePositionZ > 0.0f) // Küp ekranýn önüne geldiðinde geriye gönderiyoruz
                cubePositionZ = -3.0f;
        }

        glutPostRedisplay();
        glutTimerFunc(16, update, 0); // Her 16 ms'de bir update fonksiyonunu çaðýrarak animasyonu güncelliyoruz
    }

    int main(int argc, char** argv)
    {
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
        glutInitWindowSize(800, 600);
        glutCreateWindow("3D Küp");

        glEnable(GL_DEPTH_TEST);

        glutDisplayFunc(display);
        glutReshapeFunc(reshape);
        glutMouseFunc(mouse);
        glutMotionFunc(mouseMotion);
        glutKeyboardFunc(keyboard); // Klavye iþlemleri için keyboard fonksiyonunu ekledik

        glutTimerFunc(0, update, 0); // Ýlk olarak 0 ms sonra update fonksiyonunu çaðýrarak animasyonu baþlatýyoruz

        glutMainLoop();

        return 0;
    }

};

