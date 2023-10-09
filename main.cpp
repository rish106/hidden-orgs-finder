#include <iostream>
#include <fstream>
#include <string>

#include "hidden_orgs_finder.h"

using namespace std;

int main(int argc, char** argv) {
    if (argc > 3) {
        cout << "Too many arguments, enter '.graph' filename as 1st argument and clique size as 2nd argument(optional)\n";
        return 1;
    } else if (argc < 2) {
        cout << "'.graph' filename is required\n";
        return 1;
    }
    string filename = (string)argv[1];
    HiddenOrgsFinder* h = new HiddenOrgsFinder(filename);
    if (argc == 3) {
        int k = stoi(argv[2]);
        h->create_clauses_max(k);
    } else {
        h->create_clauses_no_common();
    }
}
