#pragma once

#include "elliptic_curve.h"

namespace ecc
{
std::array<NTL::ZZ,3> AddPoint(const std::array<NTL::ZZ,3> &point_1, const std::array<NTL::ZZ,3> &point_2, const EllipticCurve &elliptic_curve);

std::array<NTL::ZZ,3> DoublingPoint(const std::array<NTL::ZZ,3> &point, const EllipticCurve &elliptic_curve);

std::array<NTL::ZZ,3> MultiplicationPoint(const std::array<NTL::ZZ,3> &point, const NTL::ZZ &k, const EllipticCurve &elliptic_curve);

EllipticCurve RandomEllipticCurve(const NTL::ZZ &n);
}
