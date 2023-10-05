#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char** argv) {
    string graph_filename = (string)argv[1] + ".graph";
    string mapping_filename = (string)argv[1] + ".satoutput";
    string output_filename = (string)argv[1] + ".mapping";
    fstream fin_mapping, fin_graph;
    fin_mapping.open(mapping_filename, ios::in);
    fin_graph.open(graph_filename, ios::in);
    if (!fin_mapping || !fin_graph) {
        cerr << "No such file\n";
        exit(0);
    }
    string first_line;
    getline(fin_mapping, first_line);
    if (first_line == "UNSAT") {
        cerr << "Unsatisfiable clauses, nothing to map\n";
        exit(0);
    }
    int n, e, k1, k2;
    getline(fin_graph, first_line);
    fin_graph.close();
    vector<int> graph_vars;
    graph_vars.push_back(0);
    for (int i = 0; i < first_line.length(); i++) {
        if (first_line[i] != ' ') {
            graph_vars.back() = 10 * graph_vars.back() + (first_line[i] - '0');
        } else {
            graph_vars.push_back(0);
        }
    }
    n = graph_vars[0];
    e = graph_vars[1];
    bool is_maximizing_problem = true;
    if (graph_vars.size() == 4) {
        is_maximizing_problem = false;
        k1 = graph_vars[2];
        k2 = graph_vars[3];
    }
    vector<int> clauses;
    int x = -1;
    while (x != 0) {
        fin_mapping >> x;
        clauses.push_back(x);
    }
    fin_mapping.close();
    vector<int> first_clique, second_clique;
    for (int i = 0; i < n; i++) {
        if (clauses[i] > 0) {
            first_clique.push_back(i+1);
        }
    }
    ofstream fout(output_filename);
    if (!fout.is_open()) {
        cerr << "Failed to open the file for writing." << endl;
        exit(0);
    }
    fout << "#1\n";
    for (int i = 0; i < first_clique.size(); i++) {
        fout << first_clique[i] << " \n"[i == first_clique.size() - 1];
    }
    if (!is_maximizing_problem) {
        int lower_limit = n + (n + 1) * (k1 + 1);
        int upper_limit = lower_limit + n;
        for (int i = lower_limit; i < upper_limit; i++) {
            if (clauses[i] > 0) {
                second_clique.push_back(i - lower_limit + 1);
            }
        }
        fout << "#2\n";
        for (int i = 0; i < second_clique.size(); i++) {
            fout << second_clique[i] << " \n"[i == second_clique.size() - 1];
        }
    }
    fout.close();
}
