#pragma once

#include <wrl.h>
#include <d3d12.h>


class PipelineSet
{
public:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// パイプラインステートオブジェクト
	ComPtr<ID3D12PipelineState> pipelinestate;
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootsignature;
};
