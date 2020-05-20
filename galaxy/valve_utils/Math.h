#pragma once
#include "..\valve_sdk\VMatrix.h"

#define PI 3.14159265358979323846f

class Math {
public:

	void VectorAngles(const Vector &vecForward, Vector &vecAngles);
	void angle_matrix( const Vector& angles, matrix3x4_t& matrix );
	void angle_matrix( const Vector& angles, const Vector& position, matrix3x4_t& matrix );
	__forceinline matrix3x4_t angle_matrix( const Vector angles );
	matrix3x4_t angle_matrix( const Vector angle, const Vector pos );
	void VectorAngles(const Vector& forward, Vector& up, Vector& angles);

	void VectorTransform(const Vector in1, const matrix3x4_t in2, Vector &out);

	Vector CalcAngle(const Vector& vecSource, const Vector& vecDestination);

	vec_t VectorNormalize(Vector& v);

	void AngleVectors(const Vector &angles, Vector *forward);

	void NormalizeAngles(Vector& angles);

	float NormalizeYaw(float yaw);

	void AngleVectors(const Vector& angles, Vector* forward, Vector* right, Vector* up);

	void RandomSeed(int seed);

	float RandomFloat(float min, float max);

	bool Clamp(Vector &angles);

	void ClampAngles(Vector &angles);

	float GRD_TO_BOG(float GRD);

	float Distance(Vector2D point1, Vector2D point2);
}; 
extern Math g_Math;