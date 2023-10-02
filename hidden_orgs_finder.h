#include <set>
#include <string>
#include <vector>

using namespace std;

class HiddenOrgsFinder {

    private:
    int n, e, k1, k2, variables;
    vector<vector<int>> adj;
    vector<set<int>> inv_adj;
    vector<string> clauses;

    public:
    HiddenOrgsFinder(string input_filename);
    void write_to_file(string output_filename);
    void create_clauses(int k);
    void create_clauses_no_common();

};
