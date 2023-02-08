#pragma once
#include "../LowLevel/CDxRenderer.h"

class UnorededAccessView
{
private:
	Microsoft::WRL::ComPtr <ID3D11UnorderedAccessView> m_UAV;
	Microsoft::WRL::ComPtr <ID3D11Buffer> m_Buffer;

	UINT m_BufferSize = 0;

	UnorededAccessView() = delete;
	UnorededAccessView(ID3D11UnorderedAccessView* uav, ID3D11Buffer* buf, UINT BufferSize) { m_UAV.Attach(uav); m_Buffer.Attach(buf); m_BufferSize = BufferSize; }

public:
	~UnorededAccessView() {}
	static	UnorededAccessView* CreateUnorderedAccessView(UINT ByteWidth, UINT NumElements, D3D11_UAV_DIMENSION Dimension = D3D11_UAV_DIMENSION_BUFFER, UINT MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED, const void* Data = nullptr, UINT BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE);
	static UnorededAccessView* CreateUnorderedAccessView(ID3D11Buffer* Buffer, UINT NumElements, D3D11_UAV_DIMENSION Dimension = D3D11_UAV_DIMENSION_BUFFER);

	void CopyBuffer(void *Data);

	void VSSetUnorderedAccessView(UINT StartSlot);
	void PSSetUnorderedAccessView(UINT StartSlot);

};
