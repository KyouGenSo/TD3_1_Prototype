#include "StageManager.h"

void StageManager::Initialize()
{
    // ステージ1のデータ
    // ファイルから読み込みたい。
    stageMap_["Stage1"] = 
    {
        .playerTransform = 
        {
            .scale = {1.0f, 1.0f, 1.0f},
            .rotate = {0.0f, 0.0f, 0.0f},
            .translate = {0.0f, 0.0f, -150.0f},
        },
        .castleTransform = 
        {
            .scale = {10.0f, 10.0f, 10.0f},
            .rotate = {0.0f, 0.0f, 0.0f},
            .translate = {0.0f, 0.0f, -180.0f},
        },
        .bossTransform = 
        {
            .scale = {1.0f, 1.0f, 1.0f},
            .rotate = {0.0f, 0.0f, 0.0f},
            .translate = {0.0f, 0.0f, 180.0f},
        },
    };
}
