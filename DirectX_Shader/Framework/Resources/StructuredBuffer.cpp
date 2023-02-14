#include "StructuredBuffer.h"

StructuredBuffer * StructuredBuffer::CreateStructuredBuffer(UINT ByteWidth, UINT NumElements, const void * Data, UINT BindFlags)
{
	D3D11_BUFFER_DESC desc = {};
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = ByteWidth;
	desc.ByteWidth = ByteWidth * NumElements;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = BindFlags;
	desc.CPUAccessFlags = 0;

	ID3D11Buffer* buffer;
	HRESULT hr;

	if (Data)
	{
		D3D11_SUBRESOURCE_DATA resourceData;
		ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));
		resourceData.pSysMem = Data;
		resourceData.SysMemPitch = sizeof(Data);
		hr = CDxRenderer::GetRenderer()->GetDevice()->CreateBuffer(&desc, &resourceData, &buffer);
	}
	else 
	{
		hr = CDxRenderer::GetRenderer()->GetDevice()->CreateBuffer(&desc, nullptr, &buffer);
	}

	if (FAILED(hr))
	{
		throw std::runtime_error("構造化バッファの作成に失敗");
		return nullptr;
	}

	ID3D11ShaderResourceView* srv;

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
	srv_desc.Format = DXGI_FORMAT_UNKNOWN;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srv_desc.Buffer.FirstElement = 0;
	srv_desc.Buffer.NumElements = NumElements;

	hr = CDxRenderer::GetRenderer()->GetDevice()->CreateShaderResourceView(buffer, &srv_desc, &srv);

	if (FAILED(hr))
	{
		throw std::runtime_error("構造化バッファのSRV作成に失敗");
		return nullptr;
	}

	return new StructuredBuffer(srv, buffer, ByteWidth);
}

void StructuredBuffer::UpdateBuffer(const void * Data, UINT UpdateSize)
{
	if (Data == nullptr)return;

	D3D11_BOX box;
	box.left = 0;
	box.top = 0;
	box.front = 0;
	box.right = UpdateSize * m_BufferSize;
	box.bottom = 1;
	box.back = 1;

	CDxRenderer::GetRenderer()->GetDeviceContext()->UpdateSubresource(m_StructuredBuffer.Get(), 0, &box, Data, 0, 0);

}

void StructuredBuffer::VSSetStructuredBuffer(UINT StartSlot)
{
	CDxRenderer::GetRenderer()->GetDeviceContext()->VSSetShaderResources(StartSlot, 1, m_SRV.GetAddressOf());

}

void StructuredBuffer::PSSetStructuredBuffer(UINT StartSlot)
{
	CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShaderResources(StartSlot, 1, m_SRV.GetAddressOf());
}

void StructuredBuffer::GSSetStructuredBuffer(UINT StartSlot)
{
	CDxRenderer::GetRenderer()->GetDeviceContext()->GSSetShaderResources(StartSlot, 1, m_SRV.GetAddressOf());
}

void StructuredBuffer::CSSetStructuredBuffer(UINT StartSlot)
{
	CDxRenderer::GetRenderer()->GetDeviceContext()->CSSetShaderResources(StartSlot, 1, m_SRV.GetAddressOf());
}
