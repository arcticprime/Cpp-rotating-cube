#include <math.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <windows.h> // Include Windows-specific header
using namespace std;

float A, B, C;
float cubeWidth = 20;
int width = 160;
int height = 44;
float zBuffer[160 * 44];
char buffer[160 * 44];
int backgrowndASCIICode = ' ';
int distanceFromCam = 100;
float K1 = 40;

float incrementSpeed = 0.6;
float x, y, z;
float ooz;
int xp, yp;
int idx;

float calculateX(int i, int j, int k) {
    return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) + j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);
}

float calculateY(int i, int j, int k) {
    return j * cos(A) * cos(C) + k * sin(A) * cos(C) - j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) - i * cos(B) * sin(C);
}

float calculateZ(int i, int j, int k) {
    return k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B);
}

void calculateforSurface(float cubeX, float cubeY, float cubeZ, int ch) {
    x = calculateX(cubeX, cubeY, cubeZ);
    y = calculateY(cubeX, cubeY, cubeZ);
    z = calculateZ(cubeX, cubeY, cubeZ) + distanceFromCam;

    ooz = 1 / z;

    xp = (int)(width / 2 + K1 * ooz * x);
    yp = (int)(height / 2 - K1 * ooz * y);

    idx = xp + yp * width;
    if (idx >= 0 && idx < width * height) {
        if (ooz > zBuffer[idx]) {
            zBuffer[idx] = ooz;
            buffer[idx] = ch;
        }
    }
}

int main() {
    cout << "\x1b[2J"; // Clear the screen

    while (1) {
        memset(buffer, backgrowndASCIICode, width * height);
        memset(zBuffer, 0, width * height * sizeof(float));

        for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incrementSpeed) {
            for (float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += incrementSpeed) {
                calculateforSurface(cubeX, cubeY, -cubeWidth, '.');
                calculateforSurface(cubeWidth, cubeY, cubeX, '$');
                calculateforSurface(-cubeWidth, cubeY, -cubeX, '~');
                calculateforSurface(-cubeX, cubeY, cubeWidth, '#');
                calculateforSurface(cubeX, -cubeWidth, -cubeY, ';');
                calculateforSurface(cubeX, cubeWidth, cubeY, '+');
            }
        }

        cout << "\x1b[H"; // Move cursor to home position
        for (int k = 0; k < width * height; k++) {
            putchar(k % width ? buffer[k] : 10);
        }

        A += 0.01; // Increased increment for visibility
        B += 0.01;
        Sleep(30); // Slower sleep to see changes
    }

    return 0;
}
