/*---------------------------------------------------------------------------*
    Entrypoint for the Ignition - Alt Machine module.

 *---------------------------------------------------------------------------*/

#include "os.h"
#include "hsd.h"
#include "preload.h"
#include "scene.h"
#include "inline.h"
#include "audio.h"
#include "hoshi/settings.h"

#include "MachineDecl.h"
#include "MachineDefines/MachineDatLookUps.h"

char ModName[] = "Ignition - Alt Machines";
char ModAuthor[] = "Jas";
char ModVersion[] = "v1.0";

//when we boot the game
void OnBoot(HSD_Archive *archive)
{
    MachineDatStrs_Print_VcStarLight();
    
    MachineDatStrs_Override_VcStarLight_Filename(MACHINE_DAT_FILENAME_VcStarDevil);
    MachineDatStrs_Override_VcStarLight_RootNodeName(MACHINE_DAT_ROOT_NODE_NAME_VcStarDevil);
    MachineDatStrs_Print_VcStarLight();

    MachineDatStrs_Override_VcStarLight_Filename(MACHINE_DAT_FILENAME_vcStarWagon);
    MachineDatStrs_Override_VcStarLight_RootNodeName(MACHINE_DAT_ROOT_NODE_NAME_vcStarWagon);
    MachineDatStrs_Print_VcStarLight();

    //sets compact to Beta Lance
    //Root Node: vcDataStarLanceBeta
    //Filename: Lance_Beta.dat
    MachineDatStrs_Override_VcStarLight_Filename("Lance_Beta.dat");
    MachineDatStrs_Override_VcStarLight_RootNodeName("vcDataStarLance");
    MachineDatStrs_Print_VcStarLight();

    return;
}
