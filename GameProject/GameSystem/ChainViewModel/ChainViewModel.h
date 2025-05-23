#pragma once

#include "../Chain/Chain.h"
#include <memory>
#include <array>
#include <string>

// チェインのビューモデル
// ビューやモデル間のデータのやり取りを行う
class ChainViewModel
{
public:
    ChainViewModel() = default;
    ~ChainViewModel() = default;

    void Initialize();
    void Update();
    void ImGui();
    void UpdateChainData(const std::array<std::string, 4>& _chainData);

    Chain* GetChain() const { return chain_.get(); }


private:
    std::unique_ptr<Chain> chain_;
};