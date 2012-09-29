#pragma once

#ifndef MODEL_H
#define MODEL_H

namespace loki
{

namespace graphics
{

class Mesh;
class Material;

class Model
{
	CONTAINER_MACRO_VECTOR(Mesh*, Meshes);
	CONTAINER_MACRO_VECTOR(Material*, Materials);
public:
private:
	Meshes m_Meshes;
	Materials m_Materials;
};

}

}

#endif