#pragma once

#ifndef SPARSE_MATRIX_GF2_H
#define SPARSE_MATRIX_GF2_H

#include <map>
#include <omp.h>
#include <assert.h>
#include <algorithm>
#include "sparse_vector_gf2.h"

class SparseMatrixGF2
{
public:

    SparseMatrixGF2(){
        this->rows_    = NTL::conv<NTL::ZZ>(0);
        this->columns_ = NTL::conv<NTL::ZZ>(0);
    }

    ~SparseMatrixGF2(){
        this->rep_sparse_matrix_.clear();
    }

    SparseMatrixGF2(const NTL::ZZ &max_rows, const NTL::ZZ &max_columns); // konstruktor

    const std::map< NTL::ZZ, SparseVectorGF2 >* get_rep_sparse_matrix_() const; // zwracay wskaźnik na współrzędne jedynek w macierzy

    void fill(const unsigned long &r, const unsigned long &c); // dodajemy jedynke na pozycji (r,c)

    void fill(const NTL::ZZ &r, const NTL::ZZ &c); // dodajemy jedynke na pozycji (r,c)

    NTL::ZZ get_num_rows() const{ return this->rows_; } // zwraca rzeczywistę liczbe wierszy

    NTL::ZZ get_num_columns() const{ return this->columns_; } // zwraca rzeczywistę liczbe column

    NTL::ZZ get_number_of_ones(); // zwraca ilość jedynek w macierzy

    SparseVectorGF2 get_column(const unsigned long &c); // zwraca kolumnę o numerze c

    SparseVectorGF2 get_column(const NTL::ZZ &c); // zwraca kolumnę o numerze c

    void print();

private:

    NTL::ZZ rows_;

    NTL::ZZ columns_;

    std::map< NTL::ZZ, SparseVectorGF2 > rep_sparse_matrix_;

    void swap_rows(const NTL::ZZ &i, const NTL::ZZ &j);
};

inline SparseMatrixGF2 operator+(const SparseMatrixGF2 &a, const SparseMatrixGF2 &b){

    assert( a.get_num_rows() == b.get_num_rows() );
    assert( a.get_num_columns() == b.get_num_columns() );

    SparseMatrixGF2 c(a.get_num_rows(),a.get_num_columns());

    const std::map< NTL::ZZ, SparseVectorGF2 >* map_a = a.get_rep_sparse_matrix_();
    const std::map< NTL::ZZ, SparseVectorGF2 >* map_b = b.get_rep_sparse_matrix_();
          std::map< NTL::ZZ, SparseVectorGF2 >* map_c = const_cast<std::map< NTL::ZZ, SparseVectorGF2 >*>(c.get_rep_sparse_matrix_());

    map_c->insert(map_a->begin(),map_a->end());

    std::map< NTL::ZZ, SparseVectorGF2 >::iterator row;

    for(auto it = map_b->begin(); it != map_b->end(); it++){

        row = map_c->find(it->first);

        if( row != map_c->end() ){

            row->second.add(it->second);

            if( (((row->second).get_rep_sparse_vec_())->size()) == 0){
                map_c->erase(row);
            }
        }
        else{
            map_c->insert(*it);
        }
    }

    return c;
}

inline SparseMatrixGF2 operator*(const SparseMatrixGF2 &a, const SparseMatrixGF2 &b){

    assert( a.get_num_columns() == b.get_num_rows() );

    SparseMatrixGF2 c(a.get_num_rows(),b.get_num_columns());

    std::map< NTL::ZZ, SparseVectorGF2 >* map_a = const_cast<std::map< NTL::ZZ, SparseVectorGF2 >*>(a.get_rep_sparse_matrix_());
    std::map< NTL::ZZ, SparseVectorGF2 >* map_b = const_cast<std::map< NTL::ZZ, SparseVectorGF2 >*>(b.get_rep_sparse_matrix_());
    std::map< NTL::ZZ, SparseVectorGF2 >* map_c = const_cast<std::map< NTL::ZZ, SparseVectorGF2 >*>(c.get_rep_sparse_matrix_());

    //#pragma omp parallel shared(map_a, map_b, map_c)
    //{
        SparseVectorGF2 n(b.get_num_columns());
        std::set< NTL::ZZ >::iterator e;
        std::map< NTL::ZZ, SparseVectorGF2 >::iterator row;

        //unsigned long long size        = map_a->size();
        //unsigned long long thread_id   = omp_get_thread_num();
        //unsigned long long threads_num = omp_get_num_threads();
        //unsigned long long pos         = thread_id;

        //std::map< NTL::ZZ, SparseVectorGF2 >::iterator elem_map_a = map_a->begin();

        for(//std::advance(elem_map_a,thread_id);
            //pos < size;
            //std::advance(elem_map_a,threads_num) ){
            auto elem_map_a = map_a->begin(); elem_map_a != map_a->end(); elem_map_a++){

            n.clear();

            //Wersja 1
            for(e = (elem_map_a->second.get_rep_sparse_vec_())->begin(); e!= (elem_map_a->second.get_rep_sparse_vec_())->end(); e++){

                row = map_b->find(*e);

                if(row != map_b->end()){
                    n.add(row->second);
                }
            }

            //#pragma omp critical
            //{
                //DEBUG(WD) std::cout<<"thread id "<<thread_id<<" elem : "<<elem_map_a->first<<" size : "<<size<<"\n";
                map_c->insert(std::pair< NTL::ZZ,SparseVectorGF2 >(elem_map_a->first,n));
            //}

            //pos += threads_num;
        //}
    }

    return c;
}

void Gauss(SparseMatrixGF2 &M, const unsigned long &w);

SparseMatrixGF2 Transpose(SparseMatrixGF2 &M);

#endif // SPARSE_MATRIX_GF2_H
