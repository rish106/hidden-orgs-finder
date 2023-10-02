#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
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
    // unordered_map<int, pair<int, int>> mp;
    // for (int i = 0; i <= n; i++) {
    //     for (int j = 0; j <= k1; j++) {
    //         mp[n + i * (k1 + 1) + j + 1] = {i, j};
    //     }
    // }
    vector<int> clauses;
    int x = -1;
    while (x != 0) {
        fin_mapping >> x;
        clauses.push_back(x);
    }
    vector<int> clique_vertices;
    for (int i = 0; i < clauses.size(); i++) {
        if (i < n && clauses[i] > 0) {
            clique_vertices.push_back(i+1);
        // } else if (clauses[i] > 0 && i >= n) {
        //     cout << "s[" << mp[clauses[i]].first << "][" << mp[clauses[i]].second << "] is true\n";
        }
    }
    cout << "Found clique of size " << clique_vertices.size() << '\n';
    for (int it = 0; it < clique_vertices.size(); it++) {
        cout << clique_vertices[it] << " \n"[it == clique_vertices.size()-1];
    }
    ofstream fout(output_filename);
    if (!fout.is_open()) {
        cerr << "Failed to open the file for writing." << endl;
        exit(0);
    }
    if (is_maximizing_problem) {
        fout << "#1\n";
        for (int i = 0; i < clique_vertices.size(); i++) {
            fout << clique_vertices[i] << " \n"[i == clique_vertices.size() - 1];
        }
    } else {
        // separate the two cliques
        fout << "#1\n";
        for (int i = 0; i < clique_vertices.size(); i++) {
            fout << clique_vertices[i] << " \n"[i == clique_vertices.size() - 1];
        }
        fout << "#2\n";
        fout << '\n';
    }
    fout.close();
}
