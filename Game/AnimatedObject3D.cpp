#include "pch.h"
#include "AnimatedObject3D.h"
#include "model.h"
#include <windows.h>
#include "Effects.h"
#include "camera.h"
#include "CommonStates.h"
#include "DrawData.h"
#include "Helper.h"
#include "GameData.h"

#include <iostream>

#include <DirectXCollision.h>
#include <iostream>

#include <DirectXCollision.h>


ID3D11RasterizerState* AnimatedObject3D::s_pRasterState = nullptr;
int AnimatedObject3D::m_count = 0;

AnimatedObject3D::AnimatedObject3D(std::string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF)
{


	std::string filePath = "../Assets/" + _fileName + ".cmo";

	wchar_t* file = Helper::charToWChar(filePath.c_str());
	m_model = Model::CreateFromCMO(
		_pd3dDevice,        // Direct3D device pointer
		file,               // Path to the CMO file
		*_EF,               // Reference to an IEffectFactory object
		ModelLoader_CounterClockwise | ModelLoader_IncludeBones,  // Flags
		nullptr             // Optional: Pointer to store animation offset (not needed in this case)
	);
	m_world = Matrix::Identity;
	const size_t nbones = m_model->bones.size();

	m_drawBones = ModelBone::MakeArray(nbones);
	m_animBones = ModelBone::MakeArray(nbones);
	uint32_t index = 0;

	for (size_t index = 0; index < nbones; ++index)
	{
		const auto& bone = m_model->bones[index];

		// Print the name of the bone
		std::wcout << L"Bone " << index << L": " << bone.name << std::endl;
	}
	for (const auto& it : m_model->bones)
	{
		if (_wcsicmp(it.name.c_str(), L"DEF-PELVIS") == 0)
		{
			// Need to recenter the model.
			m_animBones[index] = XMMatrixIdentity();
		}

		++index;
	}
	m_model->CopyBoneTransformsTo(nbones, m_animBones.get());

}

AnimatedObject3D::~AnimatedObject3D()
{
}

void AnimatedObject3D::Tick(GameData* _GameData)
{	
	timer += _GameData->m_DeltaTime;
	

	m_world = XMMatrixRotationY(timer * 0.1f);
}

void AnimatedObject3D::Draw(DrawData* _DrawData)
{
	size_t nbones = m_model->bones.size();

	m_model->CopyAbsoluteBoneTransforms(nbones,
		m_animBones.get(), m_drawBones.get());

	m_model->Draw(_DrawData->m_pd3dImmediateContext, *_DrawData->m_states, nbones, m_drawBones.get(),
		m_world, _DrawData->m_cam->GetView(), _DrawData->m_cam->GetProj());

}

bool AnimatedObject3D::Intersects(const AnimatedObject3D& other) const
{
	return false;
}
