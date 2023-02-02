#pragma once

#include <DirectXMath.h>
#include <Windows.h>
#include <d3dx12.h>

class Model
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X

	struct Material
	{
		std::string name; // �}�e���A����
		DirectX::XMFLOAT3 ambient; // �A���r�G���g�e���x
		DirectX::XMFLOAT3 diffuse; // �f�B�t���[�Y�e���x
		DirectX::XMFLOAT3 specular; // �X�y�L�����[�e���x
		float alpha; // �A���t�@
		std::string textureFilename; // �e�N�X�`���t�@�C����
		// �R���X�g���N�^
		Material()
		{
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

	// ���_�f�[�^�\����
	struct VertexPosNormalUv
	{
		DirectX::XMFLOAT3 pos; // xyz���W
		DirectX::XMFLOAT3 normal; // �@���x�N�g��
		DirectX::XMFLOAT2 uv;  // uv���W
	};

public: // �ÓI�����o�֐�
	// OBJ�t�@�C������3D���f����ǂݍ���
	static Model* LoadFromOBJ();

	// setter
	static void SetDevice(ID3D12Device* device) { Model::device = device; }

private: // �ÓI�����o�ϐ�
	std::vector<VertexPosNormalUv> vertices;

	// �f�o�C�X
	static ID3D12Device* device;

public: // �����o�֐�
	// �}�e���A���ǂݍ���
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);
	// �e�N�X�`���ǂݍ���
	bool LoadTexture(const std::string& directoryPath, const std::string& filename);

private: // ����J�̃����o�֐�
	// OB���t�@�C������3D���f����ǂݍ���(����J)
	void LoadFromOBJInternal();

private: // �����o�ϐ�
	// �}�e���A��
	Material material;
	// ���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices;

	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;

	// �f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texbuff;

	// �f�X�N���v�^�T�C�Y
	UINT descriptorHandleIncrementSize;
};

