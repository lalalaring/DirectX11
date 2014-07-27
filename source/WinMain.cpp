#include<Windows.h>
#include"Framework\Utility\SafeDelete.h"
#include"Framework\Application\CWindow.h"
#include"Framework\Debug\CDebugLog.h"
#include"Framework\Renderer\CD3D11Device.h"
#include"Framework\Renderer\CSwapChain.h"
#include"Framework\Resource\Shader\CShaderManager.h"
#include"Framework\Resource\Model\CModelLoader.h"
#include"CConstantBuffer.h"

struct cbProjection{
	XMFLOAT4X4 matProjection;
};

struct cbChangesAtEveryFrame
{
	XMFLOAT4X4 matView;
};

struct cbChangesAtEveryObject
{
	XMFLOAT4X4	matWorld;
};

struct cbChangesAtEveryMaterial
{
	XMFLOAT4	diffuse;
	XMFLOAT4	speculer;
	float		power;
};

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
 	CWindow window;
 	CSwapChain swapChain;
	cbProjection test;
	cbChangesAtEveryFrame everyFrame;
	cbChangesAtEveryObject everyObject;
	cbChangesAtEveryMaterial everyMaterial;

	CConstantBuffer<cbProjection>	testCB(0);
	CConstantBuffer<cbChangesAtEveryFrame>  everyFrameCBuffer(1);
	CConstantBuffer<cbChangesAtEveryObject>  everyObjectCBuffer(2);
	CConstantBuffer<cbChangesAtEveryMaterial>  everyMaterialCBuffer(3);

	window.Create( hInstance, nCmdShow, L"test pmd", WINDOW_TYPE_WINDOW, DISPLAY_MODE_SVGA_800x600);
	CDebugLog::Create();

	CD3D11Device::GetInstance().Create();
	swapChain.Create( &CD3D11Device::GetInstance(), &window);
	
	HRESULT hr;
	ID3D11RenderTargetView*	pRenderTargetView;
	ID3D11DepthStencilView*	pDepthStencilView;
	ID3D11Texture2D* pBackBuffer;

	XMStoreFloat4x4( &test.matProjection, XMMatrixPerspectiveFovLH( XM_PIDIV4, (float)window.GetClientWidth()/window.GetClientHeight(), 1.0f, 100.0f));
	XMStoreFloat4x4( &everyFrame.matView, XMMatrixLookAtLH( XMVectorSet( 0,0,-20.0f,1), XMVectorSet( 0,0,0,1), XMVectorSet( 0,1,0,1)));

	testCB.Update( test);
	everyFrameCBuffer.Update( everyFrame);


	// バックバッファのレンダーターゲットビュー作成
	swapChain.GetSwapChain()->GetBuffer( 0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);

	hr = CD3D11Device::GetInstance().GetD3dDevice()->CreateRenderTargetView( pBackBuffer, nullptr, &pRenderTargetView);
	SafeRelease( pBackBuffer);


	D3D11_TEXTURE2D_DESC	textureDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC	dsvDesc;

	textureDesc.Width = window.GetClientWidth();
	textureDesc.Height = window.GetClientHeight();
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	hr = CD3D11Device::GetInstance().GetD3dDevice()->CreateTexture2D( &textureDesc, nullptr, &pBackBuffer);

	ZeroMemory( &dsvDesc, sizeof( D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvDesc.Format = textureDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	dsvDesc.Texture2D.MipSlice = 0;
	hr = CD3D11Device::GetInstance().GetD3dDevice()->CreateDepthStencilView( pBackBuffer, &dsvDesc, &pDepthStencilView);
	SafeRelease( pBackBuffer);

	// ビューポート
	D3D11_VIEWPORT	vp;
	vp.Width    = static_cast<float>(textureDesc.Width);
	vp.Height   = static_cast<float>(textureDesc.Height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	CD3D11Device::GetInstance().GetImmediateContext()->RSSetViewports(1, &vp);
	CD3D11Device::GetInstance().GetImmediateContext()->OMSetRenderTargets( 1, &pRenderTargetView, pDepthStencilView);

	ID3D11DeviceContext* pImmediate = CD3D11Device::GetInstance().GetImmediateContext();

	CModelLoader loader;
	CModelData modelData;
	loader.LoadPMD( L"resource/rapi/rapi.pmd", &modelData);

	D3D11_INPUT_ELEMENT_DESC inputElements[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXCOORD0", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	CShaderManager::GetInstance().CompileVS( L"resource/Shader/vs.fx", "vsMain", "vs_5_0");
	CShaderManager::GetInstance().CompilePS( L"resource/Shader/vs.fx", "psMain", "ps_5_0");
	CShaderManager::GetInstance().CreateInputLayout( 0, inputElements, _countof(inputElements));

	for( int i=0; i<modelData.GetNumMaterial(); ++i ){
		modelData.SetVertexShaderIndex( i, 0);
		modelData.SetPixelShaderIndex( i, 0);
		modelData.SetInputLayoutIndex( i, 0);
	}

	while(1)
	{
		if( window.DispatchedWMQuitMessage() )
			break;

		else
		{
			float clearColor[] = { 0.1f, 0.1f, 0.1f, 0.0f };
			pImmediate->ClearRenderTargetView( pRenderTargetView, clearColor );
			pImmediate->ClearDepthStencilView( pDepthStencilView,
											   D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );

			if( modelData.CanRendering() )
			{
				pImmediate->IASetVertexBuffers( 0, 1, modelData.GetVertexBuffer(), modelData.GetVertexStrides(), modelData.GetVertexOffsets());
				pImmediate->IASetPrimitiveTopology( modelData.GetPrimitiveTopology());
				pImmediate->IASetIndexBuffer( modelData.GetIndexBuffer(), modelData.GetIndexFormat(), 0);

				for( UINT i=0; i<modelData.GetNumMaterial(); ++i ){
					//pImmediate->VSSetConstantBuffers( 3, 1, )
					pImmediate->IASetInputLayout( CShaderManager::GetInstance().GetInputLayout( modelData.GetVertexShaderIndex( i, 0)));
					pImmediate->VSSetShader( CShaderManager::GetInstance().GetVertexShader( modelData.GetVertexShaderIndex( i, 0)), nullptr, 0);
					pImmediate->PSSetShader( CShaderManager::GetInstance().GetPixelShader( modelData.GetPixelShaderIndex( i, 0)), nullptr, 0);
					//pImmediate->PSSetShaderResources();
					//pImmediate->PSSetSamplers();
					pImmediate->DrawIndexed( modelData.GetIndexCounts(i), modelData.GetIndexStartLocation(i), 0);
				}
			}

			swapChain.GetSwapChain()->Present( 0, 0);
		}
	}

	return 0;
}