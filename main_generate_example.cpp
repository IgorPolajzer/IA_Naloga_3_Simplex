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

    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution aDist(0.2, 5.0);
    std::uniform_real_distribution bDist(50.0, 500.0);
    std::uniform_real_distribution cDist(1.0, 10.0);

    out << n << " " << m << "\n\n";

    // A matrix
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < N; ++j)
            out << "0 ";
        out << "\n";
    }

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j)
            out << aDist(gen) << " ";

        for (int j = 0; j < m; ++j)
            out << (i == j ? 1 : 0) << " ";

        out << "\n";
    }

    out << "\n";

    // c vector
    for (int i = 0; i < n; ++i)
        out << cDist(gen) << " ";
    for (int i = 0; i < m; ++i)
        out << "0 ";
    out << "\n\n";

    // b vector
    for (int i = 0; i < n; ++i)
        out << "0 ";
    for (int i = 0; i < m; ++i)
        out << bDist(gen) << " ";
    out << "\n";

    out.close();
    std::cout << "OK: file generated\n";
}
