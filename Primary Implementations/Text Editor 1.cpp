#include <iostream>
#include <stack>
#include <vector>
#include <conio.h>      // For _getch() and _kbhit()
#include <windows.h>    // For SetConsoleCursorPosition(), GetAsyncKeyState()
#include <chrono>       // For time tracking
using namespace std;
using namespace chrono;

class TextEditor {
private:
    stack<char> leftStack;
    stack<char> rightStack;
    vector<stack<char>> lines;
    int currentLine = 0;
    int cursorX = 0, cursorY = 0;

    vector<stack<char>> lastState; // Last state saved for undo
    time_point<steady_clock> lastChangeTime; // Time of the last change

    void setCursorPosition(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    void displayText() {
        system("cls");
        for (int i = 0; i <= currentLine; i++) {
            stack<char> temp = lines[i];
            stack<char> reversedLeft;
            while (!temp.empty()) {
                reversedLeft.push(temp.top());
                temp.pop();
            }
            while (!reversedLeft.empty()) {
                cout << reversedLeft.top();
                reversedLeft.pop();
            }
            if (i == currentLine) {
                cout << "_";
                temp = rightStack;
                while (!temp.empty()) {
                    cout << temp.top();
                    temp.pop();
                }
            }
            cout << endl;
        }
    }

public:
    TextEditor() {
        lines.push_back(stack<char>());
        lastState = lines; // Initialize lastState
        lastChangeTime = steady_clock::now();
    }

    void insert(char ch) {
        leftStack.push(ch);
        lines[currentLine] = leftStack;
        lastChangeTime = steady_clock::now(); // Record time of the last change
    }

    void moveCursorLeft() {
        if (!leftStack.empty()) {
            rightStack.push(leftStack.top());
            leftStack.pop();
            lines[currentLine] = leftStack;
        }
    }

    void moveCursorRight() {
        if (!rightStack.empty()) {
            leftStack.push(rightStack.top());
            rightStack.pop();
            lines[currentLine] = leftStack;
        }
    }

    void backspace() {
        if (!leftStack.empty()) {
            leftStack.pop();
            lines[currentLine] = leftStack;
        }
    }

    void insertNewLine() {
        lines[currentLine] = leftStack;
        currentLine++;
        if (currentLine >= lines.size()) {
            lines.push_back(stack<char>());
        }
        leftStack = stack<char>();
        rightStack = stack<char>();
        cursorX = 0;
        cursorY++;
    }

    void undo() {
        lines = lastState;
        if (currentLine >= lines.size()) currentLine = lines.size() - 1;
        leftStack = lines[currentLine];
        rightStack = stack<char>();  // Clear the rightStack after undo
        cout << "Undo performed!" << endl;
    }

    void runEditor() {
        system("cls");
        displayText();
        setCursorPosition(cursorX, cursorY);

        while (true) {
            if (_kbhit()) {
                int ch = _getch();

                if (ch == 224) {  // Arrow keys
                    ch = _getch();
                    switch (ch) {
                        case 75: moveCursorLeft(); cursorX = max(cursorX - 1, 0); break;  // Left
                        case 77: moveCursorRight(); cursorX++; break;                    // Right
                    }
                } else if (ch == 8) {  // Backspace
                    backspace();
                    cursorX = max(cursorX - 1, 0);
                } else if (ch == 13) {  // Enter
                    insertNewLine();
                } else if (ch == 27) {  // ESC to quit
                    return;
                } else if (ch >= 32 && ch <= 126) {  // Printable characters
                    insert(ch);
                    cursorX++;
                }

                // Update time and save state before processing further keys
                auto now = steady_clock::now();
                auto elapsed = duration_cast<seconds>(now - lastChangeTime).count();
                if (elapsed >= 2) {
                    lastState = lines;  // Save the last state if 2 seconds have passed
                    lastChangeTime = now;  // Reset the lastChangeTime
                }

                // Check for Ctrl+Z for undo
                if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
                    if (GetAsyncKeyState('Z') & 0x8000) {
                        undo();
                    }
                }

                displayText();
                setCursorPosition(cursorX, cursorY);
            }
        }
    }
};

int main() {
    TextEditor editor;
    cout << "Welcome to the Text Editor! (Press ESC to quit)\n";
    editor.runEditor();
    return 0;
}
