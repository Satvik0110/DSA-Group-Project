# DSA-Group-3
DSA Group 3 Project

Text processing using Stack and queue

Creating a Clone of Word software, having atleast  undo and redo functionalities.and should be performed efficiently . In use Stack and its Algorithms, Include concepts of File Handling.

//Spell Checker and Recommender
Some Statistics:
Number of words (N) in the avaerage vocab 35k
Number of characters (L) on avg 4.7

So average time complexity for:
Trie : 26^L ~ 4.4e6
Hash + Levenshtein d. :  O(N.L^2 + NlogN) ~ O(N.L^2) ~ avg = 13e6

Trie excels because of recommender systems
