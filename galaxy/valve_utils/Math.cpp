#include "Math.h"

#include <cmath>
#include <xmmintrin.h>
#include <pmmintrin.h>
Math g_Math;

//not taking credit for any of this math

static const float invtwopi = 0.1591549f;
static const float twopi = 6.283185f;
static const float threehalfpi = 4.7123889f;
static const float pi = 3.141593f;
static const float halfpi = 1.570796f;
static const __m128 signmask = _mm_castsi128_ps( _mm_set1_epi32( 0x80000000 ) );

static const __declspec(align(16)) float null[4] = { 0.f, 0.f, 0.f, 0.f };
static const __declspec(align(16)) float _pi2[4] = { 1.5707963267948966192f, 1.5707963267948966192f, 1.5707963267948966192f, 1.5707963267948966192f };
static const __declspec(align(16)) float _pi[4] = { 3.141592653589793238f, 3.141592653589793238f, 3.141592653589793238f, 3.141592653589793238f };


typedef __declspec(align(16)) union
{
	float f[4];
	__m128 v;
} m128;

__forceinline __m128 sqrt_ps( const __m128 squared )
{
	return _mm_sqrt_ps( squared );
}

__forceinline __m128 cos_52s_ps( const __m128 x )
{
	const auto c1 = _mm_set1_ps( 0.9999932946f );
	const auto c2 = _mm_set1_ps( -0.4999124376f );
	const auto c3 = _mm_set1_ps( 0.0414877472f );
	const auto c4 = _mm_set1_ps( -0.0012712095f );
	const auto x2 = _mm_mul_ps( x, x );
	return _mm_add_ps( c1, _mm_mul_ps( x2, _mm_add_ps( c2, _mm_mul_ps( x2, _mm_add_ps( c3, _mm_mul_ps( c4, x2 ) ) ) ) ) );
}

__forceinline __m128 cos_ps( __m128 angle )
{
	angle = _mm_andnot_ps( signmask, angle );
	angle = _mm_sub_ps( angle, _mm_mul_ps( _mm_cvtepi32_ps( _mm_cvttps_epi32( _mm_mul_ps( angle, _mm_set1_ps( invtwopi ) ) ) ), _mm_set1_ps( twopi ) ) );

	auto cosangle = angle;
	cosangle = _mm_xor_ps( cosangle, _mm_and_ps( _mm_cmpge_ps( angle, _mm_set1_ps( halfpi ) ), _mm_xor_ps( cosangle, _mm_sub_ps( _mm_set1_ps( pi ), angle ) ) ) );
	cosangle = _mm_xor_ps( cosangle, _mm_and_ps( _mm_cmpge_ps( angle, _mm_set1_ps( pi ) ), signmask ) );
	cosangle = _mm_xor_ps( cosangle, _mm_and_ps( _mm_cmpge_ps( angle, _mm_set1_ps( threehalfpi ) ), _mm_xor_ps( cosangle, _mm_sub_ps( _mm_set1_ps( twopi ), angle ) ) ) );

	auto result = cos_52s_ps( cosangle );
	result = _mm_xor_ps( result, _mm_and_ps( _mm_and_ps( _mm_cmpge_ps( angle, _mm_set1_ps( halfpi ) ), _mm_cmplt_ps( angle, _mm_set1_ps( threehalfpi ) ) ), signmask ) );
	return result;
}

__forceinline __m128 sin_ps( const __m128 angle )
{
	return cos_ps( _mm_sub_ps( _mm_set1_ps( halfpi ), angle ) );
}

__forceinline void sincos_ps( __m128 angle, __m128* sin, __m128* cos ) {
	const auto anglesign = _mm_or_ps( _mm_set1_ps( 1.f ), _mm_and_ps( signmask, angle ) );
	angle = _mm_andnot_ps( signmask, angle );
	angle = _mm_sub_ps( angle, _mm_mul_ps( _mm_cvtepi32_ps( _mm_cvttps_epi32( _mm_mul_ps( angle, _mm_set1_ps( invtwopi ) ) ) ), _mm_set1_ps( twopi ) ) );

	auto cosangle = angle;
	cosangle = _mm_xor_ps( cosangle, _mm_and_ps( _mm_cmpge_ps( angle, _mm_set1_ps( halfpi ) ), _mm_xor_ps( cosangle, _mm_sub_ps( _mm_set1_ps( pi ), angle ) ) ) );
	cosangle = _mm_xor_ps( cosangle, _mm_and_ps( _mm_cmpge_ps( angle, _mm_set1_ps( pi ) ), signmask ) );
	cosangle = _mm_xor_ps( cosangle, _mm_and_ps( _mm_cmpge_ps( angle, _mm_set1_ps( threehalfpi ) ), _mm_xor_ps( cosangle, _mm_sub_ps( _mm_set1_ps( twopi ), angle ) ) ) );

	auto result = cos_52s_ps( cosangle );
	result = _mm_xor_ps( result, _mm_and_ps( _mm_and_ps( _mm_cmpge_ps( angle, _mm_set1_ps( halfpi ) ), _mm_cmplt_ps( angle, _mm_set1_ps( threehalfpi ) ) ), signmask ) );
	*cos = result;

	const auto sinmultiplier = _mm_mul_ps( anglesign, _mm_or_ps( _mm_set1_ps( 1.f ), _mm_and_ps( _mm_cmpgt_ps( angle, _mm_set1_ps( pi ) ), signmask ) ) );
	*sin = _mm_mul_ps( sinmultiplier, sqrt_ps( _mm_sub_ps( _mm_set1_ps( 1.f ), _mm_mul_ps( result, result ) ) ) );
}


