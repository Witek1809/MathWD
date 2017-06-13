#include "ifp/ecm/ecm.h"
#include "ifp/canonical_distribution.h"

#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/MemoryLeakDetectorNewMacros.h"
#include "CppUTest/MemoryLeakDetectorMallocMacros.h"


TEST_GROUP(IntegerFactorizationProblem)
{
    /* Grupa testów sprawdzająca algorytmy rozwiązujące problemy
     * związane z zagadnieniem fakoryzacji */
};

TEST(IntegerFactorizationProblem, EcmFactorial)
{
    Ecm ecm;

    NTL::ZZ x,n,p,q;

    NTL::RandomPrime(p,20);
    NTL::RandomPrime(q,20);
    n = p * q;
    x = ecm.Factorize(n);

    CHECK_TRUE( x == p or x == q );

    NTL::RandomPrime(p,30);
    NTL::RandomPrime(q,30);
    n = p * q;
    x = ecm.Factorize(n);

    CHECK_TRUE( x == p or x == q );

    NTL::RandomPrime(p,40);
    NTL::RandomPrime(q,40);
    n = p * q;
    x = ecm.Factorize(n);

    CHECK_TRUE( x == p or x == q );

    /*NTL::RandomPrime(p,50);
    NTL::RandomPrime(q,50);
    n = p * q;
    x = ecm.Factorize(n);

    CHECK_TRUE( x == p or x == q );*/
}

/*
TEST(IntegerFactorizationProblem, CanonicalDistribution)
{
    NTL::ZZ n = NTL::conv<NTL::ZZ>("4767169871707374540990310594977128197321");
    std::set< std::pair<NTL::ZZ, unsigned long> > distribution = ifp::canonical_distribution(n);
    NTL::ZZ x = ifp::distribution_to_number(distribution);
    CHECK_TRUE( x == n );
}
*/

int main(int argc, char** argv)
{
    MemoryLeakWarningPlugin::turnOnThreadSafeNewDeleteOverloads();
    return CommandLineTestRunner::RunAllTests(argc, argv);


}
