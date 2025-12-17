#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

void parseLine(string line, vector<double> &numbers) {
    string number;
    for (int i = 0; i <= line.size(); i++) {
        if (line[i] != ' ') {
            number += line[i];
        } else {
            numbers.emplace_back(stod(number));
            number = "";
        }
    }

    try {
        numbers.emplace_back(stod(number));
    } catch (invalid_argument e) {
    }
}

void parseFile(ifstream& file, vector<vector<double>>& A, vector<double>& b, vector<double>& c,
    int &n, int& m) {
    string line;

    int lineNumber = 0;
    int A_size = -1;
    while (getline(file, line)) {

        if (lineNumber == 0) {
            if (line.size() < 2) throw -1;

            n = stoi(&line[0]);
            m = stoi(&line[2]);
            A_size = n + m;
        } else if (lineNumber >= 2 && lineNumber <= A_size + 1) {
            vector<double> numbers;

            parseLine(line, numbers);
            if (A_size != numbers.size()) throw -1;

            A.emplace_back(numbers);

        } else if (lineNumber == A_size + 3) {
            parseLine(line, b);
            if (A_size != b.size()) throw -1;
        } else if (lineNumber == A_size + 5) {
            parseLine(line, c);
            if (A_size != c.size()) throw -1;
        }


        lineNumber++;
    }
}

void initialize_simplex(vector<vector<double>>& A, vector<double>& b, vector<int>& N, vector<int>& B, int& v, int& n, int& m) {

    if (*ranges::min_element(b) >= 0) {
        for (int i = 1; i <= n; i++) N.emplace_back(i);
        for (int i = 1; i <= n + m; i++) B.emplace_back(n + i);
        v = 0;
    } else {
        throw -1;
    }
}

void pivot(vector<vector<double>>& A, vector<double>& b, vector<double>& c, vector<int>& N, vector<int>& B, int& v, int& l, int& e) {
    b[e] = b[l] / A[l][e];

    for (int j : N) {
        if (j != e) {
            A[e][j - 1] = A[l][j - 1] / A[l][e];
        }
    }
    A[e][l] = 1 / A[l][e];

    for (int i : B) {
        if (i != l) {
            b[i - 1] = b[i - 1] - A[i - 1][e] * b[e];

            for (int j : N) {
                if (j != e) {
                    A[i - 1][j - 1] = A[i - 1][j - 1] * A[e][j - 1];
                }
                A[i - 1][l] = -A[i - 1][e] * A[e][l];
            }
        }
    }

    v = v + c[e] * b[e];
    for (int j : N) {
        if (j != e) {
            c[j - 1] = c[j - 1] - c[e] * A[e][j - 1];
        }
    }
    c[l] = -c[e] * A[e][l];

    // Replace e with l in N.
    for (int & i : N) if (i == e) i = l;

    // Replace l with e in B.
    for (int & i : B) if (i == l) i = e;
}

/*tuple<> pivot(vector<vector<double>>& A, vector<double>& b, vector<double>& c, vector<int>& N, vector<int>& B, int v, int l, int e) {

}*/

tuple<vector<double>, int> simplex(ifstream &file) {
    vector<vector<double>> A;
    vector<double> b, c;
    vector<int> N, B;
    int n, m, v;
    try {
        parseFile(file, A, b, c, n, m);
        initialize_simplex(A, b, N, B,  n, m, v);

        int i = 0;
        while (c[i] > 0) {
            int e = 1;
            while (c[e] > 0) {
                e++;
            }

            vector<double> delta(B.size());
            for (int j : B) {
                if (A[j - 1][e] > 0) delta[j - 1] = b[j - 1] / A[j - 1][e];
                else delta[j - 1] = -1; // Infinity.
            }
            int l = *ranges::min_element(delta);

            if (delta[0] == -1) throw -1;
            pivot(A, b, c, N, B, v, l, e);

            i++;
        }

        // Concatenate N and B into N.
        // N.insert(N.end(), B.begin(), B.end());
        vector<double> x;
        for (int i = N.size() + B.size(); i++;) {
            if (i == B.size()) {
                x.push_back(b[i]);
            } else {
                x.push_back(0);
            }
        }

        return make_tuple(x, v);
    } catch (int e) {
        cout << "Failed to parse file. Check for negative values in file";
    }
}

int main (int argc, const char * argv[]) {
    string file_name = argv[1];

    ifstream file(file_name);

    if (file.is_open()) {
        tuple<vector<double>, int> result = simplex(file);
        cout << "";
    }

    return 0;
}