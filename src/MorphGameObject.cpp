// Tom Tsiliopoulos - 100616336

#include "MorphGameObject.h"
#include "TTK/OBJMesh.h"
#include <iostream>
#include "AnimationMath.h"

// Remember: the base class' constructor will get called before this one
MorphGameObject::MorphGameObject() : m_pCurrentKeyframe(0), m_pNextKeyframe(1), m_pLocalMorphTime(0.0f)
{
}

void MorphGameObject::update(float dt)
{
	// Call the base class' update to create the 4x4 transformation matrix
	GameObject::update(dt);

	std::cout << dt << std::endl;

	m_pLocalMorphTime += dt;
	if (m_pLocalMorphTime >= 1) {
		m_pLocalMorphTime = 0;
		m_pCurrentKeyframe++;
		m_pNextKeyframe++;
		if (m_pCurrentKeyframe > m_pMorphTargets.size() - 1) {
			m_pCurrentKeyframe = 0;
		}
		if (m_pNextKeyframe > m_pMorphTargets.size() - 1) {
			m_pNextKeyframe = 0;
		}
	}

	for (int i = 0; i < m_pMorphTargets[m_pCurrentKeyframe]->vertices.size(); i++)
	{
		mesh->vertices[i] = Math::lerp(m_pMorphTargets[m_pCurrentKeyframe]->vertices[i],
			m_pMorphTargets[m_pNextKeyframe]->vertices[i], m_pLocalMorphTime);
		mesh->normals[i] = Math::lerp(m_pMorphTargets[m_pCurrentKeyframe]->normals[i],
			m_pMorphTargets[m_pNextKeyframe]->normals[i], m_pLocalMorphTime);
		mesh->textureCoordinates[i] = Math::lerp(m_pMorphTargets[m_pCurrentKeyframe]->textureCoordinates[i],
			m_pMorphTargets[m_pNextKeyframe]->textureCoordinates[i], m_pLocalMorphTime);
	}
}

void MorphGameObject::addMorphTarget(std::string filePath)
{
	// Load the mesh
	std::shared_ptr<TTK::OBJMesh> newTarget = std::make_shared<TTK::OBJMesh>();
	newTarget->loadMesh(filePath);
	newTarget->primitiveType = TTK::Triangles;

	// Initialize the mesh object in the base class
	if (!mesh)
	{
		// Allocate memory if this is the first target being added
		// The mesh object in the base class is where we will store the intermediate morph targets
		// and is what will actually end up on screen
		mesh = std::make_shared<TTK::MeshBase>();
		int numVerts = newTarget->vertices.size();
		mesh->vertices.resize(numVerts);
		mesh->normals.resize(numVerts);
		mesh->textureCoordinates.resize(numVerts);
	}
	else
	{
		// Remember: in order for morphing to work correctly each target must have the same number of vertices
		// We will perform a check to make sure the new target has the same number of vertices as the existing ones

		if (mesh->vertices.size() != newTarget->vertices.size())
		{
			std::cout << "New target does not have the same number of vertices as previous target!" << std::endl;
			return;
		}
	}

	// Store it in the list of morph targets
	m_pMorphTargets.push_back(newTarget);
}
