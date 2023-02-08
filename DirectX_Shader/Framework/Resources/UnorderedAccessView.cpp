#include "UnorderedAccessView.h"


UnorededAccessView * UnorededAccessView::CreateUnorderedAccessView(UINT ByteWidth, UINT NumElements, D3D11_UAV_DIMENSION Dimension, UINT MiscFlags, const void * Data, UINT BindFlags)
{
	D3D11_BUFFER_DESC desc;
	desc.BindFlags = BindFlags;
	desc.MiscFlags = MiscFlags;
	desc.CPUAccessFlags = 
	return nullptr;
}

UnorededAccessView * UnorededAccessView::CreateUnorderedAccessView(ID3D11Buffer * Buffer, UINT NumElements, D3D11_UAV_DIMENSION Dimension = D3D11_UAV_DIMENSION_BUFFER)
{
	D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
	desc.ViewDimension = Dimension;
	desc.Format = DXGI_FORMAT_R16G16B16A16_UINT;
	desc.Buffer.FirstElement = 0;

	return nullptr;
}
