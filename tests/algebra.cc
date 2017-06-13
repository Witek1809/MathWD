#include "tools/print.h"
#include "tools/sparse_vector_gf2.h"
#include "tools/sparse_matrix_gf2.h"

#include <cstdlib>
#include <ctime>
#include <random>
#include <iostream>
#include <string>
#include <sstream>
#include <NTL/mat_GF2.h>
#include "ifp/canonical_distribution.h"
#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/MemoryLeakDetectorNewMacros.h"
#include "CppUTest/MemoryLeakDetectorMallocMacros.h"

// Zmienne globalne do testów /////////////////////////////////
unsigned long rows;
unsigned long columns;
NTL::mat_GF2 a_ntl, b_ntl;
SparseMatrixGF2 a_sparse, b_sparse;
///////////////////////////////////////////////////////////////

TEST_GROUP(SparseMatrix)
{
    /* Grupa testów dotycząca algebry macierzy rzadkich, sprawdzenia
     * poprawności arytmetyki oraz czasu działania algorytmów */
};

TEST(SparseMatrix, SparseTranspose)
{

}

TEST(SparseMatrix, SparseMultiplication)
{
    SparseMatrixGF2 c_sprase = a_sparse * b_sparse;
}

TEST(SparseMatrix, NtlMultiplication)
{
    NTL::mat_GF2 c_ntl = a_ntl * b_ntl;
}

TEST(SparseMatrix, SparseArthmetic)
{
    SparseMatrixGF2 a,b,c;

    a.fill(0,0); a.fill(0,1); a.fill(1,2); a.fill(2,2); a.fill(2,3);
    b.fill(0,0); b.fill(1,1); b.fill(2,0); b.fill(3,2); b.fill(3,3);

    // Dodawanie
    c = a + b;

    // Mnożenie
    c = a * b;

    auto wsk = c.get_rep_sparse_matrix_();
}

int main(int argc, char** argv)
{
    std::cout << std::endl <<"Testing Sparse Matrix Program of project Cribrum" << std::endl;

    //Ustawiam zakres danych testowych /////////////////////////////////////////////////////////////////
    rows = 100000;
    columns = 100000;
    a_ntl.SetDims(rows,columns);
    b_ntl.SetDims(rows,columns);
    std::srand(std::time(0));
    unsigned long i,j;
    unsigned long number_of_ones = (double)rows * (double)columns * 0.00005; // dwadzieścia procent jedynek
    std::cout << "Matrix [" << rows << ", " << columns << "]" << std::endl;
    std::cout << "Numbers of ones " << number_of_ones << std::endl << std::endl;

    for(unsigned long r=0; r<rows; r++){ // przynajmniej jedna jedynka w wierszu
        j = std::rand() % columns;
        a_ntl[r][j] = NTL::GF2(1);
        a_sparse.fill(r,j);
        j = std::rand() % columns;
        b_ntl[r][j] = NTL::GF2(1);
        b_sparse.fill(r,j);
    }

    number_of_ones = number_of_ones - rows; // odejmujemy

    for(unsigned long coeff=0; coeff<number_of_ones; coeff++){
        i = std::rand() % rows;
        j = std::rand() % columns;
        a_ntl[i][j] = NTL::GF2(1);
        a_sparse.fill(i,j);
        i = std::rand() % rows;
        j = std::rand() % columns;
        b_ntl[i][j] = NTL::GF2(1);
        b_sparse.fill(i,j);
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    MemoryLeakWarningPlugin::turnOnThreadSafeNewDeleteOverloads();
    return CommandLineTestRunner::RunAllTests(argc, argv);
}
