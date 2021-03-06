#pragma once

#ifndef SPARSE_VECTOR_GF2_H
#define SPARSE_VECTOR_GF2_H

#include <assert.h>
#include <set> // lista współrzędnych jednynek, będziemy częśto dodawac i usuwać, a w zbiorze tylko unikalne elementy
#include <NTL/ZZ.h> // zakładam, że jest więcej elementów niż 2^64

class SparseVectorGF2
{
public:

    SparseVectorGF2(){
        this->length_ = NTL::conv<NTL::ZZ>(0);
    }

    ~SparseVectorGF2(){
        this->rep_sparse_vec_.clear();
    }

    SparseVectorGF2(const NTL::ZZ &max_length){
        this->length_ = max_length;
    }

    const std::set<NTL::ZZ>* get_rep_sparse_vec_() const {
        return &(this->rep_sparse_vec_);  // zwracay wskaźnik na współrzędne jedynek
    }

    void fill(const unsigned long &e); // dodajemy jedynke na pozycji e

    void fill(const NTL::ZZ &e); // dodajemy jedynke na pozycji e

    NTL::ZZ get_length() const {
        return this->length_;  // zwraca rzeczywistę długość wektora
    }

    NTL::ZZ get_number_of_ones(){
        return NTL::conv<NTL::ZZ>(this->rep_sparse_vec_.size()); // zwraca ilość jedymek w wektorze
    }

    bool is_one_on_position(const unsigned long &p); // true jeżeli 1 jest na pozycji p w wektorze

    bool is_one_on_position(const NTL::ZZ &p); // true jeżeli 1 jest na pozycji p w wektorze

    void add(const SparseVectorGF2 &b); // dodaje do wektora wektor b

    void clear(); // czyścimy atrybut rep_sparse_vec_

    void print();

private:

    NTL::ZZ length_;

    std::set<NTL::ZZ> rep_sparse_vec_;
};

inline SparseVectorGF2 operator+(const SparseVectorGF2 &a, const SparseVectorGF2 &b){

    assert( a.get_length() == b.get_length() );

    SparseVectorGF2 c(a.get_length());

    const std::set<NTL::ZZ>* set_a = a.get_rep_sparse_vec_();
    const std::set<NTL::ZZ>* set_b = b.get_rep_sparse_vec_();
          std::set<NTL::ZZ>* set_c = const_cast<std::set<NTL::ZZ>*>(c.get_rep_sparse_vec_());

    set_c->insert(set_a->begin(),set_a->end());

    for(auto it = set_b->begin(); it != set_b->end(); it++){

        if( set_c->find(*it) != set_c->end() ){
            set_c->erase(*it);
        }
        else{
            set_c->insert(*it);
        }
    }

    return c;
}

#endif // SPARSE_VECTOR_GF2_H
