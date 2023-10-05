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
    string input_filename = (string)argv[1] + ".graph";
    string output_filename = (string)argv[1] + ".satinput";
    HiddenOrgsFinder* h = new HiddenOrgsFinder(input_filename);
    if (argc == 3) {
        int k = stoi(argv[2]);
        h->create_clauses(k);
    } else {
        h->create_clauses_no_common();
    }
    h->write_to_file(output_filename);
}
