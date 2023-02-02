#pragma once
#include <vector>
#include <cfloat>
#include "LocalPlayer.cpp"
#include "Player.cpp"
#include "Math.cpp"
#include "Level.cpp"
#include "X11Utils.cpp"

class my_util
{
private:
    Level *m_level;
    LocalPlayer *m_localPlayer;
    std::vector<Player *> *m_players;
    X11Utils *m_x11Utils;
    
    long tmp_counter1;
    bool third_person_flag;
    
    long tmp_counter2;
    bool item_glow_flag;
public:
    my_util(
          Level *level,
          LocalPlayer *localPlayer,
          std::vector<Player *> *players,
          X11Utils *x11Utils)
    {
        m_level = level;
        m_localPlayer = localPlayer;
        m_players = players;
        m_x11Utils = x11Utils;
        
        tmp_counter1 = 0;
        third_person_flag = false;
        
        tmp_counter2 = 0;
        item_glow_flag = true;
    }
    
    void glow_enemy()
    {
    	char namebuf[64 + 1] = { 0 };
        memset(namebuf, 0, 65);
        
        bool ittisita_flag = false;
    
        if (!m_level->isPlayable())
            return;
        for (int i = 0; i < m_players->size(); i++)
        {
            Player *player = m_players->at(i);
            if (!player->isValid())
                continue;
            if (player->getTeamNumber() == m_localPlayer->getTeamNumber())
                continue;
            
            
        	if (player->isKnocked()) {
	       	 	player->setGlowEnable(1);
				player->setGlowThroughWall(2);
		 		player->setGlowType(101,101,46,90);
		 	    player->setGlowColor(FLT_MAX, 0.0f, 0.0f);//red
	       	}
            else if (player->isVisible())
            {
                player->setGlowEnable(1);
                player->setGlowThroughWall(2);
                player->setGlowType(101,101,46,90);
                player->setGlowColor(0.0f, 0.0f, FLT_MAX);//BLUE
            }
            else {
                player->setGlowEnable(1);
                player->setGlowThroughWall(2);
                player->setGlowType(101,101,46,90);
                player->setGlowColor(FLT_MAX / 2, 0.0f, FLT_MAX / 2);//PURPLE
            }
        }
    }
    
