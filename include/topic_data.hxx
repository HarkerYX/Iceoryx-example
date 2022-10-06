#ifndef TOPIC_DATA_HPP
#define TOPIC_DATA_HPP

#include <cstdint>
#include <stdlib.h>

struct QuaternionObject
{
    QuaternionObject() noexcept
    {
    }
    QuaternionObject(float _w, float _x, float _y, float _z) noexcept
        : w(_w), x(_x), y(_y), z(_z)
    {
    }

    float w = 0.f;
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
};

struct MessageTopic
{
    MessageTopic() noexcept
    {
    }
    MessageTopic(uint32_t _counter) noexcept
        : counter(_counter)
    {
        data.w = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        data.x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        data.y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        data.z = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    }
    QuaternionObject data;
    uint32_t counter = 0;
};

#endif