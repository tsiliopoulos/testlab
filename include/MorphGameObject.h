#pragma once

#include "GameObject.h"
#include <vector>
#include <memory>

class MorphGameObject : public GameObject
{
	std::vector<std::shared_ptr<TTK::MeshBase>> m_pMorphTargets;
	int m_pCurrentKeyframe;
	int m_pNextKeyframe;
	float m_pLocalMorphTime; // the 't' value used for lerp between [0, 1]

public:
	MorphGameObject();

	virtual void update(float dt) override;

	void addMorphTarget(std::string filePath);
};