#pragma once
#include "../LowLevel/CDxRenderer.h"

class StructuredBuffer
{
private:
	Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> m_SRV;
	Microsoft::WRL::ComPtr <ID3D11Buffer> m_StructuredBuffer;

	UINT m_BufferSize = 0;

	StructuredBuffer() = delete;
	StructuredBuffer(ID3D11ShaderResourceView* srv, ID3D11Buffer* buf, UINT BufferSize) { 
		m_SRV.Attach(srv); 
		m_StructuredBuffer.Attach(buf); 
		m_BufferSize = BufferSize; 
	}

public:
	~StructuredBuffer(){}
	static StructuredBuffer* CreateStructuredBuffer(UINT ByteWidth, UINT NumElements, const void* Data = nullptr, UINT BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE);

	void UpdateBuffer(const void* Data, UINT UpdateSize);
	void VSSetStructuredBuffer(UINT StartSlot);
	void PSSetStructuredBuffer(UINT StartSlot);
	void GSSetStructuredBuffer(UINT StartSlot);
	void CSSetStructuredBuffer(UINT StartSlot);

	ID3D11Buffer* GetStructuredBuffer() { return m_StructuredBuffer.Get(); }
};