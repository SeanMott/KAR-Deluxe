#ifndef INI_FILE_H
#define INI_FILE_H

//defines structs and operations for parsing ini files

//defines a enum for how the operation went
typedef enum IniFile_OPResult_Type
{
    IniFile_OPResult_Type_Ok = 0, //the operation went ok, no issues, this is a genaric Ok, for furthur details get a differant type
    IniFile_OPResult_Type_Failed = 1, //the operation failed, this is a generic fail, for furthur details get a differant type

    Count
} IniFile_OPResult_Type;

#endif