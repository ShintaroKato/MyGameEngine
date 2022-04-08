#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#include <d3dx12.h>

#include <vector>
#include <string>
#include <fstream>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")



#include <DirectXTex.h>
#include <wrl.h>

#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")

#include "WinApp.h"
#include "Input.h"
#include "DirectXCommon.h"
#include "Object3d.h"
#include "SpriteCommon.h"
#include "Sprite.h"

using namespace DirectX;
using namespace Microsoft::WRL;

// 頂点データ
struct VertexPosUv
{
	XMFLOAT3 pos; // xyz座標
	XMFLOAT2 uv;  // uv座標
};

// 定数バッファ用データ構造体
struct ConstBufferData {
	XMFLOAT4 color; // 色 (RGBA)
	XMMATRIX mat;   // ３Ｄ変換行列
};



// スプライト単体頂点バッファの転送
void SpriteTransferVertexBuffer(const Sprite& sprite, const SpriteCommon& spriteCommon)
{
	HRESULT result = S_FALSE;

	// 頂点データ
	VertexPosUv vertices[] = {
		//     u     v
		{{}, {0.0f, 1.0f}}, // 左下
		{{}, {0.0f, 0.0f}}, // 左上
		{{}, {1.0f, 1.0f}}, // 右下
		{{}, {1.0f, 0.0f}}, // 右上
	};

	// 左下、左上、右下、右上
	enum { LB, LT, RB, RT };

	float left = (0.0f - sprite.anchorpoint.x) * sprite.size.x;
	float right = (1.0f - sprite.anchorpoint.x) * sprite.size.x;
	float top = (0.0f - sprite.anchorpoint.y) * sprite.size.y;
	float bottom = (1.0f - sprite.anchorpoint.y) * sprite.size.y;

	if (sprite.isFlipX)
	{// 左右入れ替え
		left = -left;
		right = -right;
	}

	if (sprite.isFlipY)
	{// 左右入れ替え
		top = -top;
		bottom = -bottom;
	}

	vertices[LB].pos = { left, bottom,  0.0f }; // 左下
	vertices[LT].pos = { left, top,     0.0f }; // 左上
	vertices[RB].pos = { right, bottom, 0.0f }; // 右下
	vertices[RT].pos = { right, top,    0.0f }; // 右上

	// 指定番号の画像が読み込み済みなら
	if (spriteCommon.texBuff[sprite.texNumber]) {
		// テクスチャ情報取得
		D3D12_RESOURCE_DESC resDesc = spriteCommon.texBuff[sprite.texNumber]->GetDesc();

		float tex_left = sprite.texLeftTop.x / resDesc.Width;
		float tex_right = (sprite.texLeftTop.x + sprite.texSize.x) / resDesc.Width;
		float tex_top = sprite.texLeftTop.y / resDesc.Height;
		float tex_bottom = (sprite.texLeftTop.y + sprite.texSize.y) / resDesc.Height;

		vertices[LB].uv = { tex_left,   tex_bottom   }; // 左下
		vertices[LT].uv = { tex_left,   tex_top      }; // 左上
		vertices[RB].uv = { tex_right,  tex_bottom   }; // 右下
		vertices[RT].uv = { tex_right,  tex_top      }; // 右上
	}

	// 頂点バッファへのデータ転送
	VertexPosUv* vertMap = nullptr;
	result = sprite.vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	sprite.vertBuff->Unmap(0, nullptr);
}

// スプライト生成
Sprite SpriteCreate(ID3D12Device* dev, int window_width, int window_height, UINT texNumber, const SpriteCommon& spriteCommon, XMFLOAT2 anchorpoint = { 0.5f,0.5f }, bool isFlipX = false, bool isFlipY = false)
{
	HRESULT result = S_FALSE;

	// 新しいスプライトを作る
	Sprite sprite{};

	// テクスチャ番号をコピー
	sprite.texNumber = texNumber;

	// アンカーポイントをコピー
	sprite.anchorpoint = anchorpoint;

	// 反転フラグをコピー
	sprite.isFlipX = isFlipX;
	sprite.isFlipY = isFlipY;

	// 頂点データ
	VertexPosUv vertices[4];

	// 指定番号の画像が読み込み済みなら
	if (spriteCommon.texBuff[sprite.texNumber]) {
		// テクスチャ情報取得
		D3D12_RESOURCE_DESC resDesc = spriteCommon.texBuff[sprite.texNumber]->GetDesc();

		// スプライトの大きさを画像の解像度に合わせる
		sprite.size = { (float)resDesc.Width, (float)resDesc.Height };
		sprite.texSize = { (float)resDesc.Width, (float)resDesc.Height };
	}

	// 頂点バッファ生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&sprite.vertBuff));

	// 頂点バッファデータ転送
	SpriteTransferVertexBuffer(sprite, spriteCommon);

	// 頂点バッファビューの作成
	sprite.vbView.BufferLocation = sprite.vertBuff->GetGPUVirtualAddress();
	sprite.vbView.SizeInBytes = sizeof(vertices);
	sprite.vbView.StrideInBytes = sizeof(vertices[0]);

	// 定数バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&sprite.constBuff));

	// 定数バッファにデータ転送
	ConstBufferData* constMap = nullptr;
	result = sprite.constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = XMFLOAT4(1, 1, 1, 1); // 色指定（RGBA）
	constMap->mat = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)window_width, (float)window_height, 0.0f, 0.0f, 1.0f);   // 平行投影行列の合成    
	sprite.constBuff->Unmap(0, nullptr);    

	// 初期化した、新しいスプライトを返す
	return sprite;
}


