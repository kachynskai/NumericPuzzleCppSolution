// PortaOne test task: the longest single-row digit puzzle
//
// Build: g++ -std=c++17 -O2 -Wall -Wextra main.cpp -o puzzle
// Run:   ./puzzle source.txt
// Works with any file of digit-only fragments (one per line, at least 2 digits).
//
// Approach: each fragment is an edge from its first two digits to its last two
// in a graph of 100 vertices (00-99). The answer is the longest trail that uses
// each edge at most once, found by depth-first search with backtracking from
// every fragment as a starting point. Length is measured in digits of the
// resulting sequence, so fragments of different lengths are handled correctly.
//
// Developed with the help of Claude (Anthropic): algorithm discussion and code review.

#include <array>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>    

using namespace std;

struct Fragment {
    string text; 
    int head; //first two numbers
    int tail; //last two numbers
};

class PuzzleSolver {
    public:
        explicit PuzzleSolver(vector<Fragment> fragments)
            : fragments_(std::move(fragments)), used_(fragments_.size(), false) {
            for (size_t i = 0; i < fragments_.size(); ++i) {
                graph_[fragments_[i].head].push_back(static_cast<int>(i));
                gain_.push_back(static_cast<int>(fragments_[i].text.size()) - 2);
            }
        }

        vector<int> longestChain() {
            for (size_t start = 0; start < fragments_.size(); ++start) {
                used_[start] = true;
                path_.assign(1, static_cast<int>(start));
                dfs(fragments_[start].tail, static_cast<int>(fragments_[start].text.size()));
                used_[start] = false;
            }
            return best_;
        }
    
        string buildAnswer(const vector<int>& chain) const {
            if (chain.empty()) {
                return "";
            }
            string result = fragments_[chain[0]].text;
            for (size_t k = 1; k < chain.size(); ++k) {
                result += fragments_[chain[k]].text.substr(2);
            }
            return result;
        }
    
        size_t fragmentCount() const { return fragments_.size(); }
    
    private:    
        vector<Fragment> fragments_;
        array<vector<int>, 100> graph_;  // graph_[i] - idxs of fragments what start with i
        vector<int> gain_; // shows how current fragment will change path (because fragments may have diff length (min 2))
        vector<char> used_; // 1 if fragment is already in the chain
        vector<int> path_; // current path
        vector<int> best_; // best path on curr moment
        int bestLength_ = 0;

        void dfs(int vertex, int length) {

            if (length > bestLength_) {
                bestLength_ = length;
                best_ = path_;
            }

            for (int i : graph_[vertex]) {
                if (used_[i]) {
                    continue;
                }
                used_[i] = true;
                path_.push_back(i);
                dfs(fragments_[i].tail, length + gain_[i]);
                path_.pop_back();
                used_[i] = false;
            }
        }
};
    
string trim(const string& s) {
    const char* spaces = " \t\r";
    const auto first = s.find_first_not_of(spaces);

    if (first == string::npos) {
        return "";
    }
    const auto last = s.find_last_not_of(spaces);
    return s.substr(first, last - first + 1);
}

bool isDigitsOnly(const string& s) {
    for (char c : s) {
        if (c < '0' || c > '9') {
            return false;
        }
    }
    return true;
}

vector<Fragment> readFragments(const string& path) {
    ifstream file(path);
    if (!file) {
        throw runtime_error("cannot open the file " + path);
    }

    vector<Fragment> fragments;
    string rawLine;

    int lineNumber = 0;

    while (getline(file, rawLine)) {
        
        ++lineNumber;
        const string line = trim(rawLine);

        if (line.empty()) {
            continue;
        }

        if (line.size() < 2 || !isDigitsOnly(line)) {
            throw runtime_error("Line " + to_string(lineNumber) + ": fragment \"" + line +
                                     "\" must contain digits only (at least 2)");
        }

        const int head = stoi(line.substr(0, 2));
        const int tail = stoi(line.substr(line.size() - 2));
        fragments.push_back({line, head, tail});
    }

    if (fragments.empty()) {
        throw runtime_error("file does not contain any fragments");
    }
    return fragments;
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <path_to_the_file>\n";
        return 1;
    }

    try {
        PuzzleSolver solver(readFragments(argv[1]));
        const vector<int> chain = solver.longestChain();
        const string answer = solver.buildAnswer(chain);

        cout << "Tolal number of fragments: " << solver.fragmentCount() << '\n'
        << "Fragments in result: " << chain.size() << '\n'
        << "Length of the result: " << answer.size() << '\n'
        << "Result: " << answer << '\n';
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}