#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Structure of a Trie node
struct TrieNode {

	// Store address of a character
    //assuming all lower case for now
	TrieNode* Trie[26];

	// Check if the character is
	// last character of a string or not
	bool isEnd;

	// Constructor function
	TrieNode()
	{

		for (int i = 0; i < 26; i++) {

			Trie[i] = NULL;
		}
		isEnd = false;
	}
};

// Function to insert a string into Trie
void InsertTrie(TrieNode* root, string s)
{

	TrieNode* temp = root;

	// Traverse the string, s
	for (int i = 0; i < s.length(); i++) {
        
        //enters if the word has a 'never seen before' prefix
        //else goes along the prefixes of previous words in the dictionary
		if (temp->Trie[s[i]-'a'] == NULL) {

			// Initialize a node
			temp->Trie[s[i]-'a'] = new TrieNode();
		}

		// Update temp
		temp = temp->Trie[s[i]-'a'];
	}

	// Mark the last character of
	// the string to true
	temp->isEnd = true;
}

// Function to print suggestions of the string
void printSuggestions(TrieNode* root, string res)
{

	// If current character is
	// the last character of a string
	if (root->isEnd == true) {

		cout << res << " ";
	}

	// Iterate over all possible
	// characters of the string
	for (int i = 0; i < 26; i++) {

		// If current character
		// present in the Trie
		if (root->Trie[i] != NULL) {

			// Insert current character
			// into Trie
			res.push_back('a'+i);
			printSuggestions(root->Trie[i], res);
			res.pop_back();
		}
	}
}

// Function to check if the string
// is present in Trie or not
bool checkPresent(TrieNode* root, string key)
{

	// Traverse the string
	for (int i = 0; i < key.length(); i++) {

		// If current character not
		// present in the Trie
		if (root->Trie[key[i]-'a'] == NULL) {

			printSuggestions(root, key.substr(0, i));

			return false;
		}

		// Update root
		root = root->Trie[key[i]-'a'];
	}
	if (root->isEnd == true) {

		return true;
	}
	printSuggestions(root, key);

	return false;
}

// Driver Code
int main()
{

	// Given array of strings
	vector<string> str = { "boy", "bay", "buoy",
						"car", "ace","boat" };

	string key = "boy";

	// Initialize a Trie
	TrieNode* root = new TrieNode();

	// Insert strings to trie
	for (int i = 0; i < str.size(); i++) {
		InsertTrie(root, str[i]);
	}

	if (checkPresent(root, key)) {

		cout << "Correct";
	}
	
	return 0;
}
