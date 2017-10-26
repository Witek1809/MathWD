#pragma once

#ifndef FACTORIAL_H
#define FACTORIAL_H

#include <NTL/ZZ.h>

namespace ifp{

class FactorizeAlgorithm{

public:

    FactorizeAlgorithm(){

    }

    ~FactorizeAlgorithm(){

    }

    virtual NTL::ZZ Factorize(const NTL::ZZ &n)=0;

protected:

    NTL::ZZ n_;
};

}
#endif // FACTORIAL_H
