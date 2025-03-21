#include "Ease.h"
#include <cmath>
#include <numbers>

float Ease::In::Linear(const float t) {
    return t;
}

float Ease::In::Quad(const float t) {
    return t * t;
}

float Ease::In::Cubic(const float t) {
    return t * t * t;
}

float Ease::In::Quart(const float t) {
    return t * t * t * t;
}

float Ease::In::Quint(const float t) {
    return t * t * t * t * t;
}

float Ease::In::Sine(const float t) {
    return 1 - cosf(t * std::numbers::pi_v<float> / 2);
}

float Ease::In::Expo(const float t) {
    return t == 0 ? 0 : powf(2, 10 * t - 10);
}

float Ease::In::Circ(const float t) {
    return 1 - sqrtf(1 - t * t);
}

float Ease::In::Back(const float t) {
    return t * t * (2.70158f * t - 1.70158f);
}

float Ease::In::Elastic(const float t) {
    return sinf(13 * std::numbers::pi_v<float> / 2 * t) * powf(2, 10 * t - 10);
}

float Ease::In::Bounce(const float t) {
    return 1 - Out::Bounce(1 - t);
}

float Ease::Out::Linear(const float t) {
    return t;
}

float Ease::Out::Quad(const float t) {
    return 1 - (1 - t) * (1 - t);
}

float Ease::Out::Cubic(const float t) {
    return 1 - powf(1 - t, 3);
}

float Ease::Out::Quart(const float t) {
    return 1 - powf(1 - t, 4);
}

float Ease::Out::Quint(const float t) {
    return 1 - powf(1 - t, 5);
}

float Ease::Out::Sine(const float t) {
    return sinf(t * std::numbers::pi_v<float> / 2);
}

float Ease::Out::Expo(const float t) {
    return t == 1 ? 1 : 1 - powf(2, -10 * t);
}

float Ease::Out::Circ(const float t) {
    return sqrtf(1 - powf(t - 1, 2));
}

float Ease::Out::Back(const float t) {
    return 1 - In::Back(1 - t);
}

float Ease::Out::Elastic(const float t) {
    return 1 - In::Elastic(1 - t);
}

float Ease::Out::Bounce(float t) {
    if (t < 1 / 2.75){
        return 7.5625f * t * t;
    } else if (t < 2 / 2.75){
        return 7.5625f * (t -= 1.5f / 2.75f) * t + 0.75f;
    } else if (t < 2.5 / 2.75){
        return 7.5625f * (t -= 2.25f / 2.75f) * t + 0.9375f;
    } else{
        return 7.5625f * (t -= 2.625f / 2.75f) * t + 0.984375f;
    }
}

