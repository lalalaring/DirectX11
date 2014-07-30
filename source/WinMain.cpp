#include<Windows.h>
#include"Framework\Utility\SafeDelete.h"
#include"Framework\Application\CWindow.h"
#include"Framework\Debug\CDebugLog.h"
#include"Framework\Renderer\CD3D11Device.h"
#include"Framework\Renderer\CSwapChain.h"
#include"Framework\Resource\Shader\CShaderManager.h"
#include"Framework\Resource\Model\CModelLoader.h"
#include"CConstantBuffer.h"

struct cbProjection
:public s_cBStruct{
	XMFLOAT4X4 matProjection;
};

struct cbChangesAtEveryFrame
:public s_cBStruct{
	XMFLOAT4X4 matView;
};

struct cbChangesAtEveryObject
:public s_cBStruct{
	XMFLOAT4X4	matWorld;
};

struct cbChangesAtEveryMaterial
:public s_cBStruct{
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

	CConstantBuffer	testCB( sizeof( cbProjection), 0);
	CConstantBuffer everyFrameCBuffer( sizeof( cbChangesAtEveryFrame), 1);
	CConstantBuffer everyObjectCBuffer( sizeof( cbChangesAtEveryObject), 2);
	CConstantBuffer everyMaterialCBuffer( sizeof( cbChangesAtEveryMaterial), 3);

	CDebugLog::Create();
	window.Create( hInstance, nCmdShow, L"test pmd", WINDOW_TYPE_WINDOW, DISPLAY_MODE_SVGA_800x600);

	CD3D11Device::GetInstance().Create();
	swapChain.Create( &CD3D11Device::GetInstance(), &window);
	
	HRESULT hr;
	ID3D11RenderTargetView*	pRenderTargetView;
	ID3D11DepthStencilView*	pDepthStencilView;
	ID3D11Texture2D* pBackBuffer;

	XMStoreFloat4x4( &test.matProjection, XMMatrixTranspose( XMMatrixPerspectiveFovLH( XM_PIDIV4, (float)window.GetClientWidth()/window.GetClientHeight(), 1.0f, 100.0f)));
	XMStoreFloat4x4( &everyFrame.matView, XMMatrixTranspose( XMMatrixLookAtLH( XMVectorSet( 0,10,-50.0f,0), XMVectorSet( 0,0,0,0), XMVectorSet( 0,1,0,0))));
	XMStoreFloat4x4( &everyObject.matWorld, XMMatrixTranspose( XMMatrixIdentity()));


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
	textureDesc.Format = DXGI_FORMAT_D32_FLOAT;
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

	ID3D11DeviceContext* pImmediate = CD3D11Device::GetInstance().GetImmediateContext();

	CModelLoader loader;
	CModelData modelData;
	loader.LoadPMD( L"resource/rapi/rapi.pmd", &modelData);

	D3D11_INPUT_ELEMENT_DESC inputElements[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		//{ "BLENDINDICES", 0, DXGI_FORMAT_R32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		//{ "BLENDWEIGHT", 0, DXGI_FORMAT_R16_UNORM,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	CShaderManager::GetInstance().CompileVS( L"resource/shader/vs.fx", "vsMain", "vs_4_0");
	CShaderManager::GetInstance().CompilePS( L"resource/shader/vs.fx", "psMain", "ps_4_0");
	hr = CShaderManager::GetInstance().CreateInputLayout( 0, inputElements, _countof(inputElements));

	for( UINT i=0; i<modelData.GetNumMaterial(); ++i ){
		modelData.SetVertexShaderIndex( i, 0);
		modelData.SetPixelShaderIndex( i, 0);
		modelData.SetInputLayoutIndex( i, 0);
	}

	// S---------- ラスタライザ ----------
	ID3D11RasterizerState* rsState;
	D3D11_RASTERIZER_DESC rsDesc;
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_NONE;
	rsDesc.FrontCounterClockwise = FALSE;
	rsDesc.DepthBias = 0;
	rsDesc.DepthBiasClamp = 0;
	rsDesc.SlopeScaledDepthBias = 0;
	rsDesc.DepthClipEnable = TRUE;
	rsDesc.ScissorEnable = FALSE;
	rsDesc.MultisampleEnable = FALSE;
	rsDesc.AntialiasedLineEnable = FALSE;
	CD3D11Device::GetInstance().GetD3dDevice()->CreateRasterizerState( &rsDesc, &rsState);
	// E---------- ラスタライザ ----------

	// S---------- デプスステンシルバッファ ----------
	ID3D11DepthStencilState* dsState;
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dsDesc.StencilEnable = FALSE;
	dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	//dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	//dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	//dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	//dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	CD3D11Device::GetInstance().GetD3dDevice()->CreateDepthStencilState( &dsDesc, &dsState);
	// E---------- デプスステンシルバッファ ----------

	// S----------ブレンドステート ----------
	ID3D11BlendState* bState;
	D3D11_BLEND_DESC blendDesc;
	float blendFactor[4] = { 0,0,0,0};
	ZeroMemory( &blendDesc, sizeof( D3D11_BLEND_DESC));
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = FALSE;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	CD3D11Device::GetInstance().GetD3dDevice()->CreateBlendState( &blendDesc, &bState);
	// E----------ブレンドステート ----------


	// S---------- サンプラステート ----------
	ID3D11SamplerState* pSamplerState;
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 0;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	hr = CD3D11Device::GetInstance().GetD3dDevice()->CreateSamplerState( &samplerDesc, &pSamplerState);

	// E---------- サンプラステート ----------

	testCB.Update( &test);
	everyFrameCBuffer.Update( &everyFrame);
	everyObjectCBuffer.Update( &everyObject);

	pImmediate->RSSetViewports(1, &vp);
	pImmediate->RSSetState( rsState);
	pImmediate->OMSetRenderTargets( 1, &pRenderTargetView, pDepthStencilView);
	//pImmediate->OMSetBlendState( bState, blendFactor, 0);
	pImmediate->OMSetDepthStencilState( dsState, 0);


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
					pImmediate->VSSetConstantBuffers( testCB.GetSrotNum(), 1, testCB.GetConstantBuffer() );
					pImmediate->VSSetConstantBuffers( everyFrameCBuffer.GetSrotNum(), 1, everyFrameCBuffer.GetConstantBuffer() );
					pImmediate->VSSetConstantBuffers( everyObjectCBuffer.GetSrotNum(), 1, everyObjectCBuffer.GetConstantBuffer() );

					pImmediate->IASetInputLayout( CShaderManager::GetInstance().GetInputLayout( modelData.GetInputLayoutIndex( i, 0)));
					pImmediate->VSSetShader( CShaderManager::GetInstance().GetVertexShader( modelData.GetVertexShaderIndex( i, 0)), nullptr, 0);
					pImmediate->PSSetShader( CShaderManager::GetInstance().GetPixelShader( modelData.GetPixelShaderIndex( i, 0)), nullptr, 0);
					pImmediate->PSSetShaderResources( 0, 1, modelData.GetShaderResourceView( i, 0));
					pImmediate->PSSetSamplers( 0, 1, &pSamplerState);
					pImmediate->DrawIndexed( modelData.GetIndexCounts(i), modelData.GetIndexStartLocation(i), 0);
				}
			}
			swapChain.GetSwapChain()->Present( 0, 0);
		}
	}

	return 0;
}