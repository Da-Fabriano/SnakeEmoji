#include <iostream>
#include <windows.h>
#include <cmath>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

// Variables
const int TARGET_FPS = 60;
const int SNAKE_SPEED_MS = 95;
const int W = 23, H = 17;
int snakeX = W / 2, snakeY = H / 2;
int foodX, foodY;
int tailX[100] = { 0 }, tailY[100] = { 0 }, tailDir[100] = { 0 };
int tailLength = 0;
int score = 0;
int skinIndex = 0;
bool gameOver = false;
enum eDirection {STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir, lastDir, nextDir;
bool inputLocked = false;
string snakeHeadSkin = "🟩";
string snakeTailSkin = "🟩";

// Erase count amount of lines
void eraseLines(int count) {
    if (count > 0) {
        cout << "\x1b[2K"; 
        for (int i = 1; i < count; i++) {
            cout << "\x1b[1A\x1b[2K"; 
        }
        cout << "\r"; 
    }
}

// Set cursor position to x,y
void setCursorPosition(int x, int y) {
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, coord);
}

// Draw main menu
void drawMainMenu() {
    setCursorPosition(0, 0);
    cout <<
        "⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜\n"
        "⬜⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬜\n"
        "⬜⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬜\n"
        "⬜⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬜\n"
        "⬜⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬜\n"
        "⬜⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬜\n"
        "⬜⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬜\n"
        "⬜⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬜\n"
        "⬜⬛⬛⬛⬛🍎⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬜\n"
        "⬜⬛⬛⬛⬛⬛⬛⬛⬛🟩🟩🟩🟩🟩🟩🟩⬛⬛⬛⬛⬛⬛⬛⬛⬜\n"
        "⬜⬛⬛⬛⬛⬛⬛⬛⬛🟩\033[32;1m-- PLAY --🟩⬛⬛⬛⬛⬛⬛⬛⬛⬜\n"
        "⬜⬛⬛⬛⬛⬛⬛⬛⬛🟩🟩🟩🟩🟩🟩🟩⬛⬛⬛⬛⬛⬛⬛⬛⬜\n"
        "⬜⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬜\n"
        "⬜⬛⬛⬛⬛⬛⬛⬛⬛⬛🟦🟦🟦🟦🟦⬛⬛⬛⬛⬛⬛⬛⬛⬛⬜\n"
        "⬜⬛⬛⬛⬛⬛⬛⬛⬛⬛🟦 \033[34;1mSKIN 🟦⬛⬛⬛⬛⬛⬛⬛⬛⬛⬜\n"
        "⬜⬛⬛⬛⬛⬛⬛⬛⬛⬛🟦🟦🟦🟦🟦⬛⬛⬛⬛⬛⬛⬛⬛⬛⬜\n"
        "⬜⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬜\n"
        "⬜ \033[30mV 0.35\033[0m ⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬜\n"
        "⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜\n";
}

// Draw skin window
void drawSkinWindow(int index) {
    setCursorPosition(0, 0);
    string col;
    switch (index) {
        case 0: col = "🟩"; break; 
        case 1: col = "🟥"; break;
        case 2: col = "🟨"; break; 
        case 3: col = "🟧"; break;
        case 4: col = "⬆️ "; break; 
        case 5: col = "🟦"; break;
        case 6: col = "🟪"; break; 
        case 7: col = "🟫"; break;
    }
    cout <<
        "⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜\n"
        "⬜⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬜\n"
        "⬜⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬜\n"
        "⬜⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬜\n"
        "⬜⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬜\n"
        "⬜⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛" << col << "⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬜\n"
        "⬜⬛⬛⬛⬛⬛⬛⬛⬛⬛" << col << col << col << "⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬜\n"
        "⬜⬛⬛⬛⬛⬛⬛⬛⬛⬛" << col << "⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬜\n"
        "⬜⬛⬛⬛⬛⬛⬛⬛⬛⬛" << col << col << col << col << col << "⬛⬛⬛⬛⬛⬛⬛⬛⬛⬜\n"
        "⬜⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛" << col << "" << col << "" << col << "⬛⬛⬛⬛⬛⬛⬛⬛⬛⬜\n"
        "⬜⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛" << col << "⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬜\n"
        "⬜⬛⬛⬛⬛⬛⬛⬛⬛⬛" << col << "" << col << "" << col << "⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬜\n"
        "⬜⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬜\n"
        "⬜⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬜\n"
        "⬜⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬜\n"
        "⬜⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬜\n"
        "⬜⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬜\n"
        "⬜⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬜\n"
        "⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜\n";
}

