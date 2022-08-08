//.link: https://gist.github.com/mmozeiko/5e727f845db182d468a34d524508ad5f

#define COBJMACROS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d11.h>
#include <dxgi1_2.h>
#include <d3dcompiler.h>

#define COBJMACROS
#define VC_EXTRALEAN
#define FOUNDATION_IMPLEMENTATION
#include "foundation.h"

int main(int Argc, char **Argv) {
  SysInit(Argc, Argv);

  window *Wnd = WndInit(gfx_api_None, 0, 0, -1, -1);

  ///////////
  // Directx.
  ID3D11Device        *Device;
  ID3D11DeviceContext *Context;
  D3D_FEATURE_LEVEL    Levels[] = {D3D_FEATURE_LEVEL_11_0};
  D3D11CreateDevice(
    null, D3D_DRIVER_TYPE_HARDWARE, null, D3D11_CREATE_DEVICE_DEBUG,
    Levels, ARRAYSIZE(Levels), D3D11_SDK_VERSION, &Device, null, &Context
  );

  ID3D11InfoQueue *Info;
  ID3D11Device_QueryInterface(Device, &IID_ID3D11InfoQueue, (void**)&Info);
  ID3D11InfoQueue_SetBreakOnSeverity(Info, D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
  ID3D11InfoQueue_SetBreakOnSeverity(Info, D3D11_MESSAGE_SEVERITY_ERROR, true);
  ID3D11InfoQueue_Release(Info);
  IDXGIFactory2 *Factory;
  CreateDXGIFactory(&IID_IDXGIFactory2, (void*)&Factory);
  IDXGISwapChain1 *SwapChain;
  DXGI_SWAP_CHAIN_DESC1 SwapChainDescriptor = {0};
  SwapChainDescriptor.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
  SwapChainDescriptor.SampleDesc.Count   = 1;
  SwapChainDescriptor.SampleDesc.Quality = 0;
  SwapChainDescriptor.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  SwapChainDescriptor.BufferCount        = 2;
  SwapChainDescriptor.Scaling            = DXGI_SCALING_NONE;
  SwapChainDescriptor.SwapEffect         = DXGI_SWAP_EFFECT_FLIP_DISCARD;
  IDXGIFactory2_CreateSwapChainForHwnd(
    Factory, (IUnknown*)Device, Wnd->SysWnd->WindowHandle,
    &SwapChainDescriptor, null, null, &SwapChain
  );
  IDXGIFactory_Release(Factory);
  IDXGISwapChain1_GetParent(SwapChain, &IID_IDXGIFactory, (void*)&Factory);
  IDXGIFactory_MakeWindowAssociation(Factory, Wnd->SysWnd->WindowHandle, DXGI_MWA_NO_ALT_ENTER);
  IDXGIFactory_Release(Factory);
  struct vertex {
    r32 Pos[2];
    r32 Uv[2];
    r32 Color[3];
  };
  ID3D11Buffer *Vbuffer;
  struct vertex TriangleData[] = {
    {{ -0.00f, +0.75f }, { 25.0f, 50.0f }, { 1, 0, 0 }},
    {{ +0.75f, -0.50f }, {  0.0f,  0.0f }, { 0, 1, 0 }},
    {{ -0.75f, -0.50f }, { 50.0f,  0.0f }, { 0, 0, 1 }},
  };
  D3D11_BUFFER_DESC VertexBuffer = {0};
  VertexBuffer.ByteWidth = sizeof(TriangleData);
  VertexBuffer.Usage = D3D11_USAGE_IMMUTABLE;
  VertexBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  D3D11_SUBRESOURCE_DATA initial = {.pSysMem = TriangleData};
  ID3D11Device_CreateBuffer(Device, &VertexBuffer, &initial, &Vbuffer);
  ID3D11InputLayout *layout;
  ID3D11VertexShader *Vshader;
  ID3D11PixelShader *Pshader;
  D3D11_INPUT_ELEMENT_DESC Desc[] = {
    {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,    0, offsetof(struct vertex, Pos),   D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, offsetof(struct vertex, Uv),    D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(struct vertex, Color), D3D11_INPUT_PER_VERTEX_DATA, 0},
  };
  const c8 ShaderSource[] =
    "struct VS_INPUT                                            \n"
    "{                                                          \n"
    "     float2 pos   : POSITION;                              \n" // these names must match D3D11_INPUT_ELEMENT_DESC array
    "     float2 uv    : TEXCOORD;                              \n"
    "     float3 color : COLOR;                                 \n"
    "};                                                         \n"
    "                                                           \n"
    "struct PS_INPUT                                            \n"
    "{                                                          \n"
    "  float4 pos   : SV_POSITION;                              \n" // these names do not matter, except SV_... ones
    "  float2 uv    : TEXCOORD;                                 \n"
    "  float4 color : COLOR;                                    \n"
    "};                                                         \n"
    "                                                           \n"
    "cbuffer cbuffer0 : register(b0)                            \n" // b0 = constant buffer bound to slot 0
    "{                                                          \n"
    "    float4x4 uTransform;                                   \n"
    "}                                                          \n"
    "                                                           \n"
    "sampler sampler0 : register(s0);                           \n" // s0 = Sampler bound to slot 0
    "                                                           \n"
    "Texture2D<float4> texture0 : register(t0);                 \n" // t0 = shader resource bound to slot 0
    "                                                           \n"
    "PS_INPUT vs(VS_INPUT input)                                \n"
    "{                                                          \n"
    "    PS_INPUT output;                                       \n"
    "    output.pos = mul(uTransform, float4(input.pos, 0, 1)); \n"
    "    output.uv = input.uv;                                  \n"
    "    output.color = float4(input.color, 1);                 \n"
    "    return output;                                         \n"
    "}                                                          \n"
    "                                                           \n"
    "float4 ps(PS_INPUT input) : SV_TARGET                      \n"
    "{                                                          \n"
    "    float4 tex = texture0.Sample(sampler0, input.uv);      \n"
    "    return input.color * tex;                              \n"
    "}                                                          \n";
  ;
  UINT Flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR | D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_WARNINGS_ARE_ERRORS;
  ID3DBlob *CompilationError;
  ID3DBlob *Vblob;
  if (FAILED(D3DCompile(ShaderSource, sizeof(ShaderSource), null, null, null, "vs", "vs_5_0", Flags, 0, &Vblob, &CompilationError))) {
    c8* ErrorMessaage = ID3D10Blob_GetBufferPointer(CompilationError);
    MessageBoxA(null, ErrorMessaage, "error", MB_OK);
    ExitProcess(1);
  }
  ID3DBlob *Pblob;
  if (FAILED(D3DCompile(ShaderSource, sizeof(ShaderSource), null, null, null, "ps", "ps_5_0", Flags, 0, &Pblob, &CompilationError))) {
    c8* ErrorMessaage = ID3D10Blob_GetBufferPointer(CompilationError);
    MessageBoxA(null, ErrorMessaage, "error", MB_OK);
    ExitProcess(1);
  }
  ID3D11Device_CreateVertexShader(Device, ID3D10Blob_GetBufferPointer(Vblob), ID3D10Blob_GetBufferSize(Vblob), null, &Vshader);
  ID3D11Device_CreatePixelShader(Device,  ID3D10Blob_GetBufferPointer(Pblob), ID3D10Blob_GetBufferSize(Pblob), null, &Pshader);
  ID3D11Device_CreateInputLayout(Device, Desc, ARRAYSIZE(Desc), ID3D10Blob_GetBufferPointer(Vblob), ID3D10Blob_GetBufferSize(Vblob), &layout);
  ID3D10Blob_Release(Pblob);
  ID3D10Blob_Release(Vblob);

  ID3D11Buffer *Ubuffer;
  D3D11_BUFFER_DESC BufferDesc = {0};
  BufferDesc.ByteWidth      = 4 * 4 * sizeof(r32);
  BufferDesc.Usage          = D3D11_USAGE_DYNAMIC;
  BufferDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
  BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  ID3D11Device_CreateBuffer(Device, &BufferDesc, null, &Ubuffer);
  ID3D11ShaderResourceView *TextureView;
  u32 Pixels[] = {
    0x80000000, 0xffffffff,
    0xffffffff, 0x80000000,
  };
  UINT TextureWidth  = 2;
  UINT TextureHeight = 2;

  D3D11_TEXTURE2D_DESC TextureDesc = {0};
  TextureDesc.Width              = TextureWidth;
  TextureDesc.Height             = TextureHeight;
  TextureDesc.MipLevels          = 1;
  TextureDesc.ArraySize          = 1;
  TextureDesc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
  TextureDesc.SampleDesc.Count   = 1;
  TextureDesc.SampleDesc.Quality = 0;
  TextureDesc.Usage              = D3D11_USAGE_IMMUTABLE;
  TextureDesc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;

  D3D11_SUBRESOURCE_DATA TextureData = {0};
  TextureData.pSysMem     = Pixels;
  TextureData.SysMemPitch = TextureWidth * sizeof(unsigned int);
  ID3D11Texture2D *Texture;
  ID3D11Device_CreateTexture2D(Device, &TextureDesc, &TextureData, &Texture);
  ID3D11Device_CreateShaderResourceView(Device, (ID3D11Resource*)Texture, null, &TextureView);
  ID3D11Texture2D_Release(Texture);

  ID3D11SamplerState *Sampler;
  D3D11_SAMPLER_DESC SamplerDesc = {0};
  SamplerDesc.Filter   = D3D11_FILTER_MIN_MAG_MIP_POINT;
  SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  ID3D11Device_CreateSamplerState(Device, &SamplerDesc, &Sampler);

  ID3D11BlendState *BlendState;
  D3D11_BLEND_DESC BlendDesc = {0};
  BlendDesc.RenderTarget[0].BlendEnable           = TRUE;
  BlendDesc.RenderTarget[0].SrcBlend              = D3D11_BLEND_SRC_ALPHA;
  BlendDesc.RenderTarget[0].DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;
  BlendDesc.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_ADD;
  BlendDesc.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_SRC_ALPHA;
  BlendDesc.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_INV_SRC_ALPHA;
  BlendDesc.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
  BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
  ID3D11Device_CreateBlendState(Device, &BlendDesc, &BlendState);

  ID3D11RasterizerState *RasterizerState;
  D3D11_RASTERIZER_DESC RasterizerDesc = {0};
  RasterizerDesc.FillMode = D3D11_FILL_SOLID;
  RasterizerDesc.CullMode = D3D11_CULL_NONE;
  ID3D11Device_CreateRasterizerState(Device, &RasterizerDesc, &RasterizerState);

  ID3D11DepthStencilState *DepthState;
  D3D11_DEPTH_STENCIL_DESC DepthStencilDec  = {0};
  DepthStencilDec.DepthEnable      = false;
  DepthStencilDec.DepthWriteMask   = D3D11_DEPTH_WRITE_MASK_ALL;
  DepthStencilDec.DepthFunc        = D3D11_COMPARISON_LESS;
  DepthStencilDec.StencilEnable    = false;
  DepthStencilDec.StencilReadMask  = D3D11_DEFAULT_STENCIL_READ_MASK;
  DepthStencilDec.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
  ID3D11Device_CreateDepthStencilState(Device, &DepthStencilDec, &DepthState);
  ID3D11RenderTargetView *RtView = null;
  ID3D11DepthStencilView *DsView = null;

  ShowWindow(Wnd->SysWnd->WindowHandle, SW_SHOWDEFAULT);

  r32   Angle         = 0;
  DWORD CurrentWidth  = 0;
  DWORD CurrentHeight = 0;

  while (!Wnd->Finish) {
    WndBeginFrame(Wnd);

    RECT ClientRect;
    GetClientRect(Wnd->SysWnd->WindowHandle, &ClientRect);
    i32 WndW = ClientRect.right  - ClientRect.left;
    i32 WndH = ClientRect.bottom - ClientRect.top;

    // Render.
    if (RtView == null || WndW != CurrentWidth || WndH != CurrentHeight) {
      if (RtView) {
        ID3D11DeviceContext_ClearState(Context);
        ID3D11RenderTargetView_Release(RtView);
        ID3D11DepthStencilView_Release(DsView);
        RtView = null;
      }
      if (WndW != 0 && WndH != 0) {
        if (FAILED(IDXGISwapChain1_ResizeBuffers(SwapChain, 0, WndW, WndH, DXGI_FORMAT_UNKNOWN, 0))) {
          MessageBoxA(null, "could to resize swap chain!", "error", MB_OK);
          ExitProcess(1);
        }

        ID3D11Texture2D *Backbuffer;
        IDXGISwapChain1_GetBuffer(SwapChain, 0, &IID_ID3D11Texture2D, (void**)&Backbuffer);
        ID3D11Device_CreateRenderTargetView(Device, (ID3D11Resource*)Backbuffer, null, &RtView);
        ID3D11Texture2D_Release(Backbuffer);

        D3D11_TEXTURE2D_DESC DepthDesc = {0};
        DepthDesc.Width = WndW;
        DepthDesc.Height = WndH;
        DepthDesc.MipLevels = 1;
        DepthDesc.ArraySize = 1;
        DepthDesc.Format = DXGI_FORMAT_D32_FLOAT;
        DepthDesc.SampleDesc.Count = 1;
        DepthDesc.SampleDesc.Quality = 0;
        DepthDesc.Usage = D3D11_USAGE_DEFAULT;
        DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

        ID3D11Texture2D *DepthStencil;
        ID3D11Device_CreateTexture2D(Device, &DepthDesc, null, &DepthStencil);
        ID3D11Device_CreateDepthStencilView(Device, (ID3D11Resource*)DepthStencil, null, &DsView);
        ID3D11Texture2D_Release(DepthStencil);
      }

      CurrentWidth  = WndW;
      CurrentHeight = WndH;
    }
    if (RtView) {
      D3D11_VIEWPORT Viewport = {0};
      Viewport.TopLeftX = 0;
      Viewport.TopLeftY = 0;
      Viewport.Width = (r32)WndW;
      Viewport.Height = (r32)WndH;
      Viewport.MinDepth = 0;
      Viewport.MaxDepth = 1;
      r32 Color[] = {0.392f, 0.584f, 0.929f, 1.f};
      ID3D11DeviceContext_ClearRenderTargetView(Context, RtView, Color);
      ID3D11DeviceContext_ClearDepthStencilView(Context, DsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

      r32 AspectRatio = (r32)WndH / WndW;
      r32 RotationMatrix[16] = {
        cosf(Angle) * AspectRatio, -sinf(Angle), 0, 0,
        sinf(Angle) * AspectRatio,  cosf(Angle), 0, 0,
                                0,            0, 0, 0,
                                0,            0, 0, 1,
      };

      D3D11_MAPPED_SUBRESOURCE Mapped;
      ID3D11DeviceContext_Map(Context, (ID3D11Resource*)Ubuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &Mapped);
      memcpy(Mapped.pData, RotationMatrix, sizeof(RotationMatrix));
      ID3D11DeviceContext_Unmap(Context, (ID3D11Resource*)Ubuffer, 0);
      ID3D11DeviceContext_IASetInputLayout(Context, layout);
      ID3D11DeviceContext_IASetPrimitiveTopology(Context, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
      UINT Stride = sizeof(struct vertex);
      UINT Offset = 0;
      ID3D11DeviceContext_IASetVertexBuffers(Context, 0, 1, &Vbuffer, &Stride, &Offset);
      ID3D11DeviceContext_VSSetConstantBuffers(Context, 0, 1, &Ubuffer);
      ID3D11DeviceContext_VSSetShader(Context, Vshader, null, 0);
      ID3D11DeviceContext_RSSetViewports(Context, 1, &Viewport);
      ID3D11DeviceContext_RSSetState(Context, RasterizerState);
      ID3D11DeviceContext_PSSetSamplers(Context, 0, 1, &Sampler);
      ID3D11DeviceContext_PSSetShaderResources(Context, 0, 1, &TextureView);
      ID3D11DeviceContext_PSSetShader(Context, Pshader, null, 0);
      ID3D11DeviceContext_OMSetBlendState(Context, BlendState, null, ~0U);
      ID3D11DeviceContext_OMSetDepthStencilState(Context, DepthState, 0);
      ID3D11DeviceContext_OMSetRenderTargets(Context, 1, &RtView, DsView);
      ID3D11DeviceContext_Draw(Context, 3, 0);
    }
    b32 EnableVsync = true;
    if (IDXGISwapChain1_Present(SwapChain, EnableVsync? 1 : 0, 0) == DXGI_STATUS_OCCLUDED) {
      if (EnableVsync)
        Sleep(10);
    }
    
    WndEndFrame(Wnd);
  }

  WndEnd(Wnd);
  SysEnd();
}
