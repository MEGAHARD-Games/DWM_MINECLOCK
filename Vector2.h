//
// Created by Nine on 08/07/2026.
//

#ifndef DWM_MINECLOCK_VECTOR2_H
#define DWM_MINECLOCK_VECTOR2_H

#include <ostream>

class Vector2
{
protected:
    float _x, _y;

public:
    Vector2() = default;
    Vector2(const float pX, const float pY) : _x(pX), _y(pY) { };
    Vector2(const Vector2& pVec2) : _x(pVec2.X()), _y(pVec2.Y()) { };

    float& X() { return _x; }
    const float& X() const { return _x; }
    float& Y() { return _y; }
    const float& Y() const { return _y; }

    static Vector2 up() { return {0.0f, 1.0f}; }
    static Vector2 down() { return {0.0f, -1.0f}; }
    static Vector2 left() { return {-1.0f, 0.0f}; }
    static Vector2 right() { return {1.0f, 0.0f}; }
    static Vector2 zero() { return {0.0f, 0.0f}; }
    static Vector2 one() { return {1.0f, 1.0f,}; }

    static float Angle(const Vector2& pVecA, const Vector2& pVecB);
    static float SignedAngle(const Vector2& pVecA, const Vector2& pVecB);
    static float Distance(const Vector2& pVecA, const Vector2& pVecB);
    static float Dot(const Vector2& pVecA, const Vector2& pVecB);
    static float NormalizedDot(const Vector2& pVecA, const Vector2& pVecB);
    static float Cross(const Vector2& pVecA, const Vector2& pVecB);

    static Vector2 ClampLength(const Vector2& pVec2, const float& pMaxLength);
    static Vector2 Lerp(const Vector2& pVecA, const Vector2& pVecB,const float& pT);
    static Vector2 Max(const Vector2& pVecA, const Vector2& pVecB);
    static Vector2 Min(const Vector2& pVecA, const Vector2& pVecB);
    static Vector2 Reflect(const Vector2& pDirection, const Vector2& pNormal);
    static Vector2 Perpendicular(const Vector2& pVec2);

    float Length() const;
    float SqrLength() const;

    Vector2 Normalized() const;


    void Normalize();


    ~Vector2();

    Vector2& operator=(const Vector2& pVec2);
    Vector2& operator+=(const Vector2& pVec2);
    Vector2& operator-=(const Vector2& pVec2);
    Vector2& operator*=(const Vector2& pVec2);
    Vector2& operator/=(const Vector2& pVec2);

    Vector2& operator*=(const float& pScalar);
    Vector2& operator/=(const float& pScalar);


    Vector2 operator-() const;
    Vector2 operator+(const Vector2& pVec2) const;
    Vector2 operator-(const Vector2& pVec2) const;
    Vector2 operator*(const Vector2& pVec2) const;
    Vector2 operator/(const Vector2& pVec2) const;

    Vector2 operator*(const float& pScalar) const;
    Vector2 operator/(const float& pScalar) const;


    bool operator==(Vector2 pVec2) const;
    bool operator!=(Vector2 pVec2) const;

    friend std::ostream& operator<<(std::ostream& stream, Vector2 const& pVec2);
};

#endif //DWM_MINECLOCK_VECTOR2_H
