#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <iomanip>

using namespace std;

using Matrix = vector<vector<double>>;


// Matrix multiplication
Matrix matrixMultiply(const Matrix& A, const Matrix& B) {

    int rowsA = A.size();
    int colsA = A[0].size();
    int rowsB = B.size();
    int colsB = B[0].size();

    if (colsA != rowsB) {
        throw invalid_argument("Matrices cannot be multiplied");
    }

    Matrix C(rowsA, vector<double>(colsB, 0.0));

    for (int i = 0; i < rowsA; i++) {
        for (int k = 0; k < colsA; k++) {
            for (int j = 0; j < colsB; j++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return C;
}


// Generate random matrix
Matrix generateMatrix(int rows, int cols) {

    random_device rd;
    mt19937 generator(rd());

    uniform_real_distribution<double> distribution(0.0, 1.0);

    Matrix matrix(rows, vector<double>(cols));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = distribution(generator);
        }
    }

    return matrix;
}


int main() {

    vector<int> sizes = {100, 200, 500, 1000};

    cout << "C++ Matrix Multiplication\n";
    cout << "-------------------------\n\n";

    for (int n : sizes) {

        Matrix A = generateMatrix(n, n);
        Matrix B = generateMatrix(n, n);

        auto start = chrono::high_resolution_clock::now();

        Matrix C = matrixMultiply(A, B);

        auto end = chrono::high_resolution_clock::now();

        chrono::duration<double> elapsed = end - start;

        cout << "Matrix size: " << n << "x" << n << endl;
        cout << "Time: "
             << fixed << setprecision(6)
             << elapsed.count()
             << " seconds\n\n";
    }

    return 0;
}
