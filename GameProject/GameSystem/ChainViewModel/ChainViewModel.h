#pragma once

#include "../Chain/Chain.h"
#include <memory>
#include <array>
#include <string>

class ChainViewModel
{
public:
    ChainViewModel() = default;
    ~ChainViewModel() = default;

    void Initialize();
    void UpdateChainData(const std::array<std::string, 4>& _chainData);

    Chain* GetChain() { return chain_.get(); }


private:
    std::unique_ptr<Chain> chain_;
};