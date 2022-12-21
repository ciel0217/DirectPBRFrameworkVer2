#pragma once

//*********************************************************
// �}�N����`
//*********************************************************
#define LIGHT_MAX		(10)


enum BLEND_MODE
{
	BLEND_MODE_NONE,		//�u�����h����
	BLEND_MODE_ALPHABLEND,	//���u�����h
	BLEND_MODE_ADD,			//���Z�u�����h
	BLEND_MODE_SUBTRACT,	//���Z�u�����h

	BLEDD_MODE_NUM
};

enum CULL_MODE
{
	CULL_MODE_NONE,			//�J�����O����
	CULL_MODE_FRONT,		//�\�̃|���S����`�悵�Ȃ�(CW)
	CULL_MODE_BACK,			//���̃|���S����`�悵�Ȃ�(CCW)

	CULL_MODE_NUM
};

enum ADDRESS_MODE
{
	ADDRESS_MODE_WRAP,
	ADDRESS_MODE_MIRROR,
	ADDRESS_MODE_CLAMP,
	ADDRESS_MODE_BORDER,

	ADDRESS_MODE_NUM
};

enum FILTER_MODE
{
	FILTER_MODE_POINT,
	FILTER_MODE_LINEAR,
	FILTER_MODE_ANISOTROPIC,

	FILTER_MODE_NUM
};

enum SCENE_TEXTURE
{
	eBaseColor = 0,
	eNormal,
	eRoughnessAndMeralicAndSpecular,
	eEmmision,

	eToneMap,
	eSceneMax,
};

enum SortingLayer {
	eBackMost = 0,
	e3DObject,
	e2DObject,


	eNone,
	eMaxLayer,
};

enum PostEffectType {
	eOnlyOpacity = 0,
	eAll,
};

enum DrawObjectRenderQueue {
	eOpacity = 2500,
	eSkyBox = 7000,
	eTransparent = 7500,

	e2D = 10000,
};

enum CollisionType {
	eBoxCollision = 0,


	eCollisionTypeMax,
};

enum ParticleRenderType
{
	eBillboard = 0,

	eTypeMax,
};


