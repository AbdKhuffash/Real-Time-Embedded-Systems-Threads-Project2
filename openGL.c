#include <GL/glut.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include "local.h"
struct MEMORY *memptr;
double xCustomer = 200;
double yCustomer = 600;
double xCart;
double yCart;
double xTeam;
double yTeam;
int hasExecutedCustomer = 1;
int hasExecutedTeam = 0;
int hasExecutedcart = 0;
int numberOfTeams = 1;

typedef struct
{
    int x;
    int y;
} openTeam;
openTeam team[5];

void myInit(void)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    gluOrtho2D(0, 1920, 0, 1080); // Adjusted window size
}

void drawRectangle(int x, int y, int width, int height, float lineWidth)
{
    glLineWidth(lineWidth); // Set line width
    glBegin(GL_LINE_LOOP);
    glVertex2i(x, y);
    glVertex2i(x + width, y);
    glVertex2i(x + width, y + height);
    glVertex2i(x, y + height);
    glEnd();
    glLineWidth(1.0); // Reset line width to default
}


void drawSquare(int x, int y, int size)
{
    glBegin(GL_POLYGON);
    glVertex2i(x, y);
    glVertex2i(x + size, y);
    glVertex2i(x + size, y + size);
    glVertex2i(x, y + size);
    glEnd();
}

void writeText(const char *text, int x, int y, int fontSize)
{
    glRasterPos2i(x, y);
    for (size_t i = 0; i < strlen(text); ++i)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12 + fontSize, text[i]);
    }
}

void drawItems(int x, int y, int width, int height, int numShelves)
{
    for (int i = 0; i < numShelves; ++i)
    {
        int shelfY = y + i * (height / numShelves);
        glBegin(GL_LINES);
        glVertex2i(x, shelfY);
        glVertex2i(x + width, shelfY);
        glEnd();
        drawRectangle(x + 30, shelfY + 20, 30, 30, 0);
        drawRectangle(x + 80, shelfY + 20, 30, 30, 0);
        drawRectangle(x + 130, shelfY + 20, 30, 30, 0);
        drawRectangle(x + 180, shelfY + 20, 30, 30, 0);
        drawRectangle(x + 230, shelfY + 20, 30, 30, 0);
    }
}

void drawList(int x, int y, int width, int height)
{
    drawRectangle(x, y, width, height, 3);
    char text[100];
    for (int i = 0; i < ITEMSCOUNT; i++)
    {
        strcpy(text, memptr->items[i].itemName);
        sprintf(text + strlen(text), " #%d", memptr->items[i].itemShelfQuantity);
        if (i >= 3 && i < 6)
        {
            writeText(text, x + (200 * (i - 3)) + 20, y + 100, 0);
        }
        else if (i >= 6 && i < 9)
        {
            writeText(text, x + (200 * (i - 6)) + 20, y + 150, 0);
        }
        else if (i >= 9 && i < 12)
        {
            writeText(text, x + (200 * (i - 9)) + 20, y + 200, 0);
        }
        else if (i >= 12 && i < 15)
        {
            writeText(text, x + (200 * (i - 12)) + 20, y + 250, 0);
        }
        else if (i >= 15 && i < 17)
        {
            writeText(text, x + (200 * (i - 15)) + 20, y + 300, 0);
        }
        else
        {
            writeText(text, x + (200 * i) + 20, y + 50, 0);
        }
    }
}

void drawShelf(int x, int y, int width, int height, int numShelves)
{
    glColor3f(0.5, 0.5, 0.5); // Gray color for shelves

    glBegin(GL_POLYGON);
    glVertex2i(x, y);
    glVertex2i(x + width, y);
    glVertex2i(x + width, y + height);
    glVertex2i(x, y + height);
    glEnd();

    glColor3f(0.0, 1.0, 0.0); // Green color for products

    // Draw items
    drawItems(x, y, width, height, numShelves);
}

//////////////////////animation/////////////////

void animateCustomers()
{

    // Check if the function has not been executed before
    if (!hasExecutedCustomer)
    {
        if (xCustomer < 1550)
        {
            xCustomer += 10;
        }
        if (yCustomer >= 300 && xCustomer == 1550)
        {
            yCustomer -= 5;
        }
    }
}

void animateCart()
{

    // Check if the function has not been executed before
    if (!hasExecutedcart)
    {
        if (xCart < 1550)
        {
            xCart += 2;
        }
        if (yCart <= 180 && xCart == 1550)
        {
            yCart += 1;
        }
        /////////////////////
        if (xTeam < 1550)
        {
            xTeam += 2;
        }
        if (yTeam >= 180 && yTeam == 1550)
        {
            yTeam += 1;
        }
        if (yTeam==180)
        {
        hasExecutedcart = 1; // to end the function animateCustomers() when the customer finish
        sleep(2);
        xCart = team[numberOfTeams - 1].x;
        yCart = team[numberOfTeams - 1].y;
        }
    }
}

//////////////////////animation/////////////////

