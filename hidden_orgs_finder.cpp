#include <fstream>
#include <iostream>
#include <string>
#include "hidden_orgs_finder.h"

using namespace std;

HiddenOrgsFinder::HiddenOrgsFinder(string filename) {
    this->filename = filename;
    string input_filename = filename + ".graph";
    ifstream fin;
    fin.open(input_filename);
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
    clause_count = 0;
    vector<vector<bool>> edge_complement(n);
    inv_adj.resize(n);
    for (int i = 0; i < n; i++) {
        edge_complement[i].assign(n, true);
    }
    int a, b, x, y;
    for (int i = 0; i < e; i++) {
        fin >> x >> y;
        x--, y--;
        a = min(x, y);
        b = max(x, y);
        edge_complement[a][b] = false;
    }
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            if (edge_complement[i][j]) {
                inv_adj[i].push_back(j);
            }
        }
        clause_count += inv_adj[i].size();
    }
    fin.close();
}

void HiddenOrgsFinder::write_first_line() {
    string output_filename = filename + ".satinput";
    fout.open(output_filename);
    if (!fout.is_open()) {
        cerr << "Failed to open the file for writing.\n";
        exit(0);
    }
    fout << "p cnf " << variables << ' ' << clause_count << '\n';
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
            fout << -t[i] << ' ' << -t[j] << " 0\n";
        }
    }
    for (int i = 0; i <= n; i++) {
        fout << s[i][0] << " 0\n";
    }
    for (int j = 1; j <= k; j++) {
        fout << -s[0][j] << " 0\n";
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= k; j++) {
            fout << -s[i][j] << ' ' << s[i-1][j] << ' ' << s[i-1][j-1] << " 0\n";
            fout << -s[i][j] << ' ' << s[i-1][j] << ' ' << t[i-1] <<  " 0\n";
            fout << s[i][j] << ' ' << -s[i-1][j] << " 0\n";
            fout << s[i][j] << ' ' << -s[i-1][j-1] << ' ' << -t[i-1] << " 0\n";
        }
    }
    fout << s[n][k] << " 0\n";
    variables += n + (n + 1) * (k + 1);
}

void HiddenOrgsFinder::create_clauses_max(int k) {
    clause_count += n + k + 2 + 4*n*k;
    variables += n + (n + 1) * (k + 1);
    write_first_line();
    variables = 0;
    create_clauses(k);
    fout.close();
}

void HiddenOrgsFinder::create_clauses_no_common() {
    int lower_limit = n + (n + 1) * (k1 + 1);
    clause_count *= 2;
    clause_count += 3*n + k1 + k2 + 4 + 4*n*k1 + 4*n*k2;
    variables += n + (n + 1) * (k1 + 1);
    variables += n + (n + 1) * (k2 + 1);
    write_first_line();
    variables = 0;
    create_clauses(k1);
    create_clauses(k2);
    for (int i = 0; i < n; i++) {
        fout << -i - 1 << ' ' << -lower_limit - i - 1 << " 0\n";
    }
    fout.close();
}