// デバッグ文字列クラスの定義
class DebugText
{
public: // 定数の宣言    
	static const int maxCharCount = 256;    // 最大文字数
	static const int fontWidth = 9;         // フォント画像内1文字分の横幅
	static const int fontHeight = 18;       // フォント画像内1文字分の縦幅
	static const int fontLineCount = 14;    // フォント画像内1行分の文字数

public: // メンバ関数
	void Initialize(ID3D12Device* dev, int window_width, int window_height, UINT texnumber, const SpriteCommon& spriteCommon);

	void Print(const SpriteCommon& spriteCommon, const std::string& text, float x, float y, float scale = 1.0f);

	void DrawAll(ID3D12GraphicsCommandList* cmdList, const SpriteCommon& spriteCommon, ID3D12Device* dev);

private: // メンバ変数     
	// スプライトデータの配列
	Sprite sprites[maxCharCount];
	// スプライトデータ配列の添え字番号
	int spriteIndex = 0;
};

void DebugText::Initialize(ID3D12Device* dev, int window_width, int window_height, UINT texnumber, const SpriteCommon& spriteCommon)
{
	// 全てのスプライトデータについて
	for (int i = 0; i < _countof(sprites); i++)
	{
		// スプライトを生成する
		sprites[i] = SpriteCreate(dev, window_width, window_height, texnumber, spriteCommon, { 0,0 });
	}
}

void DebugText::Print(const SpriteCommon& spriteCommon, const std::string& text, float x, float y, float scale)
{
	// 全ての文字について
	for (int i = 0; i < text.size(); i++)
	{
		// 最大文字数超過
		if (spriteIndex >= maxCharCount) {
			break;
		}

		// 1文字取り出す(※ASCIIコードでしか成り立たない)
		const unsigned char& character = text[i];

		// ASCIIコードの2段分飛ばした番号を計算
		int fontIndex = character - 32;
		if (character >= 0x7f) {
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		// 座標計算
		sprites[spriteIndex].position = { x + fontWidth * scale * i, y, 0 };
		sprites[spriteIndex].texLeftTop = { (float)fontIndexX * fontWidth, (float)fontIndexY * fontHeight };
		sprites[spriteIndex].texSize = { fontWidth, fontHeight };
		sprites[spriteIndex].size = { fontWidth * scale, fontHeight * scale };
		// 頂点バッファ転送
		SpriteTransferVertexBuffer(sprites[spriteIndex], spriteCommon);
		// 更新
		SpriteUpdate(sprites[spriteIndex], spriteCommon);

		// 文字を１つ進める
		spriteIndex++;
	}
}

// まとめて描画
void DebugText::DrawAll(ID3D12GraphicsCommandList* cmdList, const SpriteCommon& spriteCommon, ID3D12Device* dev)
{
	// 全ての文字のスプライトについて
	for (int i = 0; i < spriteIndex; i++)
	{
		// スプライト描画
		SpriteDraw(sprites[i], cmdList, spriteCommon, dev);
	}

	spriteIndex = 0;
}

// チャンクヘッダ
struct ChunkHeader
{
	char id[4]; // チャンク毎のID
	int32_t size;  // チャンクサイズ
};

// RIFFヘッダチャンク
struct RiffHeader
{
	ChunkHeader chunk;   // "RIFF"
	char type[4]; // "WAVE"
};

// FMTチャンク
struct FormatChunk
{
	ChunkHeader chunk; // "fmt "
	WAVEFORMATEX fmt; // 波形フォーマット
};

class XAudio2VoiceCallback : public IXAudio2VoiceCallback
{
public:
	// ボイス処理パスの開始時
	STDMETHOD_(void, OnVoiceProcessingPassStart) (THIS_ UINT32 BytesRequired) {};
	// ボイス処理パスの終了時
	STDMETHOD_(void, OnVoiceProcessingPassEnd) (THIS) {};
	// バッファストリームの再生が終了した時
	STDMETHOD_(void, OnStreamEnd) (THIS) {};
	// バッファの使用開始時
	STDMETHOD_(void, OnBufferStart) (THIS_ void* pBufferContext) {};
	// バッファの末尾に達した時
	STDMETHOD_(void, OnBufferEnd) (THIS_ void* pBufferContext) {};
	// 再生がループ位置に達した時
	STDMETHOD_(void, OnLoopEnd) (THIS_ void* pBufferContext) {};
	// ボイスの実行エラー時
	STDMETHOD_(void, OnVoiceError) (THIS_ void* pBufferContext, HRESULT Error) {};
};

// 音声データ
struct SoundData
{
	// 波形フォーマット
	WAVEFORMATEX wfex;
	// バッファの先頭アドレス
	BYTE* pBuffer;
	// バッファのサイズ
	unsigned int bufferSize;
};

// 音声読み込み
SoundData SoundLoadWave(const char* filename)
{
	// ファイル入力ストリームのインスタンス
	std::ifstream file;
	// .wavファイルをバイナリモードで開く
	file.open(filename, std::ios_base::binary);
	// ファイルオープン失敗を検出する
	assert(file.is_open());

	// RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	// ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	// タイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	// Formatチャンクの読み込み
	FormatChunk format = {};
	// チャンクヘッダーの確認
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}
	// チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	// Dataチャンクの読み込み
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	// JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK ", 4) == 0) {
		// 読み取り位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		// 再読み込み
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data ", 4) != 0) {
		assert(0);
	}

	// Dataチャンクのデータ部（波形データ）の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	// Waveファイルを閉じる
	file.close();

	// returnする為の音声データ
	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	return soundData;
}

