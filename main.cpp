#include <GL/glew.h>
#include <GL/freeglut.h>

#include <cmath>
#include <cstdlib>
#include <vector>
#include <ctime> // for random snowflakes
#include <stdio.h> // fprintf and stderr

using std::vector;
using std::abs;

const int windowWidth = 800;
const int windowHeight = 600;
GLuint circleVBO, circleVAO;
const int numSegments = 50;
GLuint shaderProgram;

void initCircleVBO() {
    // Prepare vertices for a circle (center + segments)
    vector<float> circleVertices;
    circleVertices.push_back(0.0f); // center x
    circleVertices.push_back(0.0f); // center y
    circleVertices.push_back(0.0f); // center z
    circleVertices.push_back(1.0f); // Color R
    circleVertices.push_back(1.0f); // Color G
    circleVertices.push_back(1.0f); // Color B

    for (int i = 0; i <= numSegments; i++) {
        float angle = i * 2.0f * M_PI / numSegments;
        float x = cos(angle);
        float y = sin(angle);
        circleVertices.push_back(x); // vertex x
        circleVertices.push_back(y); // vertex y
        circleVertices.push_back(0.0f); // vertex z
        circleVertices.push_back(1.0f); // Color R
        circleVertices.push_back(1.0f); // Color G
        circleVertices.push_back(1.0f); // Color B
    }

    // Generate VBO and VAO
    glGenBuffers(1, &circleVBO);
    glGenVertexArrays(1, &circleVAO);

    glBindVertexArray(circleVAO);

    glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
    glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(float), circleVertices.data(), GL_STATIC_DRAW);

    // Vertex attribute for position (x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Vertex attribute for color (r, g, b)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void drawCircle(float cx, float cy, float r, int segments = 12, float rColor = 1.0f, float gColor = 1.0f, float bColor = 1.0f) {
    glBindVertexArray(circleVAO);
    glUseProgram(shaderProgram); // Use the shader program
    glUniform3f(glGetUniformLocation(shaderProgram, "color"), rColor, gColor, bColor); // Set color uniform

    float model[16] = {
        r, 0.0f, 0.0f, 0.0f,
        0.0f, r, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        cx, cy, 0.0f, 1.0f
    };
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, model);

    glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments + 2); // Draw the circle

    glBindVertexArray(0);
    glUseProgram(0); // Unbind the shader program
}

GLuint rectVBO, rectVAO;

