#pragma once

#ifndef CANONICAL_DISTRIBUTION_H
#define CANONICAL_DISTRIBUTION_H

#include <set>
#include <NTL/ZZ.h>

namespace ifp{

bool is_power(NTL::ZZ &p, const NTL::ZZ &n, const unsigned long max_prime = 4);

bool simple_check(NTL::ZZ &p, const NTL::ZZ &n, const NTL::ZZ &max_prime = NTL::ZZ(30));

std::set< std::pair<NTL::ZZ, unsigned long> > canonical_distribution(const NTL::ZZ& n);

NTL::ZZ distribution_to_number(std::set< std::pair<NTL::ZZ, unsigned long> > &can_dis);

}
#endif // CANONICAL_DISTRIBUTION_H
