#pragma once
#include "../LowLevel/CDxRenderer.h"

class StructuredBuffer
{
private:
	Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> m_SRV;
	Microsoft::WRL::ComPtr <ID3D11Buffer> m_StructuredBuffer;

	

public:
	StructuredBuffer() = delete;
	StructuredBuffer(ID3D11ShaderResourceView* srv, ID3D11Buffer* buf) { m_SRV.Attach(srv); m_StructuredBuffer.Attach(buf); }
	~StructuredBuffer(){}
	static StructuredBuffer* CreateStructuredBuffer(UINT ByteWidth, UINT NumElements, const void* Data = nullptr, UINT BindFlags = D3D11_BIND_SHADER_RESOURCE);

	void UpdateBuffer(const void* Data);
	void VSSetStructuredBuffer(UINT StartSlot);
	void PSSetStructuredBuffer(UINT StartSlot);
	void GSSetStructuredBuffer(UINT StartSlot);
};