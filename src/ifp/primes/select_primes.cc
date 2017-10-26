#include <omp.h>
#include <time.h>
#include <random>
#include <NTL/RR.h>
#include "select_primes.h"

namespace tools
{

NTL::ZZ RandomZZModN(const NTL::ZZ &n)
{
    srand(time(0));
    long l = rand();
    NTL::ZZ number_1 = NTL::conv<NTL::ZZ>(NTL::RandomBnd(l));
    NTL::ZZ number_2 = NTL::conv<NTL::ZZ>(NTL::RandomBnd(l+10));
    return ((number_1 ^ number_2)%n)+1;
}

std::set<NTL::ZZ> SelectPrimeNumbers(const NTL::ZZ &B)
{
    std::set<NTL::ZZ> primes;

    #pragma omp parallel shared(B,primes)
    {
        NTL::ZZ stop  = B; // domyslny koniec obszaru poszukiwań //////////////////////////////////
        NTL::ZZ start = NTL::ZZ(2); // domyślny początek obszaru poszukiwań ///////////////////////
        std::set<NTL::ZZ> primes_thread; // lista liczb pierwszych zebrana przez wątek ////////////
        NTL::ZZ chunk_size = B/NTL::conv<NTL::ZZ>(omp_get_num_threads()); //rozmiar obszaru ///////

        start = start + NTL::conv<NTL::ZZ>(omp_get_thread_num())*chunk_size; // początek obszaru //

        if(omp_get_thread_num() != omp_get_num_threads()-1){ //////////////////////////////////////
            stop = start + chunk_size; // określam koniec obszaru poszukiwań dla wątku ////////////
        } /////////////////////////////////////////////////////////////////////////////////////////

        for(NTL::ZZ i = start; i <= stop; i++){
            if(NTL::ProbPrime(i,10)==1){
                primes_thread.insert(i);
            }
        }

        #pragma omp critical
        {
            primes.insert(primes_thread.begin(),primes_thread.end());
        }

    }

    return primes;
}

NTL::ZZ CalculateSmooth(const NTL::ZZ &n)
{
    NTL::ZZ root = NTL::CeilToZZ(NTL::log10(NTL::conv<NTL::RR>(n)) * NTL::log10(NTL::log10(NTL::conv<NTL::RR>(n))));
    NTL::SqrRoot(root,root);
    return NTL::CeilToZZ(NTL::pow(NTL::conv<NTL::RR>(2.718282),NTL::conv<NTL::RR>(root)));
}

}
