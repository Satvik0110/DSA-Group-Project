#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct TrieNode {
    TrieNode* children[26];
    bool isEndOfWord;
    TrieNode() : isEndOfWord(false) {
        fill(begin(children), end(children), nullptr);
    }
};

class Trie {
public:
    TrieNode* root;
    
    Trie() {
        root = new TrieNode();
    }
    
    // Insert a word into the Trie
    void insert(const string& word) {
        TrieNode* node = root;
        for (char ch : word) {
            int index = ch - 'a';
            if (node->children[index] == nullptr) {
                node->children[index] = new TrieNode();
            }
            node = node->children[index];
        }
        node->isEndOfWord = true;
    }
    
    // Search for a word in the Trie
    bool search(const string& word) {
        TrieNode* node = root;
        for (char ch : word) {
            int index = ch - 'a';
            if (node->children[index] == nullptr) {
                return false;
            }
            node = node->children[index];
        }
        return node->isEndOfWord;
    }

    // Collect suggestions
    void collectSuggestions(TrieNode* node, string& prefix, vector<string>& suggestions) {
        if (node->isEndOfWord) {
            suggestions.push_back(prefix);
        }
        for (int i = 0; i < 26 && suggestions.size() < 5; ++i) {  // Stop after 5 suggestions
            if (node->children[i] != nullptr) {
                prefix.push_back('a' + i);
                collectSuggestions(node->children[i], prefix, suggestions);
                prefix.pop_back();
            }
        }
    }

    // Get suggestions for a word
    vector<string> getSuggestions(const string& prefix) {
        TrieNode* node = root;
        vector<string> suggestions;
        for (char ch : prefix) {
            int index = ch - 'a';
            if (node->children[index] == nullptr) {
                return suggestions;  // No suggestions found
            }
            node = node->children[index];
        }
        collectSuggestions(node, const_cast<string&>(prefix), suggestions);
        return suggestions;
    }
};

// Load dictionary into Trie
void loadDictionary(Trie& trie, const string& dictionaryFile) {
    ifstream dictFile(dictionaryFile);
    string word;
    while (getline(dictFile, word)) {
        trie.insert(word);
    }
    dictFile.close();
}

// Spellcheck a file and output suggestions for misspelled words
void spellcheckAndSuggest(Trie& trie, const string& inputFile, const string& outputFile) {
    ifstream inFile(inputFile);
    ofstream outFile(outputFile);
    string word;
    
    while (inFile >> word) {
        // Remove punctuation if any
        word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end());
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        
        if (!trie.search(word)) {
            outFile << word << " Did you mean?\n";
            vector<string> suggestions = trie.getSuggestions(word.substr(0, 2));  // Use first 2 letters for suggestions
            int suggestionCount = 0;
            for (const string& suggestion : suggestions) {
                outFile << suggestion << " ";
                if (++suggestionCount >= 5) break;  // Limit to top 5 suggestions
            }
            outFile << "\n\n";
        }
    }

    inFile.close();
    outFile.close();
}

int main() {
    Trie trie;
    loadDictionary(trie, "C:/Users/sohom/Downloads/DSA-Group-3-main/DSA-Group-3-main/Primary Implementations/dictionary.txt");  // Load words from dictionary.txt

    // Spellcheck myDoc.txt and write suggestions to suggestions.txt
    spellcheckAndSuggest(trie, "C:/Users/sohom/Downloads/DSA-Group-3-main/DSA-Group-3-main/Primary Implementations/myDoc.txt", "C:/Users/sohom/Downloads/DSA-Group-3-main/DSA-Group-3-main/Primary Implementations/suggestions.txt");

    cout << "Spellcheck completed. Check suggestions.txt for results.\n";
    return 0;
}