    void item_glow__pagedown(int counter) {
    	if (m_x11Utils->keyDown(0xff56) == true) {//PAGE_DOWN
             if (tmp_counter2 == 0) {
                  item_glow_flag = !item_glow_flag;
             }
             tmp_counter2++;
        }
        else {
             tmp_counter2 = 0;
        }
    
        if (m_level->isPlayable() == true) {
		//for (size_t i = 0; i < 0x10000; i++) {
		    for (size_t i = 0; i < 15; i++) {
		        int index = (counter * 15) + i;
		        long ent_base = mem::ReadLong(offsets::REGION + offsets::ENTITY_LIST + ((index + 1) << 5));
		        if (ent_base == 0) {
		            continue;
		        }
		        
		        /*long ptrLong = ent_base + offsets::NAME;
		        std::string result = mem::ReadString(ptrLong);
		        if (result.empty()) {
		            continue;
		        }*/
		        std::string class_name = mem::get_client_class_name(ent_base);
		        //printf("class_name = %s\n", class_name.c_str());
		        if (class_name != "CPropSurvival" && class_name != "CWeaponX") {
			    	continue;
				}
		        
		        int itemID = mem::ReadInt(ent_base + 0x1648);
		        switch (itemID) {
			//case 2:     // Mastiff 
                        //case 82:     // EVA 8  
                        //case 87:     // Peacekeeper  
                        //case 92:     // Mozambique  
                        //case 123:     // Shotguns Shells  

                        //case 52:     // Prowler  
                        //case 22:     // Triple Take  
                        case 1:     // Kraber  

                        case 113:     // Bocek Bow  
                        //case 129:     // Arrows  

                        //case 7:     // L-Star  
                        //case 12:     // Havoc  
                        //case 17:     // Devotion  
                        case 57:     // Volt  
                        //case 122:     // Energy Ammo  

                        //case 27:     // Flatline  
                        //case 32:     // Hemlok  
                        //case 72:     // Spitfire  

                        case 127:     // Rampage LMG  
                        //case 128:     // C.A.R SMG  
                        //case 115:     // Repeater  
                        //case 124:     // Heavy Ammo  

                        //case 37:     // Scout  
                        //case 42:     // Alternator  
                        //case 47:     // R-99  
                        case 77:     // R-301  
                        //case 102:     // P2020  
                        case 107:     // RE-45  
                        //case 121:     // Light Ammo  

                        //case 62:     // Longbow  
                        //case 67:     // Charge  
                        //case 97:     // Wingman  
                        //case 108:     // Sentinel  
                        //case 125:     // Sniper Ammo  

                        //case 174:     // Evo Shield 1  
                        //case 175:     // Evo Shield 2  
                        case 176:     // Evo Shield 3  
                        case 177:     // Evo Shield 4  

                        //case 169:     // Shield 1  
                        //case 170:     // Shield 2  
                        case 171:     // Shield 3  
                        case 172:     // Shield 4  

                        //case 165:     // Helmet 1  
                        //case 166:     // Helmet 2  
                        case 167:     // Helmet 3  
                        case 168:     // Helmet 4  

                        //case 183:     // Backpack 1  
                        //case 184:     // Backpack 2  
                        case 185:     // Backpack 3  
                        case 186:     // Backpack 4  

                        //case 179:     // Knockdown Shield 1  
                        //case 180:     // Knockdown Shield 2  
                        case 181:     // Knockdown Shield 3  
                        case 182:     // Knockdown Shield 4  

                        //case 195:     // 1x Digital Threat  
                        //case 196:     // 3x Digital Threat  
                        //case 197:     // 2x-4x Variable AOG  
                        //case 200:     // 4x-10x Digital Threat  

                        case 191:     // 1x HCOG Classic  
                        case 192:     // 2x HCOG Bruiser  
                        //case 193:     // 1x Holo  
                        //case 194:     // 1x-2x Variable Holo  
                        //case 196:     // 3x HCOG Ranger  
                        //case 197:     // 6x Sniper  
                        //case 199:     // 4x-8x Variable Sniper  

                        //case 227:     // Standard Stock 1  
                        //case 228:     // Standard Stock 2  
                        case 229:     // Standard Stock 3  

                        //case 230:     // Sniper Stock 1  
                        //case 231:     // Sniper Stock 2  
                        //case 232:     // Sniper Stock 3  

                        //case 201:     // Barrel Stabilizer 1  
                        //case 202:     // Barrel Stabilizer 2  
                        case 203:     // Barrel Stabilizer 3  
                        case 204:     // Barrel Stabilizer 4  

                        //case 188:     // Thermite Grenade  
                        //case 189:     // Frag Grenade  
                        //case 190:     // Arc Star  

                        //case 212:     // Heavy Magazine 1  
                        //case 213:     // Heavy Magazine 2  
                        case 214:     // Heavy Magazine 3  
                        case 215:     // Heavy Magazine 4  

                        //case 216:     // Energy Magazine 1  
                        //case 217:     // Energy Magazine 2  
                        case 218:     // Energy Magazine 3  
                        case 219:     // Energy Magazine 4  

                        //case 208:     // Extended Light Mag  
                        //case 209:     // Extended Light Mag  
                        case 210:     // Extended Light Mag  
                        case 211:     // Extended Light Mag  

                        //case 159:     // Ultimate Accelerant  
                        //case 160:     // Phoenix Kit  
                        //case 161:     // Med Kit  
                        //case 162:     // Syringe  
                        case 163:     // Shield Battery  
                        //case 164:     // Shield Cell  

                        //case 220:     // Extended Sniper Mag  
                        //case 221:     // Extended Sniper Mag  
                        case 222:     // Extended Sniper Mag  
                        case 223:     // Extended Sniper Mag  

                        //case 224:     // Shotgun Bolt 1  
                        //case 225:     // Shotgun Bolt 2  
                        case 226:     // Shotgun Bolt 3  

                        //case 187:     // Throwing Knife  
                        case 233:     // Turbo Charger  

                        //case 205:     // Laser Sight 1  
                        //case 206:     // Laser Sight 2  
                        case 207:     // Laser Sight 3  

                        case 235:     // Skullpiercer Rifling  
                        case 238:     // Anvil Receiver  
                        case 239:     // Double Tap Trigger  
		                if (item_glow_flag == true) {
		                    mem::WriteInt(ent_base + 0x02c0, 1363184265);
		                }
		                else {
		                    mem::WriteInt(ent_base + 0x02c0, 0);
		                }
		                break;
		        }
		    }
        }
    }
    
