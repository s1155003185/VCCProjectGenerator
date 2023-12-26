# Versioning Common Codebase Project / Versioning Coding Cooperation Project
# VCCProjectGenerator

## Objective
Auto handle project update if using VCCModule. 

## Compile
1. Pull VCCProjectGenerator
2. cd VCCProjectGenerator
3. make release
4. program in bin/Release

The Git template folder will stored in ~\Document\vcc first.

## Command - Version / Add / Update
vpg <Mode>
[-w <Workspace>] [-i <Interface>] [-v <tag version>] [-p <Plugin>]
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
-w Workspace, Full Path or Path relative to current directory. Default Current Directory.
-i Interface. Value can be
    - CPPDLL
    - CPPEXE
    - CPPCOMPLEX
    - VCCDLL
    - VCCEXE
    - VCCCOMPLEXE
-v tag version. If not specify, use master.
-p Plugin Module. This param can be stated multiple times. Value can be
    - VCC/Versioning/Git

#### Enum to Object (Will generate to UI under same folder)
-type File Path of Object Type. Path relative to -lib. Must only have one enum class. If UI has multi libraries, must different name to avoid compile error.
-obj Path (including subpath) that contains Object Property Types. Path relative to -lib. Objects must be stated in Object Type enum class.
-act Path (including subpath) that contains Actions. Path relative to -lib.
-err Path (including subpath) that contains Errors. Path relative to -lib.

#### Control
-SourceFullHistory If first pull Git response, then pull whole batch history. Default 10 lastest history.
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