void Math::VectorAngles(const Vector &vecForward, Vector &vecAngles)
{
	Vector vecView;
	if (vecForward[1] == 0.f && vecForward[0] == 0.f)
	{
		vecView[0] = 0.f;
		vecView[1] = 0.f;
	}
	else
	{
		vecView[1] = atan2(vecForward[1], vecForward[0]) * 180.f / 3.14159265358979323846f;

		if (vecView[1] < 0.f)
			vecView[1] += 360.f;

		vecView[2] = sqrt(vecForward[0] * vecForward[0] + vecForward[1] * vecForward[1]);

		vecView[0] = atan2(vecForward[2], vecView[2]) * 180.f / 3.14159265358979323846f;
	}

	vecAngles[0] = -vecView[0];
	vecAngles[1] = vecView[1];
	vecAngles[2] = 0.f;
}

void sin_cos( float radian, float* sin, float* cos )
{
	*sin = std::sin( radian );
	*cos = std::cos( radian );
}

void Math::VectorAngles(const Vector& forward, Vector& up, Vector& angles) {
	Vector left = CrossProduct(up, forward);
	left.NormalizeInPlace();

	float forwardDist = forward.Length2D();

	if (forwardDist > 0.001f) {
		angles.x = atan2f(-forward.z, forwardDist) * 180 / M_PI;
		angles.y = atan2f(forward.y, forward.x) * 180 / M_PI;

		float upZ = (left.y * forward.x) - (left.x * forward.y);
		angles.z = atan2f(left.z, upZ) * 180 / M_PI;
	}
	else {
		angles.x = atan2f(-forward.z, forwardDist) * 180 / M_PI;
		angles.y = atan2f(-left.x, left.y) * 180 / M_PI;
		angles.z = 0;
	}
}

void Math::angle_matrix( const Vector& angles, matrix3x4_t& matrix )
{
	float sr, sp, sy, cr, cp, cy;

	sin_cos( DEG2RAD( angles.x ), &sp, &cp );
	sin_cos( DEG2RAD( angles.y ), &sy, &cy );
	sin_cos( DEG2RAD( angles.z ), &sr, &cr );

	// matrix = (YAW * PITCH) * ROLL
	matrix[0][0] = cp * cy;
	matrix[1][0] = cp * sy;
	matrix[2][0] = -sp;

	const auto crcy = cr * cy;
	const auto crsy = cr * sy;
	const auto srcy = sr * cy;
	const auto srsy = sr * sy;
	matrix[0][1] = sp * srcy - crsy;
	matrix[1][1] = sp * srsy + crcy;
	matrix[2][1] = sr * cp;

	matrix[0][2] = (sp * crcy + srsy);
	matrix[1][2] = (sp * crsy - srcy);
	matrix[2][2] = cr * cp;

	matrix[0][3] = 0.0f;
	matrix[1][3] = 0.0f;
	matrix[2][3] = 0.0f;
}
void matrix_set_column( const Vector& in, int column, matrix3x4_t& out )
{
	out[0][column] = in.x;
	out[1][column] = in.y;
	out[2][column] = in.z;
}
void Math::angle_matrix( const Vector& angles, const Vector& position, matrix3x4_t& matrix )
{
	angle_matrix( angles, matrix );
	matrix_set_column( position, 3, matrix );
}
__forceinline matrix3x4_t Math::angle_matrix( const Vector angles )
{
	matrix3x4_t result{};

	m128 angle, sin, cos;
	angle.f[0] = DEG2RAD( angles.x );
	angle.f[1] = DEG2RAD( angles.y );
	angle.f[2] = DEG2RAD( angles.z );
	sincos_ps( angle.v, &sin.v, &cos.v );

	result[0][0] = cos.f[0] * cos.f[1];
	result[1][0] = cos.f[0] * sin.f[1];
	result[2][0] = -sin.f[0];

	const auto crcy = cos.f[2] * cos.f[1];
	const auto crsy = cos.f[2] * sin.f[1];
	const auto srcy = sin.f[2] * cos.f[1];
	const auto srsy = sin.f[2] * sin.f[1];

	result[0][1] = sin.f[0] * srcy - crsy;
	result[1][1] = sin.f[0] * srsy + crcy;
	result[2][1] = sin.f[2] * cos.f[0];

	result[0][2] = sin.f[0] * crcy + srsy;
	result[1][2] = sin.f[0] * crsy - srcy;
	result[2][2] = cos.f[2] * cos.f[0];

	return result;
}
matrix3x4_t Math::angle_matrix( const Vector angle, const Vector pos )
{
	auto result = angle_matrix( angle );
	result[0][3] = pos.x;
	result[1][3] = pos.y;
	result[2][3] = pos.z;
	return result;
}

