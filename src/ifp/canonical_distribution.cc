#include "ecm/ecm.h"
#include <sstream>
#include <NTL/RR.h>
#include <algorithm>
#include "ecm/ecm.h"
#include "canonical_distribution.h"

namespace ifp{

bool is_power(NTL::ZZ &p, const NTL::ZZ &n, const unsigned long max_prime){

    NTL::ZZ root_n;
    NTL::ZZ prime = NTL::ZZ(2);

    for(unsigned long i=0; i<max_prime; i++){

        root_n = NTL::RoundToZZ(NTL::pow(NTL::conv<NTL::RR>(n),NTL::RR(1.0/(double)NTL::conv<long>(prime))));

        if( NTL::power(root_n,NTL::conv<long>(prime)) == n){
            p = root_n;
            return true;
        }
        else{
            prime = NTL::NextPrime(prime+1);
        }
    }

    return false;

}

bool simple_check(NTL::ZZ &p, const NTL::ZZ &n, const NTL::ZZ &max_prime){

    NTL::ZZ prime = NTL::ZZ(2);

    while(prime<max_prime){

        if( (n%prime) == NTL::ZZ(0)){
            p = prime;
            return true;
        }
        else{
            prime = NTL::NextPrime(prime+1);
        }
    }

    return false;

}

std::set<std::pair<NTL::ZZ, unsigned long> > canonical_distribution(const NTL::ZZ &n){

    std::set< std::pair<NTL::ZZ, unsigned long> > distribution; // rozkład kanoniczny liczby n

    // Zmienne pomocnicze ///////////////////////////////
    NTL::ZZ p; // liczba pierwsza ///////////////////////
    Ecm ecm; // algorytm ecm ////////////////////////////
    bool info = false; // status zadania faktoryzacji ///
    unsigned long power = 0; // potęga w rozkładzie /////
    NTL::ZZ temporary_value = n; // wartość pomocnicza //

    while(true){ // dopóki rozkład nie zostanie wyznaczony //////////////
        p = temporary_value;
        while( !NTL::ProbPrime(p) and p != NTL::ZZ(1) ){
            info = simple_check(p,p);
            if( info == false ){
                info = is_power(p,p);
                if( info == false){
                    p = ecm.Factorize(temporary_value);
                }
            }
        }
        if(p != NTL::ZZ(1)){
            power = 0;
            while( temporary_value % p == NTL::ZZ(0) ){
                temporary_value = temporary_value/p;
                power++;
            }
            distribution.insert(std::make_pair(p,power));
        }
        else{
            break; //(WD) wszystkie elementy rozkładu został odnalezione
        }
    }
    return distribution; //wracamy rokład kanoniczny liczby n //////////
}

NTL::ZZ distribution_to_number(std::set<std::pair<NTL::ZZ, unsigned long> > &can_dis){
    NTL::ZZ n = NTL::ZZ(1);
    for(auto e = can_dis.begin(); e!=can_dis.end(); e++){
        n = n * NTL::power(e->first,e->second);
    }
    return n;
}

}
