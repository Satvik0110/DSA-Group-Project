#include <iostream>
#include <stack>
#include <vector>
#include <conio.h>      // For _getch() and _kbhit()
#include <windows.h>    // For SetConsoleCursorPosition()
#include <chrono>
using namespace std;
using namespace chrono;

const int MAX_UNDO_SIZE = 10; // Set a limit for the undo stack size

class TextEditor {
private:
    stack<char> leftStack;   // Stores the text to the left of the cursor
    stack<char> rightStack;  // Stores the text to the right of the cursor
    vector<stack<char>> lines;  // Each line is a separate stack
    int currentLine = 0;        // Track which line the cursor is on
    int cursorX = 0, cursorY = 0; // Tracks the cursor's x and y position (for console display)

    vector<vector<stack<char>>> undoStack; // Undo stack to store previous states
    time_point<steady_clock> lastChangeTime; // Time of the last change

    // Helper function to set cursor position in the console
    void setCursorPosition(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    // Helper function to display the current text
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

    // Function to check if the state has changed
    bool isStateChanged() {
        if (undoStack.empty()) return true; // If the undo stack is empty, consider it a change

        const auto& lastState = undoStack.back();
        if (lastState.size() != lines.size()) return true; // If the number of lines is different

        for (size_t i = 0; i < lines.size(); ++i) {
            if (lines[i].size() != lastState[i].size()) return true; // Different line sizes

            stack<char> temp1 = lines[i];
            stack<char> temp2 = lastState[i];
            while (!temp1.empty() && !temp2.empty()) {
                if (temp1.top() != temp2.top()) return true; // Characters differ
                temp1.pop();
                temp2.pop();
            }

            // If one stack is empty and the other is not, consider it a change
            if (!temp1.empty() || !temp2.empty()) return true;
        }

        return false; // No changes detected
    }

public:
    TextEditor() {
        // Initially start with one empty line
        lines.push_back(stack<char>());
        undoStack.push_back(lines); // Initialize undo stack with the initial state
        lastChangeTime = steady_clock::now();
    }

    // Insert a character at the current cursor position
    void insert(char ch) {
        leftStack.push(ch);
        lines[currentLine] = leftStack;

        if (isStateChanged()) {
            undoStack.push_back(lines); // Push only if the state has changed
            // Limit the undo stack size
            if (undoStack.size() > MAX_UNDO_SIZE) {
                undoStack.erase(undoStack.begin()); // Remove the oldest state if limit exceeded
            }
        }

        lastChangeTime = steady_clock::now(); // Record time of the last change
    }

    // Move the cursor to the left
    void moveCursorLeft() {
        if (!leftStack.empty()) {
            rightStack.push(leftStack.top());
            leftStack.pop();
            lines[currentLine] = leftStack;
        }
    }

    // Move the cursor to the right
    void moveCursorRight() {
        if (!rightStack.empty()) {
            leftStack.push(rightStack.top());
            rightStack.pop();
            lines[currentLine] = leftStack;
        }
    }

    // Move the cursor up
    void moveCursorUp() {
        if (currentLine > 0) {
            lines[currentLine] = leftStack;    // Store current line state
            currentLine--;                     // Move to the previous line
            leftStack = lines[currentLine];    // Load the previous line's leftStack
            rightStack = stack<char>();        // Clear the rightStack (cursor at end of previous line)
            cursorY = max(cursorY - 1, 0);     // Update cursor Y position
            cursorX = leftStack.size();        // Move the cursor to the end of the previous line
        }
    }

    // Move the cursor down
    void moveCursorDown() {
        if (currentLine < lines.size() - 1) {
            lines[currentLine] = leftStack;    // Store current line state
            currentLine++;                     // Move to the next line
            leftStack = lines[currentLine];    // Load the next line's leftStack
            rightStack = stack<char>();        // Clear the rightStack
            cursorY++;                         // Update cursor Y position
            cursorX = leftStack.size();        // Move the cursor to the end of the next line
        }
    }

    // Backspace (delete character before the cursor)
    void backspace() {
        if (!leftStack.empty()) {
            leftStack.pop();
            lines[currentLine] = leftStack;

            if (isStateChanged()) {
                undoStack.push_back(lines); // Push only if the state has changed
                // Limit the undo stack size
                if (undoStack.size() > MAX_UNDO_SIZE) {
                    undoStack.erase(undoStack.begin()); // Remove the oldest state if limit exceeded
                }
            }

            cursorX = max(cursorX - 1, 0);
        } else if (currentLine > 0) {
            cursorX = lines[currentLine - 1].size();  // Move cursor to the end of the previous line
            stack<char> previousLine = lines[currentLine - 1];
            while (!rightStack.empty()) {
                previousLine.push(rightStack.top());
                rightStack.pop();
            }
            lines[currentLine - 1] = previousLine;    // Merge the current line's rightStack with the previous line
            lines.erase(lines.begin() + currentLine); // Remove the current empty line
            currentLine--;                            // Move to the previous line
            leftStack = lines[currentLine];           // Load the left stack of the previous line
        }
    }

    // Delete (delete character after the cursor)
    void deleteChar() {
        if (!rightStack.empty()) {
            rightStack.pop();
        }
    }

    // Insert a newline (Enter key)
    void insertNewLine() {
        lines[currentLine] = leftStack;   // Store current line's state
        if (isStateChanged()) {
            undoStack.push_back(lines); // Push only if the state has changed
            // Limit the undo stack size
            if (undoStack.size() > MAX_UNDO_SIZE) {
                undoStack.erase(undoStack.begin()); // Remove the oldest state if limit exceeded
            }
        }

        currentLine++;                    // Move to the next line
        if (currentLine >= lines.size()) {
            lines.push_back(stack<char>()); // Create a new line if necessary
        }
        leftStack = stack<char>();        // Clear left stack for the new line
        rightStack = stack<char>();       // Clear right stack
        cursorX = 0;                      // Reset cursor position
        cursorY++;                        // Move cursor to the next line
    }

    void undo() {
        if (undoStack.size() > 1) { // Check if there's an undo state available
            undoStack.pop_back(); // Remove the current state
            lines = undoStack.back(); // Restore the previous state
            if (currentLine >= lines.size()) currentLine = lines.size() - 1;
            leftStack = lines[currentLine];
            rightStack = stack<char>();  // Clear the rightStack after undo
        }
    }

    // Main function to handle real-time editing
    void runEditor() {
        system("cls");
        displayText();
        setCursorPosition(cursorX, cursorY);

        while (true) {
            if (_kbhit()) {  // Checks if the keyboard gave a signal
                int ch = _getch();

                if (ch == 224) {  // Special keys (arrows, delete)
                    ch = _getch();  // Get the actual code

                    switch (ch) {
                        case 75: // Left arrow key
                            moveCursorLeft();
                            cursorX = max(cursorX - 1, 0);
                            break;
                        case 77: // Right arrow key
                            moveCursorRight();
                            cursorX++;
                            if(rightStack.empty()) leftStack.push(' ');
                            break;
                        case 83: // Delete key (ASCII code 83)
                            deleteChar();
                            break;
                        case 72:  // Up arrow key
                            moveCursorUp();
                            cursorY = max(cursorY - 1, 0);
                            break;
                        case 80:  // Down arrow key
                            moveCursorDown();
                            cursorY++;
                            break;
                    }
                } else if (ch == '\r') { // Enter key
                    insertNewLine();
                } else if (ch == '\b') { // Backspace key
                    backspace();
                } else if (ch == 26) { // Ctrl + Z for undo
                    undo();
                } else {
                    insert(ch); // Insert the character
                }

                displayText(); // Refresh display
                setCursorPosition(cursorX, cursorY); // Update cursor position
            }

         
            
        }
    }
};

int main() {
    TextEditor editor;
    editor.runEditor(); // Start the text editor
    return 0;
}