Vector Math::CalcAngle(const Vector& vecSource, const Vector& vecDestination)
{
	Vector qAngles;
	Vector delta = Vector((vecSource[0] - vecDestination[0]), (vecSource[1] - vecDestination[1]), (vecSource[2] - vecDestination[2]));
	float hyp = sqrtf(delta[0] * delta[0] + delta[1] * delta[1]);
	qAngles[0] = (float)(atan(delta[2] / hyp) * (180.0f / M_PI));
	qAngles[1] = (float)(atan(delta[1] / delta[0]) * (180.0f / M_PI));
	qAngles[2] = 0.f;
	if (delta[0] >= 0.f)
		qAngles[1] += 180.f;

	return qAngles;
}

void Math::VectorTransform(const Vector in1, const matrix3x4_t in2, Vector &out) {
	out[0] = DotProduct(in1, Vector(in2[0][0], in2[0][1], in2[0][2])) + in2[0][3];
	out[1] = DotProduct(in1, Vector(in2[1][0], in2[1][1], in2[1][2])) + in2[1][3];
	out[2] = DotProduct(in1, Vector(in2[2][0], in2[2][1], in2[2][2])) + in2[2][3];
}

vec_t Math::VectorNormalize(Vector& v)
{
	vec_t l = v.Length();

	if (l != 0.0f)
	{
		v /= l;
	}
	else
	{
		v.x = v.y = 0.0f; v.z = 1.0f;
	}

	return l;
}

void Math::AngleVectors(const Vector &angles, Vector *forward)
{
	Assert(s_bMathlibInitialized);
	Assert(forward);

	float	sp, sy, cp, cy;

	sy = sin(DEG2RAD(angles[1]));
	cy = cos(DEG2RAD(angles[1]));

	sp = sin(DEG2RAD(angles[0]));
	cp = cos(DEG2RAD(angles[0]));

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}

void Math::NormalizeAngles(Vector& angles)
{
	for (auto i = 0; i < 3; i++) {
		while (angles[i] < -180.0f) angles[i] += 360.0f;
		while (angles[i] >  180.0f) angles[i] -= 360.0f;
	}
}

float Math::NormalizeYaw(float yaw)
{
	if (yaw > 180)
		yaw -= (round(yaw / 360) * 360.f);
	else if (yaw < -180)
		yaw += (round(yaw / 360) * -360.f);

	return yaw;
}



void Math::AngleVectors(const Vector& angles, Vector* forward, Vector* right, Vector* up)
{
	float sp, sy, sr, cp, cy, cr;

	sin_cos(GRD_TO_BOG(angles.x), &sp, &cp);
	sin_cos(GRD_TO_BOG(angles.y), &sy, &cy);
	sin_cos(GRD_TO_BOG(angles.z), &sr, &cr);

	if (forward != nullptr)
	{
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right != nullptr)
	{
		right->x = -1 * sr * sp * cy + -1 * cr * -sy;
		right->y = -1 * sr * sp * sy + -1 * cr * cy;
		right->z = -1 * sr * cp;
	}

	if (up != nullptr)
	{
		up->x = cr * sp * cy + -sr * -sy;
		up->y = cr * sp * sy + -sr * cy;
		up->z = cr * cp;
	}
}

void Math::RandomSeed(int seed)
{
	static auto random_seed = reinterpret_cast<void(*)(int)>(GetProcAddress(GetModuleHandleA("vstdlib.dll"), "RandomSeed"));

	random_seed(seed);
}

float Math::RandomFloat(float min, float max)
{
	static auto random_float = reinterpret_cast<float(*)(float, float)>(GetProcAddress(GetModuleHandleA("vstdlib.dll"), "RandomFloat"));

	return random_float(min, max);
}

void Math::ClampAngles(Vector &angles) {
	if (angles.y > 180.0f)
		angles.y = 180.0f;
	else if (angles.y < -180.0f)
		angles.y = -180.0f;

	if (angles.x > 89.0f)
		angles.x = 89.0f;
	else if (angles.x < -89.0f)
		angles.x = -89.0f;

	angles.z = 0;
}

bool Math::Clamp(Vector &angles)
{
	Vector a = angles;
	NormalizeAngles(a);
	ClampAngles(a);

	if (isnan(a.x) || isinf(a.x) ||
		isnan(a.y) || isinf(a.y) ||
		isnan(a.z) || isinf(a.z)) {
		return false;
	}
	else {
		angles = a;
		return true;
	}	
}

float Math::GRD_TO_BOG(float GRD) {
	return (M_PI / 180) * GRD;
}

float Math::Distance(Vector2D point1, Vector2D point2)
{
	float diffY = point1.y - point2.y;
	float diffX = point1.x - point2.x;
	return sqrt((diffY * diffY) + (diffX * diffX));
}