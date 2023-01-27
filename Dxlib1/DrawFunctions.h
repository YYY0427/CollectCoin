#pragma once
#include <tchar.h>

//自分用ライブラリ用名前空間
namespace my 
{
	/// <summary>
	/// 画像をロードする
	/// </summary>
	/// <param name="path">パス文字列</param>
	/// <returns>画像のハンドル</returns>
	int MyLoadGraph(const TCHAR* path);
}

namespace Game
{
	// ウインドウモード設定
	constexpr bool kWindowMode = true;
	// ウインドウ名
	const char* const kTitleText = "パックマン";
	// ウインドウサイズ
	constexpr int kScreenWidth = 640;
	constexpr int kScreenHeight = 720;
	// カラーモード
	constexpr int kColorDepth = 32;		// 32 or 16
};
