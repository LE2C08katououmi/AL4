#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <string>
#include <vector>
#include <DirectXMath.h>


using namespace DirectX;

class Model
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // 静的メンバ関数

	// ONBJファイルから3Dモデルを読み込む
	static Model* LoadFromOBJ(const std::string& modelName);

	// setter
	static void SetDevice(ID3D12Device* device) {
		Model::device = device;
	}

public: // サブクラス

	// 頂点データ構造体
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;  // uv座標
	};

	// 定数バッファ用データ構造体B1
	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient;	//	アンビエント係数
		float pad1;			//	パディング
		XMFLOAT3 diffuse;	//	ディフューズ係数
		float pad2;			//	パディング
		XMFLOAT3 specular;	//	スペキュラー係数
		float alpha;		//	アルファ
	};

	// マテリアル
	struct Material
	{
		std::string name;	//	マテリアル
		XMFLOAT3 ambient;	//	アンビエント影響度
		XMFLOAT3 diffuse;	//	ディフューズ影響度
		XMFLOAT3 specular;	//	スペキュラー影響度
		float alpha;	//	アルファ
		std::string textureFilename;	//	テクスチャファイル名
		// コンストラクタ
		Material()
		{
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

public: // メンバ関数

	/// <summary>
	/// マテリアル読み込み
	/// </summary>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <returns>成否</returns>
	void LoadTexture(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>
	void InitializeDescriptorHeap();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	/// <param name="rootParamIndexMaterial">マテリアル用ルートパラメータ番号</param>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial);


	// 各種バッファ作成
	void CreateBuffers();

private: // 静的メンバ変数

	// デバイス
	static ID3D12Device* device;


private: // メンバ変数
	// デスクリプタサイズ
	UINT descriptorHandleIncrementSize;
	// コマンドリスト
	ID3D12GraphicsCommandList* cmdList;
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	ComPtr<ID3D12PipelineState> pipelinestate;
	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// ビュー行列
	XMMATRIX matView;
	// 射影行列
	XMMATRIX matProjection;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView;
	// マテリアル
	Material material;

	// 頂点座インデック配列
	std::vector<unsigned short> indices;

	std::vector<VertexPosNormalUv> vertices;

	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1;

private: // 非公開のメンバ関数

	// OBJファイルから3Dモデルを読み込む(非公開)
	void LoadFromOBJInternal(const std::string& modelName);
};