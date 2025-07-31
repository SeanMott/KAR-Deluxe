#ifndef MACHINE_DECL
#define MACHINE_DECL

//defines the functions and structs needed for loading and handling decl data of machines

#include "datatypes.h"
#include "fst/fst.h"
#include "os.h"
#include "hsd.h"

// Hoshi
#include "hoshi/log.h"

#include "IO/IniFile.h"

//defines the directory for loading machines
#define MACHINE_CUSTOM_DECL_LOADING_DIR "CT/Machines"

//defines the directory reserved for base machines
#define MACHINE_BASE_DECL_LOADING_DIR "CT/Base/Machines"

#define MACHINE_DECL_INI_FILE_KEY_LENGTH_LIMIT 256
#define MACHINE_DECL_INI_FILE_VALUE_LENGTH_LIMIT 256

//defines a machine decl
typedef struct MachineDecl
{
    u8 machineID; //the ID associated with the machine, unique for every machine
    u8 machineType; //is it a star (0) or a wheel (1)
    u8 isDefaultMachine; //what catagory are we storing it in
    u16 entryNum; //the entry num used when loading from the filesys
    char* filename; //name of the decl file we are loading
    char* rootNodeName; //the root name of the node
    char* GUIName; //the name to display for GUI
    char* GUIDesc; //the description to display for GUI
    char* authors; //the authors
} MachineDecl;

//callback when we're done loading the ini file
void MachineDecl_IniFile_LoadFile_Callback(int r3, int *is_loaded)
{
    (*is_loaded) = 1;
    return;
}

//defines a chunk of raw ini data
typedef struct MachineDecl_IniBlob
{
    int length; //the entire length of the string
    char* data; //the raw string data
} MachineDecl_IniBlob;

//loads a ini file for machine data
//returns a IniFile_OPResult_Type, check IniFile.h for more details
//IniFile_OPResult_Type_Ok = 0, //the operation went ok, no issues, this is a genaric Ok, for furthur details get a differant type
//IniFile_OPResult_Type_Failed = 1, //the operation failed, this is a generic fail, for furthur details get a differant type
IniFile_OPResult_Type MachineDecl_IniFile_Load(int entryNum, MachineDecl_IniBlob* iniBlob)
{
    // load this file
    DVDFileInfo file_info;
    DVDFastOpen(entryNum, &file_info);
    iniBlob->length = file_info.length;
    DVDClose(&file_info);
    iniBlob->data = (char*)HSD_MemAlloc(OSRoundUp32B(iniBlob->length));

    // load file
    volatile int is_loaded = 0;
    File_Read(entryNum, 0, (void*)iniBlob->data, OSRoundUp32B(iniBlob->length), 0x21, 1, MachineDecl_IniFile_LoadFile_Callback, (void *)&is_loaded);

    // wait to load
    while (!is_loaded)
        ;

    OSReport("Loaded Machine Decl file \"%s\"\n", FST_GetFilenameFromEntrynum(entryNum));
    //OSReport("Loaded Machine Decl file \"%s\" || %s\n", FST_GetFilenameFromEntrynum(entryNum), iniBlob->data);

    return IniFile_OPResult_Type_Ok;
}

