//
// Created by Nine on 08/07/2026.
//

#include "Vector2.h"
#include <cmath>

//Static float methods
float Vector2::Angle(const Vector2& pVecA, const Vector2& pVecB)
{
    return acosf(Dot(pVecA,pVecB) / (pVecA.Length() * pVecB.Length()));
}

float Vector2::SignedAngle(const Vector2& pVecA, const Vector2& pVecB)
{
    return atan2f(Cross(pVecA,pVecB), Dot(pVecA,pVecB));
}

float Vector2::Cross(const Vector2& pVecA, const Vector2& pVecB)
{
    return pVecA.X()*pVecB.Y() + pVecA.Y()*pVecB.X();
}

float Vector2::Distance(const Vector2& pVecA, const Vector2& pVecB)
{
    return (pVecA - pVecB).Length();
}

float Vector2::Dot(const Vector2& pVecA, const Vector2& pVecB)
{
    return pVecA.X() * pVecB.X() + pVecA.Y() * pVecB.Y();
}

float Vector2::NormalizedDot(const Vector2& pVecA, const Vector2& pVecB)
{
    return Dot(pVecA.Normalized(),pVecB.Normalized());
}

//Static Vector2 methods
Vector2 Vector2::ClampLength(const Vector2& pVec2, const float& pMaxLength)
{
    float sqrMax = pMaxLength*pMaxLength;
    float sqrLength = pVec2.SqrLength();
    return pVec2 * (static_cast<float>(sqrLength < sqrMax)*sqrtf(sqrLength) + static_cast<float>(sqrLength > sqrMax)*pMaxLength);
}

Vector2 Vector2::Lerp(const Vector2& pVecA, const Vector2& pVecB, const float& pT)
{
    return pVecA * (1.0f-pT) + pVecB * pT;
}

Vector2 Vector2::Max(const Vector2& pVecA, const Vector2& pVecB)
{
    return {
        static_cast<float>(pVecA.X() > pVecB.X())*pVecA.X() + static_cast<float>(pVecB.X() > pVecA.X())*pVecB.X(),
        static_cast<float>(pVecA.Y() > pVecB.Y())*pVecA.Y() + static_cast<float>(pVecB.Y() > pVecA.Y())*pVecB.Y()
    };
}

Vector2 Vector2::Min(const Vector2& pVecA, const Vector2& pVecB)
{
    return {
        static_cast<float>(pVecA.X() < pVecB.X())*pVecA.X() + static_cast<float>(pVecB.X() < pVecA.X())*pVecB.X(),
        static_cast<float>(pVecA.Y() < pVecB.Y())*pVecA.Y() + static_cast<float>(pVecB.Y() < pVecA.Y())*pVecB.Y()
    };
}

Vector2 Vector2::Reflect(const Vector2& pDirection, const Vector2& pNormal)
{
    return pDirection - pNormal*2.0f*Dot(pDirection, pNormal);
}

///<summary>counter-clockwise</summary>
Vector2 Vector2::Perpendicular(const Vector2& pVec2)
{
    return {-pVec2.Y(),pVec2.X()};
}

//End static methods


float Vector2::Length() const
{
    return sqrtf(_x * _x + _y * _y);
}

float Vector2::SqrLength() const
{
    return _x * _x + _y * _y;
}

Vector2 Vector2::Normalized() const
{
    float length = Length();
    length = static_cast<float>(length == 0.0f) + length;
    return { _x / length, _y / length };
}

void Vector2::Normalize()
{
    float length = Length();
    length = static_cast<float>(length == 0.0f) + length;
    *this /= length;
}

Vector2::~Vector2()
{
    _x = _y = 0.0f;
}

//operators

Vector2& Vector2::operator=(const Vector2& pVec2) { _x = pVec2.X(); _y = pVec2.Y(); return *this;}
Vector2& Vector2::operator+=(const Vector2& pVec2) { _x += pVec2.X(); _y += pVec2.Y(); return *this;}
Vector2& Vector2::operator-=(const Vector2& pVec2) { _x -= pVec2.X(); _y -= pVec2.Y(); return *this;}
Vector2& Vector2::operator*=(const Vector2& pVec2) { _x *= pVec2.X(); _y *= pVec2.Y(); return *this;}
Vector2& Vector2::operator/=(const Vector2& pVec2) { _x /= pVec2.X(); _y /= pVec2.Y(); return *this;}

Vector2& Vector2::operator*=(const float& pScalar) { _x *= pScalar; _y *= pScalar;  return *this;};
Vector2& Vector2::operator/=(const float& pScalar) { _x /= pScalar; _y /= pScalar; return *this;};


Vector2 Vector2::operator-() const { return {_x, -_y }; }
Vector2 Vector2::operator+(const Vector2& pVec2) const { return {_x + pVec2.X(), _y + pVec2.Y()}; }
Vector2 Vector2::operator-(const Vector2& pVec2) const { return {_x - pVec2.X(), _y - pVec2.Y() };}
Vector2 Vector2::operator*(const Vector2& pVec2) const { return {_x * pVec2.X(), _y * pVec2.Y()}; }
Vector2 Vector2::operator/(const Vector2& pVec2) const { return {_x / pVec2.X(), _y / pVec2.Y()}; }

Vector2 Vector2::operator*(const float& pScalar) const { return {_x * pScalar, _y * pScalar}; }
Vector2 Vector2::operator/(const float& pScalar) const { return {_x / pScalar, _y / pScalar}; }


bool Vector2::operator==(Vector2 pVec2) const { return _x == pVec2.X() && _y == pVec2.Y(); }
bool Vector2::operator!=(Vector2 pVec2) const { return _x != pVec2.X() || _y != pVec2.Y(); }


std::ostream& operator<<(std::ostream& stream, Vector2 const& pVec2)
{
    return stream << pVec2.X() << " " << pVec2.Y();
};
