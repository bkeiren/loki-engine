#pragma once

#ifndef MATH_COMMON_H
#define MATH_COMMON_H

namespace loki
{

namespace math
{

extern const glm::vec3 GlobalX;
extern const glm::vec3 GlobalY;
extern const glm::vec3 GlobalZ;

#define UNIT_X	loki::math::GlobalX
#define UNIT_Y	loki::math::GlobalY
#define UNIT_Z	loki::math::GlobalZ

#define FORWARD	UNIT_X
#define SIDE	UNIT_Z
#define UP		UNIT_Y

#define ZEROVECTOR	glm::vec3(0.0f, 0.0f, 0.0f)

namespace color
{

extern const glm::vec3 Red;
extern const glm::vec4 RedA;
extern const glm::vec3 Green;
extern const glm::vec4 GreenA;
extern const glm::vec3 Blue;
extern const glm::vec4 BlueA;

extern const glm::vec3 Yellow;
extern const glm::vec4 YellowA;
extern const glm::vec3 Turqiose;
extern const glm::vec4 TurqioseA;
extern const glm::vec3 Purple;
extern const glm::vec4 PurpleA;

extern const glm::vec3 White;
extern const glm::vec4 WhiteA;
extern const glm::vec3 Black;
extern const glm::vec4 BlackA;

}

}

}

#endif