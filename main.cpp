#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct TrieNode {
    TrieNode* children[26];
    bool isEndOfWord;

    TrieNode() {
        isEndOfWord = false;
        for(int i=0; i<26; i++)
            children[i] = nullptr;
    }
};

class SpellChecker {
    TrieNode* root;

public:
    SpellChecker() {
        root = new TrieNode();
    }

    void toLower(string &s) {
        for(char &c : s)
            if(c >= 'A' && c <= 'Z')
                c = c - 'A' + 'a';
    }

    void insert(string word) {
        TrieNode* node = root;
        for(char ch : word) {
            if(ch < 'a' || ch > 'z') continue;
            int index = ch - 'a';
            if(!node->children[index])
                node->children[index] = new TrieNode();
            node = node->children[index];
        }
        node->isEndOfWord = true;
    }

    bool search(string word) {
        TrieNode* node = root;
        for(char ch : word) {
            if(ch < 'a' || ch > 'z') return false;
            int index = ch - 'a';
            if(!node->children[index])
                return false;
            node = node->children[index];
        }
        return node->isEndOfWord;
    }

    void collectWords(TrieNode* node, string prefix, vector<string> &words) {
        if(node->isEndOfWord)
            words.push_back(prefix);
        for(int i=0; i<26; i++) {
            if(node->children[i])
                collectWords(node->children[i], prefix + char('a'+i), words);
        }
    }

    bool suggest(string prefix) {
        TrieNode* node = root;
        for(char ch : prefix) {
            if(ch < 'a' || ch > 'z') {
                cout << "Invalid input for suggestion.\n";
                return false;
            }
            int index = ch - 'a';
            if(!node->children[index]) {
                return false;
            }
            node = node->children[index];
        }
        vector<string> words;
        collectWords(node, prefix, words);

        if(words.empty()) {
            return false;
        } else {
            cout << "Did you mean:\n";
            for(auto &w : words)
                cout << "  " << w << "\n";
            return true;
        }
    }

    void suggestAllPossible(string word) {
        while(!word.empty()) {
            if(suggest(word))
                return;
            word.pop_back();
        }
        cout << "No suggestions found.\n";
    }

    bool deleteWord(TrieNode* node, string word, int depth=0) {
        if(!node)
            return false;

        if(depth == (int)word.size()) {
            if(!node->isEndOfWord)
                return false;
            node->isEndOfWord = false;
            for(int i=0; i<26; i++)
                if(node->children[i])
                    return false;
            return true;
        }

        int index = word[depth] - 'a';
        if(deleteWord(node->children[index], word, depth+1)) {
            delete node->children[index];
            node->children[index] = nullptr;
            return !node->isEndOfWord && isLeaf(node);
        }
        return false;
    }

    bool isLeaf(TrieNode* node) {
        for(int i=0; i<26; i++)
            if(node->children[i])
                return false;
        return true;
    }

    void deleteWord(string word) {
        if(deleteWord(root, word))
            cout << "'" << word << "' deleted.\n";
        else
            cout << "'" << word << "' not found or couldn't be deleted.\n";
    }

    void spellCheck(string word) {
        if(search(word)) {
            cout << "✅ The word '" << word << "' is correct (found).\n";
        } else {
            cout << "❌ The word '" << word << "' is NOT found.\n";
            suggestAllPossible(word);
        }
    }

    void loadDictionaryFromFile(const string &filename) {
        ifstream file(filename);
        if(!file) {
            cout << "Error opening dictionary file: " << filename << "\n";
            return;
        }
        string word;
        int count = 0;
        while(getline(file, word)) {
            toLower(word);
            insert(word);
            count++;
        }
        cout << count << " words loaded from " << filename << "\n";
    }

    void menu() {
        int choice;
        string word;
        while(true) {
            cout << "\n------ Spell Checker Menu ------\n";
            cout << "1. Spell Check a Word\n";
            cout << "2. Insert a Word\n";
            cout << "3. Search a Word\n";
            cout << "4. Delete a Word\n";
            cout << "5. Exit\n";
            cout << "Enter choice: ";
            cin >> choice;

            switch(choice) {
                case 1:
                    cout << "Enter word to spell check: ";
                    cin >> word;
                    toLower(word);
                    spellCheck(word);
                    break;
                case 2:
                    cout << "Enter word to insert: ";
                    cin >> word;
                    toLower(word);
                    insert(word);
                    cout << "'" << word << "' inserted.\n";
                    break;
                case 3:
                    cout << "Enter word to search: ";
                    cin >> word;
                    toLower(word);
                    if(search(word))
                        cout << "'" << word << "' found.\n";
                    else
                        cout << "'" << word << "' NOT found.\n";
                    break;
                case 4:
                    cout << "Enter word to delete: ";
                    cin >> word;
                    toLower(word);
                    deleteWord(word);
                    break;
                case 5:
                    cout << "Exiting...\n";
                    return;
                default:
                    cout << "Invalid choice, try again.\n";
            }
        }
    }
};

int main() {
    SpellChecker checker;

    checker.loadDictionaryFromFile("C:\\Users\\HOUSE OF TRADING\\Documents\\spell checker 1\\spellchecker1\\bin\\Debug\\words.txt");


    string userWord;
    cout << "\nEnter a word to check spelling: ";
    cin >> userWord;
    checker.toLower(userWord);
    checker.spellCheck(userWord);

    checker.menu();

    return 0;
}

