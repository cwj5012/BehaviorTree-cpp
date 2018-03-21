#include "draw.h"

#include <string>
#include <vector>

#define FONT GLUT_BITMAP_8_BY_13

const float DEG2RAD = 3.14159f / 180.0f;

const float BG_R_COLOR = 1;
const float BG_G_COLOR = 1;
const float BG_B_COLOR = 1;

const GLfloat NODE_X_MARGIN = 0.06f;

const float MOVE_FRACTION = 0.1f;

BT::ControlNode *tree;

float node_x = 0.0;
float node_y = 0.4;

float x_offset = 0.01;
float y_offset = 0.15;

int depth;
bool init = false;

// Draw Ellipse
void drawEllipse(float xpos, float ypos, float xradius, float yradius) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 359; i++) {
        // convert degrees into radians
        float degInRad = i * DEG2RAD;
        glVertex2d(xpos + cos(degInRad) * xradius,
                   ypos + sin(degInRad) * yradius);
    }
    glEnd();
}

// Draw a Circle
void drawCircle(float radius) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i <= 360; i++) {
        float degInRad = i * 3.14142f / 180.0f;
        glVertex2f((GLfloat) cos(degInRad) * radius,
                   (GLfloat) sin(degInRad) * radius);
    }
    glEnd();
}

// Draw text
void drawString(void *font, const char *string, float x, float y, float z) {
    renderBitmapString(x, y, font, string);
}

// Get node line number, this will convert to height later
int compute_node_lines(const char *string) {
    const char *c;
    int i = 0;
    int new_line_num = 1;
    glRasterPos2f(node_x, node_y);
    for (c = string; *c != '\0'; c++) {
        if ((*c == '\n') || ((*c == ' ' && i > 6) || i > 9)) {
            new_line_num++;
            i = 0;
            continue;
        }
        i++;
    }
    return new_line_num;
}

// Get node width to show
int compute_max_width(const char *string) {
    const char *current_char;
    int current_line_width = 0;
    int max_width = 0;

    glRasterPos2f(node_x, node_y);
    for (current_char = string; *current_char != '\0'; current_char++) {
        if ((*current_char == '\n') ||
            ((*current_char == ' ' && current_line_width > 6) ||
             current_line_width > 9)) {
            if (current_line_width > max_width) {
                max_width = current_line_width;
            }
            current_line_width = 0;
            continue;
        }
        current_line_width++;
    }

    // if the lable fits in a single line
    if (max_width == 0) {
        max_width = current_line_width;
    }
    return max_width;
}

// Render text
void renderBitmapString(float x, float y, void *font, const char *string) {
    const char *c;
    int i = 0;
    int new_line_num = 0;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        if ((*c == '\n') || ((*c == ' ' && i > 6) || i > 9)) {
            new_line_num++;
            glRasterPos2f(x, y - 0.035f * (new_line_num));
            i = 0;
            continue;
        }
        i++;
        glutBitmapCharacter(font, *c);
    }
}

