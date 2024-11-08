# Terminal Based Text Editor

This is a custom text editor developed in C++ with terminal support, designed to function similarly to popular word processors. It includes real-time typing, cursor movement, file handling, word count, autocomplete, undo/redo, auto-capitalization,spell check and color changing feature.

## Features

### 1. **Real-Time Typing**
   - **Functionality**: As you type, the text appears instantly on the terminal screen.
   - **Core Functions**: `runEditor()`, `displayText()`
   - **Description**: Captures each key press and updates the display without delay, providing an immediate typing response.

### 2. **Cursor Movement**
   - **Functionality**: Move the cursor using arrow keys (left, right, up, down).
   - **Core Functions**: `moveCursorLeft()`,`moveCursorRight()`,`moveCursorUp()`, `moveCursorDown()`,`setCursorPosition()`
   - **Description**: Allows easy navigation through the document using the arrow keys. The cursor can move horizontally or vertically in the text.

### 3. **Backspace/Delete**
   - **Functionality**: Move the cursor using arrow keys (left, right, up, down).
   - **Core Functions**: `backspace()`,`deleteChar()`
   - **Description**: Allows user to use the backspace and delete keys to rectify mistakes.
### 4. **File Handling**
   - **Functionality**: Open and save files to/from the system.
   - **Core Functions**: `updateTextFile()`
   - **Description**: Saves your work to a new txt file, which gets updated simultaneously as the user types on the terminal.

### 5. **Word Count**
   - **Functionality**: Display the number of words in the document.
   - **Core Functions**: `word_count()`
   - **Description**: Calculates and displays the word count in real-time as the user types, ensuring up-to-date information.

### 6. **Auto-Complete**
   - **Functionality**: Auto-completes words based on partially typed input.
   - **Description**: Uses a predefined list of words stored in a HashMap to provide suggestions. The user can press `Tab` to auto-complete the word.

### 7. **Undo/Redo**
   - **Functionality**: Undo and redo recent changes to the document.
   - **Core Functions**: `undo()`,`redo()`, `isStateChanged()`
   - **Description**: Allows multiple levels of undo(using `Ctrl+Z`) and redo(using `Ctrl+Y`) operations using a stack to track changes. Undo reverts the last change, and redo reinstates it.

### 8. **Auto-Capitalization**
   - **Functionality**: Automatically capitalizes the first letter of each new sentence as well as after a punctuation mark.
   - **Core Functions**: `shouldCapitalize()`, `insert_Capital()`, `insertCapitalNewLine()`
   - **Description**: Detects when the user starts a new sentence and capitalizes the first letter, improving typing speed and accuracy.

### 9. **Color Change**
   - **Functionality**: Changes color of text in the terminal
   - **Core Functions**: `setTextColour()`
   - **Description**: Pn pressing `Ctrl+R`, changes color of text on the terminal from a fixed set of colors for beautification.
   
### 9. **Spell Check**
   

## Setting up the application

### Prerequisites
- C++ Compiler (like GCC)
- Terminal or Console to run the application

### Installation
Clone the repository:
```bash
git clone https://github.com/yourusername/your-repo-name.git
```

## Contributions

- [Sarah Fatima](https://github.com/sarahfatima1205) — Implemented **Undo**, **Redo**, and **Auto Capitalization** features.

- [Satvik Garg](https://github.com/Satvik0110) — Developed **Real-time Display**, **File Handling**, and **Autocomplete** functionality.

- [Sohom Sarkar](https://github.com/Sohom-Sarkar) — Added **Cursor Movement**, **Backspace & Delete**, and **Spellchecker** features.

- [Soham Kulkarni](https://github.com/soham-k-11) — Integrated **Color Change** and **Live Word Count** functionality.