    void invisible_my_weapon()
    {
    	if (m_level->isPlayable() == false) {
    		return;
    	}
    
        long MyLocalplayer = mem::ReadLong(offsets::REGION + offsets::LOCAL_PLAYER);

		long ViewModelHandle = mem::ReadLong(MyLocalplayer + offsets::OFFSET_ViewModels) & 0xFFFF; //m_hViewModels
		long ViewModelPtr = mem::ReadLong(offsets::REGION + offsets::ENTITY_LIST + ViewModelHandle * 0x20);
		    
		mem::WriteInt(ViewModelPtr + 0x3C8, 1);
		mem::WriteInt(ViewModelPtr + 0x3D0, 2);
		char w_buf[4] = { 101,101,46,0 };
		mem::writebytearray(ViewModelPtr + 0x2C4, w_buf, 4);
		mem::WriteFloat(ViewModelPtr + 0x1D0, 1.f);
		mem::WriteFloat(ViewModelPtr + 0x1D4, 19.f);
		mem::WriteFloat(ViewModelPtr + 0x1D8, 20.f);
    }
    
    void process_thirdperson__pageup() {
        if (m_x11Utils->keyDown(0xff55) == true) {//PAGE_UP
             if (tmp_counter1 == 0) {
                  third_person_flag = !third_person_flag;
             }
             tmp_counter1++;
        }
        else {
             tmp_counter1 = 0;
        }
        
        if (!m_level->isPlayable()) {
            return;
        }
    
        long MyLocalplayer = mem::ReadLong(offsets::REGION + offsets::LOCAL_PLAYER);
        
		if (third_person_flag == true) {
    		if (m_localPlayer->isZooming() == false/*m_x11Utils->keyDown(0xff55) == true*/) {
    	 	    mem::WriteInt(offsets::REGION + offsets::thirdperson_override + 0x6c, 1);
    		    mem::WriteInt(MyLocalplayer + offsets::m_thirdPersonShoulderView, 1);
    		}
    		else {
    		    mem::WriteInt(offsets::REGION + offsets::thirdperson_override + 0x6c, -1);
    		    mem::WriteInt(MyLocalplayer + offsets::m_thirdPersonShoulderView, -1);
    		}
    	}
    	else {
    		mem::WriteInt(offsets::REGION + offsets::thirdperson_override + 0x6c, -1);
    		mem::WriteInt(MyLocalplayer + offsets::m_thirdPersonShoulderView, -1);
    	}
    }
    
    void speed_down__delete() {
        if (!m_level->isPlayable()) {
            return;
        }
    
        if (m_x11Utils->keyDown(0xffff) == true) {//DELETE
            mem::WriteFloat(offsets::REGION + offsets::OFFSET_TIMESCALE + 0x68, std::numeric_limits<float>::min());
            //mem::WriteFloat(offsets::REGION + offsets::OFFSET_TIMESCALE + 0x68, 0.0f);
        }
        else {
            mem::WriteFloat(offsets::REGION + offsets::OFFSET_TIMESCALE + 0x68, 1.0f);
        }
    }
    
};
