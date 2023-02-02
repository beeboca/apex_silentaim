#pragma once

#include <fcntl.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <assert.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <chrono>
#include <thread>
#include "define_util.cpp"
#include "Utils.cpp"
#include "Offsets.cpp"
#include "Level.cpp"
#include "LocalPlayer.cpp"
#include "Player.cpp"
#include "Aimbot.cpp"
#include "X11Utils.cpp"
#include "my_util.cpp"

void my_ptrace_write_1byte(pid_t pid, long addr, char val) {
	long prev_val = ptrace(PTRACE_PEEKDATA, pid, addr, 0);
	long write_val = prev_val;
	write_val &= (long)(0xFFFFFFFFFFFFFF00);
	write_val |= (long)(val & 0xFF);
	assert(ptrace(PTRACE_POKEDATA, pid, (void*)addr, (void*)write_val) == 0);
}

void my_ptrace_write_2byte(pid_t pid, long addr, short val) {
	long prev_val = ptrace(PTRACE_PEEKDATA, pid, addr, 0);
	long write_val = prev_val;
	write_val &= (long)(0xFFFFFFFFFFFF0000);
	write_val |= (long)(val & 0xFFFF);
	assert(ptrace(PTRACE_POKEDATA, pid, (void*)addr, (void*)write_val) == 0);
}

void my_ptrace_write_4byte(pid_t pid, long addr, int val) {
	long prev_val = ptrace(PTRACE_PEEKDATA, pid, addr, 0);
	long write_val = prev_val;
	write_val &= (long)(0xFFFFFFFF00000000);
	write_val |= (long)(val & 0xFFFFFFFF);
	assert(ptrace(PTRACE_POKEDATA, pid, (void*)addr, (void*)write_val) == 0);
}

void my_ptrace_write_8byte(pid_t pid, long addr, long val) {
	assert(ptrace(PTRACE_POKEDATA, pid, (void*)addr, (void*)val) == 0);
}

void func_copy(pid_t pid, long apex_addr, char* copy_func, size_t length) {
	for (int i = 0; i < length; i++) {
		my_ptrace_write_1byte(pid, (long)(apex_addr + i), copy_func[i]);
	}
}

struct CUserCmd {
	int command_number; //0x0000
	int tick_count; //0x0004
	float command_time; //0x0008
	my_vector viewangles; //0x000C
	char pad_0x0018[0x14]; //0x0018
	float forwardmove; //0x002C
	float sidemove; //0x0030
	float upmove; //0x0034
	int buttons; //0x0038
	char impulse; //0x003C
    /*
    .
    .
    .
    */
};

