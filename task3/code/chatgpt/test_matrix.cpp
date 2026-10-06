#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>
#include <stdexcept>

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


// Compare two matrices
bool matricesEqual(const Matrix& A, const Matrix& B) {

    if (A.size() != B.size()) {
        return false;
    }

    for (size_t i = 0; i < A.size(); i++) {

        if (A[i].size() != B[i].size()) {
            return false;
        }

        for (size_t j = 0; j < A[i].size(); j++) {

            if (fabs(A[i][j] - B[i][j]) > 1e-9) {
                return false;
            }
        }
    }

    return true;
}


// Test 1: basic multiplication
void testBasicMultiplication() {

    Matrix A = {
        {1, 2},
        {3, 4}
    };

    Matrix B = {
        {5, 6},
        {7, 8}
    };

    Matrix expected = {
        {19, 22},
        {43, 50}
    };

    Matrix result = matrixMultiply(A, B);

    assert(matricesEqual(result, expected));

    cout << "Test 1 passed: Basic multiplication\n";
}


// Test 2: identity matrix
void testIdentityMatrix() {

    Matrix A = {
        {2, 3},
        {4, 5}
    };

    Matrix identity = {
        {1, 0},
        {0, 1}
    };

    Matrix result = matrixMultiply(A, identity);

    assert(matricesEqual(result, A));

    cout << "Test 2 passed: Identity matrix\n";
}


// Test 3: zero matrix
void testZeroMatrix() {

    Matrix A = {
        {1, 2},
        {3, 4}
    };

    Matrix zero = {
        {0, 0},
        {0, 0}
    };

    Matrix expected = {
        {0, 0},
        {0, 0}
    };

    Matrix result = matrixMultiply(A, zero);

    assert(matricesEqual(result, expected));

    cout << "Test 3 passed: Zero matrix\n";
}


// Test 4: rectangular matrices
void testRectangularMatrices() {

    Matrix A = {
        {1, 2, 3},
        {4, 5, 6}
    };

    Matrix B = {
        {7, 8},
        {9, 10},
        {11, 12}
    };

    Matrix expected = {
        {58, 64},
        {139, 154}
    };

    Matrix result = matrixMultiply(A, B);

    assert(matricesEqual(result, expected));

    cout << "Test 4 passed: Rectangular matrices\n";
}


// Test 5: incompatible matrices
void testInvalidDimensions() {

    Matrix A = {
        {1, 2},
        {3, 4}
    };

    Matrix B = {
        {1, 2, 3}
    };

    bool exceptionThrown = false;

    try {
        matrixMultiply(A, B);
    }
    catch (const invalid_argument&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);

    cout << "Test 5 passed: Invalid dimensions\n";
}


int main() {

    testBasicMultiplication();
    testIdentityMatrix();
    testZeroMatrix();
    testRectangularMatrices();
    testInvalidDimensions();

    cout << "\nAll unit tests passed!\n";

    return 0;
}
