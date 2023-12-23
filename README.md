# Versioning Common Codebase Project / Versioning Coding Cooperation Project
# VCCProjectGenerator

## Objective
Auto handle project update if using VCCModule. 

## Compile
1. Create an empty folder 
2. Pull VCCProjectGeneratorEXE and VCCProjectGeneratorDLL
3. make VCCProjectGeneratorDLL
4. make VCCProjectGeneratorEXE
5. program in Debug folder or Release Folder

## Command - Version
vccproj -v

## Command - Add
vccproj -A
[-w <Workspace>] [-l <DLL Directory>] [-i=<Interface> <Interface Directory>] [-s <Source Directory>]
[-SourceForceRefresh] [-SourceFullHistory] [-ExcludeExternalUnitTest]

## Command - Update
vccproj -U 
[-w <Workspace>] [-l <DLL Directory>] [-i=<Interface> <Interface Directory>] [-s <Source Directory>]
[-type <Object Type File Path>] [-obj <Object Property Type Directory>] [-act <Action Type Directory>] [-err <Error Type Directory>]
[-SourceForceRefresh] [-SourceFullHistory] [-ExcludeExternalUnitTest] [-ForceUpdateVCCModel] 

### Definition
#### Mode
-v Get Version
-A Add Mode
-U Update Mode

#### GitSource
-s Git Source Copy of VCCModule. Default Document\VCCProject

#### Project Workspace
##### Note <> means path. Support "path". If lib and int not stated, then it is a c++ application without dll.
-w Workspace, Full Path or Path relative to current directory. Default Current Directory.
-l VCCModule c++ dll project directory. Path relative to wsp. If only dll project, no need to state. Mandatory if have UI project.
-i UI project directory. Interface can be CPP, VCC, JAVA, SWITF (only CPP support at the moment). Path relative to wsp. Link lib to UI. If UI Project already exists, then just link but create. Mandatory if have UI project.

#### Enum to Object (Will generate to UI under same folder)
-type File Path of Object Type. Path relative to -lib. Must only have one enum class. If UI has multi libraries, must different name to avoid compile error.
-obj Path (including subpath) that contains Object Property Types. Path relative to -lib. Objects must be stated in Object Type enum class.
-act Path (including subpath) that contains Actions. Path relative to -lib.
-err Path (including subpath) that contains Errors. Path relative to -lib.

#### Control
-SourceForceRefresh Force pull Git response in -src.
-SourceFullHistory If first pull Git response, then pull whole batch history. Default 10 lastest history.
-ExcludeExternalUnitTest Ignore unit test under external. If want to customize VCCModule, recommend to keep unit test.
-ForceUpdateVCCModel Force update project from -src.



## Release Log

### 202312-23 Combine VCCProjectGeneratorEXE and VCCProjectGeneratorDLL and rename to VCCProjectGenerator

### 2023-12-18 Rename library and executable to libvpg and vpg

### 2023-12-06 Change Project and MakeFile to CPP Complex Mode

### 2023-11-04 Generate Object Type File and Class Files

### 2023-10-15 Project Name
-	Add long project name

### 2023-04-30 First Release
Initialization