// Reset game
void resetGame () {
    snakeX = W / 2; snakeY = H / 2;
    dir = STOP; lastDir = STOP; nextDir = STOP;
    inputLocked = false;
    tailLength = 0; score = 0; gameOver = false;

    bool food_on_snake;
    do {
        food_on_snake = false;
        foodX = rand() % W;
        foodY = rand() % H;

        if (foodX == snakeX && foodY == snakeY) food_on_snake = true;
    } while (food_on_snake);
}

// Arrow skin
string arrowSkin(int direction) {
    switch (direction) {
        case LEFT: return "⬅️ "; case RIGHT: return "➡️ ";
        case UP:   return "⬆️ "; case DOWN:  return "⬇️ ";
        default:   return "⬆️ ";
    }
}

// Main code
int main() {

    SetConsoleOutputCP(CP_UTF8);
    
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(out, &cursorInfo);

    srand((unsigned)time(NULL));
    system("cls");

    do {
        foodX = rand() % W;
        foodY = rand() % H;
    } while (foodX == snakeX && foodY == snakeY);

    for (int i = 0; i < H + 4; ++i) cout << "\n";

    bool returnToMain = true;
    while (returnToMain) {
        returnToMain = false; 
        resetGame();
        drawMainMenu();

        string input;
        while (true) {
            cout << "\033[30mType command \033[0m(\033[32mplay\033[0m, \033[32mskin\033[0m):";
            getline(cin, input);

            if (input == "play") {
                eraseLines(2);
                break;
            }
            else if (input == "skin") {
                eraseLines(2);
                drawSkinWindow(skinIndex);

                while (true) {
                    cout << "\033[30mType command \033[0m(\033[32mnext\033[0m, \033[32mprev\033[0m, \033[32mexit\033[0m): ";
                    getline(cin, input);

                    if (input == "next" && skinIndex < 7) skinIndex++;
                    else if (input == "prev" && skinIndex > 0) skinIndex--;
                    else if (input == "exit") {
                        eraseLines(2);
                        drawMainMenu();
                        break;
                    }

                    eraseLines(2);
                    drawSkinWindow(skinIndex);

                    switch (skinIndex) {
                        case 0: snakeHeadSkin = "🟩"; snakeTailSkin = "🟩"; break;
                        case 1: snakeHeadSkin = "🟥"; snakeTailSkin = "🟥"; break;
                        case 2: snakeHeadSkin = "🟨"; snakeTailSkin = "🟨"; break;
                        case 3: snakeHeadSkin = "🟧"; snakeTailSkin = "🟧"; break;
                        case 4: break; 
                        case 5: snakeHeadSkin = "🟦"; snakeTailSkin = "🟦"; break;
                        case 6: snakeHeadSkin = "🟪"; snakeTailSkin = "🟪"; break;
                        case 7: snakeHeadSkin = "🟫"; snakeTailSkin = "🟫"; break;
                    }
                }
            } else {
                eraseLines(2);
            }
        }    
        
        DWORD lastTime = GetTickCount();
        DWORD lastDrawTime = GetTickCount();
        const DWORD drawMS = 1000 / TARGET_FPS;

        while (!gameOver){
            DWORD currentTime = GetTickCount();

            while (_kbhit()) {
                char key = tolower(_getch());
                eDirection pressedDir = STOP;

                if (key == 'a') pressedDir = LEFT;
                else if (key == 'd') pressedDir = RIGHT;
                else if (key == 'w') pressedDir = UP;
                else if (key == 's') pressedDir = DOWN;

                if (pressedDir != STOP) {
                    if (!inputLocked) {
                        if ((pressedDir == LEFT && lastDir != RIGHT) ||
                            (pressedDir == RIGHT && lastDir != LEFT) ||
                            (pressedDir == UP && lastDir != DOWN) ||
                            (pressedDir == DOWN && lastDir != UP)) {
                            dir = pressedDir;
                            inputLocked = true;
                        }
                    } else {
                        if ((pressedDir == LEFT && dir != RIGHT) ||
                            (pressedDir == RIGHT && dir != LEFT) ||
                            (pressedDir == UP && dir != DOWN) ||
                            (pressedDir == DOWN && dir != UP)) {
                            nextDir = pressedDir;
                        }
                    }
                }
            }

            // Snake logic
            if (currentTime - lastTime >= SNAKE_SPEED_MS) {
                lastTime = currentTime;

                int prevX = snakeX;
                int prevY = snakeY;

                // Move head
                switch(dir)
                {
                    case LEFT: snakeX--; break;
                    case RIGHT: snakeX++; break;
                    case UP: snakeY--; break;
                    case DOWN: snakeY++; break;
                    default: break;
                }

                if (dir != STOP) lastDir = dir;

                if (nextDir != STOP) {
                    dir = nextDir;
                    nextDir = STOP;
                    inputLocked = true;
                } else {
                    inputLocked = false;
                }
                
                // Wall Collision
                if (snakeX < 0) snakeX = W - 1;
                else if (snakeX >= W) snakeX = 0;
                if (snakeY < 0) snakeY = H - 1;
                else if (snakeY >= H) snakeY = 0;

                // Food Collision
                if (snakeX == foodX && snakeY == foodY) {
                    score++;
                    if (score < 100) tailLength++; 
                    
                    bool food_on_snake;
                    do {
                        food_on_snake = false;
                        foodX = rand() % W; foodY = rand() % H;
                        if (foodX == snakeX && foodY == snakeY) food_on_snake = true;
                        for (int k = 0; k < tailLength; ++k) {
                            if (tailX[k] == foodX && tailY[k] == foodY) food_on_snake = true;
                        }
                    } while (food_on_snake);
                }

                // Tail Movement
                for (int k = tailLength - 1; k > 0; --k) {
                    tailX[k] = tailX[k - 1];
                    tailY[k] = tailY[k - 1];
                    tailDir[k] = tailDir[k - 1];
                }
                if (tailLength > 0) {
                    tailX[0] = prevX;
                    tailY[0] = prevY;
                    tailDir[0] = lastDir;
                }

                // Tail Collision
                for (int k = 0; k < tailLength; ++k) {
                    if (tailX[k] == snakeX && tailY[k] == snakeY) {
                        while (true) {
                            setCursorPosition(W / 2 + 9, H /2 + 0);
                            cout << " \033[31;1mGAME-OVER\033[0m\n";
                            setCursorPosition(W / 2 + 9, H / 2 + 1);
                            if (score <= 9) {
                                cout << " \033[30;1mScore:>\033[33;1m" << score << "\033[30;1m<\n\033[0m";
                            }
                            else if (score <= 99 ) {
                                cout << " \033[30;1mScore:>\033[33;1m" << score << "\033[0m\n";
                            }
                            else {
                                cout << " \033[30;1mScore:\033[33;1m" << score << "\033[0m\n";
                            }

                            setCursorPosition(0, H + 2);
                            eraseLines(1);

                            cout << "\033[30mType command \033[0m(\033[32mreplay\033[0m, \033[32mmain\033[0m, \033[32mleave\033[0m): ";
                            getline(cin, input);
                            if (input == "replay") {
                                eraseLines(3);
                                resetGame();
                                break; 
                            }
                            else if (input == "main") {
                                eraseLines(3);
                                returnToMain = true;
                                gameOver = true;
                                break;
                            }
                            else if (input == "leave") {
                                system("cls");
                                gameOver = true; 
                            }
                            else eraseLines(2);
                        }
                    }
                }
            }

            // Render
            if (currentTime - lastDrawTime >= drawMS && !gameOver) {
                lastDrawTime = currentTime;
                
                setCursorPosition(0, 0);

                // Top border
                for (int i = 0; i < W + 2; ++i)
                    cout << "⬜";
                cout << "\n";

                // Game area
                for (int y = 0; y < H; ++y) {
                    cout << "⬜";
                    for (int x = 0; x < W; ++x) {

                        if (x == snakeX && y == snakeY) {
                            cout << (skinIndex == 4 ? arrowSkin(dir) : snakeHeadSkin);
                        }
                        else if (x == foodX && y == foodY) {
                            cout << "🍎";
                        }
                        else {
                            bool isBody = false;

                            for (int k = 0; k < tailLength; ++k) {
                                if (tailX[k] == x && tailY[k] == y) {
                                    cout << (skinIndex == 4 ? arrowSkin(tailDir[k]) : snakeTailSkin);
                                    isBody = true;
                                    break;
                                }
                            }

                            if (!isBody)
                                cout << "⬛";
                        }
                    }
                    cout << "⬜\n";
                }

                // Bottom border
                for (int i = 0; i < W + 2; ++i)
                    cout << "⬜";

                cout << "\n\033[30mScore: \033[33m" << score << "\033[0m\n";
            }

            Sleep(1); 
        }
    }

    return 0;
}