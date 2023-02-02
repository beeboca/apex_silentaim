#pragma once
#include <iostream>
#include "Utils.cpp"
#include "Offsets.cpp"
#include "Memory.cpp"

struct bone_t
{
    char pad[0xCC];
    float x;
    char pad2[0xC];
    float y;
    char pad3[0xC];
    float z;
};

class Player
{
private:
    int m_entityListIndex;
    float m_lastVisibleTime;
    long m_basePointer = 0;

public:
    Player(int entityListIndex)
    {
        m_entityListIndex = entityListIndex;
    }
    long getUnresolvedBasePointer()
    {
        long unresolvedBasePointer = offsets::REGION + offsets::ENTITY_LIST + ((m_entityListIndex + 1) << 5);
        return unresolvedBasePointer;
    }
    long getBasePointer()
    {
        if (m_basePointer == 0)
            m_basePointer = mem::ReadLong(getUnresolvedBasePointer());
        return m_basePointer;
    }
    int get_m_entityListIndex() {
        return m_entityListIndex;
    }
    void markForPointerResolution()
    {
        m_basePointer = 0;
    }
    void get_bone_pos(int bone_id, my_vector* bonepos) {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::OFFSET_ORIGIN;
        my_vector pos;
        pos.x = mem::ReadFloat(ptrLong);
        pos.y = mem::ReadFloat(ptrLong + 4);
        pos.z = mem::ReadFloat(ptrLong + 8);

        long bone_ptr = mem::ReadLong(basePointer + offsets::OFFSET_BONES);
        int boneloc = (bone_id * 0x30);

        bonepos->x = pos.x + mem::ReadFloat(bone_ptr + boneloc + 0xCC);
        bonepos->y = pos.y + mem::ReadFloat(bone_ptr + boneloc + 0xCC + 4 + 0xC);
        bonepos->z = pos.z + mem::ReadFloat(bone_ptr + boneloc + 0xCC + 4 + 0xC + 4 + 0xC);
    }
    bool isDead()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::LIFE_STATE;
        short result = mem::ReadShort(ptrLong);
        return result > 0;
    }
    bool isKnocked()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::BLEEDOUT_STATE;
        short result = mem::ReadShort(ptrLong);
        return result > 0;
    }
    std::string getName()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::NAME;
        std::string result = mem::ReadString(ptrLong);
        return result;
    }
    void getusername(char* buf, int size) {
    	long nameIndex = mem::ReadLong(getBasePointer() + offsets::OFFSET_NAME_INDEX);
	long nameOffset = mem::ReadLong(offsets::REGION + offsets::OFFSET_NAMELIST + ((nameIndex - 1) << 4));
	mem::readbytearray(nameOffset, buf, size);
    }
    bool isValid()
    {
        return getBasePointer() > 0 && !isDead();
    }
    std::string getInvalidReason()
    {
        if (getBasePointer() == 0)
            return "Unresolved base pointer";
        else if (isDead())
            return "Player is dead";
        else if (getName().empty())
            return "Name is empty";
        else
            return "Player is valid";
    }
    float getLocationX()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::LOCAL_ORIGIN;
        float result = mem::ReadFloat(ptrLong);
        return result;
    }
    float getLocationY()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::LOCAL_ORIGIN + sizeof(float);
        float result = mem::ReadFloat(ptrLong);
        return result;
    }
    float getLocationZ()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::LOCAL_ORIGIN + sizeof(float) + sizeof(float);
        float result = mem::ReadFloat(ptrLong);
        return result;
    }
    int getTeamNumber()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::TEAM_NUMBER;
        int result = mem::ReadInt(ptrLong);
        return result;
    }
    void setGlowColor(float r, float g, float b)
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::OFFSET_GLOW_COLOR;
        mem::WriteFloat(ptrLong, r);
        mem::WriteFloat(ptrLong + 4, g);
        mem::WriteFloat(ptrLong + 8, b);
    }
    void setGlowType(char t1, char t2, char t3, char t4)
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::OFFSET_GLOW_MODE;
        char w_buf[4] = {t1, t2, t3, t4};
        mem::writebytearray(ptrLong, w_buf, 4);
    }
    int getGlowEnable()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::GLOW_ENABLE;
        int result = mem::ReadInt(ptrLong);
        return result;
    }
    void setGlowEnable(int glowEnable)
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::GLOW_ENABLE;
        mem::WriteInt(ptrLong, glowEnable);
    }
    int getGlowThroughWall()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::GLOW_THROUGH_WALL;
        int result = mem::ReadInt(ptrLong);
        return result;
    }
    void setGlowThroughWall(int glowThroughWall)
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::GLOW_THROUGH_WALL;
        mem::WriteInt(ptrLong, glowThroughWall);
    }
    float getLastVisibleTime()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::LAST_VISIBLE_TIME;
        float result = mem::ReadFloat(ptrLong);
        return result;
    }
    bool isVisible()
    {
        const float lastVisibleTime = getLastVisibleTime();
        const bool isVisible = lastVisibleTime > m_lastVisibleTime;
        m_lastVisibleTime = lastVisibleTime;
        return isVisible;
    }
    void print()
    {
        std::cout << "Player[" + std::to_string(m_entityListIndex) + "]:\n";
        std::cout << "\tUnresolvedBasePointer:\t\t\t" + mem::convertPointerToHexString(getUnresolvedBasePointer()) + "\n";
        std::cout << "\tBasePointer:\t\t\t\t" + mem::convertPointerToHexString(getBasePointer()) + "\n";
        std::cout << "\tIsValid:\t\t\t\t" + std::to_string(isValid()) + "\n";
        std::cout << "\tInvalidReason:\t\t\t\t" + getInvalidReason() + "\n";
        if (!isValid())
        {
            std::cout << "\tLocationOriginX:\t\t\t" + utils::convertNumberToString(getLocationX()) + "\n";
            std::cout << "\tLocationOriginY:\t\t\t" + utils::convertNumberToString(getLocationY()) + "\n";
            std::cout << "\tLocationOriginZ:\t\t\t" + utils::convertNumberToString(getLocationZ()) + "\n";
            std::cout << "\tTeamNumber:\t\t\t\t" + utils::convertNumberToString(getTeamNumber()) + "\n";
            std::cout << "\tGlowEnable:\t\t\t\t" + utils::convertNumberToString(getGlowEnable()) + "\n";
            std::cout << "\tGlowThroughWall:\t\t\t" + utils::convertNumberToString(getGlowThroughWall()) + "\n";
        }
    }
};