// Draw all type of node
void
draw_node(float x, float y, int node_type, const char *leafName, int status) {
    auto NODE_WIDTH = 0.04f;
    auto NODE_HEIGHT = 0.02f;
    auto node_pos_x = x + NODE_WIDTH / 2 - 0.005f;
    auto node_pos_y = y - NODE_HEIGHT / 2.0f;
    switch (node_type) {
        case BT::SELECTORSTAR:
            drawString(FONT, "?*", node_pos_x, node_pos_y, 0);
            break;
        case BT::SEQUENCESTAR:
            drawString(FONT, ">*", node_pos_x, node_pos_y, 0);
            break;
        case BT::SELECTOR:
            drawString(FONT, "?", node_pos_x, node_pos_y, 0);
            break;
        case BT::SEQUENCE:
            drawString(FONT, ">", node_pos_x, node_pos_y, 0);
            break;
        case BT::PARALLEL:
            drawString(FONT, "=", node_pos_x, node_pos_y, 0);
            break;
        case BT::DECORATOR:
            drawString(FONT, "D", node_pos_x, node_pos_y, 0);
            break;
        case BT::ACTION:
            NODE_HEIGHT = 0.02f * compute_node_lines(leafName);
            NODE_WIDTH = 0.02f * compute_max_width(leafName);
            break;
        case BT::CONDITION:
            NODE_HEIGHT = 0.02f * compute_node_lines(leafName);
            NODE_WIDTH = 0.02f * compute_max_width(leafName);
            break;
        default:
            break;
    }

    switch (status) {
        case BT::RUNNING:
            glColor3ub(255, 165,
                       0);    // Orange    橙色	    #FFA500	    255,165,0
            break;
        case BT::SUCCESS:
            glColor3ub(0, 255, 0);      // Lime	    闪光绿	    #00FF00 	0,255,0
            break;
        case BT::FAILURE:
            glColor3ub(220, 20, 60);    // Crimson	深红(猩红)	#DC143C	    220,20,60
            break;
        case BT::IDLE:
            glColor3ub(211, 211,
                       211); // LightGrey	浅灰色	    #D3D3D3	    211,211,211
            break;
        case BT::HALTED:
            glColor3ub(0, 191, 255);    // DeepSkyBlue	深天蓝	#00BFFF	    0,191,255
            break;
        default:
            break;
    }

    // glEnable(GL_LINE_SMOOTH);
    // glBegin(GL_LINE_LOOP);
    // glLineWidth(1.5);

    switch (node_type) {
        case BT::CONDITION:
            glBegin(GL_QUADS);
            glVertex2f((GLfloat) (x + NODE_WIDTH), (GLfloat) (y - 0.08));
            glVertex2f((GLfloat) (x + NODE_WIDTH), (GLfloat) (y + 0.015));
            glVertex2f((GLfloat) (x), (GLfloat) (y + 0.015));
            glVertex2f((GLfloat) (x), (GLfloat) (y - 0.08));
            glColor3f(0.0, 0.0, 0.0);
            glEnd();
            break;
        case BT::ACTION:
            glBegin(GL_QUADS);
            glVertex2f((GLfloat) (x + NODE_WIDTH), (GLfloat) (y - 0.08));
            glVertex2f((GLfloat) (x + NODE_WIDTH), (GLfloat) (y + 0.015));
            glVertex2f((GLfloat) (x), (GLfloat) (y + 0.015));
            glVertex2f((GLfloat) (x), (GLfloat) (y - 0.08));
            glColor3f(0.0, 0.0, 0.0);
            glEnd();
            break;
        default:
            glBegin(GL_LINE_LOOP);
            glVertex2f((GLfloat) (x + NODE_WIDTH), (GLfloat) (y - NODE_HEIGHT));
            glVertex2f((GLfloat) (x + NODE_WIDTH), (GLfloat) (y + NODE_HEIGHT));
            glVertex2f((GLfloat) (x), (GLfloat) (y + NODE_HEIGHT));
            glVertex2f((GLfloat) (x), (GLfloat) (y - NODE_HEIGHT));
            glColor3f(0.0, 0.0, 0.0);
            glEnd();
            break;
    }

    // Render text
    switch (node_type) {
        case BT::ACTION:
            renderBitmapString((x + 0.015f), (y - 0.03f), FONT, leafName);
            break;
        case BT::CONDITION:
            renderBitmapString((x + 0.015f), (y - 0.03f), FONT, leafName);
            break;
        default:
            break;
    }
}

// draw the edge connecting one node to the other
void draw_edge(GLfloat parent_x, GLfloat parent_y, GLfloat parent_size,
               GLfloat child_x, GLfloat child_y, GLfloat child_size) {
    glLineWidth(1.0);
    glColor3f(0.0, 0.0, 0.0);

    GLfloat above_spacing = 0.04;

    glBegin(GL_LINES);
    glVertex3f(parent_x, parent_y - parent_size, 0);
    glVertex3f(parent_x, child_y + child_size + above_spacing, 0);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(parent_x, child_y + child_size + above_spacing, 0);
    glVertex3f(child_x, child_y + child_size + above_spacing, 0);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(child_x, child_y + child_size + above_spacing, 0);
    glVertex3f(child_x, child_y + child_size, 0);
    glEnd();
}

// draw the edge connecting one node to the other
void draw_straight_edge(GLfloat parent_x, GLfloat parent_y, GLfloat parent_size,
                        GLfloat child_x, GLfloat child_y, GLfloat child_size) {
    glLineWidth(1.5);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(parent_x, parent_y - parent_size, 0.0);
    glVertex3f(child_x, child_y + child_size, 0);
    glEnd();
}

