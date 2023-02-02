#pragma once
#include <cfloat>
#include <vector>
#include "LocalPlayer.cpp"
#include "Player.cpp"
#include "Math.cpp"
#include "Level.cpp"
#include "math.h"
#include "X11Utils.cpp"

#define AIMBOT_CIRCLE_DISTANCE_2D 300
#define AIMBOT_DISTANCE_3D 200//(meter)

class Aimbot
{
private:
    Level *m_level;
    LocalPlayer *m_localPlayer;
    std::vector<Player *> *m_players;
    X11Utils *m_x11Utils;

    Player *m_lockedOnPlayer = nullptr;

    long tmp_counter;
    bool silentaim_enable_flag;
public:
    Aimbot(Level *level,
           LocalPlayer *localPlayer,
           std::vector<Player *> *players,
           X11Utils *x11Utils)
    {
        m_level = level;
        m_localPlayer = localPlayer;
        m_players = players;
        m_x11Utils = x11Utils;

        tmp_counter = 0;
        silentaim_enable_flag = true;
        set_silentaim_enable_flag(silentaim_enable_flag);
    }
    void set_silentaim_enable_flag(bool flag) {
        //test_rw_addr+32 = silentaim_enable_flag(4byte) 0=disable else=enable
        if (flag == false) {
            //printf("disable silentaim\n");
            mem::WriteInt(offsets::OFFSET_SILENTAIM_TEST_RW_ADDR + 32, 0);
        }
        else {
            //printf("enable silentaim\n");
            mem::WriteInt(offsets::OFFSET_SILENTAIM_TEST_RW_ADDR + 32, 1);
        }
    }
    void watch_silentaim_key() {
        if (m_x11Utils->keyDown(0xff63) == true) {//INSERT
             if (tmp_counter == 0) {
                  silentaim_enable_flag = !silentaim_enable_flag;
                  
                  if (silentaim_enable_flag == true) {
                      printf("\a");//BEEP SOUND
                      fflush(stdout);
                      std::this_thread::sleep_for(std::chrono::milliseconds(200));
                      printf("\a");//BEEP SOUND
                      fflush(stdout);
                  }
                  else {
                      printf("\a");//BEEP SOUND
                      fflush(stdout);
                  }

                  set_silentaim_enable_flag(silentaim_enable_flag);
             }
             tmp_counter++;
        }
        else {
             tmp_counter = 0;
        }
    }
    void update()
    {
        if (!m_level->isPlayable())
        {
            m_lockedOnPlayer = nullptr;
            return;
        }
        if (m_localPlayer->isDead())
        {
            m_lockedOnPlayer = nullptr;
            return;
        }
        if (m_localPlayer->isKnocked())
        {
            m_lockedOnPlayer = nullptr;
            return;
        }
        if (!m_localPlayer->isInAttack())
        {
            m_lockedOnPlayer = nullptr;
            return;
        }

        if (m_level->isTrainingArea())
        {
            int target_index = findClosestEnemyIndex_TrainingArea();
            //printf("target_index = %d\n", target_index);
            if (target_index == -1) {
                return;
            }
            Player target_npc(target_index);

            execute_all(&target_npc);
        }
        else
        {
            m_lockedOnPlayer = findClosestEnemy();
            if (m_lockedOnPlayer == nullptr) {
                return;
            }

            execute_all(m_lockedOnPlayer);
        }
    }

