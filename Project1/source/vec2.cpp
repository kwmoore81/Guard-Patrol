#include "vec2.h"



vec2 vec2::fromAngle(float angle)
{
	return{ cosf(angle), sinf(angle) };
}

float vec2::magnitude() const
{
	return sqrt(x*x + y*y);
}

float vec2::angle() const
{
	return atan2f(y, x);
}

vec2 vec2::normal() const
{
	return *this / magnitude();
}

vec2 perp(vec2 &a)
{
	return vec2({ -a.y, a.x });
}

vec2 vec2::normalize()
{
	vec2 norm;

	norm.x = x / magnitude();
	norm.y = y / magnitude();

	return norm;
}

vec2 min(const vec2 & a, const vec2 & b)
{
	if (a < b)
		return a;

	else
		return b;
}

vec2 max(const vec2 & a, const vec2 & b)
{
	if (a > b)
		return a;

	else
		return b;
}

vec2 mix(const vec2 & start, const vec2 & end, float t)
{
	vec2 mixed;

	mixed.x = start.x * (1 - t) + end.x * t;
	mixed.y = start.x * (1 - t) + end.x * t;

	return mixed;
}

vec2 lerp(const vec2 & start, const vec2 & end, float t)
{
	vec2 lerped;

	lerped.x = (1 - t) * start.x + t * end.x;
	lerped.y = (1 - t) * start.y + t * end.y;

	return lerped;
}

vec2 clamp(const vec2 &a, const vec2 &min, const vec2 &max)
{
	vec2 clamped;

	clamped.x = fmax(fmin(min.x, a.x), min.x);
	clamped.y = fmax(fmin(min.y, a.y), min.x);

	return clamped;
}

float dot(const vec2 & lhs, const vec2 & rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y;
}

vec2 reflect(const vec2 & incident, const vec2 & normal)
{
	vec2 reflection;

	reflection.x = incident.x - 2 * (incident.x * normal.x) * normal.x;
	reflection.y = incident.y - 2 * (incident.y * normal.y) * normal.y;

	return reflection;
}

vec2 project(const vec2 & a, const vec2 & b)
{
	return dot(a, b) * b.normal();
}

vec2 snap(const vec2 & val, const vec2 & lower, const vec2 & upper)
{
	return{ val.x - lower.x < upper.x - val.x ? lower.x : upper.x,
		val.y - lower.y < upper.y - val.y ? lower.y : upper.y };
}

vec2 operator+(const vec2 & lhs, const vec2 & rhs)
{
	return vec2(lhs.x + rhs.x, lhs.y + rhs.y);
}

vec2 operator+=(const vec2 & lhs, const vec2 & rhs)
{
	vec2 plEqVec2;

	plEqVec2.x = lhs.x + rhs.x;
	plEqVec2.y = lhs.y + rhs.y;

	return plEqVec2;
}

vec2 operator-(const vec2 & lhs, const vec2 & rhs)
{
	return vec2(lhs.x - rhs.x, lhs.y - rhs.y);
}

vec2 operator-=(const vec2 & lhs, const vec2 & rhs)
{
	vec2 minusVec2;

	minusVec2.x = lhs.x - rhs.x;
	minusVec2.y = lhs.y - rhs.y;

	return minusVec2;
}

vec2 operator*(float lhs, const vec2 & rhs)
{
	return rhs * lhs;
}

vec2 operator*(const vec2 & lhs, float rhs)
{
	return vec2(lhs.x * rhs, lhs.y * rhs);
}


vec2 operator*=(const vec2 & lhs, float rhs)
{
	vec2 multVec2;

	multVec2.x = lhs.x * rhs;
	multVec2.y = lhs.y * rhs;

	return multVec2;
}

vec2 operator/(const vec2 & lhs, float rhs)
{
	return vec2(lhs.x / rhs, lhs.y / rhs);
}

vec2 operator/=(const vec2 & lhs, float rhs)
{
	vec2 divVec2;

	divVec2.x = lhs.x / rhs;
	divVec2.y = lhs.y / rhs;

	return divVec2;
}

inline bool operator==(const vec2 & lhs, const vec2 & rhs)
{
	/*if (fabs(lhs.x - rhs.x < FLT_EPSILON) && fabs(lhs.y - rhs.y < FLT_EPSILON)) { return true; }

	else { return false; }*/

	return rhs.x - FLT_EPSILON < lhs.x && lhs.x < rhs.x + FLT_EPSILON &&
		rhs.y - FLT_EPSILON < lhs.y && lhs.y < rhs.y + FLT_EPSILON;
}

bool operator!=(const vec2 & lhs, const vec2 & rhs)
{
	if (fabs(lhs.x - rhs.x < FLT_EPSILON) && fabs(lhs.y - rhs.y < FLT_EPSILON)) { return false; }

	else { return true; }
}

bool operator<(const vec2 & lhs, const vec2 & rhs)
{
	if (fabs(lhs.x < (rhs.x - FLT_EPSILON)) && fabs(lhs.y < (rhs.y - FLT_EPSILON))) { return true; }

	else { return false; }
}

bool operator<=(const vec2 & lhs, const vec2 & rhs)
{
	if (fabs(lhs.x - rhs.x <= FLT_EPSILON) && fabs(lhs.y - rhs.y <= FLT_EPSILON)) { return true; }

	else { return false; }
}

bool operator>(const vec2 & lhs, const vec2 & rhs)
{
	if (fabs(lhs.x > (rhs.x + FLT_EPSILON)) && fabs(lhs.y > (rhs.y + FLT_EPSILON))) { return true; }

	else { return false; }
}

bool operator>=(const vec2 & lhs, const vec2 & rhs)
{
	if (fabs(lhs.x - rhs.x >= -FLT_EPSILON) && fabs(lhs.y - rhs.y >= -FLT_EPSILON)) { return true; }

	else { return false; }
}
