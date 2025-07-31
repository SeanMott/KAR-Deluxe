#ifndef MACHINE_DAT_LOOK_UPS_H
#define MACHINE_DAT_LOOK_UPS_H

//defines static strings and such for statically looking up machines

#include "os.h"
#include <string.h>

#include "code_patch/code_patch.h"
#include "event.h"
#include "machine.h"
#include "os.h"

//Compact Star
static char* MACHINE_DAT_FILENAME_VcStarLight = (char*)(0x804B12B0);
static char* MACHINE_DAT_ROOT_NODE_NAME_VcStarLight = (char*)(0x804B12C0);

//prints the current filename and root name for a machine
void MachineDatStrs_Print_VcStarLight()
{
    OSReport("Compact (VC Star Light) is currently set to Filename: \"%s\" || Root Node: \"%s\"\n",
    MACHINE_DAT_FILENAME_VcStarLight,MACHINE_DAT_ROOT_NODE_NAME_VcStarLight);
}

//overrides the current filename for a machine
void MachineDatStrs_Override_VcStarLight_Filename(const char* filename)
{
    strcpy(MACHINE_DAT_FILENAME_VcStarLight, filename);
}

//overrides the current root node name for a machine
void MachineDatStrs_Override_VcStarLight_RootNodeName(const char* rootName)
{
    strcpy(MACHINE_DAT_ROOT_NODE_NAME_VcStarLight, rootName);
}

//shadow star
static char* MACHINE_DAT_FILENAME_VcStarDevil = (char*)(0x804B12F0);
static char* MACHINE_DAT_ROOT_NODE_NAME_VcStarDevil = (char*)(0x804B1300);

//prints the current filename and root name for a machine
void MachineDatStrs_Print_DevilStar()
{
    OSReport("Shadow (VC Star Devil) is currently set to Filename: \"%s\" || Root Node: \"%s\"\n",
    MACHINE_DAT_FILENAME_VcStarDevil, MACHINE_DAT_ROOT_NODE_NAME_VcStarDevil);
}

//overrides the current filename for a machine
void MachineDatStrs_Override_DevilStar_Filename(const char* filename)
{
    strcpy(MACHINE_DAT_FILENAME_VcStarDevil, filename);
}

//overrides the current root node name for a machine
void MachineDatStrs_Override_DevilStar_RootNodeName(const char* rootName)
{
    strcpy(MACHINE_DAT_ROOT_NODE_NAME_VcStarDevil, rootName);
}

//hydra
//MegaStruct_StarFileNamesAndHSDNames_"VcStarHydra.dat" = .data:0x804B1310; // type:object size:0x10 data:string
//Struct_String_VCStarNameHSDNode_"vcDataStarHydra" = .data:0x804B1320; // type:object size:0x10 data:string

//bulk
//MegaStruct_StarFileNamesAndHSDNames_"VcStarHeavy.dat" = .data:0x804B1330; // type:object size:0x10 data:string
//Struct_String_VCStarNameHSDNode_"vcDataStarHeavy" = .data:0x804B1340; // type:object size:0x10 data:string

//slick
//Struct_String_VCStarNameHSDNode_"VcStarSlick.dat" = .data:0x804B1350; // type:object size:0x10 data:string
//Struct_String_VCStarNameHSDNode_"vcDataStarSlick" = .data:0x804B1360; // type:object size:0x10 data:string

//wagon
static char* MACHINE_DAT_FILENAME_vcStarWagon = (char*)(0x804B13C0); // type:object size:0x10 data:string
static char* MACHINE_DAT_ROOT_NODE_NAME_vcStarWagon = (char*)(0x804B13D0); // type:object size:0x10 data:string

//swerve
//Struct_String_VCStarNameFilename_"VcStarRuins.dat" = .data:0x804B1408; // type:object size:0x10 data:string
//Struct_String_VCStarNameHSDNode_"vcDataStarRuins" = .data:0x804B1418; // type:object size:0x10 data:string

//turbo
//Struct_String_VCStarNameHSDNode_"VcStarTurbo.dat" = .data:0x804B1428; // type:object size:0x10 data:string
//Struct_String_VCStarNameHSDNode_"vcDataStarTurbo" = .data:0x804B1438; // type:object size:0x10 data:string

//wing kirby
//Struct_String_VCStarNameHSDNode_"VcWingKirby.dat" = .data:0x804B14D8; // type:object size:0x10 data:string
//Struct_String_VCStarNameHSDNode_"vcDataWingKirby" = .data:0x804B14E8; // type:object size:0x10 data:string

//VS Dedede wheel
//String_HSDName_"VcWheelVsDedede.dat" = .data:0x804B1BE0; // type:object size:0x14 data:string
//String_HSDName_"vcDataWheelVsDedede" = .data:0x804B1BF4; // type:object size:0x14 data:string

// example code to be executed when an event is triggered
/*CODEPATCH_HOOKCREATE(0x8015e780,     // Memory address to begin executing custom code
                     "lwz 5,0x0(1)\nlwz 4,0x0(1)\n\t",  // ASM instructions to execute before calling our C function. In this case we move r27 (incoming event index) into r3 (to pass into our function).
                     MachineDatStrs_Print_VcStarLight, // Pointer to our C function
                     "",             // ASM instructions to execute after calling our C function (none).
                     0);             // Return address. Use 0 to branch back to the injection site.

//applys a path
void MachineLookup_Patch()
{
    CODEPATCH_HOOKAPPLY(0x8015e780);
}*/

#endif