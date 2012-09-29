#pragma once

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Types.h"	// Need this for client apps including this file.

namespace loki
{

class Transform
{
public:
	Transform();
	~Transform();

	const quat& GetOrientation();
	const vec3& GetTranslation();
	const mat4& GetMatrix();

	vec3 GetOrientationVector();

	vec3 GetEulerAngles();
	float GetPitch() const;
	float GetYaw() const;
	float GetRoll() const;

	void SetOrientation( const quat& _Orientation );
	void SetTranslation( const vec3& _Translation );
	void SetMatrix( const mat4& _Matrix );

	void TranslateLocal( const vec3& _Translation );
	void TranslateWorld( const vec3& _Translation );
	void RotateXLocal( float _Angle );
	void RotateYLocal( float _Angle );
	void RotateZLocal( float _Angle );
	void RotateLocal( vec3& _Axis, float _Angle );
	void RotateXWorld( float _Angle );
	void RotateYWorld( float _Angle );
	void RotateZWorld( float _Angle );
	void RotateWorld( vec3& _Axis, float _Angle );

	//////////////////////////////////////////////////////////////////////////
	// If this returns true, it means that the next call to GetTranslation,
	// GetOrientation or GetMatrix will trigger a regeneration of either the
	// internal matrix or the internal translation vector and orientation
	// quaternion. Although this is no issue, it might be useful to have this
	// information.
	//////////////////////////////////////////////////////////////////////////
	bool IsDirty() const;

	bool operator == ( Transform& _Transform );
	bool operator != ( Transform& _Transform );
private:
	void _GenerateMatrix();
	void _GenerateTranslationOrientation();

	quat m_Orientation;
	vec3 m_Translation;
	mat4 m_Transformation;

	bool m_MatrixIsDirty;
	bool m_TranslationOrientationAreDirty;
};

}

#endif