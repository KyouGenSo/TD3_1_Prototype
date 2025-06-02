#include "SoundManager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <imgui.h>

void SoundManager::Initialize(const std::string& _filename)
{
    this->LoadFromFile(_filename);
    this->DecodeToSoundData();
    this->LoadAll();
}

void SoundManager::Finalize()
{
    for (auto& hnd : playHandleMap_)
    {
        Audio::GetInstance()->StopWave(hnd.second);
    }
}

void SoundManager::LoadAll()
{
    for (auto& data : soundDataMap_)
    {
        // サウンドデータを読み込む
        std::filesystem::path path = data.second.path;
        if (path.extension() == ".mp3")
        {
            data.second.handle = Audio::GetInstance()->LoadMP3File(data.second.path);
        }
        else if (path.extension() == ".wav")
        {
            data.second.handle = Audio::GetInstance()->LoadWaveFile(data.second.path);
        }
    }
}

void SoundManager::ImGui()
{
    #ifdef _DEBUG

    if (ImGui::Begin("SoundManager"))
    {
        for (auto& data : soundDataMap_)
        {
            if (ImGui::TreeNode(data.first.c_str()))
            {
                ImGui::Text("Path: %s", data.second.path.c_str());
                ImGui::Text("Volume: %f", data.second.volume);
                ImGui::Text("Pitch: %f", data.second.pitch);
                ImGui::Text("Loop: %s", data.second.isLoop ? "true" : "false");
                ImGui::Text("Handle: %u", data.second.handle);

                if (ImGui::Button("Play"))
                {
                    if (playHandleMap_.find(data.second.handle) != playHandleMap_.end())
                    {
                        Audio::GetInstance()->StopWave(playHandleMap_[data.second.handle]);
                        playHandleMap_.erase(data.second.handle);
                    }

                    playHandleMap_[data.second.handle] = this->PlayNoLoop(data.second.handle);
                }
                ImGui::SameLine(); 
                if (ImGui::Button("Stop"))
                {
                    if (playHandleMap_.find(data.second.handle) != playHandleMap_.end())
                    {
                        Audio::GetInstance()->StopWave(playHandleMap_[data.second.handle]);
                        playHandleMap_.erase(data.second.handle);
                    }
                }
                ImGui::TreePop();
            }
        }
    }
    ImGui::End();

    #endif // _DEBUG
}

SoundManager::SoundData SoundManager::SearchSoundData(const std::string& _name) const
{
    // サウンドデータを検索
    auto it = soundDataMap_.find(_name);
    if (it != soundDataMap_.end())
    {
        return it->second;
    }

    // 見つからなかった場合は空を返す
    return {};
}

SoundManager::SoundData SoundManager::SearchSoundData(const uint32_t _handle) const
{
    // サウンドデータを検索
    for (auto& data : soundDataMap_)
    {
        if (data.second.handle == _handle)
        {
            return data.second;
        }
    }
    // 見つからなかった場合は空を返す
    return {};
}

uint32_t SoundManager::Play(const std::string& _name) const
{
    auto data = SearchSoundData(_name);
    return Audio::GetInstance()->Play(
        data.handle, 
        data.isLoop, 
        data.volume
    );
}

uint32_t SoundManager::Play(const uint32_t _handle) const
{
    for (auto& data : soundDataMap_)
    {
        if (data.second.handle == _handle)
        {
            return Audio::GetInstance()->Play(
                data.second.handle, 
                data.second.isLoop, 
                data.second.volume
            );
        }
    }
    return 0;
}

uint32_t SoundManager::PlayNoLoop(const uint32_t _handle) const
{
    for (auto& data : soundDataMap_)
    {
        if (data.second.handle == _handle)
        {
            return Audio::GetInstance()->Play(
                data.second.handle, 
                false,
                data.second.volume
            );
        }
    }
    return 0;
}

void SoundManager::SetVolumeMultiply(float _multiply)
{
    Audio::GetInstance()->SetAllVolume(_multiply);
}

