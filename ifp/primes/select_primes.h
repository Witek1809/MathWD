#pragma once

#include <set>
#include <NTL/ZZ.h>

namespace tools
{
NTL::ZZ RandomZZModN(const NTL::ZZ &n);

std::set<NTL::ZZ> SelectPrimeNumbers(const NTL::ZZ &B);

NTL::ZZ CalculateSmooth(const NTL::ZZ &n);
}
