#include <iostream>
#include <fstream>
#include <vector>
#include <random>

int main() {
    int n, m;
    std::string filename;

    std::cout << "Enter n: ";
    std::cin >> n;
    std::cout << "Enter m: ";
    std::cin >> m;
    std::cout << "Filename: ";
    std::cin >> filename;

    int N = n + m;

    std::ofstream out("test_files/" + filename + ".txt");
    if (!out) {
        std::cerr << "Cannot open file\n";
        return 1;
    }

    out << n << " " << m << "\n\n";

    // A matrix
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < N; ++j)
            out << "0 ";
        out << "\n";
    }

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (j <= i)
                out << rand() % 1000 << " ";
            else
                out << "0 ";
        }

        for (int j = 0; j < m; ++j)
            out << "0 ";

        out << "\n";
    }

    out << "\n";

    // b vector
    for (int i = 0; i < n; ++i)
        out << "0 ";
    for (int i = 0; i < m; ++i)
        out << rand() % 1000 << " ";
    out << "\n\n";

    // c vector
    for (int i = 0; i < n; ++i)
        out << rand() % 1000 << " ";
    for (int i = 0; i < m; ++i)
        out << "0 ";
    out << "\n";

    out.close();
    std::cout << "OK: file generated\n";
}
