
#include <fstream>
#include <iostream>
using namespace std;

struct Fragment {
    string text; 
    int head; //first two numbers
    int tail; //last two numbers
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

bool isDigitsOnly(const std::string& s) {
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
            throw runtime_error("row " + to_string(lineNumber) + ": fragment \"" + line +
                                     "\" should contain digits only (min 2)");
        }
        const int head = stoi(line.substr(0, 2));
        const int tail = stoi(line.substr(line.size() - 2));
        fragments.push_back({line, head, tail});
    }

    if (fragments.empty()) {
        throw runtime_error("file does not contains any fragments");
    }
    return fragments;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "use: " << argv[0] << " <path_to_the_file>\n";
        return 1;
    }
    try {
        const vector<Fragment> fragments = readFragments(argv[1]);
        cout << "Number of fragments: " << fragments.size() << '\n';

        for (const Fragment& f : fragments) {
            cout << f.text << ": " << f.head << " -> " << f.tail << '\n';
        }
        
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}