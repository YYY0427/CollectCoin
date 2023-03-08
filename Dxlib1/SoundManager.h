#pragma once
#include <unordered_map>
#include <string>

/// <summary>
/// SEの管理を行うシングルトンクラス
/// </summary>
class SoundManager
{
private:
	std::unordered_map<std::string, int> nameAndHandleTable_;
	int LoadSoundFile(const char* fileName);
	SoundManager();
	~SoundManager();

	// コピーも代入も禁止する
	SoundManager(const SoundManager&) = delete;
	void operator = (const SoundManager&) = delete;
public:
	/// <summary>
	/// SoundManager使用者はGetInstance()を通した参照からしか利用できない
	/// </summary>
	/// <returns></returns>
	static SoundManager& GetInstance()
	{
		// 唯一の実態
		static SoundManager instance;

		// それの参照を返す
		return instance;
	}

	/// <summary>
	/// 指定のサウンドを鳴らす
	/// </summary>
	/// <param name="name">サウンド名</param>
	void Play(const char* name);
};