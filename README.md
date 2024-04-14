# VCCProjectGenerator
## Versioning Common Codebase Project / Versioning Coding Cooperation Project

## Versioning Common Codebase Project Introduction
When starting a new project, it is time consuming to create basic structure. In fact, all projects having similar structure and lots of similar functions. This project is to extract common factor to enhance development efficience.

Versioning Common Codebase Project contains following members:
Versionin Common Codebase Project - Project Generator (https://github.com/s1155003185/VCCProjectGenerator)
Versionin Common Codebase Project - Project Generator VSCode Extension (https://github.com/s1155003185/VCCProjectGeneratorVSCodeExtension)
Versionin Common Codebase Project - Simple C++ Template (https://github.com/s1155003185/VCCSimpleTemplateCPP)
Versionin Common Codebase Project - VCCModule (https://github.com/s1155003185/VCCModule)

## Versionin Common Codebase Project - Project Generator Feature
1. Auto Create C++ Project
2. Auto Create, Update, Generate VCCModule (Any Interface + C++ DLL Moduel)

## Versionin Common Codebase Project - Simple C++ Template Feature


## Versionin Common Codebase Project - VCCModule Feature
1. 

## Compile
1. Pull VCCProjectGenerator
2. cd VCCProjectGenerator
3. make release
4. program in bin/Release

The Git template folder will stored in ~\Document\vcc first.

## Command - Version / Add / Update
vpg <Mode> -i <Interface>
[-w <Workspace>]  [-v <tag version>] [-p <Plugin>] [-d <deep>]
[--ExcludeUnitTest] [--ExcludeExternalUnitTest]

## Command - Generate
vpg -G 
[-w <Workspace>] [-i <Interface>]
[-type <Object Type File Path>] [-obj <Object Property Type Directory>] [-act <Action Type Directory>] [-err <Error Type Directory>]

### Definition
#### Mode
<Mode> Must be -V or -A or -U 
where
-V Get Version
-A Add Mode
-U Update Mode
-G Generate Mode

#### Project Workspace
-i Interface. Value can be
    - CPPDLL
    - CPPEXE
    - CPPCOMPLEX
    - VCCDLL
    - VCCEXE
    - VCCCOMPLEXE

-w Workspace. Default Current Directory.
-v tag/branch version. Default master.
-p Plugin Module. This param can be stated multiple times. Value can be
    - VCC/Versioning/Git
-d History Depth. Default all history

#### Enum to Object (Will generate to UI under same folder)
-type File Path of Object Type. Path relative to -lib. Must only have one enum class. If UI has multi libraries, must different name to avoid compile error.
-obj Path (including subpath) that contains Object Property Types. Path relative to -lib. Objects must be stated in Object Type enum class.
-act Path (including subpath) that contains Actions. Path relative to -lib.
-err Path (including subpath) that contains Errors. Path relative to -lib.

#### Control
-ExcludeUnitTest Dont generate unit test.
-ExcludeExternalUnitTest Ignore unit test under external. If want to customize VCCModule, recommend to keep unit test.

## Release Log

### 202312-23 Combine VCCProjectGeneratorEXE and VCCProjectGeneratorDLL and rename to VCCProjectGenerator

### 2023-12-18 Rename library and executable to libvpg and vpg

### 2023-12-06 Change Project and MakeFile to CPP Complex Mode

### 2023-11-04 Generate Object Type File and Class Files

### 2023-10-15 Project Name
-	Add long project name

### 2023-04-30 First Release
Initialization
