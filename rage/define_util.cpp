#pragma once
#include <math.h>
#include "Offsets.cpp"
#include "Memory.cpp"
#include "Main.cpp"

#define MONITOR_WIDTH 2560
#define MONITOR_HEIGHT 1440

struct my_vector {
    float x;
    float y;
    float z;

    my_vector operator-(my_vector ape)
    {
        return { x - ape.x, y - ape.y, z - ape.z };
    }

    my_vector operator+(my_vector ape)
    {
        return { x + ape.x, y + ape.y, z + ape.z };
    }

    my_vector operator*(float ape)
    {
        return { x * ape, y * ape, z * ape };
    }

    my_vector operator/(float ape)
    {
        return { x / ape, y / ape, z / ape };
    }

    my_vector operator/=(float ape)
    {
        x /= ape;
        y /= ape;
        z /= ape;

        return *this;
    }

    my_vector operator+=(my_vector ape)
    {
        return { x += ape.x, y += ape.y, z += ape.z };
    }

    my_vector operator-=(my_vector ape)
    {
        return { x -= ape.x, y -= ape.y, z -= ape.z };
    }

    void Normalize()
    {
        while (x > 89.0f)
            x -= 180.f;

        while (x < -89.0f)
            x += 180.f;

        while (y > 180.f)
            y -= 360.f;

        while (y < -180.f)
            y += 360.f;
    }

    float Length()
    {
        return sqrt((x * x) + (y * y) + (z * z));
    }

    float Length2D()
    {
        return sqrt((x * x) + (y * y));
    }

    float DistTo(my_vector ape)
    {
        return (*this - ape).Length();
    }

    float Dist2D(my_vector ape)
    {
        return (*this - ape).Length2D();
    }
};

bool WorldToScreen(my_vector world, my_vector& screen)
{
    unsigned long render = mem::ReadLong(offsets::REGION + offsets::OFFSET_RENDER);
    unsigned long matrix = mem::ReadLong(render + offsets::OFFSET_MATRIX);
    float m_vMatrix[16];
    for (int i = 0; i < 16; i++) {
        m_vMatrix[i] = mem::ReadFloat(matrix + (i * sizeof(float)));
    }
    float w = m_vMatrix[12] * world.x + m_vMatrix[13] * world.y + m_vMatrix[14] * world.z + m_vMatrix[15];

    if (w < 0.01f) return false;

    screen.x = m_vMatrix[0] * world.x + m_vMatrix[1] * world.y + m_vMatrix[2] * world.z + m_vMatrix[3];
    screen.y = m_vMatrix[4] * world.x + m_vMatrix[5] * world.y + m_vMatrix[6] * world.z + m_vMatrix[7];

    float invw = 1.0f / w;
    screen.x *= invw;
    screen.y *= invw;

    float x = MONITOR_WIDTH / 2;
    float y = MONITOR_HEIGHT / 2;

    x += 0.5 * screen.x * MONITOR_WIDTH + 0.5;
    y -= 0.5 * screen.y * MONITOR_HEIGHT + 0.5;

    screen.x = x;
    screen.y = y;

    return true;
}
