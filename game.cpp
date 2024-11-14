#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

using namespace std;

GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

void display()
{
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);

    // draw triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // disable vertex arrays
    glDisableClientState(GL_VERTEX_ARRAY);

    glutSwapBuffers();
}

int main(int argc, char** argv)
{
    // init GLUT and create window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Setup OpenGL");

    // init GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        cerr << "Error: " << glewGetErrorString(err) << endl;
        return -1;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glutDisplayFunc(display);

    glutMainLoop();

    return 0;
}
