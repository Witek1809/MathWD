#include <omp.h>
#include "ecm.h"
#include <NTL/RR.h>
#include "ifp/primes/select_primes.h"

#define ZERO NTL::ZZ(0)
#define ONE NTL::ZZ(1)
#define TWO NTL::ZZ(2)

Ecm::Ecm(const int number_of_threads)
{
    if(number_of_threads == (-1)){

        int threads = 1;
        #pragma omp parallel shared(threads)
        {
            #pragma omp critical
            {
                threads = omp_get_num_threads();
            }
        }
        this->number_of_threads_ = threads;
    }
    else{
        this->number_of_threads_ = number_of_threads;
    }
}

std::list<NTL::ZZ> Ecm::SelectListK(const NTL::ZZ &smooth, const std::set<NTL::ZZ> &prime_list)
{
    long j = 1;
    std::list<NTL::ZZ> k_list;
    NTL::ZZ k;

    for(auto iter = prime_list.begin(); iter!=prime_list.end(); iter++){

        j = 1;

        while( smooth > NTL::power(*iter,j)){
            j++;
        }

        k = NTL::power(*iter,(j-1));

        if( k > ZERO ){
            k_list.push_back(k);
        }
    }

    return k_list;
}

NTL::ZZ Ecm::SelectK(const std::set<NTL::ZZ> &prime_list)
{
    NTL::ZZ k = ONE;
    NTL::ZZ C = ONE;

    NTL::ZZ start, stop, start_stop, r, c;

    for(auto iter = prime_list.begin() ; iter != prime_list.end(); iter++){
        NTL::SqrRoot(start_stop,*iter);
        start = (*iter) + ONE - (TWO*start_stop);
        stop  = (*iter) + ONE + (TWO*start_stop);
        start_stop = stop - start;
        C = start + tools::RandomZZModN(start_stop);
        r = NTL::conv<NTL::ZZ>(NTL::log10(NTL::conv<NTL::RR>(*iter)));
        c = NTL::conv<NTL::ZZ>(NTL::log10(NTL::conv<NTL::RR>(*iter)));
        k = k * NTL::conv<NTL::ZZ>(NTL::pow(NTL::conv<NTL::RR>(*iter),NTL::conv<NTL::RR>(r/(c+ONE) + ONE)));
    }

    return k;
}

std::array<NTL::ZZ, 4> Ecm::EcmMainBody(const NTL::ZZ &n, const std::list<NTL::ZZ> &k_list, const std::array<NTL::ZZ, 3> &point_q, const EllipticCurve &elliptic_curve)
{
    NTL::ZZ GCD = ONE;
    NTL::ZZ D = ONE;
    std::array<NTL::ZZ, 4> result { ONE, ONE, ONE, ONE };
    std::array<NTL::ZZ, 3> new_point = { ONE, ONE, ONE };

    for(auto iter = k_list.begin(); iter!=k_list.end(); iter++){
        new_point = ecc::MultiplicationPoint(point_q, *iter, elliptic_curve);
        D = D * new_point[2];
    }

    GCD = NTL::GCD(D,n);

    result[0] = new_point[0];
    result[1] = new_point[1];
    result[2] = new_point[2];
    result[3] = GCD;

    return result;
}

NTL::ZZ Ecm::Factorize(const NTL::ZZ &n)
{
    NTL::ZZ GCD = ONE;
    NTL::ZZ Factor = ONE;
    bool loop_status = false;
    EllipticCurve elliptic_curve;
    std::array<NTL::ZZ, 3> Q = { ONE, ONE, ONE};
    std::array<NTL::ZZ, 4> result = { ONE, ONE, ONE, ONE};

    this->smooth_ = tools::CalculateSmooth(n);
    this->primes_ = tools::SelectPrimeNumbers(smooth_);
    this->k_ = SelectK(primes_);
    this->k_list_ = SelectListK(smooth_,primes_);


    #pragma omp parallel shared(Factor,loop_status) firstprivate(GCD,Q,elliptic_curve,result)
    {

        while(!loop_status){

            elliptic_curve = ecc::RandomEllipticCurve(n);
            Q = ecc::MultiplicationPoint(elliptic_curve.start_point_, k_, elliptic_curve);
            GCD = NTL::GCD(Q[2],n);

            if( GCD == ONE or GCD == n ){

                result = EcmMainBody(n, k_list_, Q, elliptic_curve);
                Q[0] = result[0];
                Q[1] = result[1];
                Q[2] = result[2];
                GCD =  result[3];

                if(GCD!=ONE and GCD!=n){
                    #pragma omp critical
                    {
                        loop_status = true;
                        Factor = GCD;
                    }
                }
            }

        }

    }

    this->primes_.clear();
    this->k_list_.clear();

    return Factor;
}
