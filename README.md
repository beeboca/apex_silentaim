# apex_silentaim  
  
LINUX ONLY!  
  
Usage:  
  
1:Change the constants in the define_util.cpp file  
#define MONITOR_WIDTH 2560
#define MONITOR_HEIGHT 1440  

2:Run the following command inside the apex_silentaim folder  
make  
sudo ./a.out
  
You can change the range by changing the following constants in Aimbot.cpp  
#define AIMBOT_CIRCLE_DISTANCE_2D 300  
#define AIMBOT_DISTANCE_3D 100  

You can toggle headshot only in Aimbot.cpp line 126 & 127.

Just comment out the top or bottom for whichever you choose

target->get_bone_pos(8, &targetHead);// headshot only

//target->get_bone_pos(3, &targetHead);//bodyshot

  
key:  
Insert -> enable/disable silentaim  
Delete -> speed down  
PageUp -> enable/disable thirdperson mode  
PageDown -> enable/disable item glow  
  

Operation confirmed on "Ubuntu 20.04.5 LTS"  & "Pop!_OS 22.04 LTS"
  
