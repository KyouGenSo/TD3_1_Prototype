#include "RandomGenerator.h"

thread_local std::mt19937 RandomGenerator::gen_;
thread_local std::random_device RandomGenerator::rd_;

void RandomGenerator::Initialize()
{
    gen_ = std::mt19937(rd_());
}
