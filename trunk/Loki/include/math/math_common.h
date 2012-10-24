#pragma once

#ifndef MATH_COMMON_H
#define MATH_COMMON_H

namespace loki
{

//namespace glm
//{

extern const math::vec3 GlobalX;
extern const math::vec3 GlobalY;
extern const math::vec3 GlobalZ;

#define UNIT_X	loki/*::math*/::GlobalX
#define UNIT_Y	loki/*::math*/::GlobalY
#define UNIT_Z	loki/*::math*/::GlobalZ

#define FORWARD	UNIT_X
#define SIDE	UNIT_Z
#define UP		UNIT_Y

#define ZEROVECTOR		math::vec3(0.0f, 0.0f, 0.0f)
#define IDENTITYMAT3	math::mat3(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f)
#define IDENTITYMAT4	math::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f)

namespace color
{

extern const math::vec3 Red;
extern const math::vec4 RedA;
extern const math::vec3 Green;
extern const math::vec4 GreenA;
extern const math::vec3 Blue;
extern const math::vec4 BlueA;

extern const math::vec3 Yellow;
extern const math::vec4 YellowA;
extern const math::vec3 Turqiose;
extern const math::vec4 TurqioseA;
extern const math::vec3 Purple;
extern const math::vec4 PurpleA;

extern const math::vec3 White;
extern const math::vec4 WhiteA;
extern const math::vec3 Black;
extern const math::vec4 BlackA;

}

//}

}

#endif