void SoundManager::LoadFromFile(const std::string& _filename)
{
    std::stringstream ss;
    std::ifstream file{};
    std::string fullPath = kJsonDir + _filename;
    file.open(fullPath);

    // ファイルが開けたか確認
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << fullPath << std::endl;
        assert(false && "Failed to open file");
        return;
    }

    // ファイルの内容を文字列ストリームに読み込む
    ss << file.rdbuf();

    file.close();

    // JSONをパース
    auto& j = jsonData_;
    try
    {
        j = json::parse(ss.str());
    }
    catch (const json::parse_error& e)
    {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        assert(false && "Failed to parse JSON");
        return;
    }
}

void SoundManager::DecodeToSoundData()
{
    for (auto& data : jsonData_.items())
    {
        SoundData soundData;
        soundData.path = data.value().at("Path").get<std::string>();
        soundData.volume = data.value().at("Volume").get<float>();
        soundData.pitch = data.value().at("Pitch").get<float>();
        soundData.isLoop = data.value().at("Loop").get<bool>();

        soundDataMap_[data.key()] = soundData;
    }
}

void SoundGroup::Initialize(bool _enableRandom)
{
    pTimer_ = std::make_unique<Timer>();
    enableRandom_ = _enableRandom;
}

void SoundGroup::Finalize() const
{
    Audio::GetInstance()->StopWave(currentPlayHandle_);
}

void SoundGroup::Update()
{
    bool isPlaying = false;
    if (currentPlayHandle_ != 0)
    {
        isPlaying = Audio::GetInstance()->IsPlaying(currentPlayHandle_);
    }
    
    
    // サウンドグループが無効な場合
    if (enabled_ == false)
    {
        if (isPlaying)
        {
            Audio::GetInstance()->StopWave(currentPlayHandle_);
            currentPlayHandle_ = 0;
        }
        return;
    }

    if (isPlaying) return;

    if (soundGroup_.empty()) return;

    if (!pTimer_->GetIsStart()) pTimer_->Start();

    if (pTimer_->GetNow<float>() > interval_)
    {
        if (enableRandom_)
        {
            this->RandomPick();
        }
        else
        {
            this->SequentialPick();
        }

        currentPlayHandle_ = SoundManager::GetInstance()->PlayNoLoop(soundGroup_[currentSoundGroupIndex_]);
        pTimer_->Reset();
    }
}

void SoundGroup::ImGui()
{
    #ifdef _DEBUG

    bool isOpen = ImGui::Begin((soundGroupName_ + " SoundGroup").c_str());

    if (isOpen)
    {
        ImGui::Text("Enabled: %s", enabled_ ? "true" : "false");
        ImGui::Text("SoundCount: %zu", soundGroup_.size());
        ImGui::DragFloat("Interval", &interval_, 0.01f, 0.1f, FLT_MAX, "%.2f");
        if (ImGui::Button("Play"))
        {
            this->Start();
        }
        ImGui::SameLine();
        if (ImGui::Button("Stop"))
        {
            this->Stop();
        }
        ImGui::SeparatorText("Current Playing");
        ImGui::Text("PlayHandle: %u", currentPlayHandle_);
        ImGui::Text("SoundHandle: %u", soundGroup_[currentSoundGroupIndex_]);
    }
    
    ImGui::End();

    #endif // _DEBUG
}

void SoundGroup::AddSound(const uint32_t _handle)
{
    soundGroup_.push_back(_handle);
}

void SoundGroup::RandomPick()
{
    // currentSoundGroupIndex_をランダムに選択
    int randomIndex = 0;
    do
    {
        if (soundGroup_.size() == 1u) break;
        randomIndex = RandomGenerator::Generate<int>(0, static_cast<int>(soundGroup_.size() - 1));
    }
    while (currentSoundGroupIndex_ == randomIndex);
    currentSoundGroupIndex_ = randomIndex;
}

void SoundGroup::SequentialPick()
{
    // currentSoundGroupIndex_を順番に選択
    currentSoundGroupIndex_++;
    if (currentSoundGroupIndex_ >= soundGroup_.size())
    {
        currentSoundGroupIndex_ = 0;
    }
}