// 音声データ解放
void SoundUnload(SoundData* soundData)
{
	// バッファのメモリを解放
	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

// 音声再生
void SoundPlayWave(IXAudio2* xAudio2, const SoundData& soundData) {

	HRESULT result;

	// 波形フォーマットを元にSourceVoiceの生成
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	// 再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	// 波形データの再生
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

#pragma region WindowsAPI初期化
	WinApp* winApp = nullptr;
	winApp = new WinApp();

	winApp->Initialize();
#pragma endregion WindowsAPI初期化

#pragma region DirectX初期化処理
	// DirectX初期化処理　ここから
#ifdef _DEBUG
	//デバッグレイヤーをオンに
	ComPtr<ID3D12Debug> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
#endif

	DirectXCommon* dxCommon = nullptr;

	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	HRESULT result;
	

	// スプライト
	SpriteCommon* spriteCommon = nullptr;

	spriteCommon = new SpriteCommon();
	spriteCommon->Initialize(dxCommon->GetDev());
	const int SPRITES_NUM = 1;
	Sprite sprites[SPRITES_NUM];

	//ポインター置き場
	Input* input = nullptr;

	input = new Input();
	input->Initialize(winApp);

	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;
	XAudio2VoiceCallback voiceCallback;

	// XAudioエンジンのインスタンスを生成
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result));

	// マスターボイスを生成
	result = xAudio2->CreateMasteringVoice(&masterVoice);
	assert(SUCCEEDED(result));

	// 音声読み込み
	SoundData soundData1 = SoundLoadWave("Resources/Alarm01.wav");

	// 音声再生
	//SoundPlayWave(xAudio2.Get(), soundData1);

	// DirectX初期化処理　ここまで
#pragma endregion DirectX初期化処理

