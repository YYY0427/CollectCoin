#pragma once
#include <unordered_map>
#include <string>

/// <summary>
/// SEの管理を行うシングルトンクラス
/// </summary>
class SoundManager
{
private:
	// 変更したサウンド情報をファイルに書き込む
	struct SoundConfigInfo
	{
		char signature[5];			// "SND_"
		float version;				// 1.0
		unsigned short volumeSE;	// 0～255	
		unsigned short volumeBGM;	// 0～255
	};

	int volumeSE_ = 255;
	int volumeBGM_ = 255;

	std::unordered_map<std::string, int> nameAndHandleTable_;
	int LoadSoundFile(const char* fileName, const char* ext);

	SoundManager();

	// コピーも代入も禁止する
	SoundManager(const SoundManager&) = delete;
	void operator = (const SoundManager&) = delete;

	void LoadSoundConfig();
public:
	~SoundManager();

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
	void Play(const char* name);		// SE
	void PlayMusic(const char* path);	// BGM
	void PlayJingle(const char* path);	// ジングル(プレイヤー死亡、ゲームクリア、ゲームオーバーの音)

	/// <summary>
	/// SEのボリュームを設定する
	/// </summary>
	/// <param name="volume"></param>
	void SetSEVolume(int volume);
	int GetSEVolume() const;

	/// <summary>
	/// BGMのボリュームを設定する
	/// </summary>
	/// <param name="volume"></param>
	void SetBGMVolume(int volume);
	int GetBGMVolume() const;

	/// <summary>
	/// 指定のサウンドが再生中かチェック
	/// </summary>
	/// <param name="name">サウンド名</param>
	/// <returns> true 再生中  false 再生されていない</returns>
	bool CheckSE(const char* name);

	/// <summary>
	/// 指定のサウンドの音量の変更
	/// </summary>
	/// <param name="name">サウンド名、音量</param>
	void ChangeSEVolume(const char* name, int volume);

	/// <summary>
	/// サウンド情報をセーブ
	/// </summary>
	void SaveSoundConfig();
};