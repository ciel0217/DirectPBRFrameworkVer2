#pragma once

//*********************************************************
// マクロ定義
//*********************************************************
#define LIGHT_MAX		(10)


enum BLEND_MODE
{
	BLEND_MODE_NONE,		//ブレンド無し
	BLEND_MODE_ALPHABLEND,	//αブレンド
	BLEND_MODE_ADD,			//加算ブレンド
	BLEND_MODE_SUBTRACT,	//減算ブレンド

	BLEDD_MODE_NUM
};

enum CULL_MODE
{
	CULL_MODE_NONE,			//カリング無し
	CULL_MODE_FRONT,		//表のポリゴンを描画しない(CW)
	CULL_MODE_BACK,			//裏のポリゴンを描画しない(CCW)

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