__attribute__((naked)) bool WriteUsercmdDeltaToBuffer(long a1, long a2, long a3, long a4, int a5) {
    //__asm__ __volatile__("ret");

    //__asm__ __volatile__("movabs rax,0x14062D760");
    //__asm__ __volatile__("rex.W jmp rax");

/*
0:  48 89 5c 24 08          mov    QWORD PTR [rsp+0x8],rbx
5:  57                      push   rdi
6:  48 81 ec 50 02 00 00    sub    rsp,0x250
d:  4c 8b d9                mov    r11,rcx
10: 4c 63 d2                movsxd r10,edx
13: 48 8d 4c 24 20          lea    rcx,[rsp+0x20]
18: 49 8b f8                mov    rdi,r8
1b: e8 90 59 16 00          call   0x1659b0 @@@@@@@@@@@@@@@@@@@@@@@@@
20: 33 db                   xor    ebx,ebx
22: 41 83 f9 ff             cmp    r9d,0xffffffff
26: 75 07                   jne    label0x2f
28: 4c 8d 44 24 20          lea    r8,[rsp+0x20]
2d: eb 59                   jmp    label0x88
                            label0x2f:
2f: 41 83 fa ff             cmp    r10d,0xffffffff
33: 75 05                   jne    label0x3a
35: 4d 8b c3                mov    r8,r11
38: eb 0a                   jmp    label0x44
                            label0x3a:
3a: 4d 69 c2 40 0e 00 00    imul   r8,r10,0xe40
41: 4d 03 c3                add    r8,r11
                            label0x44:
44: b8 f1 19 76 05          mov    eax,0x57619f1
49: 41 8b c9                mov    ecx,r9d
4c: 41 f7 e9                imul   r9d
4f: c1 fa 04                sar    edx,0x4
52: 8b c2                   mov    eax,edx
54: c1 e8 1f                shr    eax,0x1f
57: 03 d0                   add    edx,eax
59: 69 c2 ee 02 00 00       imul   eax,edx,0x2ee
5f: 2b c8                   sub    ecx,eax
61: 48 63 c1                movsxd rax,ecx
64: 48 69 d0 28 02 00 00    imul   rdx,rax,0x228
6b: 48 8b c3                mov    rax,rbx
6e: 49 03 90 f8 00 00 00    add    rdx,QWORD PTR [r8+0xf8]
75: 4c 8d 44 24 20          lea    r8,[rsp+0x20]
7a: 44 39 0a                cmp    DWORD PTR [rdx],r9d
7d: 48 0f 44 c2             cmove  rax,rdx
81: 48 85 c0                test   rax,rax
84: 4c 0f 45 c0             cmovne r8,rax
                            label0x88:
88: 41 83 fa ff             cmp    r10d,0xffffffff
8c: 74 0a                   je     label0x98
8e: 49 69 d2 40 0e 00 00    imul   rdx,r10,0xe40
95: 4c 03 da                add    r11,rdx
                            label0x98:
98: 44 8b 8c 24 80 02 00    mov    r9d,DWORD PTR [rsp+0x280]
9f: 00
a0: b8 f1 19 76 05          mov    eax,0x57619f1
a5: 41 f7 e9                imul   r9d
a8: 41 8b c9                mov    ecx,r9d
ab: c1 fa 04                sar    edx,0x4
ae: 8b c2                   mov    eax,edx
b0: c1 e8 1f                shr    eax,0x1f
b3: 03 d0                   add    edx,eax
b5: 69 c2 ee 02 00 00       imul   eax,edx,0x2ee
bb: 2b c8                   sub    ecx,eax
bd: 48 63 c1                movsxd rax,ecx
c0: 48 8b cf                mov    rcx,rdi
c3: 48 69 d0 28 02 00 00    imul   rdx,rax,0x228
ca: 49 03 93 f8 00 00 00    add    rdx,QWORD PTR [r11+0xf8]
d1: 44 39 0a                cmp    DWORD PTR [rdx],r9d
d4: 48 0f 44 da             cmove  rbx,rdx
d8: 48 8d 54 24 20          lea    rdx,[rsp+0x20]
dd: 48 85 db                test   rbx,rbx
e0: 48 0f 45 d3             cmovne rdx,rbx
e4: e8 67 5b 16 00          call   0x165c50 @@@@@@@@@@@@@@@@@@@@@@@@@@@@
e9: 80 7f 14 00             cmp    BYTE PTR [rdi+0x14],0x0
ed: 48 8b 9c 24 60 02 00    mov    rbx,QWORD PTR [rsp+0x260]
f4: 00
f5: 0f 94 c0                sete   al
f8: 48 81 c4 50 02 00 00    add    rsp,0x250
ff: 5f                      pop    rdi
100:    c3                  ret 
*/


    __asm__ __volatile__("mov    QWORD PTR [rsp+0x8],rbx");
    __asm__ __volatile__("push   rdi");
    __asm__ __volatile__("sub    rsp,0x250");
    __asm__ __volatile__("mov    r11,rcx");
    __asm__ __volatile__("movsxd r10,edx");
    __asm__ __volatile__("lea    rcx,[rsp+0x20]");
    __asm__ __volatile__("mov    rdi,r8");
    //__asm__ __volatile__("call   0x1659b0 @@@RELATIVE");
    __asm__ __volatile__("movabs rax,0x140793110");
    __asm__ __volatile__("call rax");
    __asm__ __volatile__("xor    ebx,ebx");
    __asm__ __volatile__("cmp    r9d,0xffffffff");
    __asm__ __volatile__("jne    label0x2f");
    __asm__ __volatile__("lea    r8,[rsp+0x20]");
    __asm__ __volatile__("jmp    label0x88");
    __asm__ __volatile__("label0x2f:");
    __asm__ __volatile__("cmp    r10d,0xffffffff");
    __asm__ __volatile__("jne    label0x3a");
    __asm__ __volatile__("mov    r8,r11");
    __asm__ __volatile__("jmp    label0x44");
    __asm__ __volatile__("label0x3a:");
    __asm__ __volatile__("imul   r8,r10,0xe40");
    __asm__ __volatile__("add    r8,r11");
    __asm__ __volatile__("label0x44:");
    __asm__ __volatile__("mov    eax,0x57619f1");
    __asm__ __volatile__("mov    ecx,r9d");
    __asm__ __volatile__("imul   r9d");
    __asm__ __volatile__("sar    edx,0x4");
    __asm__ __volatile__("mov    eax,edx");
    __asm__ __volatile__("shr    eax,0x1f");
    __asm__ __volatile__("add    edx,eax");
    __asm__ __volatile__("imul   eax,edx,0x2ee");
    __asm__ __volatile__("sub    ecx,eax");
    __asm__ __volatile__("movsxd rax,ecx");
    __asm__ __volatile__("imul   rdx,rax,0x228");
    __asm__ __volatile__("mov    rax,rbx");
    __asm__ __volatile__("add    rdx,QWORD PTR [r8+0xf8]");
    __asm__ __volatile__("lea    r8,[rsp+0x20]");
    __asm__ __volatile__("cmp    DWORD PTR [rdx],r9d");
    __asm__ __volatile__("cmove  rax,rdx");
    __asm__ __volatile__("test   rax,rax");
    __asm__ __volatile__("cmovne r8,rax");
    __asm__ __volatile__("label0x88:");
    __asm__ __volatile__("cmp    r10d,0xffffffff");
    __asm__ __volatile__("je     label0x98");
    __asm__ __volatile__("imul   rdx,r10,0xe40");
    __asm__ __volatile__("add    r11,rdx");
    __asm__ __volatile__("label0x98:");
    __asm__ __volatile__("mov    r9d,DWORD PTR [rsp+0x280]");
    __asm__ __volatile__("mov    eax,0x57619f1");
    __asm__ __volatile__("imul   r9d");
    __asm__ __volatile__("mov    ecx,r9d");
    __asm__ __volatile__("sar    edx,0x4");
    __asm__ __volatile__("mov    eax,edx");
    __asm__ __volatile__("shr    eax,0x1f");
    __asm__ __volatile__("add    edx,eax");
    __asm__ __volatile__("imul   eax,edx,0x2ee");
    __asm__ __volatile__("sub    ecx,eax");
    __asm__ __volatile__("movsxd rax,ecx");
    __asm__ __volatile__("mov    rcx,rdi");
    __asm__ __volatile__("imul   rdx,rax,0x228");
    __asm__ __volatile__("add    rdx,QWORD PTR [r11+0xf8]");
    __asm__ __volatile__("cmp    DWORD PTR [rdx],r9d");
    __asm__ __volatile__("cmove  rbx,rdx");
    __asm__ __volatile__("lea    rdx,[rsp+0x20]");
    __asm__ __volatile__("test   rbx,rbx");
    __asm__ __volatile__("cmovne rdx,rbx");
    //__asm__ __volatile__("call   0x165c50 @@@RELATIVE");



__asm__ __volatile__("push rax");
__asm__ __volatile__("push rbx");
__asm__ __volatile__("push rcx");
__asm__ __volatile__("push rdx");
__asm__ __volatile__("push Rsi");
__asm__ __volatile__("push Rdi");
__asm__ __volatile__("push Rbp");
__asm__ __volatile__("push Rsp");
__asm__ __volatile__("push r8");
__asm__ __volatile__("push r9");
__asm__ __volatile__("push r10");
__asm__ __volatile__("push r11");
__asm__ __volatile__("push r12");
__asm__ __volatile__("push r13");
__asm__ __volatile__("push r14");
__asm__ __volatile__("push r15");

/*
################################################################################
test_rw_addr = 0x14ea21000

test_rw_addr = workspace_1(8byte)
test_rw_addr+8 = silentaim_x(float)
test_rw_addr+12 = silentaim_y(float)
test_rw_addr+16 = silentaim_z(float)
test_rw_addr+20 = prevangle_x(float)
test_rw_addr+24 = prevangle_y(float)
test_rw_addr+28 = prevangle_z(float)
test_rw_addr+32 = silentaim_enable_flag(4byte) 0=disable else=enable
test_rw_addr+36 = safe_silentaim_enable_flag(4byte) 0=disable else=enable
################################################################################
*/
__asm__ __volatile__("mov    rbx,0x14ea21000");

//*(test_rw_addr+36) = *(test_rw_addr+32)
__asm__ __volatile__("mov    r15d, DWORD PTR [rbx+32]");
__asm__ __volatile__("mov    DWORD PTR [rbx+36], r15d");

//if (safe_silentaim_enable_flag == 0) goto end_1;
__asm__ __volatile__("cmp    DWORD PTR [rbx+36],0x0");
__asm__ __volatile__("je     end_1");

__asm__ __volatile__("mov    QWORD PTR [rbx],rdx");//*test_rw_addr = cmd_to (CUserCmd*)

__asm__ __volatile__("mov    eax, DWORD PTR [rdx+0xc]");
__asm__ __volatile__("mov    DWORD PTR [rbx+20],eax");//*(test_rw_addr+20) = preview angle x
__asm__ __volatile__("mov    eax, DWORD PTR [rdx+0xc+4]");
__asm__ __volatile__("mov    DWORD PTR [rbx+24],eax");//*(test_rw_addr+24) = preview angle y
__asm__ __volatile__("mov    eax, DWORD PTR [rdx+0xc+8]");
__asm__ __volatile__("mov    DWORD PTR [rbx+28],eax");//*(test_rw_addr+28) = preview angle z

__asm__ __volatile__("test   BYTE PTR [rdx+0x38],0x1");
__asm__ __volatile__("je     no_attack");

__asm__ __volatile__("mov    eax, DWORD PTR [rbx+8]");//eax = silentaim_x
__asm__ __volatile__("mov    DWORD PTR [rdx+0xc],eax");
__asm__ __volatile__("mov    eax, DWORD PTR [rbx+12]");//eax = silentaim_y
__asm__ __volatile__("mov    DWORD PTR [rdx+0xc+4],eax");
__asm__ __volatile__("mov    eax, DWORD PTR [rbx+16]");//eax = silentaim_z
__asm__ __volatile__("mov    DWORD PTR [rdx+0xc+8],eax");

//__asm__ __volatile__("mov    DWORD PTR [rdx+0xc],0x00000000");
//__asm__ __volatile__("mov    DWORD PTR [rdx+0xc+4],0x00000000");
//__asm__ __volatile__("mov    DWORD PTR [rdx+0xc+8],0x00000000");

__asm__ __volatile__("no_attack:");
__asm__ __volatile__("end_1:");

__asm__ __volatile__("pop r15");
__asm__ __volatile__("pop r14");
__asm__ __volatile__("pop r13");
__asm__ __volatile__("pop r12");
__asm__ __volatile__("pop r11");
__asm__ __volatile__("pop r10");
__asm__ __volatile__("pop r9");
__asm__ __volatile__("pop r8");
__asm__ __volatile__("pop Rsp");
__asm__ __volatile__("pop Rbp");
__asm__ __volatile__("pop Rdi");
__asm__ __volatile__("pop Rsi");
__asm__ __volatile__("pop rdx");
__asm__ __volatile__("pop rcx");
__asm__ __volatile__("pop rbx");
__asm__ __volatile__("pop rax");

    
    //call WriteUsercmd
    __asm__ __volatile__("movabs rax,0x1407933B0");//rcx=buffer, rdx=to, r8=from
    __asm__ __volatile__("call rax");



__asm__ __volatile__("push rax");
__asm__ __volatile__("push rbx");
__asm__ __volatile__("push rcx");
__asm__ __volatile__("push rdx");
__asm__ __volatile__("push Rsi");
__asm__ __volatile__("push Rdi");
__asm__ __volatile__("push Rbp");
__asm__ __volatile__("push Rsp");
__asm__ __volatile__("push r8");
__asm__ __volatile__("push r9");
__asm__ __volatile__("push r10");
__asm__ __volatile__("push r11");
__asm__ __volatile__("push r12");
__asm__ __volatile__("push r13");
__asm__ __volatile__("push r14");
__asm__ __volatile__("push r15");

/*
################################################################################
test_rw_addr = 0x14ea21000

test_rw_addr = workspace_1(8byte)
test_rw_addr+8 = silentaim_x(float)
test_rw_addr+12 = silentaim_y(float)
test_rw_addr+16 = silentaim_z(float)
test_rw_addr+20 = prevangle_x(float)
test_rw_addr+24 = prevangle_y(float)
test_rw_addr+28 = prevangle_z(float)
test_rw_addr+32 = silentaim_enable_flag(4byte) 0=disable else=enable
test_rw_addr+36 = safe_silentaim_enable_flag(4byte) 0=disable else=enable
################################################################################
*/
__asm__ __volatile__("mov    rbx,0x14ea21000");

//if (safe_silentaim_enable_flag == 0) goto end_2;
__asm__ __volatile__("cmp    DWORD PTR [rbx+36],0x0");
__asm__ __volatile__("je     end_2");

__asm__ __volatile__("mov    rdx, QWORD PTR [rbx]");
__asm__ __volatile__("mov    eax, DWORD PTR [rbx+20]");
__asm__ __volatile__("mov    DWORD PTR [rdx+0xc],eax");
__asm__ __volatile__("mov    eax, DWORD PTR [rbx+24]");
__asm__ __volatile__("mov    DWORD PTR [rdx+0xc+4],eax");
__asm__ __volatile__("mov    eax, DWORD PTR [rbx+28]");
__asm__ __volatile__("mov    DWORD PTR [rdx+0xc+8],eax");

__asm__ __volatile__("end_2:");

__asm__ __volatile__("pop r15");
__asm__ __volatile__("pop r14");
__asm__ __volatile__("pop r13");
__asm__ __volatile__("pop r12");
__asm__ __volatile__("pop r11");
__asm__ __volatile__("pop r10");
__asm__ __volatile__("pop r9");
__asm__ __volatile__("pop r8");
__asm__ __volatile__("pop Rsp");
__asm__ __volatile__("pop Rbp");
__asm__ __volatile__("pop Rdi");
__asm__ __volatile__("pop Rsi");
__asm__ __volatile__("pop rdx");
__asm__ __volatile__("pop rcx");
__asm__ __volatile__("pop rbx");
__asm__ __volatile__("pop rax");



    __asm__ __volatile__("cmp    BYTE PTR [rdi+0x14],0x0");
    __asm__ __volatile__("mov    rbx,QWORD PTR [rsp+0x260]");
    __asm__ __volatile__("sete   al");
    __asm__ __volatile__("add    rsp,0x250");
    __asm__ __volatile__("pop    rdi");
    __asm__ __volatile__("ret ");
}

