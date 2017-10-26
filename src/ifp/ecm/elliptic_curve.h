#pragma once

#include <NTL/ZZ.h>

namespace ecc
{


class EllipticCurve
{

public:

    EllipticCurve(){

    }

    ~EllipticCurve(){

    }

    NTL::ZZ a_;
    NTL::ZZ b_;
    NTL::ZZ n_;

    std::array<NTL::ZZ,3> start_point_;

    EllipticCurve(const std::array<NTL::ZZ,3> &start_point, const NTL::ZZ &a, const NTL::ZZ &b, const NTL::ZZ &n)
    {
        this->n_ = n;
        this->a_ = a % this->n_;
        this->b_ = b % this->n_;
        this->start_point_[0] = start_point[0];
        this->start_point_[1] = start_point[1];
        this->start_point_[2] = start_point[2];
    }
};

}
