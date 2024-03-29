#include "UnorderedAccessView.h"


UnorderedAccessView * UnorderedAccessView::CreateUnorderedAccessView(UINT ByteWidth, UINT NumElements, D3D11_UAV_DIMENSION Dimension, UINT MiscFlags, const void * Data, UINT BindFlags)
{
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = BindFlags;
	desc.MiscFlags = MiscFlags;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.ByteWidth = ByteWidth * NumElements;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.StructureByteStride = ByteWidth;

	HRESULT hr;
	ID3D11Buffer* buffer;

	if (Data)
	{
		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = Data;
		data.SysMemPitch = sizeof(Data);
		hr = CDxRenderer::GetRenderer()->GetDevice()->CreateBuffer(&desc, &data, &buffer);
	}
	else 
	{
		hr = CDxRenderer::GetRenderer()->GetDevice()->CreateBuffer(&desc, nullptr, &buffer);
	}
	
	if (FAILED(hr))
	{
		throw std::runtime_error("バッファの作成に失敗");
		return nullptr;
	}

	D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc = {};
	uav_desc.Format = DXGI_FORMAT_UNKNOWN;
	uav_desc.ViewDimension = Dimension;
	uav_desc.Buffer.FirstElement = 0;
	uav_desc.Buffer.NumElements = NumElements;

	ID3D11UnorderedAccessView* uav;
	hr = CDxRenderer::GetRenderer()->GetDevice()->CreateUnorderedAccessView(buffer, &uav_desc, &uav);

	if (FAILED(hr))
	{
		throw std::runtime_error("UAVの作成に失敗");
		return nullptr;
	}

	return new UnorderedAccessView(uav, buffer, 0);
}

UnorderedAccessView * UnorderedAccessView::CreateUnorderedAccessView(ID3D11Buffer * Buffer, UINT NumElements, D3D11_UAV_DIMENSION Dimension)
{
	HRESULT hr;
	
	D3D11_UNORDERED_ACCESS_VIEW_DESC desc = {};
	desc.ViewDimension = Dimension;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.Buffer.FirstElement = 0;
	desc.Buffer.NumElements = NumElements;
	
	ID3D11UnorderedAccessView* uav;
	hr = CDxRenderer::GetRenderer()->GetDevice()->CreateUnorderedAccessView(Buffer, &desc, &uav);

	if (FAILED(hr))
	{
		throw std::runtime_error("UAVの作成に失敗");
		return nullptr;
	}

	return new UnorderedAccessView(uav, Buffer, 0);
}

void UnorderedAccessView::CSSetUnorderedAccessView(UINT StartSlot)
{
	CDxRenderer::GetRenderer()->GetDeviceContext()->CSSetUnorderedAccessViews(StartSlot, 1, m_UAV.GetAddressOf(), 0);
}
