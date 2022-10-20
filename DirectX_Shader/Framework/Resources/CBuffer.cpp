#include "CBuffer.h"
#include "../LowLevel/CDxRenderer.h"

ID3D11Buffer * CBuffer::CreateBuffer(UINT byte_width, UINT bind_flags, const void * data)
{
	// 定数バッファ生成
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = byte_width;
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = bind_flags;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	//hBufferDesc.StructureByteStride = sizeof(float);

	ID3D11Buffer* buffer;
	HRESULT hr;
	if (!data) {
		hr = CDxRenderer::GetRenderer()->GetDevice()->CreateBuffer(&hBufferDesc, nullptr, &buffer);
	}
	else {
		D3D11_SUBRESOURCE_DATA resourceData;
		ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));
		resourceData.pSysMem = data;
		hr = CDxRenderer::GetRenderer()->GetDevice()->CreateBuffer(&hBufferDesc, &resourceData, &buffer);
	}

	if (FAILED(hr)) {
		//エラー処理
		int a = 1;
		return nullptr;
	}
	return buffer;
}

void CBuffer::UpdateBuffer(const void * data)
{
	CDxRenderer::GetRenderer()->GetDeviceContext()->UpdateSubresource(m_CBuffer.Get(), 0, nullptr, data, 0, 0);
}

void CBuffer::VSSetCBuffer(UINT start_slot)
{
	CDxRenderer::GetRenderer()->GetDeviceContext()->VSSetConstantBuffers(start_slot, 1, m_CBuffer.GetAddressOf());
}

void CBuffer::PSSetCBuffer(UINT start_slot)
{
	CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetConstantBuffers(start_slot, 1, m_CBuffer.GetAddressOf());
}

void CBuffer::GSSetCBuffer(UINT start_slot)
{
	CDxRenderer::GetRenderer()->GetDeviceContext()->GSSetConstantBuffers(start_slot, 1, m_CBuffer.GetAddressOf());
}
