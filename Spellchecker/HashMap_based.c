#include <iostream>
#include <unordered_set>
#include <vector>
#include <string>
#include <algorithm>
#include <limits.h>

using namespace std;

// Function to compute the Levenshtein Distance between two strings
//Understand from https://www.youtube.com/watch?v=MiqoA-yF-0M
int levenshteinDistance(const string& s1, const string& s2) {
    int n = s1.size(), m = s2.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1));

    // Initialize the Dynamic Prog table
    for (int i = 0; i <= n; ++i) dp[i][0] = i;
    for (int j = 0; j <= m; ++j) dp[0][j] = j;

    // Fill the DP table
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = min({dp[i - 1][j - 1], dp[i - 1][j], dp[i][j - 1]}) + 1;
            }
        }
    }
    return dp[n][m];
}

// Dictionary using unordered_set for easier lookup
//A complicated Hash Table can be implemented in future
class SpellChecker {
private:
    unordered_set<string> dictionary;

public:
    // Add a word to the dictionary
    void addWord(const string& word) {
        dictionary.insert(word);
    }

    // Check if a word is correctly spelled
    bool isCorrect(const string& word) {
        return dictionary.find(word) != dictionary.end();
    }

    // Suggest closest words if the input is misspelled
    vector<string> suggestWords(const string& word, int maxSuggestions = 3) {
        vector<pair<int, string>> candidates;

        // Compute Levenshtein distance for all words in the dictionary
        for (const auto& dictWord : dictionary) {
            int distance = levenshteinDistance(word, dictWord);
            candidates.push_back({distance, dictWord});
        }

        // Sort the candidates based on the distance and prepare suggestions
        sort(candidates.begin(), candidates.end());

        vector<string> suggestions;
        for (int i = 0; i < min(maxSuggestions, (int)candidates.size()); ++i) {
            suggestions.push_back(candidates[i].second);
        }

        return suggestions;
    }
};


int main() {
    SpellChecker spellChecker;

    // Add words to the dictionary
    spellChecker.addWord("car");
    spellChecker.addWord("ace");
    spellChecker.addWord("bay");
    spellChecker.addWord("buoy");
    spellChecker.addWord("boy");

    string input;
    cout << "Enter a word: ";
    cin >> input;

    if (spellChecker.isCorrect(input)) {
        cout << "The word is correctly spelled!" << endl;
    } else {
        cout << "The word is misspelled. Did you mean:" << endl;
        vector<string> suggestions = spellChecker.suggestWords(input);

        for (const auto& suggestion : suggestions) {
            cout << " - " << suggestion << endl;
        }
    }

    return 0;
}
