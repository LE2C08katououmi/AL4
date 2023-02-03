﻿#include "GameScene.h"
#include "Collision.h"
#include <sstream>
#include <iomanip>
#include <cassert>

using namespace DirectX;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete spriteBG;
	delete model_;
	delete model2_;
	delete object3d_;
	delete object3d2_;
	delete object3d3_;
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);

	this->dxCommon = dxCommon;
	this->input = input;

	// デバッグテキスト用テクスチャ読み込み
	Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png");
	// デバッグテキスト初期化
	debugText.Initialize(debugTextTexNumber);

	// テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/background.png");
	Sprite::LoadTexture(2, L"Resources/texture.png");

	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });

	model_ = Model::LoadFromOBJ("triangle_mat");
	model2_ = Model::LoadFromOBJ("triangle");
	model3_ = Model::LoadFromOBJ("ground");

	// 3Dオブジェクト生成
	object3d_ = Object3d::Create();
	object3d2_ = Object3d::Create();
	object3d3_ = Object3d::Create();
	object3d_->SetModel(model_);
	object3d2_->SetModel(model2_);
	object3d3_->SetModel(model3_);
	object3d2_->SetPosition({ -5,0,-5 });
	object3d3_->SetPosition({ 0,0,0 });
	object3d_->Update();

	sphere.center = XMVectorSet(0, 2, 0, 1);
	sphere.radius = 1.0f;

	plane.normal = XMVectorSet(0, 1, 0, 0);
	plane.distance = 0.0f;
}

void GameScene::Update()
{
	//// オブジェクト移動
	//if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
	//{
	//	// 現在の座標を取得
	//	XMFLOAT3 position = object3d_->GetPosition();

	//	// 移動後の座標を計算
	//	if (input->PushKey(DIK_UP)) { position.y += 1.0f; }
	//	else if (input->PushKey(DIK_DOWN)) { position.y -= 1.0f; }
	//	if (input->PushKey(DIK_RIGHT)) { position.x += 1.0f; }
	//	else if (input->PushKey(DIK_LEFT)) { position.x -= 1.0f; }

	//	// 座標の変更を反映
	//	object3d_->SetPosition(position);
	//}

	//// カメラ移動
	//if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A))
	//{
	//	if (input->PushKey(DIK_W)) { Object3d::CameraMoveVector({ 0.0f,-1.0f,0.0f }); }
	//	else if (input->PushKey(DIK_S)) { Object3d::CameraMoveVector({ 0.0f,+1.0f,0.0f }); }
	//	if (input->PushKey(DIK_D)) { Object3d::CameraMoveVector({ -1.0f,0.0f,0.0f }); }
	//	else if (input->PushKey(DIK_A)) { Object3d::CameraMoveVector({ +1.0f,0.0f,0.0f }); }
	//}

	object3d_->Update();
	object3d2_->Update();
	object3d3_->Update();

	{
		XMVECTOR moveY = XMVectorSet(0, 0.01f, 0, 0);
		if (input->PushKey(DIK_W)) { sphere.center += moveY; }
		else if (input->PushKey(DIK_S)) { sphere.center -= moveY; }

		XMVECTOR moveX = XMVectorSet(0.01f, 0, 0, 0);
		if (input->PushKey(DIK_D)) { sphere.center += moveX; }
		else if (input->PushKey(DIK_A)) { sphere.center -= moveX; }
	}

	std::ostringstream spherestr;
	spherestr << "Sphere("
		<< std::fixed << std::setprecision(2)
		<< sphere.center.m128_f32[0] << ","
		<< sphere.center.m128_f32[1] << ","
		<< sphere.center.m128_f32[2] << ")";

	debugText.Print(spherestr.str(), 50, 180, 1.0f);

	XMVECTOR inter;
	bool hit = Collision::CheckSphere2Plane(sphere, plane, &inter);
	if (hit) {
		debugText.Print("HIT", 50, 200, 1.0f);
		spherestr.str("");
		spherestr.clear();
		spherestr << "("
			<< std::fixed << std::setprecision(2)
			<< inter.m128_f32[0] << ","
			<< inter.m128_f32[1] << ","
			<< inter.m128_f32[2] << ")";

		debugText.Print(spherestr.str(), 50, 220, 1.0f);
	}
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	//spriteBG->Draw();
	//sprite1->Draw();
	//sprite2->Draw();

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(cmdList);

	// 3Dオブクジェクトの描画
	object3d_->Draw();
	object3d2_->Draw();
	object3d3_->Draw();

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}