void drawFilledCircle(double x, double y, int radius, int segments)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // Center of the circle

    for (int i = 0; i <= segments; ++i)
    {
        float theta = (2.0f * M_PI * i) / segments;
        float dx = radius * cosf(theta);
        float dy = radius * sinf(theta);
        glVertex2f(x + dx, y + dy);
    }

    glEnd();
}

void drawCustomer(double x, double y)
{
    glColor3f(0.0, 0.0, 1.0);       // Blue color for customers
    drawFilledCircle(x, y, 20, 20); // Adjusted circle size for customers
}

#include <GL/glut.h>

void drawCart(int width, int height, int wheelRadius)
{
    // Draw the body of the cart (a square)
    glColor3f(0.5, 0.5, 0.5); // Gray color for the body
    drawRectangle(xCart + 200, yCart + 10, width, height, 0);
    glColor3f(0.5, 0.5, 0.5); // Gray color for the body
    drawFilledCircle(xCart + 220, yCart + 5, wheelRadius, wheelRadius);
    drawFilledCircle(xCart + 260, yCart + 5, wheelRadius, wheelRadius);
    glLineWidth(5);
    glBegin(GL_LINES);
    glVertex2f(xCart + 200, yCart + 30);
    glVertex2f(xCart + 170, yCart + 50);
    glEnd();
    drawSquare(xCart+100,yCart,40);
}

void ifCustomerFinish()
{
    if ((int)xCustomer == 1550 && (int)yCustomer == 300)
    {
        hasExecutedCustomer = 1; // to end the function animateCustomers() when the customer finish
        sleep(2);
        xCustomer = 200;
        yCustomer = 600;
    }
}



void enterCustomer()
{
    drawCustomer(xCustomer, yCustomer);
    writeText("Customer", xCustomer - 30, yCustomer + 30, 0);
    hasExecutedCustomer = 0;
    animateCustomers();
}

void moveCart()
{
    hasExecutedcart = 0;
    animateCart();
}

void createTeams()
{
    glColor3f(1.0, 0.0, 0.0); // Red color for team
    for (int i = 0; i < numberOfTeams; i++)
    {
        drawSquare(team[i].x, team[i].y, 80); // Adjusted team size
        char text[50] = "Team #";
        sprintf(text + strlen(text), "%d", i + 1);
        writeText(text, team[i].x, team[i].y + 100, 0);
    }
}

void drawSupermarket()
{
    // Draw store
    glColor3f(1.0, 1.0, 1.0); // White color for store
    drawSquare(100, 40, 500); // Adjusted store size
    glColor3f(1.0, 0.0, 0.0); // Red color
    writeText("Store", 320, 290, 0);

    drawRectangle(660, 950, 600, 100, 5); // drow the supermarket
    writeText("WELCOME TO SUPERMARKET", 850, 990, 0);

    // Draw teams at the top
    createTeams();

    // Draw supermarket with shelves
    drawShelf(1600, 250, 300, 400, 5); // Adjusted supermarket size and shelf count
    // enter customer //
    // enterCustomer(1);
    drawCustomer(200, 600);
    writeText("Customer", 170, 630, 0);
    ifCustomerFinish();
    drawCart(80, 40, 10);
    drawList(1350, 670, 550, 380);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    // Draw the supermarket
    // Example usage of drawCart
    if (memptr->customerEntered == 1)
    {
        enterCustomer();
    }
    if (memptr->moveCart == 1)
    {
        moveCart();

    }
    char runtime[50] = "runtime #";
    sprintf(runtime + strlen(runtime), "%d", memptr->runtime);
    char THRESHOLD[50] = "THRESHOLD #";
    sprintf(THRESHOLD + strlen(THRESHOLD), "%d", memptr->THRESHOLD);
    glColor3f(1.0, 0.0, 0.0);
    writeText(runtime, 40, 1000, 0);
    writeText(THRESHOLD, 40, 950, 0);
    drawSupermarket();
    glutSwapBuffers();
    glutPostRedisplay();
}

char *shmptr;

int shmid;

int main(int argc, char **argv)
{

    // ================GETTING AND ATTACHING MEMORY================= //
    pid_t ppid = getppid();
    if ((shmid = shmget((int)ppid, 0, 0)) != -1)
    {
        if ((shmptr = (char *)shmat(shmid, (char *)0, 0)) == (char *)-1)
        {
            perror("shmat -- OPEN -- attach");
            exit(1);
        }
        memptr = (struct MEMORY *)shmptr;
    }
    else
    {
        perror("shmget -- OPEN -- access1");
        exit(2);
    }

    numberOfTeams = memptr->numOfTeams;
    for (int i = 0; i < numberOfTeams; i++)
    {
        team[i].x = 700 + (i * 120);
        team[i].y = 70;
    }
    xCart = team[numberOfTeams - 1].x;
    yCart = team[numberOfTeams - 1].y;

    xTeam=xCart;
    yTeam=yCart;

    memptr->openGLID = getpid();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1920, 1080); // Adjusted window size
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Supermarket Simulation");

    myInit();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