#pragma region 描画初期化処理

	// スプライト共通データ生成
	spriteCommon = SpriteCommonCreate(dxCommon->GetDev(), winApp->window_width, winApp->window_height);
	// スプライト共通テクスチャ読み込み
	SpriteCommonLoadTexture(spriteCommon, 0, L"Resources/texture.png", dxCommon->GetDev());
	SpriteCommonLoadTexture(spriteCommon, 1, L"Resources/house.png", dxCommon->GetDev());

	// スプライトの生成
	for (int i = 0; i < _countof(sprites); i++)
	{
		int texNumber = rand() % 2;
		sprites[i] = SpriteCreate(dxCommon->GetDev(), winApp->window_width, winApp->window_height, texNumber, spriteCommon, { 0,0 }, false, false);

		// スプライトの座標変更
		sprites[i].position.x = 1280/2;
		sprites[i].position.y = 720/2;
		//sprites[i].isInvisible = true;
		//sprites[i].position.x = (float)(rand() % 1280);
		//sprites[i].position.y = (float)(rand() % 720);

		//sprites[i].rotation = (float)(rand() % 360);
		//sprites[i].rotation = 0;

		//sprites[i].size.x = 400.0f;
		//sprites[i].size.y = 100.0f;
		// 頂点バッファに反映
		SpriteTransferVertexBuffer(sprites[i], spriteCommon);
	}

	// デバッグテキスト
	DebugText debugText;

	// デバッグテキスト用のテクスチャ番号を指定
	const int debugTextTexNumber = 2;
	// デバッグテキスト用のテクスチャ読み込み
	SpriteCommonLoadTexture(spriteCommon, debugTextTexNumber, L"Resources/debugfont.png", dxCommon->GetDev());
	// デバッグテキスト初期化
	debugText.Initialize(dxCommon->GetDev(), winApp->window_width, winApp->window_height, debugTextTexNumber, spriteCommon);

	// WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	result = LoadFromWICFile(
		L"Resources/house.png",   //「Resources」フォルダの「texture.png」
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	const Image* img = scratchImg.GetImage(0, 0, 0); // 生データ抽出

	// リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels);

	// テクスチャ用バッファの生成
	ComPtr<ID3D12Resource> texbuff = nullptr;
	result = dxCommon->GetDev()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&texbuff));

	// テクスチャバッファにデータ転送
	result = texbuff->WriteToSubresource(
		0,
		nullptr, // 全領域へコピー
		img->pixels,    // 元データアドレス
		(UINT)img->rowPitch,    // 1ラインサイズ
		(UINT)img->slicePitch   // 1枚サイズ
	);

	// シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	// 3Dオブジェクト静的初期化
	Object3d::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
	// obj.からモデルデータ読み込み
	Model* model = Model::LoadFromOBJ("triangle_mat");
	// 3Dオブジェクト生成
	Object3d* object3d = Object3d::Create();
	// オブジェクトにモデルを紐づける
	object3d->SetModel(model);

#pragma endregion 描画初期化処理

	int counter = 0; // アニメーションの経過時間カウンター

	while (true)  // ゲームループ
	{
#pragma region ウィンドウメッセージ処理
		if (winApp->ProcessMessage())
		{
			break;
		}
#pragma endregion ウィンドウメッセージ処理

#pragma region DirectX毎フレーム処理
		// DirectX毎フレーム処理　ここから

		input->Update();
		object3d->Update();

		//if (input->TriggerKey(DIK_0)) // 数字の0キーが押されていたら
		//{
		//    OutputDebugStringA("Hit 0\n");  // 出力ウィンドウに「Hit 0」と表示
		//}        

		//float clearColor[] = { 0.1f,0.25f, 0.5f,0.0f }; // 青っぽい色

		//if (input->TriggerKey(DIK_SPACE))     // スペースキーが押されていたら
		//{
		//    // 画面クリアカラーの数値を書き換える
		//    clearColor[1] = 1.0f;
		//}

		//// 座標操作
		//if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
		//{
		//    if (input->PushKey(DIK_UP)) { object3ds[0].position.y += 1.0f; }
		//    else if (input->PushKey(DIK_DOWN)) { object3ds[0].position.y -= 1.0f; }
		//    if (input->PushKey(DIK_RIGHT)) { object3ds[0].position.x += 1.0f; }
		//    else if (input->PushKey(DIK_LEFT)) { object3ds[0].position.x -= 1.0f; }
		//}

		//// X座標,Y座標を指定して表示
		//debugText.Print(spriteCommon, "Hello,DirectX!!", 200, 100);
		//// X座標,Y座標,縮尺を指定して表示
		//debugText.Print(spriteCommon, "Nihon Kogakuin", 200, 200, 2.0f);

		//sprite.rotation = 45;
		//sprite.position = {1280/2, 720/2, 0};
		//sprite.color = {0, 0, 1, 1};
		
		// DirectX毎フレーム処理　ここまで
#pragma endregion DirectX毎フレーム処理

#pragma region グラフィックスコマンド

		dxCommon->PreDraw();

		// 3Dオブジェクト描画前処理
		Object3d::PreDraw(dxCommon->GetCmdList());

		object3d->Draw();

		Object3d::PostDraw();

		// ４．描画コマンドここから

		// スプライト共通コマンド
		SpriteCommonBeginDraw(spriteCommon, dxCommon->GetCmdList());
		// スプライト描画
		for (int i = 0; i < _countof(sprites); i++)
		{
			SpriteDraw(sprites[i], dxCommon->GetCmdList(), spriteCommon, dxCommon->GetDev());
		}
		// デバッグテキスト描画
		debugText.DrawAll(dxCommon->GetCmdList(), spriteCommon, dxCommon->GetDev());

		// ４．描画コマンドここまで

		dxCommon->PostDraw();


#pragma endregion グラフィックスコマンド

	}

	delete input;
	delete object3d;
	delete model;

	// XAudio2解放
	xAudio2.Reset();
	// 音声データ解放
	SoundUnload(&soundData1);

#pragma region WindowsAPI後始末
	winApp->Finalize();
	delete winApp;
#pragma endregion WindowsAPI後始末

	return 0;
}