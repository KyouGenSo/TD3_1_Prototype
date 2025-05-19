#include "ReinforcementManager.h"

#include <TextureManager.h>

#include <fstream>
#include <iostream>

void ReinforcementManager::Initialize(const std::string& _filename)
{
    this->LoadFromFile(kReinforcementDir_ + _filename);
    this->LoadImages(kReinforcementDir_ + _filename);
}

void ReinforcementManager::Notify(const std::string& _event)
{
    if (_event == "onHit")
    {
        for (auto& reinforcement : reinforcementList_)
        {
            if (reinforcement->GetEvent() == _event)
            {
                reinforcement->OnHit();
            }
        }
    }
}

void ReinforcementManager::RegisterReinforcement(IReinforcement* _reinforcement)
{
    reinforcementList_.push_back(_reinforcement);
}

void ReinforcementManager::UnregisterReinforcement(IReinforcement* _reinforcement)
{
    auto it = std::remove(reinforcementList_.begin(), reinforcementList_.end(), _reinforcement);
    if (it != reinforcementList_.end())
    {
        reinforcementList_.erase(it, reinforcementList_.end());
    }
}

ReinforcementData ReinforcementManager::GetRandomCard(const std::string& _filename) const
{
    auto& data = reinforcementData_.at(kReinforcementDir_ + _filename);
    int begin = 0;
    int end = static_cast<int>(data.size() - 1);

    int randValue = RandomGenerator::GetInstance()->Generate(begin, end);

    auto it = data.begin();
    for (int i = 0; i < randValue; ++i)
    {
        ++it;
    }

    ReinforcementData result = {};
    result.name = (*it)["name"].get<std::string>();
    result.description = (*it)["description"].get<std::string>();
    result.imagepath = (*it)["image"].get<std::string>();

    return result;
}

void ReinforcementManager::LoadFromFile(const std::string& _filename)
{
    for (auto& data : reinforcementData_)
    {
        if (data.first == _filename)
        {
            // 既に読み込まれている場合は何もしない
            return;
        }
    }


    std::stringstream ss;
    std::ifstream file;
    file.open(_filename);

    // ファイルが開けたか確認
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << _filename << std::endl;
        assert(false && "Failed to open file");
        return;
    }

    // ファイルの内容を文字列ストリームに読み込む
    ss << file.rdbuf();

    file.close();

    // JSONをパース
    auto& j = reinforcementData_[_filename];
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

void ReinforcementManager::LoadImages(const std::string& _filename)
{
    auto tm = TextureManager::GetInstance();

    for (auto& data : reinforcementData_.at(_filename))
    {
        tm->LoadTexture(data["image"].get<std::string>());
    }
}
