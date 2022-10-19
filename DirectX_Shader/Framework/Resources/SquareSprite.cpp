#include "SquareSprite.h"

SquareSprite::SquareSprite()
{
	m_Verticies.push_back({
		D3DXVECTOR3(-0.5f, -0.5f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f),
		D3DXVECTOR2(0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(0.0f,0.0f, 0.0f) });
	m_Verticies.push_back({
		D3DXVECTOR3(0.5f, -0.5f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f),
		D3DXVECTOR2(1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(0.0f,0.0f, 0.0f) });
	m_Verticies.push_back({
		D3DXVECTOR3(-0.5f, 0.5f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f),
		D3DXVECTOR2(0.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(0.0f,0.0f, 0.0f) });
	m_Verticies.push_back({
		D3DXVECTOR3(0.5f, 0.5f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f),
		D3DXVECTOR2(1.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(0.0f,0.0f, 0.0f) });

	m_Indices.push_back(0);
	m_Indices.push_back(1);
	m_Indices.push_back(2);
	m_Indices.push_back(2);
	m_Indices.push_back(1);
	m_Indices.push_back(3);
}
