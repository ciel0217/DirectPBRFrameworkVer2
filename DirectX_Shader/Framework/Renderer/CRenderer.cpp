#include "CRenderer.h"
#include "../Resources/CBuffer.h"
#include "../Resources/DevelopStruct.h"

std::unique_ptr<CBuffer> CRenderer::m_MaterialCBuffer;
std::unique_ptr<CBuffer> CRenderer::m_WorldMatrixCBuffer;
std::unique_ptr<CBuffer> CRenderer::m_InverseWorldMatrixCBuffer;

void CRenderer::SetUpRenderer()
{
	if (!m_MaterialCBuffer)
		m_MaterialCBuffer.reset(new CBuffer(CBuffer::CreateBuffer(sizeof(MATERIAL_CBUFFER), D3D11_BIND_CONSTANT_BUFFER, nullptr)));
		//m_MaterialCBuffer = std::make_unique<CBuffer>(CBuffer::CreateBuffer(sizeof(MATERIAL_CBUFFER), D3D11_BIND_CONSTANT_BUFFER, nullptr));

	if (!m_WorldMatrixCBuffer)
		m_WorldMatrixCBuffer.reset(new CBuffer(CBuffer::CreateBuffer(sizeof(D3DXMATRIX), D3D11_BIND_CONSTANT_BUFFER, nullptr)));

	if(!m_InverseWorldMatrixCBuffer)
		m_InverseWorldMatrixCBuffer.reset(new CBuffer(CBuffer::CreateBuffer(sizeof(D3DXMATRIX), D3D11_BIND_CONSTANT_BUFFER, nullptr)));
}
