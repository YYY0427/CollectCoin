#pragma once

namespace Game
{
	//ウィンドウモード設定
#ifdef _DEBUG
	constexpr bool kWindowMode = true;		// デバックの版場合
#else
	constexpr bool kWindowMode = false;		// リリース版の場合
#endif

	// ゲームウィンドウ名
	const char* const kTitleText = "ＧＡＭＥ";

	//ウィンドウサイズ
	constexpr int kScreenWidth = 1600;
	constexpr int kScreenHeight = 900;

	//カラーモード
	constexpr int kColorDepth = 32;   //32 or 16

};