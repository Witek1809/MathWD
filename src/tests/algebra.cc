#include "tools/print.h"
#include "tools/sparse_vector_gf2.h"
#include "tools/sparse_matrix_gf2.h"

#include <omp.h>
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
SparseMatrixGF2 a_sparse(NTL::conv<NTL::ZZ>(100000), NTL::conv<NTL::ZZ>(100000));
SparseMatrixGF2 b_sparse(NTL::conv<NTL::ZZ>(100000), NTL::conv<NTL::ZZ>(100000));
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
    a_ntl.kill();
    b_ntl.kill();
    c_ntl.kill();
}


TEST(SparseMatrix, SparseArthmetic)
{
    //std::cout<<"\n";

    SparseMatrixGF2 a(NTL::conv<NTL::ZZ>(4), NTL::conv<NTL::ZZ>(4));
    SparseMatrixGF2 b(NTL::conv<NTL::ZZ>(4), NTL::conv<NTL::ZZ>(4));
    SparseMatrixGF2 c(NTL::conv<NTL::ZZ>(4), NTL::conv<NTL::ZZ>(4));

    a.fill(0,0); a.fill(0,1);
    a.fill(1,2);
    a.fill(2,2);
    a.fill(2,3);
    a.fill(3,0); a.fill(3,1); a.fill(3,3);

    b.fill(0,0);
    b.fill(1,1);
    b.fill(2,0);
    b.fill(3,2); b.fill(3,3);

    // Dodawanie
    c = a + b;

    //std::cout<<"a matrix \n";
    //a.print();
    //std::cout<<"b matrix \n";
    //b.print();
    //std::cout<<"c matrix \n";
    //c.print();

    //std::cout<<c.get_num_rows()<<" "<<c.get_num_columns()<<"\n";

    // Mnożenie
    c = a * b;

    //std::cout<<"c matrix \n";
    //c.print();

    std::map< NTL::ZZ, SparseVectorGF2 >* map_c = const_cast<std::map< NTL::ZZ, SparseVectorGF2 >*>(c.get_rep_sparse_matrix_());

    #pragma omp parallel shared(map_c)
    {
        std::map< NTL::ZZ, SparseVectorGF2 >::iterator elem_map_c = map_c->begin();

        std::advance(elem_map_c, omp_get_thread_num());

        #pragma omp critical
        {
            //std::cout<<"id "<<omp_get_thread_num()<<" is "<<elem_map_c->first<<" begin : "<<map_c->begin()->first<<"\n";
        }
    }

}

TEST(SparseMatrix, GenerateMatixToTest){

    //Ustawiam zakres danych testowych /////////////////////////////////////////////////////////////////
    rows = 100000;
    columns = 100000;
    a_ntl.SetDims(rows,columns);
    b_ntl.SetDims(rows,columns);
    std::srand(std::time(0));
    unsigned long i,j;
    unsigned long number_of_ones = (double)rows * (double)columns * 0.00005; // procent jedynek

    //std::cout << "Matrix [" << rows << ", " << columns << "]" << std::endl;
    //std::cout << "Numbers of ones " << number_of_ones << std::endl << std::endl;

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

}

int main(int argc, char** argv)
{
    MemoryLeakWarningPlugin::turnOnThreadSafeNewDeleteOverloads();
    return CommandLineTestRunner::RunAllTests(argc, argv);
}