//parses the decl str into a decl struct
//0 = false/failed, 1 = true/went well
u8 MachineDecl_ParseIniData(const MachineDecl_IniBlob* iniBlob, MachineDecl* decl)
{
    //NULL sanitiy checks
    if(!decl)
    {
        OSReport("Igniton Error || Machine Decl - Ini Parsing || Func: \"MachineDecl_ParseIniData\" || "
        "The given Machine Decl is a NULL ptr. Check that the machine data was allocated correctly in the global array before calling this function.\n");
        return 0;
    }
    else if(!iniBlob)
    {
        OSReport("Igniton Error || Machine Decl - Ini Parsing || Func: \"MachineDecl_ParseIniData\" || File: \"%s\" || "
        "The given Ini Blob is a NULL ptr. Check that the ini file was loaded correctly before calling this function.\n", decl->filename);
        return 0;
    }

    //checks if it's not zero
    if(!iniBlob->length)
    {
        OSReport("Igniton Warning || Machine Decl - Ini Parsing || Func: \"MachineDecl_ParseIniData\" || File: \"%s\" || "
        "Can not parse the given Ini blob into Machine data as the file has nothing inside.\n", decl->filename);
        return 0;
    }

    //goes through the whole file and parse each line
    int bufferMemCount_key = sizeof(char) * MACHINE_DECL_INI_FILE_KEY_LENGTH_LIMIT;
    char key[MACHINE_DECL_INI_FILE_KEY_LENGTH_LIMIT];

    int bufferMemCount_value = sizeof(char) * MACHINE_DECL_INI_FILE_VALUE_LENGTH_LIMIT;
    char value[MACHINE_DECL_INI_FILE_VALUE_LENGTH_LIMIT];
    

    /*
    ini files are formated as
        
    version <data>
    rootName <data>
    filename <data>
    type <data>
    ID <data>
    catagory <data>
    title <data>
    desc <data>
    author <data>
    
    ---Lance star example ini
    version 1.0.0
    rootName Lance
    filename Lance
    type Star
    ID 33
    catagory Modded
    title Lance
    desc  Ram your foes and friends alike in this walking beetle of pain!
    author TacoTikiTaco - Taco
    */
   #define MACHINE_DECL_SPEC_ENTRY_COUNT 9
   u8 catagoryIndex = 0; //what catagory are we pulling in

    u16 fileIndex = 0; u8 lineIndex = 0; 
    
    u8 hasKey = 0;
    u8 keyIndex = 0;

    u16 valueIndex = 0;

    while(fileIndex < iniBlob->length || catagoryIndex >= MACHINE_DECL_SPEC_ENTRY_COUNT)
    {
        //if search for key
        if(!hasKey)
        {
            if(iniBlob->data[fileIndex] == ' ')
            {
                hasKey = 1;
            }
            else
            {
                key[keyIndex] = iniBlob->data[fileIndex];
                keyIndex++;
            }
        }
        
        //if for value
        else
        {
            if(iniBlob->data[fileIndex] == '\n' || iniBlob->data[fileIndex] == '\0')
            {
                value[valueIndex - 1] = '\0';
                OSReport("Machine Decl Ini Parsing || File: \"%s\" || Key: \"%s\", Value: \"%s\"\n", decl->filename, key, value);

                

                //copies into the right catagory
                switch(catagoryIndex)
                {
                    //version <data> 0
                    //rootName <data> 1
                    //filename <data> 2
                    //type <data> 3
                    //ID <data> 4
                    //catagory <data> 5
                    //title <data> 6
                    case 6:
                        decl->GUIName = (char*)HSD_MemAlloc(OSRoundUp32B(valueIndex - 1));
                        strcpy(decl->GUIName, value);
                    break;
                    //desc <data> 7
                   case 7:
                        decl->GUIDesc = (char*)HSD_MemAlloc(OSRoundUp32B(valueIndex - 1));
                        strcpy(decl->GUIDesc, value);
                    break;
                    //author <data> 8
                }
                //decl->GUIName = "Dragon";

                memset(key, '\0', bufferMemCount_key);
                memset(value, '\0', bufferMemCount_value);

                hasKey = 0; keyIndex = 0; valueIndex = 0;

                //increments the catagory
                catagoryIndex++;

                if(catagoryIndex == MACHINE_DECL_SPEC_ENTRY_COUNT)
                    catagoryIndex = 0;
            }
            else
            {
                value[valueIndex] = iniBlob->data[fileIndex];
                valueIndex++;
            }
        }

        fileIndex++;
    }

    return 1;
}

//the total number of custom machines we are loading
u8 machineDeclData_CurrentIndex_CustomMachine = 0;

//the total amount of base machines we should be loading, any less or more and it's a error
#define IGNITION_MACHINE_LOADER_BASE_MACHINE_COUNT 20
u8 machineDeclData_CurrentIndex_BaseMachine = 0;

//the array of loaded machine decl data
MachineDecl* machineDelcDatas_Custom;
MachineDecl machineDelcDatas_Base[IGNITION_MACHINE_LOADER_BASE_MACHINE_COUNT];

//when the file is loaded
void MachineDecl_LoadFile_Callback(int r3, int *is_loaded)
{
    (*is_loaded) = 1;
    return;
}