    void execute_all(Player* target) {
        my_vector localHead;
        m_localPlayer->get_camera_pos(&localHead);
        my_vector targetHead;
        target->get_bone_pos(8, &targetHead);// headshot only
        //target->get_bone_pos(3, &targetHead);//bodyshot

        m_localPlayer->PredictPos(target->getBasePointer(), &targetHead);

        my_vector oldAngle;
        m_localPlayer->get_viewangles(&oldAngle);

        my_vector newAngle = CalcAngle(localHead, targetHead);

        my_vector punchangle;
        my_vector breathangle;
        m_localPlayer->get_punchangles(&punchangle);

        newAngle -= punchangle;

        newAngle.Normalize();

        m_localPlayer->set_viewangles(&newAngle);//normal aimbot
        
        //mem::WriteFloat(offsets::OFFSET_SILENTAIM_TEST_RW_ADDR + 8, newAngle.x);
        //mem::WriteFloat(offsets::OFFSET_SILENTAIM_TEST_RW_ADDR + 12, newAngle.y);
        //mem::WriteFloat(offsets::OFFSET_SILENTAIM_TEST_RW_ADDR + 16, 0.0f);
    }
    my_vector CalcAngle(my_vector src, my_vector dst)
    {
        my_vector origin = dst - src;
        float dist = sqrt(origin.x * origin.x + origin.y * origin.y + origin.z * origin.z);
        my_vector angles = { 0.f, 0.f, 0.f };
        angles.x = -asinf(origin.z / dist) * (180 / M_PI);
        angles.y = atan2f(origin.y, origin.x) * (180 / M_PI);
        return angles;
    }
    Player *findClosestEnemy()
    {
        Player *closestPlayer = nullptr;
        float closest2DPlayerDistance = std::numeric_limits<float>::max();
        for (int i = 0; i < m_players->size(); i++)
        {
            Player *player = m_players->at(i);
            if (!player->isValid())
                continue;
            if (player->isKnocked())
                continue;
            if (player->getTeamNumber() == m_localPlayer->getTeamNumber())
                continue;
            if (!player->isVisible())
                continue;
            my_vector lp_head;
            m_localPlayer->get_camera_pos(&lp_head);
            my_vector enem_head;
            player->get_bone_pos(8, &enem_head);
            float dist_lp_enem = lp_head.DistTo(enem_head);
            if (math::distanceToMeters(dist_lp_enem) > AIMBOT_DISTANCE_3D) {
                continue;
            }

            my_vector enem_head_screen;
            if (WorldToScreen(enem_head, enem_head_screen) == false) {
                continue;
            }
            my_vector screenMiddle;
            screenMiddle.x = MONITOR_WIDTH / 2;
            screenMiddle.y = MONITOR_HEIGHT / 2;
            float enem_scr_dist_2d = screenMiddle.Dist2D(enem_head_screen);
            if (enem_scr_dist_2d > AIMBOT_CIRCLE_DISTANCE_2D) {
                continue;
            }
            if (closestPlayer == nullptr)
            {
                closestPlayer = player;
                closest2DPlayerDistance = enem_scr_dist_2d;
            }
            else if (enem_scr_dist_2d < closest2DPlayerDistance)
            {
                closestPlayer = player;
                closest2DPlayerDistance = enem_scr_dist_2d;
            }
        }
        return closestPlayer;
    }

    int findClosestEnemyIndex_TrainingArea()
    {
        std::vector<Player> npc;

        for (size_t i = 0; i < 20000; i++) {
            long ent_base = mem::ReadLong(offsets::REGION + offsets::ENTITY_LIST + ((i + 1) << 5));
            if (ent_base == 0) {
                continue;
            }
            
            /*long ptrLong = ent_base + offsets::NAME;
            std::string result = mem::ReadString(ptrLong);
            if (result.empty()) {
                continue;
            }*/
            std::string class_name = mem::get_client_class_name(ent_base);
            //printf("[%d]class_name = %s\n", i, class_name.c_str());
            if (class_name != "CAI_BaseNPC") {
                continue;
            }

            mem::WriteInt(ent_base + offsets::GLOW_ENABLE, 1);
            mem::WriteInt(ent_base + offsets::GLOW_THROUGH_WALL, 2);
            char w_buf[4] = {101,101,46,90};
            mem::writebytearray(ent_base + offsets::OFFSET_GLOW_MODE, w_buf, 4);
            mem::WriteFloat(ent_base + offsets::OFFSET_GLOW_COLOR, FLT_MAX);
            mem::WriteFloat(ent_base + offsets::OFFSET_GLOW_COLOR + 4, FLT_MAX);
            mem::WriteFloat(ent_base + offsets::OFFSET_GLOW_COLOR + 8, FLT_MAX);

            npc.push_back(Player(i));
        }

        Player *closestPlayer = nullptr;
        float closest2DPlayerDistance = std::numeric_limits<float>::max();
        for (int i = 0; i < npc.size(); i++)
        {
            //printf("i = %d\n", i);
            Player* c_npc = &(npc.at(i));
            //if (!c_npc->isValid())
                //continue;
            //if (c_npc->isDead())
                //continue;
            //if (c_npc->isKnocked())
                //continue;
            //if (c_npc->getTeamNumber() == c_npc->getTeamNumber())
                //continue;
            if (!c_npc->isVisible())
                continue;

            my_vector lp_head;
            m_localPlayer->get_camera_pos(&lp_head);
            my_vector npc_head;
            c_npc->get_bone_pos(8, &npc_head);

            float dist_lp_npc = lp_head.DistTo(npc_head);
            if (math::distanceToMeters(dist_lp_npc) > AIMBOT_DISTANCE_3D) {
                continue;
            }

            my_vector npc_head_screen;
            if (WorldToScreen(npc_head, npc_head_screen) == false) {
                continue;
            }
            my_vector screenMiddle;
            screenMiddle.x = MONITOR_WIDTH / 2;
            screenMiddle.y = MONITOR_HEIGHT / 2;
            float npc_scr_dist_2d = screenMiddle.Dist2D(npc_head_screen);
            if (npc_scr_dist_2d > AIMBOT_CIRCLE_DISTANCE_2D) {
                continue;
            }
            if (closestPlayer == nullptr)
            {
                closestPlayer = c_npc;
                closest2DPlayerDistance = npc_scr_dist_2d;
            }
            else if (npc_scr_dist_2d < closest2DPlayerDistance)
            {
                closestPlayer = c_npc;
                closest2DPlayerDistance = npc_scr_dist_2d;
            }
        }
        return (closestPlayer != nullptr) ? closestPlayer->get_m_entityListIndex() : -1;
    }
};
