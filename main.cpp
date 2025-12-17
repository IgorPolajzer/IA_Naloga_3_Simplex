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
        for (int i = 1; i <= m; i++) B.emplace_back(n + i);
        v = 0;
    } else {
        throw -2; // Negativna vrednost v sektorju b.
    }
}

void pivot(vector<vector<double>>& A, vector<double>& b, vector<double>& c, vector<int>& N, vector<int>& B, int& v, int& l, int& e) {
    b[e - 1] = b[l - 1] / A[l - 1][e - 1];

    for (int j : N) {
        if (j != e) {
            A[e - 1][j - 1] = A[l - 1][j - 1] / A[l - 1][e - 1];
        }
    }
    A[e - 1][l - 1] = 1 / A[l - 1][e - 1];

    for (int i : B) {
        if (i != l) {
            b[i - 1] = b[i - 1] - A[i - 1][e - 1] * b[e - 1];

            for (int j : N) {
                if (j != e) {
                    A[i - 1][j - 1] = A[i - 1][j - 1] - A[i - 1][e - 1] * A[e - 1][j - 1];
                }
            }
            A[i - 1][l - 1] = -A[i - 1][e - 1] * A[e - 1][l - 1];
        }
    }

    v = v + c[e - 1] * b[e - 1];
    for (int j : N) {
        if (j != e) {
            c[j - 1] = c[j - 1] - c[e - 1] * A[e - 1][j - 1];
        }
    }
    c[l - 1] = -c[e - 1] * A[e - 1][l - 1];

    // Replace e with l in N.
    for (int & i : N) if (i == e) i = l;

    // Replace l with e in B.
    for (int & i : B) if (i == l) i = e;
}

tuple<vector<double>, int> simplex(ifstream &file) {
    vector<vector<double>> A;
    vector<double> b, c;
    vector<int> N, B;
    int n, m, v;
    try {
        parseFile(file, A, b, c, n, m);
        initialize_simplex(A, b, N, B, v, n, m);

        bool hasPositive = true;
        while (hasPositive) {

            // Check if any c[k] > 0 where k in N and choose e.
            hasPositive = false;
            int e = -1;
            for (int num : N) {
                if (c[num - 1] > 0) {
                    e = num;
                    hasPositive = true;
                    break;
                }
            }

            if (!hasPositive) break;

            vector<double> delta(n + m);
            for (int j : B) {
                if (A[j - 1][e - 1] > 0) delta[j - 1] = b[j - 1] / A[j - 1][e - 1];
                else delta[j - 1] = INT_MAX; // Infinity.
            }

            // Find the index l in B with minimum delta value
            int l = B[0];
            double minDelta = delta[l - 1];
            for (int j : B) {
                if (delta[j - 1] < minDelta) {
                    minDelta = delta[j - 1];
                    l = j;
                }
            }


            if (minDelta  == INT_MAX) throw -3; // Neomejen program
            pivot(A, b, c, N, B, v, l, e);
        }

        vector<double> x(n + m);
        for (int i = 1; i <= n + m; i++) {
            bool isInB = false;
            for (int j : B) {
                if (j == i) {
                    x[i - 1] = b[i - 1];
                    isInB = true;
                    break;
                }
            }
            if (!isInB) {
                x[i - 1] = 0;
            }
        }

        return make_tuple(x, v);
    } catch (int e) {
        if (e == -1) {
            cout << "Napaka pri razčlenjevanju datoteke." << endl;
        } else if (e == -2) {
            cout << "Negativna vrednost v sektorju b." << endl;
        } else if (e == -3) {
            cout << "Neomejen program." << endl;
        }
        return make_tuple(vector<double>(), -1);
    }
}

int main (int argc, const char * argv[]) {
    string file_name = argv[1];

    ifstream file(file_name);

    if (file.is_open()) {
        tuple<vector<double>, int> result = simplex(file);
        vector<double> x = get<0>(result);
        int z = get<1>(result);

        if (z != -1) {
            for (int i = 0; i < x.size(); i++) {
                cout << "x" << (i + 1) << ": " << x[i] << endl;
            }
            cout << endl << "z: " << z << endl;
        }
    }

    return 0;
}