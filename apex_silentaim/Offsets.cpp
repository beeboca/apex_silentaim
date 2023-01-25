#pragma once
namespace offsets
{
    // buttons
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    const long IN_ATTACK = 0x076687d8; // [Buttons] -> in_attack
    // core
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    const long REGION = 0x140000000;     // [Mine]          -> Region
    const long LEVEL = 0x144bf10;        // [Miscellaneous] -> LevelName
    const long LOCAL_PLAYER = (0x01edd7e0 + 0x8); // [Miscellaneous] -> LocalPlayer
    const long ENTITY_LIST = 0x1b2c6e8;  // [Miscellaneous] -> cl_entitylist
    const long OFFSET_RENDER = 0x7665f38;
    const long OFFSET_MATRIX = 0x11a210;
    // entity
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    const long LOCAL_ORIGIN = 0x0158; // [DataMap.CBaseViewModel]    -> m_localOrigin
    // player
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    const long OFFSET_ORIGIN = 0x014c; //m_vecAbsOrigin
    const long GLOW_ENABLE = (0x03c0 + 0x8);        // [RecvTable.DT_HighlightSettings] -> m_highlightServerContextID + 0x8
    const long GLOW_THROUGH_WALL = (0x03c0 + 0x10); // [RecvTable.DT_HighlightSettings] -> m_highlightServerContextID + 0x10
    const long TEAM_NUMBER = 0x044c;                // [RecvTable.DT_BaseEntity]        -> m_iTeamNum
    const long NAME = 0x0589;                       // [RecvTable.DT_BaseEntity]        -> m_iName
    const long LIFE_STATE = 0x0798;                 // [RecvTable.DT_Player]            -> m_lifeState
    const long VEC_PUNCH_WEAPON_ANGLE = 0x24a8;     // [DataMap.C_Player]               -> m_currentFrameLocalPlayer.m_vecPunchWeapon_Angle
    const long VIEW_ANGLE = (0x25a4 - 0x14);        // [DataMap.C_Player]               -> m_ammoPoolCapacity - 0x14
    const long OFFSET_CAMERAPOS = 0x1f48;
    const long OFFSET_BREATH_ANGLES = (VIEW_ANGLE - 0x10);
    const long OFFSET_BONES = (0x0e98 + 0x48); //m_nForceBone + 0x48

    const long OFFSET_WEAPON = 0x1a14; //m_latestPrimaryWeapons
    const long OFFSET_BULLET_SPEED = 0x1ef0; //CWeaponX!m_flProjectileSpeed
    const long OFFSET_BULLET_SCALE = 0x1ef8; //CWeaponX!m_flProjectileScale
    const long OFFSET_ZOOM_FOV = (0x16b8 + 0x00b8); //m_playerData + m_curZoomFOV
    const long OFFSET_AMMO = 0x1670; //m_ammoInClip

    const long BLEEDOUT_STATE = 0x2738;             // [RecvTable.DT_Player]            -> m_bleedoutState
    const long ZOOMING = 0x1c51;                    // [RecvTable.DT_Player]            -> m_bZooming
    const long LAST_VISIBLE_TIME = 0x1a70;          // [Miscellaneous]                  -> CPlayer!lastVisibleTime  1st offset from dump

    //#######################################################
    
    const long OFFSET_NAME_INDEX = 0x38;
    const long OFFSET_NAMELIST = 0xbbdc630;
    /*
        std::string Player::GetName(uintptr_t player)
	{
		uintptr_t nameIndex = Driver.rpm<uintptr_t>(player + OFFSET_NAME_INDEX);
		uintptr_t nameOffset = Driver.rpm<uintptr_t>(globals.nameList + ((nameIndex - 1) << 4));
		char buf[64] = { 0 };
		Driver.ReadRaw(globals.pID, nameOffset, (UINT_PTR)&buf, 64);
		return buf;
	}
    */
    
    const long OFFSET_GLOW_DISTANCE = 0x3b4;
    const long OFFSET_GLOW_COLOR = 0x1d0;
    const long OFFSET_GLOW_MODE = 0x2c4;
    
    const long OFFSET_ViewModels = 0x2d58;
    
    const long thirdperson_override = 0x01b11020;
    const long m_thirdPersonShoulderView = 0x36c8;
    
    const long OFFSET_TIMESCALE = 0x014c72c0;//host_timescale


    const long OFFSET_SILENTAIM_TEST_RW_ADDR = 0x14ea21000;

}