void initRectangleVBO() {
    // Define the vertices for a rectangle (2 triangles)
    vector<float> rectVertices = {
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // Bottom-left corner
        1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // Bottom-right corner
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // Top-right corner
        0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f   // Top-left corner
    };

    // Generate VBO and VAO
    glGenBuffers(1, &rectVBO);
    glGenVertexArrays(1, &rectVAO);

    glBindVertexArray(rectVAO);

    glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
    glBufferData(GL_ARRAY_BUFFER, rectVertices.size() * sizeof(float), rectVertices.data(), GL_STATIC_DRAW);

    // Vertex attribute for position (x, y)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Vertex attribute for color (r, g, b)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void drawRectangle(float x, float y, float width, float height, float r, float g, float b) {
    glBindVertexArray(rectVAO);
    glUseProgram(shaderProgram); // Use the shader program
    glUniform3f(glGetUniformLocation(shaderProgram, "color"), r, g, b); // Set color uniform

    float model[16] = {
        width, 0.0f, 0.0f, 0.0f,
        0.0f, height, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        x, y, 0.0f, 1.0f
    };
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, model);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // Draw the rectangle

    glBindVertexArray(0);
    glUseProgram(0); // Unbind the shader program
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
    drawCircle(x, y, size);
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
    glUseProgram(shaderProgram); // Use the shader program
    glUniform3f(glGetUniformLocation(shaderProgram, "color"), r, g, b); // Set color uniform

    float model[16] = {
        base, 0.0f, 0.0f, 0.0f,
        0.0f, height, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        x, y, 0.0f, 1.0f
    };
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, model);

    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(-0.5f, -1.0f);
    glVertex2f(0.5f, -1.0f);
    glEnd();

    glUseProgram(0); // Unbind the shader program
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

    glUseProgram(shaderProgram); // Use the shader program

    // Draw the line
    glUniform3f(glGetUniformLocation(shaderProgram, "color"), 0.3f, 0.3f, 0.3f); // Set color uniform
    float model[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        clampX + hookWidth / 2, y + hookHeight + 100.0f, 0.0f, 1.0f
    };
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, model);
    glLineWidth(4);
    glBegin(GL_LINES);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.0f, -100.0f);
    glEnd();

    // Draw the hook body
    glUniform3f(glGetUniformLocation(shaderProgram, "color"), 0.7f, 0.7f, 0.7f); // Set color uniform
    model[12] = clampX;
    model[13] = y;
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, model);
    glBegin(GL_POLYGON);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(hookWidth, 0.0f);
    glVertex2f(hookWidth * 0.8f, hookHeight);
    glVertex2f(hookWidth * 0.2f, hookHeight);
    glEnd();

    // Draw the hook curve
    glUniform3f(glGetUniformLocation(shaderProgram, "color"), 0.4f, 0.4f, 0.4f); // Set color uniform
    model[12] = clampX + hookWidth / 2;
    model[13] = y + hookHeight;
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, model);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    for (int i = 0; i <= 20; ++i) {
        float angle = i * 2.0f * M_PI / 20;
        float xOffset = hookCurveRadius * cos(angle);
        float yOffset = hookCurveRadius * sin(angle);
        glVertex2f(xOffset, yOffset);
    }
    glEnd();

    // Draw the hook circles
    drawCircle(clampX + hookWidth * 0.2f, y + hookHeight * 0.7f, 3.0f, 12, 0.1f, 0.1f, 0.1f);
    drawCircle(clampX + hookWidth * 0.8f, y + hookHeight * 0.7f, 3.0f, 12, 0.1f, 0.1f, 0.1f);

    glUseProgram(0); // Unbind the shader program
}

