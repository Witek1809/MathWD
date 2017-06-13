#pragma once

#ifndef SPARSE_MATRIX_GF2_H
#define SPARSE_MATRIX_GF2_H

#include <map>
#include <algorithm>
#include "sparse_vector_gf2.h"

class SparseMatrixGF2
{
public:
    SparseMatrixGF2(){}
    ~SparseMatrixGF2(){}
    SparseMatrixGF2(const NTL::ZZ &max_rows, const NTL::ZZ &max_columns); // konstruktor
    const std::map< NTL::ZZ, SparseVectorGF2 >* get_rep_sparse_matrix_() const; // zwracay wskaźnik na współrzędne jedynek w macierzy
    void fill(const unsigned long &r, const unsigned long &c); // dodajemy jedynke na pozycji (r,c)
    void fill(const NTL::ZZ &r, const NTL::ZZ &c); // dodajemy jedynke na pozycji (r,c)
    NTL::ZZ get_num_rows() const{ return this->rows_; } // zwraca rzeczywistę liczbe wierszy
    NTL::ZZ get_num_columns() const{ return this->columns_; } // zwraca rzeczywistę liczbe column
    NTL::ZZ get_number_of_ones(); // zwraca ilość jedynek w macierzy
    SparseVectorGF2 get_column(const unsigned long &c); // zwraca kolumnę o numerze c
    SparseVectorGF2 get_column(const NTL::ZZ &c); // zwraca kolumnę o numerze c
private:
    NTL::ZZ rows_;
    NTL::ZZ columns_;
    std::map< NTL::ZZ, SparseVectorGF2 > rep_sparse_matrix_;
    void swap_rows(const NTL::ZZ &i, const NTL::ZZ &j);
};

inline SparseMatrixGF2 operator+(const SparseMatrixGF2 &a, const SparseMatrixGF2 &b){

    SparseMatrixGF2 c;

    //FIXME(WD) Dadaj tylko gdy rozmiar sie zgadza ? //

    std::map< NTL::ZZ, SparseVectorGF2 >* map_a = const_cast<std::map< NTL::ZZ, SparseVectorGF2 >*>(a.get_rep_sparse_matrix_());
    std::map< NTL::ZZ, SparseVectorGF2 >* map_b = const_cast<std::map< NTL::ZZ, SparseVectorGF2 >*>(b.get_rep_sparse_matrix_());
    std::map< NTL::ZZ, SparseVectorGF2 >* map_c = const_cast<std::map< NTL::ZZ, SparseVectorGF2 >*>(c.get_rep_sparse_matrix_());

    map_c->insert(map_a->begin(),map_a->end());
    std::map< NTL::ZZ, SparseVectorGF2 >::iterator row;
    for(auto it = map_b->begin(); it != map_b->end(); it++){
        row = map_c->find(it->first);
        if( row != map_c->end() ){
            row->second = row->second + it->second;
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

    SparseVectorGF2 n;
    SparseMatrixGF2 c;

    //FIXME(WD) Pomnóż tylko gdy rozmiar sie zgadza ? //

    std::map< NTL::ZZ, SparseVectorGF2 >* map_a = const_cast<std::map< NTL::ZZ, SparseVectorGF2 >*>(a.get_rep_sparse_matrix_());
    std::map< NTL::ZZ, SparseVectorGF2 >* map_b = const_cast<std::map< NTL::ZZ, SparseVectorGF2 >*>(b.get_rep_sparse_matrix_());
    std::map< NTL::ZZ, SparseVectorGF2 >* map_c = const_cast<std::map< NTL::ZZ, SparseVectorGF2 >*>(c.get_rep_sparse_matrix_());

    std::set< NTL::ZZ >::iterator e;
    std::map< NTL::ZZ, SparseVectorGF2 >::iterator row;

    for(auto elem_map_a = map_a->begin(); elem_map_a != map_a->end(); elem_map_a++){

        n.clear();

        //Wersja 1
        for(e = elem_map_a->second.get_rep_sparse_vec_()->begin(); e!= elem_map_a->second.get_rep_sparse_vec_()->end(); e++){
            row = map_b->find(*e);
            if(row != map_b->end()){
                n.add(row->second);
            }
        }

        map_c->insert(std::pair< NTL::ZZ,SparseVectorGF2 >(elem_map_a->first,n));
    }

    return c;
}

void Gauss(SparseMatrixGF2 &M, const unsigned long &w);

SparseMatrixGF2 Transpose(SparseMatrixGF2 &M);

#endif // SPARSE_MATRIX_GF2_H