// Keyboard callback function
void keyboard(unsigned char key, int x, int y) {}

void keyboard_release(unsigned char key, int x, int y) {}

void processSpecialKeys(int key, int xx, int yy) {
    switch (key) {
        case GLUT_KEY_UP :
            node_y += MOVE_FRACTION;
            break;
        case GLUT_KEY_DOWN :
            if (node_y >= 0) { node_y -= MOVE_FRACTION; }
            break;
        case GLUT_KEY_LEFT:
            node_x -= MOVE_FRACTION;
            break;
        case GLUT_KEY_RIGHT:
            node_x += MOVE_FRACTION;
            break;
        default:
            break;
    }
}

// Update tree image
void
updateTree(BT::TreeNode *tree, GLfloat x_pos, GLfloat y_pos, GLfloat y_offset,
           int depth) {
    auto *node = dynamic_cast<BT::ControlNode *> (tree);
    if (node == nullptr) {
        // A leaf node, draw it
        draw_node(x_pos,
                  (GLfloat) y_pos,
                  tree->DrawType(),
                  tree->get_name().c_str(),
                  tree->get_color_status());
    } else {
        // A control flow node, draw it and its children
        draw_node((GLfloat) x_pos,
                  (GLfloat) y_pos,
                  tree->DrawType(),
                  tree->get_name().c_str(),
                  tree->get_color_status());

        std::vector<BT::TreeNode *> children = node->GetChildren();

        int M = node->GetChildrenNumber();
        std::vector<GLfloat> children_x_end;
        std::vector<GLfloat> children_x_middle_relative;

        GLfloat max_x_end = 0;
        GLfloat current_x_end = 0;

        for (int i = 0; i < M; i++) {
            if (children[i]->DrawType() != BT::ACTION
                && children[i]->DrawType() != BT::CONDITION) {
                current_x_end = 0.04;
                children_x_middle_relative.push_back(0.02);
            } else {
                current_x_end = 0.02f * compute_max_width(
                        children[i]->get_name().c_str());
                children_x_middle_relative.push_back(current_x_end / 2);
            }

            if (i < M - 1) {
                max_x_end = max_x_end +
                            current_x_end +
                            NODE_X_MARGIN;
            } else {
                max_x_end = max_x_end + current_x_end;
            }
            children_x_end.push_back(max_x_end);
        }

        GLfloat x_shift = x_pos - max_x_end / 2;

        for (int i = 0; i < M; i++) {
            if (i > 0) {
                updateTree(children[i],
                           x_shift + children_x_end.at(i - 1u),
                           y_pos - y_offset,
                           y_offset,
                           depth + 1);

                draw_edge(x_pos + 0.015f,
                          y_pos, 0.02,
                          x_shift + children_x_end.at(i - 1u) +
                          children_x_middle_relative.at(i),
                          y_pos - y_offset, 0.02);
            } else {
                updateTree(children[i],
                           x_shift,
                           y_pos - y_offset,
                           y_offset,
                           depth + 1);

                draw_edge(x_pos + 0.015f,
                          y_pos,
                          0.02,
                          x_shift + children_x_middle_relative.at(i),
                          y_pos - y_offset,
                          0.02);
            }
        }
    }
}

// This will call updateTree function
void display() {
    // clear the draw buffer
    glClearColor(BG_R_COLOR, BG_G_COLOR, BG_B_COLOR, 0.1);
    // Erase everything
    glClear(GL_COLOR_BUFFER_BIT);

    updateTree(tree, node_x, node_y, y_offset, 1);

    glutSwapBuffers();
    glutPostRedisplay();
}

// Main entry to draw tree
void drawTree(BT::ControlNode *tree_) {
    if (!init) {
        int argc = 0;
        glutInit(&argc, nullptr);
        init = true;
        glutInitDisplayMode(
                GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
        glEnable(GL_MULTISAMPLE);
    }

    tree = tree_;
    depth = tree->Depth();

    glutInitWindowSize(1024, 800);
    glutCreateWindow("BT test");

    glClearColor(1.0, 1.0, 1.0, 0.1);

    glutDisplayFunc(display);

    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboard_release);
    glutSpecialFunc(processSpecialKeys);

    glutMainLoop();
}
