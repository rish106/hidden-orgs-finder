#include <fstream>
#include <iostream>
#include <string>
#include "hidden_orgs_finder.h"

using namespace std;

string to_str_with_space(int n) {
    return to_string(n) + ' ';
}

HiddenOrgsFinder::HiddenOrgsFinder(string input_filename) {
    fstream fin;
    fin.open(input_filename, ios::in);
    if (!fin) {
        cerr << "No such file\n";
        exit(0);
    }
    string first_line;
    getline(fin, first_line);
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
    if (graph_vars.size() == 4) {
        k1 = graph_vars[2];
        k2 = graph_vars[3];
    }
    variables = 0;
    inv_adj.resize(n);
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            inv_adj[i].insert(j);
        }
    }
    int a, b, x, y;
    for (int i = 0; i < e; i++) {
        fin >> x >> y;
        x--, y--;
        a = min(x, y);
        b = max(x, y);
        inv_adj[a].erase(b);
    }
    fin.close();
}

void HiddenOrgsFinder::write_to_file(string output_filename) {
    ofstream fout(output_filename);
    if (!fout.is_open()) {
        cerr << "Failed to open the file for writing.\n";
        exit(0);
    }
    fout << "p cnf " << variables << ' ' << clauses.size() << '\n';
    for (int i = 0; i < clauses.size(); i++) {
        fout << clauses[i] << "0\n";
    }
    fout.close();
}

void HiddenOrgsFinder::create_clauses(int k) {
    int s[n+1][k+1];
    int t[n];
    for (int i = 0; i < n; i++) {
        t[i] = variables + i + 1;
    }
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= k; j++) {
            s[i][j] = variables + n + (i * (k + 1)) + j + 1;
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j : inv_adj[i]) {
            clauses.push_back(to_str_with_space(-t[i]) + to_str_with_space(-t[j]));
        }
    }
    clauses.push_back(to_str_with_space(s[0][0]));
    for (int j = 1; j <= k; j++) {
        clauses.push_back(to_str_with_space(-s[0][j]));
    }
    for (int i = 1; i <= n; i++) {
        clauses.push_back(to_str_with_space(s[i][0]));
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= k; j++) {
            clauses.push_back(to_str_with_space(-s[i][j]) + to_str_with_space(s[i-1][j]) + to_str_with_space(s[i-1][j-1]));
            clauses.push_back(to_str_with_space(-s[i][j]) + to_str_with_space(s[i-1][j]) + to_str_with_space(t[i-1]));
            clauses.push_back(to_str_with_space(s[i][j]) + to_str_with_space(-s[i-1][j]));
            clauses.push_back(to_str_with_space(s[i][j]) + to_str_with_space(-s[i-1][j-1]) + to_str_with_space(-t[i-1]));
        }
    }
    clauses.push_back(to_str_with_space(s[n][k]));
    variables += n + (n + 1) * (k + 1);
}

void HiddenOrgsFinder::create_clauses_no_common() {
    create_clauses(k1);
    create_clauses(k2);
    int lower_limit = n + (n + 1) * (k1 + 1);
    for (int i = 0; i < n; i++) {
        clauses.push_back(to_str_with_space(-i - 1) + to_str_with_space(-lower_limit - i - 1));
    }
}
