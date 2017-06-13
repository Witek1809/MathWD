#include <algorithm>
#include "sparse_vector_gf2.h"

void SparseVectorGF2::clear(){
    this->rep_sparse_vec_.clear();
}

void SparseVectorGF2::fill(const unsigned long &e){
    this->rep_sparse_vec_.insert(NTL::conv<NTL::ZZ>(e));
}

void SparseVectorGF2::fill(const NTL::ZZ &e){
    this->rep_sparse_vec_.insert(e);
}

bool SparseVectorGF2::is_one_on_position(const unsigned long &p){
    return (this->is_one_on_position(NTL::conv<NTL::ZZ>(p)));
}

bool SparseVectorGF2::is_one_on_position(const NTL::ZZ &p){
    auto it = std::find(this->rep_sparse_vec_.begin(), this->rep_sparse_vec_.end(), p);
    if(it != this->rep_sparse_vec_.end()){
        return true;
    }
    else{
        return false;
    }
}

void SparseVectorGF2::add(const SparseVectorGF2 &b){
    for(auto it = b.get_rep_sparse_vec_()->begin(); it != b.get_rep_sparse_vec_()->end(); it++){
        if( this->rep_sparse_vec_.find(*it) != this->rep_sparse_vec_.end() ){
            this->rep_sparse_vec_.erase(*it);
        }
        else{
            this->rep_sparse_vec_.insert(*it);
        }
    }

}
