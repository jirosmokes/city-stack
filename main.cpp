#include <GL/freeglut.h>

#include <cmath>
#include <cstdlib>
#include <vector>
#include <ctime> // for random snowflakes

using std::vector;
using std::abs;

const int windowWidth = 800;
const int windowHeight = 600;

void drawCircle(float cx, float cy, float r, int numSegments, float rColor, float gColor, float bColor) {
    glColor3f(rColor, gColor, bColor);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= numSegments; i++) {
        float angle = i * 2.0f * M_PI / numSegments;
        float x = r * cos(angle);
        float y = r * sin(angle);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}

void drawRectangle(float x, float y, float width, float height, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void drawWindow(float x, float y, float width, float height) {
    drawRectangle(x, y, width, height, 0.2f, 0.2f, 0.2f);
    drawRectangle(x + 2.0f, y + 2.0f, width - 4.0f, height - 4.0f, 0.4f, 0.4f, 0.4f);
}

void drawWindows(float x, float y, float width, float height, int rows, int cols) {
    float windowWidth = width / 5.0f;
    float windowHeight = height / 10.0f;
    float horizontalSpacing = (width - (cols * windowWidth)) / (cols + 1);
    float verticalSpacing = (height - (rows * windowHeight)) / (rows + 1);

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            float windowX = x + horizontalSpacing + col * (windowWidth + horizontalSpacing);
            float windowY = y + verticalSpacing + row * (windowHeight + verticalSpacing);
            drawWindow(windowX, windowY, windowWidth, windowHeight);
        }
    }
}
void drawBuildingBase(float x, float y, float width, float height, float r, float g, float b) {
    drawRectangle(x, y, width, height, r, g, b);
}

void drawBuildingRoof(float x, float y, float width, float height, float r, float g, float b) {
    drawRectangle(x, y + height, width, 10.0f, 0.15f, 0.15f, 0.15f);
}

struct Snowflake {
    float x, y;
    float size;
    float speed;
};

vector<Snowflake> snowflakes;

void initSnowflakes(int numSnowflakes) {
    snowflakes.clear();
    for (int i = 0; i < numSnowflakes; ++i) {
        Snowflake snowflake;
        snowflake.x = rand() % windowWidth;
        snowflake.y = rand() % windowHeight;
        snowflake.size = (rand() % 5) + 2;
        snowflake.speed = (rand() % 3) + 1;
        snowflakes.push_back(snowflake);
    }
}

void drawSnowflake(float x, float y, float size) {
    glColor3f(1.0f, 1.0f, 1.0f);
    drawCircle(x, y, size, 12, 1.0f, 1.0f, 1.0f);
}

void updateSnowflakes() {
    for (auto& snowflake : snowflakes) {
        snowflake.y -= snowflake.speed;
        if (snowflake.y < 0) {
            snowflake.y = windowHeight;
            snowflake.x = rand() % windowWidth;
        }
    }
}

void drawSnowflakes() {
    for (const auto& snowflake : snowflakes) {
        drawSnowflake(snowflake.x, snowflake.y, snowflake.size);
    }
}

void drawBuilding(float x, float y, float width, float height, float r, float g, float b) {
    drawBuildingBase(x, y, width, height, r, g, b);
    drawWindows(x, y, width, height, 5, 4);
    drawBuildingRoof(x, y, width, height, r, g, b);
}

void drawTriangle(float x, float y, float base, float height, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y);
    glVertex2f(x - base / 2, y - height);
    glVertex2f(x + base / 2, y - height);
    glEnd();
}

void drawChristmasTree(float x, float y) {
    // draw the trunk
    drawRectangle(x - 5, y, 10, 20, 0.55f, 0.27f, 0.07f); // brown color

    // draw the leaves (three layers)
    drawTriangle(x, y + 20, 30, 30, 0.0f, 0.5f, 0.0f); // green color
    drawTriangle(x, y + 40, 25, 25, 0.0f, 0.5f, 0.0f); // green color
    drawTriangle(x, y + 55, 20, 20, 0.0f, 0.5f, 0.0f); // green color
}



float clampX = 385.0f;
float clampSpeed = 10.0f;

