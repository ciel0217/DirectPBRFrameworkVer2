#pragma once
#include "../LowLevel/DirectX.h"

class CBuffer {
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;

public:
	CBuffer(ID3D11Buffer* buffer) { m_CBuffer.Attach(buffer); }
	~CBuffer() {};

	static ID3D11Buffer* CreateBuffer(UINT byte_width, UINT bind_flags, const void* data);

	void UpdateBuffer(const void* data);
	void VSSetCBuffer(UINT start_slot);
	void PSSetCBuffer(UINT start_slot);
	void GSSetCBuffer(UINT start_slot);
	void CSSetCBuffer(UINT start_slot);
};