void updateClamp(int value) {
    clampX += clampSpeed;
    if (clampX > windowWidth - 40.0f || clampX < 0) {
        clampSpeed = -clampSpeed;
    }
    glutPostRedisplay();
    glutTimerFunc(16, updateClamp, 0);
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

GLuint bgVBO, bgVAO;
GLuint groundVBO, groundVAO;

void initBackgroundVBO() {
    // Define the vertices for the background (2 triangles forming a quad)
    vector<float> bgVertices = {
        0.0f, 0.0f, 0.0f, // Bottom-left corner
        windowWidth, 0.0f, 0.0f, // Bottom-right corner
        windowWidth, windowHeight, 0.0f, // Top-right corner
        0.0f, windowHeight, 0.0f // Top-left corner
    };

    // Generate VBO and VAO
    glGenBuffers(1, &bgVBO);
    glGenVertexArrays(1, &bgVAO);

    glBindVertexArray(bgVAO);

    glBindBuffer(GL_ARRAY_BUFFER, bgVBO);
    glBufferData(GL_ARRAY_BUFFER, bgVertices.size() * sizeof(float), bgVertices.data(), GL_STATIC_DRAW);

    // Vertex attribute for position (x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void initGroundVBO() {
    // Define the vertices for the ground (2 triangles forming a quad)
    vector<float> groundVertices = {
        0.0f, 0.0f, 0.0f, // Bottom-left corner
        windowWidth, 0.0f, 0.0f, // Bottom-right corner
        windowWidth, 100.0f, 0.0f, // Top-right corner
        0.0f, 100.0f, 0.0f // Top-left corner
    };

    // Generate VBO and VAO
    glGenBuffers(1, &groundVBO);
    glGenVertexArrays(1, &groundVAO);

    glBindVertexArray(groundVAO);

    glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
    glBufferData(GL_ARRAY_BUFFER, groundVertices.size() * sizeof(float), groundVertices.data(), GL_STATIC_DRAW);

    // Vertex attribute for position (x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void drawBackground() {
    glBindVertexArray(bgVAO);
    glUseProgram(shaderProgram); // Use the shader program
    glUniform3f(glGetUniformLocation(shaderProgram, "color"), 0.7f, 0.9f, 1.0f); // Set color uniform

    float model[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, model);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // Draw the background

    glBindVertexArray(0);
    glUseProgram(0); // Unbind the shader program
}

void drawGround() {
    glBindVertexArray(groundVAO);
    glUseProgram(shaderProgram); // Use the shader program
    glUniform3f(glGetUniformLocation(shaderProgram, "color"), 0.6f, 1.0f, 0.6f); // Set color uniform

    float model[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, model);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // Draw the ground

    glBindVertexArray(0);
    glUseProgram(0); // Unbind the shader program
}

void drawStylizedSkyBackground() {
    drawBackground();
    drawGround();

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

float sunRotationAngle = 0.0f;

void drawPixelatedSun(float x, float y, float size) {
    float pixelSize = size / 8.0f;
    float colors[4][3] = {
        {1.0f, 1.0f, 0.0f}, // Yellow
        {1.0f, 0.9f, 0.0f}, // Light Orange
        {1.0f, 0.8f, 0.0f}, // Orange
        {1.0f, 0.7f, 0.0f}  // Dark Orange
    };

    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glRotatef(sunRotationAngle, 0.0f, 0.0f, 1.0f);
    glTranslatef(-x, -y, 0.0f);

    for (int i = -6; i < 6; ++i) {
        for (int j = -6; j < 6; ++j) {
            int distance = abs(i) + abs(j);
            int colorIndex = distance < 3 ? 0 : (distance < 5 ? 1 : (distance < 7 ? 2 : 3));
            drawRectangle(x + i * pixelSize, y + j * pixelSize, pixelSize, pixelSize, colors[colorIndex][0], colors[colorIndex][1], colors[colorIndex][2]);
        }
    }

    glPopMatrix();
}

void updateSunRotation(int value) {
    sunRotationAngle += 1.0f;
    if (sunRotationAngle >= 360.0f) {
        sunRotationAngle -= 360.0f;
    }
    glutPostRedisplay();
    glutTimerFunc(16, updateSunRotation, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawStylizedSkyBackground();

    // Draw the sun
    drawPixelatedSun(700, 500, 75);

    updateHousePositions();

    for (auto & house: fallingHouses) {
        drawHouse(house.x, house.y);
    }

    drawCraneHook(clampX, 450.0f);

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
    glUseProgram(shaderProgram); // Use the shader program
    float left = 0;
    float right = windowWidth / zoomFactor;
    float bottom = 0;
    float top = windowHeight / zoomFactor;
    float projection[16] = {
        2.0f / (right - left), 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f / (top - bottom), 0.0f, 0.0f,
        0.0f, 0.0f, -1.0f, 0.0f,
        -(right + left) / (right - left), -(top + bottom) / (top - bottom), 0.0f, 1.0f
    };
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, projection);
    glUseProgram(0); // Unbind the shader program
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

void checkShaderCompilation(GLuint shader) {
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);
    }
}

void checkProgramLinking(GLuint program) {
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    }
}

void initShaders() {
    // Vertex shader
    const char* vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec3 aColor;
        uniform mat4 model;
        uniform mat4 projection;
        void main() {
            gl_Position = projection * model * vec4(aPos, 1.0);
        }
    )";

    // Fragment shader
    const char* fragmentShaderSource = R"(
        #version 330 core
        uniform vec3 color;
        out vec4 FragColor;
        void main() {
            FragColor = vec4(color, 1.0);
        }
    )";

    // Compile shaders and link program
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    checkShaderCompilation(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    checkShaderCompilation(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkProgramLinking(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void init() {
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        exit(1);
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    initShaders(); // Initialize shaders
    updateProjection(); 
    srand(time(0)); // seed random number generator
    initSnowflakes(100); // initialize 100 snowflakes
    initCircleVBO(); // Initialize Circle VBO
    initRectangleVBO(); // Initialize Rectangle VBO
    initBackgroundVBO(); // Initialize Background VBO
    initGroundVBO(); // Initialize Ground VBO
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
    glutTimerFunc(16, updateSunRotation, 0);
    glutMainLoop();
    return 0;
}
