#include "RandomGenerator.h"

void RandomGenerator::Initialize()
{
    gen_ = std::mt19937(rd_());
}