void print_hex(char* byte_buf, size_t buf_size) {
    for (int i = 0; i < buf_size; i++) {
        if ((i % 0x10) == 0) {
            printf("\n");
        }
    	printf("%02hhx ", byte_buf[i]);
    }
    
    printf("\n");
}

int main(int argc, char *argv[])
{
    if (getuid())
    {
        printf("MUST RUN AS ROOT!\n");
        return -1;
    }
    if (mem::GetPID() == 0)
    {
        printf("GAME NOT FOUND. EXITING!\n");
        return -1;
    }
    
    
    /*
	b1234567890/.local/share/Steam/steamapps/common/Apex Legends/r5apex.exe
	140001000-140fa1000 r-xp 00000000 00:00 0
	140fa1000-14133b000 r--p 00000000 00:00 0
	14133b000-14ea22000 rw-p 00000000 00:00 0
	14ea22000-14eb21000 r--p 00000000 00:00 0
	14eb21000-14eb54000 r-xp 00000000 00:00 0
	170000000-170001000 r--p 00000000 08:02 14427037 
    */
    /*
    cinput_ptr = 0x1410fa870
	cinput_ptr + sizeof(void*)*0 = 0x1406261b0
	cinput_ptr + sizeof(void*)*1 = 0x14062e2c0
	cinput_ptr + sizeof(void*)*2 = 0x14062e3e0
	cinput_ptr + sizeof(void*)*3 = 0x14062d8c0
	cinput_ptr + sizeof(void*)*4 = 0x14062bab0 <- CreateMove
	cinput_ptr + sizeof(void*)*5 = 0x14062a7f0
	cinput_ptr + sizeof(void*)*6 = 0x14062d760 <- WriteUsercmdDeltaToBuffer
	cinput_ptr + sizeof(void*)*7 = 0x14062d0f0
	cinput_ptr + sizeof(void*)*8 = 0x14062d2f0
	
	
	
	.rdata:00000001410FA890                 dq offset sub_14062BAB0 <- CreateMove
    */
    unsigned long cinput_ptr = mem::ReadLong(offsets::REGION + 0x01ec4080);
    //printf("cinput_ptr = %p\n", cinput_ptr);
    //for (int i = 0; i < 10; i++) {	
    //    printf("cinput_ptr + sizeof(void*)*%d = %p\n", i, mem::ReadLong(cinput_ptr + sizeof(void*)*i));
    //}


    pid_t pid = mem::GetPID();
    assert(ptrace(PTRACE_ATTACH, pid, NULL, NULL) == 0);
    wait(NULL);
    
    func_copy(pid, 0x14eb21000, (char*)WriteUsercmdDeltaToBuffer, 0x2000);
    //assert(ptrace(PTRACE_POKEDATA, pid, (void*)(cinput_ptr + sizeof(void*)*6), (void*)0x14eb21000) == 0);
    //for (int i = 0; i < 0x200; i++) {
        //printf("write[%d] = %02hhx\n", i, mem::ReadChar(0x14EB21000 + i));
   // }


    assert(ptrace(PTRACE_DETACH, pid, NULL, NULL) == 0);


    //for (int i = 0; i < 10; i++) {
    //    printf("[AFTER] cinput_ptr + sizeof(void*)*%d = %p\n", i, mem::ReadLong(cinput_ptr + sizeof(void*)*i));
    //}
    
    
    
    Level *level = new Level();
    LocalPlayer *localPlayer = new LocalPlayer();
    X11Utils *x11Utils = new X11Utils();
    std::vector<Player *> *players = new std::vector<Player *>;
    for (int i = 0; i < 60; i++)
    {
        players->push_back(new Player(i));
    }
    my_util* myutil = new my_util(level, localPlayer, players, x11Utils);
    Aimbot *aimbot = new Aimbot(level, localPlayer, players, x11Utils);
    aimbot->set_silentaim_enable_flag(true);

    // Main loop
    printf("SILENTAIM TEST STARTING MAIN LOOP\n");
    int counter = 0;
    
    while (1)
    {
        try
        {
            // resolve pointers
            localPlayer->markForPointerResolution();
            for (int i = 0; i < players->size(); i++)
            {
                Player *player = players->at(i);
                player->markForPointerResolution();
            }

            aimbot->watch_silentaim_key();
            aimbot->update();
            
            myutil->item_glow__pagedown(counter);
            //myutil->invisible_my_weapon();
            myutil->process_thirdperson__pageup();
            myutil->speed_down__delete();
            myutil->glow_enemy();
            
            
            // all ran fine
            if (counter % 1000 == 0) {
                printf("UPDATE[%d] OK. \n", counter);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        catch (...)
        {
            printf("UPDATE[%d] ERROR (LOADING SCREEN?). SLEEPING FOR 10 SECONDS\n", counter);
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }
        counter++;
        if (counter > 1000)
            counter = 0;
    }
}