//when a machine decl is loaded, add it to the count
void MachineDecl_OnMachineDeclFileLoaded_Custom_Count(int entryNum)
{
    machineDeclData_CurrentIndex_CustomMachine++;
}

//when a custom machine decl is loaded, add it to the  array
void MachineDecl_OnMachineDeclFileLoaded_Custom_AddToArray(int entryNum)
{
    machineDelcDatas_Custom[machineDeclData_CurrentIndex_CustomMachine].entryNum = entryNum;
    machineDelcDatas_Custom[machineDeclData_CurrentIndex_CustomMachine].filename = FST_GetFilenameFromEntrynum(entryNum);

    OSReport("added machine %d \"%s\"\n",
              machineDeclData_CurrentIndex_CustomMachine,
              machineDelcDatas_Custom[machineDeclData_CurrentIndex_CustomMachine].filename,
              FST_GetFilePathFromEntrynum(entryNum));

    MachineDecl_IniBlob iniBlob;
    IniFile_OPResult_Type IOIniResult = MachineDecl_IniFile_Load(machineDelcDatas_Custom[machineDeclData_CurrentIndex_CustomMachine].entryNum, &iniBlob);

    //parses the ini file into the decl data
    u8 IOResult = MachineDecl_ParseIniData(&iniBlob, &(machineDelcDatas_Custom[machineDeclData_CurrentIndex_CustomMachine]));

    machineDeclData_CurrentIndex_CustomMachine++;
}

//when a base machine decl is loaded, add it to the  array
void MachineDecl_OnMachineDeclFileLoaded_Base_AddToArray(int entryNum)
{
    machineDelcDatas_Base[machineDeclData_CurrentIndex_BaseMachine].entryNum = entryNum;
    machineDelcDatas_Base[machineDeclData_CurrentIndex_BaseMachine].filename = FST_GetFilenameFromEntrynum(entryNum);

    OSReport("added machine %d \"%s\"\n",
              machineDeclData_CurrentIndex_BaseMachine,
              machineDelcDatas_Base[machineDeclData_CurrentIndex_BaseMachine].filename,
              FST_GetFilePathFromEntrynum(entryNum));

    MachineDecl_IniBlob iniBlob;
    IniFile_OPResult_Type IOIniResult = MachineDecl_IniFile_Load(machineDelcDatas_Base[machineDeclData_CurrentIndex_BaseMachine].entryNum, &iniBlob);

    //parses the ini file into the decl data
    u8 IOResult = MachineDecl_ParseIniData(&iniBlob, &(machineDelcDatas_Base[machineDeclData_CurrentIndex_BaseMachine]));

    machineDeclData_CurrentIndex_BaseMachine++;
}

//loads all the machine decls
void MachineDecl_LoadDecls()
{
    //---loads the base machines
    OSReport("Loading Base Game Machines from directory %s\n", MACHINE_BASE_DECL_LOADING_DIR);

    //this second call gets the machines
    FST_ForEachInFolder(MACHINE_BASE_DECL_LOADING_DIR, ".ini", 1, (void (*)(int, void *))MachineDecl_OnMachineDeclFileLoaded_Base_AddToArray, 0);

    //---loads the custom machines
    OSReport("Loading Custom Machines from directory %s\n", MACHINE_CUSTOM_DECL_LOADING_DIR);

    //this first call gets the count of how many machines we have
    FST_ForEachInFolder(MACHINE_CUSTOM_DECL_LOADING_DIR, ".ini", 1, (void (*)(int, void *))MachineDecl_OnMachineDeclFileLoaded_Custom_Count, 0);

    //allocates space for the array of custom machines
    machineDelcDatas_Custom = (MachineDecl*)HSD_MemAlloc(sizeof(MachineDecl) * machineDeclData_CurrentIndex_CustomMachine);
    OSReport("Allocated %d slots for custom Machines \n", machineDeclData_CurrentIndex_CustomMachine);

    //this second call gets the machines since we now have allocated space
    FST_ForEachInFolder(MACHINE_CUSTOM_DECL_LOADING_DIR, ".ini", 1, (void (*)(int, void *))MachineDecl_OnMachineDeclFileLoaded_Custom_AddToArray, 0);

    //done loading
    // /return;
}

#endif