void drawCraneHook(float x, float y) {
    float hookWidth = 20.0f;
    float hookHeight = 60.0f;
    float hookCurveRadius = 15.0f;

    glColor3f(0.3f, 0.3f, 0.3f);
    glLineWidth(4);
    glBegin(GL_LINES);
    glVertex2f(clampX + hookWidth / 2, y + hookHeight + 100.0f);
    glVertex2f(clampX + hookWidth / 2, y + hookHeight);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.7f, 0.7f, 0.7f);
    glVertex2f(clampX, y);
    glColor3f(0.5f, 0.5f, 0.5f);
    glVertex2f(clampX + hookWidth, y);
    glVertex2f(clampX + hookWidth * 0.8f, y + hookHeight);
    glVertex2f(clampX + hookWidth * 0.2f, y + hookHeight);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex2f(clampX + hookWidth / 2, y + hookHeight);
    for (int i = 0; i <= 20; ++i) {
        float angle = i * 2.0f * M_PI / 20;
        float xOffset = hookCurveRadius * cos(angle);
        float yOffset = hookCurveRadius * sin(angle);
        glVertex2f(clampX + hookWidth / 2 + xOffset, y + hookHeight + yOffset);
    }
    glEnd();

    drawCircle(clampX + hookWidth * 0.2f, y + hookHeight * 0.7f, 3.0f, 12, 0.1f, 0.1f, 0.1f);
    drawCircle(clampX + hookWidth * 0.8f, y + hookHeight * 0.7f, 3.0f, 12, 0.1f, 0.1f, 0.1f);
}

void updateClamp(int value) {
    clampX += clampSpeed;
    if (clampX > windowWidth - 40.0f || clampX < 0) {
        clampSpeed = -clampSpeed;
    }
    glutPostRedisplay();
    glutTimerFunc(16, updateClamp, 0);
}

float sunRotationAngle = 0.0f;

void drawSunWithSpikes() {
    glPushMatrix();
    glTranslatef(700, 500, 0);
    glRotatef(sunRotationAngle, 0.0f, 0.0f, 1.0f);
    glTranslatef(-700, -500, 0);

    drawCircle(700, 500, 50, 50, 1.0f, 1.0f, 0.0f);

    float angleIncrement = 2.0f * M_PI / 24;
    for (int i = 0; i < 24; i++) {
        float angle = i * angleIncrement;
        float x1 = 700 + 50 * cos(angle);
        float y1 = 500 + 50 * sin(angle);
        float spikeLength = (i % 2 == 0) ? 70 : 60;
        float x2 = 700 + spikeLength * cos(angle);
        float y2 = 500 + spikeLength * sin(angle);
        float colorFactor = (float)i / 24.0f;

        glColor3f(1.0f, 0.5f * colorFactor, 0.0f);
        glLineWidth(2 + (i % 3));
        glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();
    }

    glPopMatrix();
}

struct FallingHouse {
    float x, y;
    bool isFalling;
};

vector<FallingHouse> fallingHouses;

void drawHouse(float x, float y) {
    drawBuilding(x, y, 50.0f, 40.0f, 0.8f, 0.6f, 0.4f);
}

void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        FallingHouse newHouse;
        newHouse.x = clampX;
        newHouse.y = 450.0f;
        newHouse.isFalling = true;
        fallingHouses.push_back(newHouse);
    }
}

float stackHeight = 100.0f;

void restartGame() {
    fallingHouses.clear();
    stackHeight = 100.0f;
    clampX = 385.0f;
    clampSpeed = 2.0f;
}

void updateHousePositions() {
    for (auto& house : fallingHouses) {
        if (house.isFalling) {
            house.y -= 7.5f;
            bool landed = false;

            if (fallingHouses.size() == 1 && house.y <= 100) {
                house.y = 100;
                landed = true;
            }

            if (!landed) {
                for (auto& otherHouse : fallingHouses) {
                    if (&house != &otherHouse && !otherHouse.isFalling) {
                        if (abs(house.x - otherHouse.x) < 50.0f && house.y <= otherHouse.y + 40.0f) {
                            house.y = otherHouse.y + 40.0f;
                            landed = true;
                            break;
                        }
                    }
                }
            }

            if (landed) {
                house.isFalling = false;
                stackHeight = house.y + 40.0f;
            }
        }
    }

    for (auto& house : fallingHouses) {
        if (house.y <= 0) {
            fallingHouses.clear();
            stackHeight = 100.0f;
            break;
        }
    }
}

