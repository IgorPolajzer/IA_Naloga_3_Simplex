#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

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

void parseFile(ifstream& file, vector<vector<double>>& A, vector<double>& b, vector<double>& c, vector<int>& N, vector<int>& B) {
    string line;

    int lineNumber = 0;
    int A_size = -1;
    while (getline(file, line)) {

        if (lineNumber == 0) {
            if (line.size() < 2) throw -1;

            int N_size = stoi(&line[0]);
            int B_size = stoi(&line[2]);
            A_size = N_size + B_size;

            for (int i = 1; i <= N_size + B_size; i++) {
                if (i <= N_size) N.push_back(i);
                else if (i > N_size) B.push_back(i);
            }
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

void initialize_simplex(vector<vector<double>>& A, vector<double>& b, vector<double>& c, vector<int>& N, vector<int>& B, int& v) {

    if (*ranges::min_element(b) >= 0) {
        int n = A.size();
        int m = A[0].size();

        for (int i = 1; i < n; i++) N.emplace_back(i);
        for (int i = 1; i < n + m; i++) B.emplace_back(n + i);
        v = 0;
    } else {
        throw -1;
    }
}

void simplex(ifstream& file) {
    vector<vector<double>> A;
    vector<double> b, c;
    vector<int> N, B;
    int v;
    try {
        parseFile(file, A, b, c, N, B);
        // vector<int> N, B; // RESET FOR TESTING PURPOSES, should be fixed later
        //initialize_simplex(A, b, c, N, B, v);

        /*int i = 0;
        while (c[i] > 0) {
            int e = 1;
            while (c[e] > 0) {
                e++;
            }

            vector<double> delta(B.size());
            for (double j : B) {
                if (A[j][e] > 0) {
                    delta[j] = b[j] / A[j]/
                }
            }


            i++;
        }*/

        cout << A.size() << " " << b.size() << " " << c.size() << " " << N.size() << endl;
    } catch (int e) {
        cout << "Failed to parse file. Check for negative values in file";
    }
}

int main (int argc, const char * argv[]) {
    string file_name = argv[1];

    ifstream file(file_name);

    if (file.is_open()) {
        simplex(file);
    }

    return 0;
}