#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>

using namespace std;

bool option_names_only = false;
bool option_ignore_case = false;
string search_pattern;

void parse_arguments(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        if (string(argv[i]) == "-n") {
            option_names_only = true;
        } else if (string(argv[i]) == "-i") {
            option_ignore_case = true;
        } else {
            search_pattern = argv[i];
        }
    }
}

bool contains_search_pattern(const string& str) {
    if (option_ignore_case) {
        string str_lower = str;
        transform(str_lower.begin(), str_lower.end(), str_lower.begin(), ::tolower);
        string search_pattern_lower = search_pattern;
        transform(search_pattern_lower.begin(), search_pattern_lower.end(), search_pattern_lower.begin(), ::tolower);
        return str_lower.find(search_pattern_lower) != string::npos;
    } else {
        return str.find(search_pattern) != string::npos;
    }
}

int main(int argc, char* argv[]) {
    parse_arguments(argc, argv);
    vector<string> env;
    char** envp = environ;
    while (*envp != NULL) {
        env.push_back(string(*envp));
        envp++;
    }
    for (const auto& var : env) {
        if (!search_pattern.empty() && !contains_search_pattern(var)) {
            continue;
        }
        if (option_names_only) {
            size_t equals_pos = var.find('=');
            if (equals_pos != string::npos) {
                cout << var.substr(0, equals_pos) << endl;
            }
        } else {
            cout << var << endl;
        }
    }
    return 0;
}








