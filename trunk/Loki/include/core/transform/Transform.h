#pragma once

#ifndef TRANSFORM_H
#define TRANSFORM_H

namespace loki
{

class Transform
{
public:
	Transform();
	~Transform();

	const glm::quat& GetOrientation();
	const glm::vec3& GetTranslation();
	const glm::mat4& GetMatrix();

	glm::vec3 GetOrientationVector();

	glm::vec3 GetEulerAngles();
	float GetPitch() const;
	float GetYaw() const;
	float GetRoll() const;

	void SetOrientation( const glm::quat& _Orientation );
	void SetTranslation( const glm::vec3& _Translation );
	void SetMatrix( const glm::mat4& _Matrix );

	void TranslateLocal( const glm::vec3& _Translation );
	void TranslateWorld( const glm::vec3& _Translation );
	void RotateXLocal( float _Angle );
	void RotateYLocal( float _Angle );
	void RotateZLocal( float _Angle );
	void RotateLocal( glm::vec3& _Axis, float _Angle );
	void RotateXWorld( float _Angle );
	void RotateYWorld( float _Angle );
	void RotateZWorld( float _Angle );
	void RotateWorld( glm::vec3& _Axis, float _Angle );

	bool IsDirty() const;

	bool operator == ( Transform& _Transform );
	bool operator != ( Transform& _Transform );
private:
	void _GenerateMatrix();
	void _GenerateTranslationOrientation();

	glm::quat m_Orientation;
	glm::vec3 m_Translation;
	glm::mat4 m_Transformation;

	bool m_MatrixIsDirty;
	bool m_TranslationOrientationAreDirty;
};

}

#endif