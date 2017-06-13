#pragma once

#include <set>
#include <list>
#include "elliptic_curve.h"
#include "elliptic_curve_arthmetic.h"

class Ecm
{

public:

    Ecm(const int number_of_threads = (-1));
    NTL::ZZ Factorize(const NTL::ZZ &n);

private:

    NTL::ZZ k_;
    NTL::ZZ smooth_;
    bool is_init_ = false;
    int number_of_threads_;
    std::set<NTL::ZZ> primes_;
    std::list<NTL::ZZ> k_list_;

    std::list<NTL::ZZ> SelectListK(const NTL::ZZ &smooth, const std::set<NTL::ZZ> &prime_list);

    NTL::ZZ SelectK(const std::set<NTL::ZZ> &prime_list);

    std::array<NTL::ZZ, 4> EcmMainBody(const NTL::ZZ &n,
                                       const std::list<NTL::ZZ> &k_list,
                                       const std::array<NTL::ZZ, 3> &point_q, const EllipticCurve &elliptic_curve);

};
