#pragma once

#include <DirectXMath.h>
#include <Windows.h>
#include <d3dx12.h>

class Model
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス

	struct Material
	{
		std::string name; // マテリアル名
		DirectX::XMFLOAT3 ambient; // アンビエント影響度
		DirectX::XMFLOAT3 diffuse; // ディフューズ影響度
		DirectX::XMFLOAT3 specular; // スペキュラー影響度
		float alpha; // アルファ
		std::string textureFilename; // テクスチャファイル名
		// コンストラクタ
		Material()
		{
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

	// 頂点データ構造体
	struct VertexPosNormalUv
	{
		DirectX::XMFLOAT3 pos; // xyz座標
		DirectX::XMFLOAT3 normal; // 法線ベクトル
		DirectX::XMFLOAT2 uv;  // uv座標
	};

	// 定数バッファ用データ構造体B1
	struct ConstBufferDataB1
	{
		DirectX::XMFLOAT3 ambient; //アンビエント係数
		float pad1;		  //パディング
		DirectX::XMFLOAT3 diffuse; //ディフューズ係数
		float pad2;		  //パディング
		DirectX::XMFLOAT3 specular;//スペキュラー係数
		float alpha;	  //アルファ
	};

public: // 静的メンバ関数
	// OBJファイルから3Dモデルを読み込む
	static Model* LoadFromOBJ();

	// setter
	static void SetDevice(ID3D12Device* device) { Model::device = device; }

private: // 静的メンバ変数
	std::vector<VertexPosNormalUv> vertices;

	// デバイス
	static ID3D12Device* device;

public: // メンバ関数
	// マテリアル読み込み
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);
	// テクスチャ読み込み
	bool LoadTexture(const std::string& directoryPath, const std::string& filename);
	// デスクリプタヒープの初期化
	void InitializeDescriptorHeap();
	// 各種バッファ生成
	void CreateBuffers();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name-"cmdList">描画コマンドリスト</param>
	/// <param name-"rootParamIndexMaterial">マテリアル用ルートパラメータ番号</param>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial);

private: // 非公開のメンバ関数
	// OBｊファイルから3Dモデルを読み込む(非公開)
	void LoadFromOBJInternal();

private: // メンバ変数
	// マテリアル
	Material material;
	// 頂点インデックス配列
	std::vector<unsigned short> indices;

	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1;

	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;

	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff;

	// デスクリプタサイズ
	UINT descriptorHandleIncrementSize;

	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;
};

