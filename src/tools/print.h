#pragma once

#ifndef PRINT_H
#define PRINT_H

#include "print.h"
#include "sparse_vector_gf2.h"
#include "sparse_matrix_gf2.h"

void PrintSet(const std::set<NTL::ZZ> &set_);

void PrintSet(const std::set<NTL::ZZ>* set_);

void PrintMap(const std::map< NTL::ZZ, SparseVectorGF2 >* map_);

#endif //PRINT_H
