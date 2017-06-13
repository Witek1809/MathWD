#include "elliptic_curve_arthmetic.h"
#include "ifp/primes/select_primes.h"

namespace ecc
{
std::array<NTL::ZZ,3> AddPoint(const std::array<NTL::ZZ, 3> &point_1, const std::array<NTL::ZZ, 3> &point_2, const EllipticCurve &elliptic_curve)
{
        /*
            http://en.wikibooks.org/wiki/Cryptography/Prime_Curve/Standard_Projective_Coordinates

            U1 = Y2*Z1
            U2 = Y1*Z2
            V1 = X2*Z1
            V2 = X1*Z2
            if (V1 == V2)
              if (U1 != U2)
                return POINT_AT_INFINITY
              else
                return POINT_DOUBLE(X1, Y1, Z1)
            U = U1 - U2
            V = V1 - V2
            W = Z1*Z2
            A = U^2*W - V^3 - 2*V^2*V2
            X3 = V*A
            Y3 = U*(V^2*V2 - A) - V^3*U2
            Z3 = V^3*W
            return (X3, Y3, Z3)
        */

        NTL::ZZ u = ((point_2[1]*point_1[2]) - (point_1[1]*point_2[2])) % elliptic_curve.n_;
        NTL::ZZ v = ((point_2[0]*point_1[2]) - (point_1[0]*point_2[2])) % elliptic_curve.n_;

        NTL::ZZ A = ((NTL::power(u,2)*point_1[2]*point_2[2]) - (NTL::power(v,3)) - (NTL::ZZ(2)*NTL::power(v,2)*point_1[0]*point_2[2])) % elliptic_curve.n_;

        std::array<NTL::ZZ,3> new_point;
        new_point[0] = (v*A) % elliptic_curve.n_;
        new_point[1] = (u*((NTL::power(v,2)*point_1[0]*point_2[2]) - A) - (NTL::power(v,3)*point_1[1]*point_2[2])) % elliptic_curve.n_;
        new_point[2] = (NTL::power(v,3)*point_1[2]*point_2[2])% elliptic_curve.n_;

        return new_point;

}

std::array<NTL::ZZ,3> DoublingPoint(const std::array<NTL::ZZ, 3> &point, const EllipticCurve &elliptic_curve)
{
    /*
        http://en.wikibooks.org/wiki/Cryptography/Prime_Curve/Standard_Projective_Coordinates

        if (Y == 0)
             return POINT_AT_INFINITY
            W = a*Z^2 + 3*X^2
            S = Y*Z
            B = X*Y*S
            H = W^2 - 8*B
            X' = 2*H*S
            Y' = W*(4*B - H) - 8*Y^2*S^2
            Z' = 8*S^3
            return (X', Y', Z')
     */

    NTL::ZZ W = ((elliptic_curve.a_)*NTL::power(point[2],2)) + (NTL::ZZ(3)*(NTL::power(point[0],2))) % elliptic_curve.n_;
    NTL::ZZ S = (point[1]*point[2]) % elliptic_curve.n_;
    NTL::ZZ B = (point[0]*point[1]*S) % elliptic_curve.n_;
    NTL::ZZ H = (NTL::power(W,2) - (NTL::ZZ(8)*B)) % elliptic_curve.n_;

    std::array<NTL::ZZ,3> new_point;
    new_point[0] = (NTL::ZZ(2)*H*S) % elliptic_curve.n_;
    new_point[1] = ((W*((NTL::ZZ(4)*B) - H)) - (NTL::ZZ(8)*NTL::power(point[1],2)*NTL::power(S,2))) % elliptic_curve.n_;
    new_point[2] = (NTL::ZZ(8)*NTL::power(S,3)) % elliptic_curve.n_;

    return new_point;
}

std::array<NTL::ZZ,3> MultiplicationPoint(const std::array<NTL::ZZ, 3> &point, const NTL::ZZ &k, const EllipticCurve &elliptic_curve)
{
    /*
        Montgomery ladder
        http://en.wikipedia.org/wiki/Elliptic_curve_point_multiplication
        * R0 := 0
              * R1 := P
              * for i from m downto 0 do
                 * if di = 0 then
                    * R1 := R0 + R1
                    * R0 := 2R0
                 * else
                    * R0 := R0 + R1
                    * R1 := 2R1
              * Return R0
    */


    std::array<NTL::ZZ,3> R0 = point;
    std::array<NTL::ZZ,3> R1 = DoublingPoint(point,elliptic_curve);
    std::array<NTL::ZZ,3> tmp;

    for(long i = (NTL::NumBits(k)-2); i>=0; i--){
        if(NTL::bit(k,i) == 0){
            tmp = R0;
            R0 = DoublingPoint(R0, elliptic_curve);
            R1 = AddPoint(tmp, R1, elliptic_curve);
        }
        else if(NTL::bit(k,i) == 1){
            R0 = AddPoint(R0, R1, elliptic_curve);
            R1 = DoublingPoint(R1, elliptic_curve);
        }
    }

    return R0;
}

EllipticCurve RandomEllipticCurve(const NTL::ZZ &n)
{
    NTL::ZZ a = tools::RandomZZModN(n);
    NTL::ZZ x = tools::RandomZZModN(n);
    NTL::ZZ y = tools::RandomZZModN(n);

    /*
     * POSTAĆ KRZYWEJ Y^2 * Z = X^3 + aX*Z^2 + bZ^3
     * PUNKT POCZĄTKOWY P(x, y, 1)
    */

    NTL::ZZ b = ((NTL::power(y,2)) - (NTL::power(x,3)) - (a*x)) % n;

    std::array<NTL::ZZ, 3> start_point = {x,y,NTL::ZZ(1)};

    EllipticCurve elliptic_curve = EllipticCurve(start_point,a,b,n);

    return elliptic_curve;
}
}
