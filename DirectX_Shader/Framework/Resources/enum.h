#pragma once


enum SortingLayer {
	eBackMost=0,
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

enum PlayerBulletType {
	eBullet = 0,
	eRocket,
};