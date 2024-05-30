#include <iostream>
#include <string>
#include <cmath>
#include <raylib.h>

#define GRID 9

using namespace std;

const int width = 670;
const int height = 800;

Color background = { 67, 85, 133,255 };
Color darkblue = { 54, 48, 98 ,255 };
Color textcolor = { 245, 232, 199,255 };
Color Blue = {21, 52, 72,255 };

class Sudoku {
public:
    Rectangle button1;
    Rectangle button2;
    Rectangle button3;
    Rectangle ExitButton;

    Sudoku() {
        button1 = { 230, 500, 200, 60 };
        button2 = { 275, 700, 120, 50 };
        button3 = { 275, 700, 120, 50 };
        ExitButton = { 230,600,200,60 };
    }
};

void DrawNums(int sudoku[GRID][GRID]) {
    for (int i = 0; i < GRID; i++) {
        for (int j = 0; j < GRID; j++) {
            if (sudoku[i][j] != 0) {
                int posY = (i + 1) * 60 + 100;
                int posX = (j + 1) * 60 + 20;
                string character = to_string(sudoku[i][j]);
                DrawText(character.c_str(), posX, posY, 40, textcolor);
            }
        }
    }
}

bool ispossible(int grid[GRID][GRID], int row, int col, int val, int n) {
    for (int i = 0; i < n; i++) {
        if (grid[row][i] == val || grid[i][col] == val) {
            return false;
        }
    }
    int no = sqrt(n);
    int r = row - row % no;
    int c = col - col % no;
    for (int i = r; i < r + no; i++) {
        for (int j = c; j < c + no; j++) {
            if (grid[i][j] == val) {
                return false;
            }
        }
    }
    return true;
}

bool SudokuSolver(int grid[GRID][GRID], int i, int j, int n) {
    if (i == n) {
        return true;
    }
    if (j == n) {
        return SudokuSolver(grid, i + 1, 0, n);
    }
    if (grid[i][j] != 0) {
        return SudokuSolver(grid, i, j + 1, n);
    }
    for (int val = 1; val <= n; val++) {
        if (ispossible(grid, i, j, val, n)) {
            grid[i][j] = val;
            bool check = SudokuSolver(grid, i, j + 1, n);
            if (check) {
                return true;
            }
            grid[i][j] = 0;
        }
    }
    return false;
}

void DrawGrid() {
    for (int i = 0; i <= 9; i++) {
        DrawLineEx({ (float)((i + 1) * 60), 150.0f }, { (float)((i + 1) * 60), 690.0f }, 3, SKYBLUE);
        DrawLineEx({ 60.0f, (float)(90 + (i + 1) * 60) }, { 600.0f, (float)(90 + (i + 1) * 60) }, 3, SKYBLUE);
    }
}

enum GameState {
    MAINSCREEN,
    GAMESCREEN,
    SOLSCREEN,
    QUITSCREEN
};

int main() {
    int sudoku[GRID][GRID] = { 0 };
    cout << "Enter your Sudoku puzzle (9x9 grid, with 0 for empty cells):" << endl;
    for (int i = 0; i < GRID; i++) {
        for (int j = 0; j < GRID; j++) {
            cin >> sudoku[i][j];
        }
    }
    
    InitWindow(width, height, "Sudoku Solver");
    SetTargetFPS(60);
   
    Sudoku s;
    GameState currentstate = MAINSCREEN;
    Texture2D logo1 = LoadTexture("finallogo.png");
    while (!WindowShouldClose() && currentstate != QUITSCREEN) {
   
        ClearBackground(WHITE);

        switch (currentstate) {
        case MAINSCREEN:
            ClearBackground(background);
            DrawTexture(logo1,80,160,WHITE);
            DrawRectangleRec(s.button1, darkblue);
            DrawRectangleRec(s.ExitButton, darkblue);
            DrawText("GENERATE", 249, 515, 30, textcolor);
            DrawText("EXIT", 290, 615, 30, textcolor);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mousePoint = GetMousePosition();
                if (CheckCollisionPointRec(mousePoint, s.button1)) {
                    currentstate = GAMESCREEN;
                }
            }
            if (CheckCollisionPointRec(GetMousePosition(), s.ExitButton)) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    currentstate = QUITSCREEN;
                }
            }
            break;

        case GAMESCREEN:
            ClearBackground(background);
            DrawRectangleRec({60, 150, 540, 540},darkblue);
            DrawText("SUDOKU PUZZLE", 120, 80, 50,SKYBLUE );
            DrawGrid();
            DrawNums(sudoku);

            DrawRectangleRec(s.button2, SKYBLUE);
            DrawText("SOLVE", 292, 712, 27, textcolor);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mousePoint = GetMousePosition();
                if (CheckCollisionPointRec(mousePoint, s.button2)) {
                    currentstate = SOLSCREEN;
                }
            }
            break;

        case SOLSCREEN:
            ClearBackground(background);
            DrawRectangleRec({ 60, 150, 540, 540 }, darkblue);
            DrawText("SUDOKU SOLUTION", 86, 80, 50, SKYBLUE);
            DrawGrid();

            DrawRectangleRec(s.button3, SKYBLUE);
            DrawText("DONE", 302, 712, 27, textcolor);
            if (SudokuSolver(sudoku, 0, 0, 9)) {
                DrawNums(sudoku);
            }
            else {
                DrawText("No solution found!", 200, 150, 30, RED);
            }
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mousePoint = GetMousePosition();
                if (CheckCollisionPointRec(mousePoint, s.button3)) {
                    currentstate = MAINSCREEN;
                }
            }
            break;
        case QUITSCREEN:
            CloseWindow();
            break;     
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
