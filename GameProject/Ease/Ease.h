#pragma once
namespace Ease{
    namespace In {
        float Linear(const float t);
        float Quad(const float t);
        float Cubic(float t);
        float Quart(float t);
        float Quint(float t);
        float Sine(float t);
        float Expo(float t);
        float Circ(float t);
        float Back(float t);
        float Elastic(float t);
        float Bounce(float t);
    }
    namespace Out{
        float Linear(float t);
        float Quad(float t);
        float Cubic(float t);
        float Quart(float t);
        float Quint(float t);
        float Sine(float t);
        float Expo(float t);
        float Circ(float t);
        float Back(float t);
        float Elastic(float t);
        float Bounce(float t);
    }
};

