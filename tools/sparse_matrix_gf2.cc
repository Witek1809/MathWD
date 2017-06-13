#include "sparse_matrix_gf2.h"

SparseMatrixGF2::SparseMatrixGF2(const NTL::ZZ &max_rows, const NTL::ZZ &max_columns){
    this->rows_ = max_rows;
    this->columns_ = max_columns;
}

void SparseMatrixGF2::fill(const unsigned long &r, const unsigned long &c){

    std::map< NTL::ZZ, SparseVectorGF2 >::iterator it = this->rep_sparse_matrix_.find(NTL::conv<NTL::ZZ>(r));

    if(it != this->rep_sparse_matrix_.end()){
        (it->second).fill(c);
    }
    else{
        SparseVectorGF2 Vec(this->columns_);
        Vec.fill(c);
        this->rep_sparse_matrix_.insert( std::pair< NTL::ZZ,SparseVectorGF2 >(NTL::conv<NTL::ZZ>(r),Vec) );
    }
}

void SparseMatrixGF2::fill(const NTL::ZZ &r, const NTL::ZZ &c){

    std::map< NTL::ZZ, SparseVectorGF2 >::iterator it = this->rep_sparse_matrix_.find(r);

    if(it != this->rep_sparse_matrix_.end()){
        (it->second).fill(c);
    }
    else{
        SparseVectorGF2 Vec(this->columns_);
        Vec.fill(c);
        this->rep_sparse_matrix_.insert( std::pair< NTL::ZZ,SparseVectorGF2 >(r,Vec) );
    }
}

const std::map< NTL::ZZ, SparseVectorGF2 >* SparseMatrixGF2::get_rep_sparse_matrix_() const{
    return &(this->rep_sparse_matrix_);
}

SparseVectorGF2 SparseMatrixGF2::get_column(const unsigned long &c){
    return this->get_column(NTL::conv<NTL::ZZ>(c));
}

SparseVectorGF2 SparseMatrixGF2::get_column(const NTL::ZZ &c){

    SparseVectorGF2 column;

    for(auto it = this->rep_sparse_matrix_.begin(); it != this->rep_sparse_matrix_.end(); it++){
        if((it->second).is_one_on_position(c)){
            column.fill(it->first);
        }

    }
    return column;
}

void SparseMatrixGF2::swap_rows(const NTL::ZZ &i, const NTL::ZZ &j){
    
    /*std::map< NTL::ZZ, SparseVectorGF2 >::iterator row1 = this->rep_sparse_vec_.find(i);
    std::map< NTL::ZZ, SparseVectorGF2 >::iterator row2 = this->rep_sparse_vec_.find(j);
    
    if(row1 != row2 and row1 != this->rep_sparse_vec_.end() and  row2 != this->rep_sparse_vec_.end() ){
        
    }
    else if(){
        
    }*/
}

void Gauss(SparseMatrixGF2 &M, const unsigned long &w){
    
}

SparseMatrixGF2 Transpose(SparseMatrixGF2 &M){
    SparseMatrixGF2 MT(M.get_num_columns(),M.get_num_rows());
    for(unsigned long c=0; c < M.get_num_columns(); c++){
        M.get_column(c);
        //MT.get_rep_sparse_matrix_()->insert(M.get_column(c));
    }
    return MT;
}
