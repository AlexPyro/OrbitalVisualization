#include <iostream>
#include <freeglut.h>
#include <fstream>
#include <glut.h>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cstdlib>

struct Point {
    double x, y, z;
};

// Variables pour la position de la caméra
GLfloat cameraPosX = 0.0f;
GLfloat cameraPosY = 0.0f;
GLfloat cameraPosZ = 30.0f;  // Augmenter le recul initial

// Variables pour l'orientation de la caméra
GLfloat cameraYaw = 0.0f;
GLfloat cameraPitch = 0.0f;
GLfloat cameraRoll = 0.0f;

void clearConsole()
{
    system("cls");
}

void updateCamera()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    clearConsole();

    std::cout << "Camera Roll : " << cameraRoll << " degres";
    std::cout << "\n\nCamera Roll : " << cameraPitch << " degres";
    std::cout << "\n\nZoom : " << cameraPosZ;

    const GLfloat distanceToCenter = 10.0f;
    const GLfloat cameraPosXTranslated = cameraPosX;
    const GLfloat cameraPosYTranslated = cameraPosY;
    const GLfloat cameraPosZTranslated = cameraPosZ - distanceToCenter;

    // Appliquer la translation vers le point (0, 0, r)
    glTranslatef(-cameraPosXTranslated, -cameraPosYTranslated, -cameraPosZTranslated);

    // Appliquer les rotations autour du point (0, 0, 0)
    gluLookAt(
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 20.0f,
        0.0f, 1.0f, 0.0f);

    // Inverser la translation
    glTranslatef(cameraPosXTranslated, cameraPosYTranslated, cameraPosZTranslated);

    // Appliquer la rotation à la sphère elle-même
    glRotatef(-cameraRoll, 0.0f, 1.0f, 0.0f); // Rotation autour de l'axe Y
    glRotatef(-cameraPitch, 1.0f, 0.0f, 0.0f); // Rotation autour de l'axe X
}

void specialKeys(int key, int x, int y)
{
    const GLfloat cameraSpeed = 0.5f;

    switch (key) {
    case GLUT_KEY_UP:

        cameraPitch += 3 * cameraSpeed;
        break;
    case GLUT_KEY_DOWN:

        cameraPitch -= 3 * cameraSpeed;
        break;
    case GLUT_KEY_LEFT:

        cameraRoll += 3 * cameraSpeed;
        break;
    case GLUT_KEY_RIGHT:

        cameraRoll -= 3 * cameraSpeed;
        break;
    }

    updateCamera();
    glutPostRedisplay();
}

std::vector<Point> readEllipsePoints(const char* filename)
{
    std::ifstream inputFile(filename);
    std::vector<Point> points;

    if (!inputFile.is_open())
    {
        std::cerr << "Erreur lors de l'ouverture du fichier." << std::endl;
        return points;
    }

    double x, y, z;

    while (inputFile >> x >> y >> z)
    {
        points.push_back({ x , y , z  });
    }

    inputFile.close();
    return points;
}

void keyboard(unsigned char key, int x, int y)
{
    const GLfloat zoomSpeed = 0.5f;

    switch (key) {
    case '+':

        cameraPosZ -= zoomSpeed;
        break;
    case '-':

        cameraPosZ += zoomSpeed;
        break;
    }

    updateCamera();
    glutPostRedisplay();
}

void generateEllipsePoints(const char* filename, double a, double b, double c, int numPoints)
{
    std::ofstream outputFile(filename);

    if (!outputFile.is_open())
    {
        std::cerr << "Erreur lors de l'ouverture du fichier." << std::endl;
        return;
    }

    for (int i = 0; i < numPoints; ++i)
    {
        double t = 2.0 * M_PI * static_cast<double>(i) / static_cast<double>(numPoints);

        double x = a * std::cos(t);
        double y = b * std::sin(t);
        double z = c * std::sin(t);

        outputFile << x << " " << y << " " << z << std::endl;
    }

    outputFile.close();
}

void displaySphereEllipse()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Afficher la sphère au centre
    glColor3f(0.0, 0.0, 1.0);
    glPushMatrix();
    glRotatef(-cameraRoll, 0.0f, 1.0f, 0.0f); // Appliquer la même rotation à la sphère
    //glutWireSphere(6.371, 60, 20);              // Rayon de 0.5, avec 20 division verticales et 20 divisions horizontales
    glutSolidSphere(6.371, 60, 20);              // Rayon de 0.5, avec 20 division verticales et 20 divisions horizontales
    glPopMatrix();

    // Afficher la sphère au centre
    glColor3f(0.0, 0.0, 1.0);
    glPushMatrix();
    glRotatef(-cameraRoll, 0.0f, 1.0f, 0.0f); // Appliquer la même rotation à la sphère
    glutWireSphere(6.371, 60, 20);
    glPopMatrix();

    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP); // Relie les points entre eux par des lignes
    for (const auto& point : readEllipsePoints("ellipse_coordinates.txt"))
    {
        glVertex3f(GLfloat(point.x), GLfloat(point.y), GLfloat(point.z));
    }


    glEnd();

    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINE_LOOP);
    for (const auto& point : readEllipsePoints("ellipse_coordinates2.txt"))
    {
        glVertex3f(GLfloat(point.x), GLfloat(point.y), GLfloat(point.z));
    }


    glEnd();

    glutSwapBuffers();
}

int main(int argc, char** argv)
{
    // Générer le fichier avec les coordonnées de l'ellipse
    generateEllipsePoints("ellipse_coordinates.txt", 12.0, 8.0, 3.0, 100);
    generateEllipsePoints("ellipse_coordinates2.txt", 14.0, 10.0, 10.0, 100);

    // Initialiser GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Visualisation de l'Ellipse en 3D");

    // Configurer la fonction d'affichage
    glutDisplayFunc(displaySphereEllipse);

    // Configurer la gestion des touches spéciales
    glutSpecialFunc(specialKeys);

    // Configurer la gestion des touches du clavier
    glutKeyboardFunc(keyboard);

    // Configuration OpenGL
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0, 1.0, 1.0, 1500.0); // Ajustez la distance de la vue et le zoom

    updateCamera();

    // Lancer la boucle principale GLUT
    glutMainLoop();

    return 0;
}