void drawCloud(float x, float y, float size) {
    glColor4f(1.0f, 1.0f, 1.0f, 0.7f);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            drawRectangle(x + i * size * 0.6f, y + j * size * 0.6f, size, size, 1.0f, 1.0f, 1.0f);
        }
    }
}

void drawModernBuilding(float x, float y, float width, float height) {
    drawRectangle(x, y, width, height, 0.1f, 0.1f, 0.1f);
    float verticalLineSpacing = width / 10.0f;
    for (float i = x + verticalLineSpacing; i < x + width; i += verticalLineSpacing) {
        drawRectangle(i, y, 2.0f, height, 0.4f, 0.4f, 0.4f);
    }

    glColor3f(0.2f, 0.5f, 0.8f);
    int rows = height / 30;
    int cols = width / 30;
    float windowWidth = width / cols;
    float windowHeight = height / rows;
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            float windowX = x + col * windowWidth;
            float windowY = y + row * windowHeight;
            drawRectangle(windowX, windowY, windowWidth * 0.8f, windowHeight * 0.8f, 0.2f, 0.5f, 0.8f);
        }
    }
}

void drawStylizedSkyBackground() {
    glBegin(GL_QUADS);
    glColor3f(0.7f, 0.9f, 1.0f);
    glVertex2f(0, windowHeight);
    glVertex2f(windowWidth, windowHeight);
    glColor3f(0.2f, 0.4f, 0.7f);
    glVertex2f(windowWidth, 10);
    glVertex2f(0, 10);
    glEnd();

    drawModernBuilding(0, 100, 120, 300);
    drawModernBuilding(150, 100, 130, 350);
    drawModernBuilding(300, 100, 150, 250);
    drawModernBuilding(500, 100, 180, 400);

    drawCloud(50, 500, 30);
    drawCloud(100, 520, 40);
    drawCloud(400, 480, 35);
    drawCloud(350, 495, 35);
    drawCloud(600, 500, 50);
    drawCloud(700, 420, 25);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawStylizedSkyBackground();

    glBegin(GL_QUADS);
    glColor3f(0.6f, 1.0f, 0.6f);
    glVertex2f(0, 0);
    glVertex2f(windowWidth, 0);
    glColor3f(0.2f, 0.6f, 0.2f);
    glVertex2f(windowWidth, 100);
    glVertex2f(0, 100);
    glEnd();

    drawSunWithSpikes();
    updateHousePositions();

    for (auto & house: fallingHouses) {
        drawHouse(house.x, house.y);
    }

    drawCraneHook(clampX, 450.0f);

    sunRotationAngle += 0.5f;
    if (sunRotationAngle >= 360.0f) {
        sunRotationAngle -= 360.0f;
    }

    updateSnowflakes(); // update snowflake positions
    drawSnowflakes(); // draw snowflakes

    drawChristmasTree(100, 100);
    drawChristmasTree(150, 100);
    drawChristmasTree(300, 100);

    drawChristmasTree(500, 100); // draw christmas tree on the right
    drawChristmasTree(550, 100); // draw christmas tree on the right
    drawChristmasTree(700, 100); // draw christmas tree on the right
    drawChristmasTree(750, 100); // draw christmas tree on the right

    glutSwapBuffers();
}

float zoomFactor = 1.0f;

void updateProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float left = 0;
    float right = windowWidth / zoomFactor;
    float bottom = 0;
    float top = windowHeight / zoomFactor;
    glOrtho(left, right, bottom, top, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void handleKeyboard(unsigned char key, int x, int y) {
    if (key == '+') {
        zoomFactor *= 1.1f;
    } else if (key == '-') {
        zoomFactor *= 0.9f;
    }
    updateProjection();
    glutPostRedisplay();
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    updateProjection();
    srand(time(0)); // seed random number generator
    initSnowflakes(100); // initialize 100 snowflakes
}

int main(int argc, char ** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Building Stacking Game");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeyboard);
    glutMouseFunc(mouseClick);
    glutTimerFunc(16, updateClamp, 0);
    glutMainLoop();
    return 0;
}
