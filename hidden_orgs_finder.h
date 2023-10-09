#include <fstream>
#include <string>
#include <vector>

using namespace std;

class HiddenOrgsFinder {

    private:
    int n, e, k1, k2, variables;
    long long clause_count;
    string filename;
    ofstream fout;
    vector<vector<int>> inv_adj;
    void create_clauses(int k);
    void write_first_line();

    public:
    HiddenOrgsFinder(string filename);
    void create_clauses_max(int k);
    void create_clauses_no_common();

};
