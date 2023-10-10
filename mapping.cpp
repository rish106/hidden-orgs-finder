#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char** argv) {
    string graph_filename = (string)argv[1] + ".graph";
    string mapping_filename = (string)argv[1] + ".satoutput";
    string output_filename = (string)argv[1] + ".mapping";
    ifstream fin_mapping, fin_graph;
    fin_mapping.open(mapping_filename);
    fin_graph.open(graph_filename);
    ofstream fout(output_filename);
    if (!fin_mapping || !fin_graph) {
        cerr << "Graph or satoutput file missing\n";
        return 1;
    }
    if (!fout.is_open()) {
        cerr << "Failed to open the file for writing.\n";
        return 1;
    }
    string first_line;
    getline(fin_mapping, first_line);
    if (first_line == "UNSAT") {
        fout << 0 << '\n';
        fout.close();
        return 0;
    }
    int n, e, k1, k2;
    first_line.clear();
    getline(fin_graph, first_line);
    fin_graph.close();
    vector<int> graph_vars;
    graph_vars.push_back(0);
    for (int i = 0; i < first_line.length(); i++) {
        if (first_line[i] != ' ') {
            graph_vars[graph_vars.size() - 1] = 10 * graph_vars.back() + (first_line[i] - '0');
        } else {
            graph_vars.push_back(0);
        }
    }
    n = graph_vars[0];
    e = graph_vars[1];
    int problem_num = stoi(argv[2]);
    bool is_maximizing_problem = (problem_num == 2);
    if (!is_maximizing_problem) {
        k1 = graph_vars[2];
        k2 = graph_vars[3];
    }
    vector<bool> sat_variables;
    int x = -1;
    while (x != 0) {
        fin_mapping >> x;
        sat_variables.push_back(x > 0);
    }
    fin_mapping.close();
    vector<int> first_clique;
    for (int i = 0; i < n && i < sat_variables.size(); i++) {
        if (sat_variables[i]) {
            first_clique.push_back(i+1);
        }
    }
    fout << "#1\n";
    for (int i = 0; i < first_clique.size(); i++) {
        fout << first_clique[i] << " \n"[i == first_clique.size() - 1];
    }
    if (!is_maximizing_problem) {
        vector<int> second_clique;
        int lower_limit = n + (n + 1) * (k1 + 1);
        int upper_limit = lower_limit + n;
        for (int i = lower_limit; i < upper_limit && i < sat_variables.size(); i++) {
            if (sat_variables[i]) {
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
