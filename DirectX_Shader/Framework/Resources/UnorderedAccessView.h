#pragma once
#include "../LowLevel/CDxRenderer.h"

class UnorderedAccessView
{
private:
	Microsoft::WRL::ComPtr <ID3D11UnorderedAccessView> m_UAV;
	Microsoft::WRL::ComPtr <ID3D11Buffer> m_Buffer;

	UINT m_BufferSize = 0;

	UnorderedAccessView() = delete;
	UnorderedAccessView(ID3D11UnorderedAccessView* uav, ID3D11Buffer* buf, UINT BufferSize) { m_UAV.Attach(uav); m_Buffer.Attach(buf); m_BufferSize = BufferSize; }

public:
	~UnorderedAccessView() {}
	static	UnorderedAccessView* CreateUnorderedAccessView(UINT ByteWidth, UINT NumElements, D3D11_UAV_DIMENSION Dimension = D3D11_UAV_DIMENSION_BUFFER, UINT MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED, const void* Data = nullptr, UINT BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE);
	static UnorderedAccessView* CreateUnorderedAccessView(ID3D11Buffer* Buffer, UINT NumElements, D3D11_UAV_DIMENSION Dimension = D3D11_UAV_DIMENSION_BUFFER);

	template<typename T>
	void CopyBuffer(T* Data, unsigned int NumElements)
	{
		T* data;
		D3D11_MAPPED_SUBRESOURCE sub_res;
		ZeroMemory(&sub_res, sizeof(D3D11_MAPPED_SUBRESOURCE));
		HRESULT hr;
		hr = CDxRenderer::GetRenderer()->GetDeviceContext()->Map(m_Buffer.Get(), 0, D3D11_MAP_READ, 0, &sub_res);
		data = (T*)sub_res.pData;

		CDxRenderer::GetRenderer()->GetDeviceContext()->Unmap(m_Buffer.Get(), 0);
		memcpy(Data, data, sizeof(T) * NumElements);
	}

	void CSSetUnorderedAccessView(UINT StartSlot);

};
