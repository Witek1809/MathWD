#pragma once

#ifndef SIEVE_H
#define SIEVE_H

#include <NTL/ZZ.h>

namespace ifp{

class Sieve{

public:

    Sieve(){

    }

    ~Sieve(){

    }

    virtual NTL::ZZ Factorize(const NTL::ZZ &n)=0;

protected:

    virtual void SelectBase() = 0;
    virtual void Sieve() = 0;
    virtual void SelectResult()=0;
};

}
#endif // SIEVE_H
