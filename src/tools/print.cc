#include "print.h"
#include <iostream>

void PrintSet(const std::set<NTL::ZZ> &set_){

    std::cout << "my set :";

    for(auto it = set_.begin(); it!=set_.end(); it++){
        std::cout << *it << " ";
    }

    std::cout << std::endl;
}

void PrintSet(const std::set<NTL::ZZ>* set_){

    std::cout << "my set :";

    for(auto it = set_->begin(); it!=set_->end(); it++){
        std::cout << *it << " ";
    }

    std::cout << std::endl;
}

void PrintMap(const std::map< NTL::ZZ, SparseVectorGF2 >* map_){
    for(auto it = map_->begin(); it != map_->end(); it++){
        auto vec = (it->second).get_rep_sparse_vec_();
        for(auto e = vec->begin(); e != vec->end(); e++){
            std::cout << "(" << it->first << "," << *e << ")" << " ";
        }
        std::cout << std::endl;
    }
}
