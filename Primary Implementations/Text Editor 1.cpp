#include <iostream>
#include <stack>
#include <vector>
#include <conio.h>      // For _getch() and _kbhit()
#include <windows.h>    // For SetConsoleCursorPosition()
#include<fstream>
#include <unordered_map>
using namespace std;

const int MAX_UNDO_SIZE = 100; // Set a limit for the undo stack size

class TextEditor {
private:
    stack<char> leftStack;   // Stores the text to the left of the cursor
    stack<char> rightStack;  // Stores the text to the right of the cursor
    vector<stack<char>> lines;  // Each line is a separate stack
    int currentLine = 0;        // Track which line the cursor is on
    int cursorX = 0, cursorY = 0; // Tracks the cursor's x and y position (for console display)
    vector<vector<stack<char>>> undoStack; // Undo stack to store previous states
    vector<vector<stack<char>>> redoStack; // Redo stack for redo functionality
    unordered_map<string,string> autocompleteWords = {
    {"add", "address"},
    {"adm", "administration"},
    {"ag", "agreement"},
    {"ans", "answer"},
    {"app", "application"},
    {"arg", "argument"},
    {"ass", "assignment"},
    {"aut", "automatic"},
    {"beg", "beginning"},
    {"bel", "believe"},
    {"ben", "benefit"},
    {"bet", "between"},
    {"bro", "brother"},
    {"bu", "business"},
    {"cal", "calendar"},
    {"cap", "capacity"},
    {"cha", "character"},
    {"cho", "choice"},
    {"cla", "class"},
    {"cli", "client"},
    {"com", "communication"},
    {"con", "contract"},
    {"cor", "correction"},
    {"cou", "country"},
    {"cre", "credit"},
    {"dec", "decision"},
    {"del", "delivery"},
    {"dep", "department"},
    {"dev", "development"},
    {"dir", "direction"},
    {"dis", "discussion"},
    {"doc", "document"},
    {"dra", "draft"},
    {"edu", "education"},
    {"eff", "effect"},
    {"emp", "employee"},
    {"enc", "encourage"},
    {"equ", "equipment"},
    {"est", "establish"},
    {"eve", "event"},
    {"exp", "experience"},
    {"fin", "financial"},
    {"fol", "following"},
    {"for", "formation"},
    {"fun", "function"},
    {"gen", "general"},
    {"gro", "group"},
    {"gui", "guidance"},
    {"hea", "health"},
    {"his", "history"},
    {"ide", "idea"},
    {"imp", "important"},
    {"ind", "individual"},
    {"inf", "information"},
    {"int", "interest"},
    {"inv", "investment"},
    {"jud", "judgment"},
    {"jus", "justice"},
    {"lan", "language"},
    {"leg", "legal"},
    {"lev", "level"},
    {"lib", "library"},
    {"loc", "location"},
    {"man", "management"},
    {"mat", "material"},
    {"mea", "measure"},
    {"mem", "member"},
    {"met", "method"},
    {"mil", "military"},
    {"nat", "national"},
    {"nee", "necessary"},
    {"net", "network"},
    {"not", "notice"},
    {"obj", "object"},
    {"off", "office"},
    {"ope", "operation"},
    {"org", "organization"},
    {"par", "parent"},
    {"pat", "pattern"},
    {"per", "performance"},
    {"pla", "platform"},
    {"pol", "policy"},
    {"pos", "position"},
    {"pre", "presentation"},
    {"pro", "program"},
    {"pub", "public"},
    {"qui", "quickly"},
    {"rea", "reason"},
    {"rec", "recommend"},
    {"rel", "relationship"},
    {"rep", "report"},
    {"res", "resource"},
    {"res", "response"},
    {"rev", "review"},
    {"sec", "section"},
    {"ser", "service"},
    {"sig", "significant"},
    {"sim", "similar"},
    {"soc", "social"},
    {"sta", "standard"},
    {"str", "structure"},
    {"sys", "system"},
    {"the", "theory"},
    {"typ", "typical"},
    {"uni", "university"},
    {"val", "value"},
    {"vie", "view"},
    {"wor", "worker"}
};


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
    }

    // Insert a character at the current cursor position
    void insert(char ch) {
        leftStack.push(ch);
        lines[currentLine] = leftStack;
        redoStack.clear(); // Clear the redo stack when a new character is inserted

        if (isStateChanged()) {
            undoStack.push_back(lines); // Push only if the state has changed
            // Limit the undo stack size
            if (undoStack.size() > MAX_UNDO_SIZE) {
                undoStack.erase(undoStack.begin()); // Remove the oldest state if limit exceeded
            }
        }
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
            rightStack = stack<char>();        // Clear rightStack
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
            redoStack.clear(); // Clear the redo stack when delete is used
        }
    }

    // Insert a newline (Enter key)
    void insertNewLine() {
        lines[currentLine] = leftStack;   // Store current line's state
        redoStack.clear(); // Clear the redo stack when a new line is inserted

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
            redoStack.push_back(lines);  // Store current state in redo before undoing
            undoStack.pop_back();        // Remove the current state
            lines = undoStack.back();    // Restore the previous state
            if (currentLine >= lines.size()) currentLine = lines.size() - 1;
            leftStack = lines[currentLine];
            rightStack = stack<char>();  // Clear the rightStack after undo
        }
    }

    void redo() {
        if (!redoStack.empty()) {
            undoStack.push_back(lines);   // Save current state to undo stack
            lines = redoStack.back();     // Restore the redo state
            redoStack.pop_back();         // Remove the redo state
            if (currentLine >= lines.size()) currentLine = lines.size() - 1;
            leftStack = lines[currentLine];
            rightStack = stack<char>();  // Clear rightStack after redo
        }
    }

    void save(){
        ofstream file("myDoc.txt");
        for (const auto& lineStack : lines) {
        // We need to output characters in the order they were added, so reverse the stack
        std::stack<char> tempStack = lineStack;  // Make a copy of the current stack
        std::stack<char> reverseStack;

        // Reverse the stack to maintain original order of characters
        while (!tempStack.empty()) {
            reverseStack.push(tempStack.top());
            tempStack.pop();
        }

        // Write characters from the reversed stack into the file
        while (!reverseStack.empty()) {
            file << reverseStack.top();
            reverseStack.pop();
        }
        file << '\n';
    }
    file.close();
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
                        case 72: // Up arrow key
                            moveCursorUp();
                            break;
                        case 80: // Down arrow key
                            moveCursorDown();
                            break;
                        case 83: // Delete key (ASCII code 83)
                            deleteChar();
                            break;
                    }
                }
                else if (ch == 8) {  // Backspace
                    backspace();
                }
                else if (ch == 13) {  // Enter key
                    insertNewLine();
                }
                else if (ch == 26) {  // Ctrl + Z (Undo)
                    undo();
                }
                else if (ch == 25) {  // Ctrl + Y (Redo)
                    redo();
                }
                 else if (ch == 19) {  // Ctrl + S (Save)
                    save();
                }
                else if (ch == 27) {  // ESC key to exit
                    break;
                }
                else {  // Regular character input
                    insert(ch);
                    cursorX++;
                }

                displayText();
                setCursorPosition(cursorX, cursorY);
            }
        }
    }
};

int main() {
    TextEditor editor;
    editor.runEditor();
    return 0;
}
