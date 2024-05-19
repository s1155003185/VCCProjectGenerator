# VCCModuel / VCCProjectGenerator / VCCProjectGenerator VSCode Extension
### Versioning Common Codebase Project / Versioning Coding Cooperation Project
Cross plaform project to handle Any Interface with C++ dll.
Maintain those standard already stable long time ago. No reason to implement twice.

Note: Still in initialize version, will have full review when official release

## Features
- Easy update project to model version instead of rewrite codebase. Just Update Project Genertor to newest version, execute Update and Generation.
- Create program structure like sql, the remaining is just handle specific logic.
- Mainly handle those already stable and optimize features so that dont need to implement from the beginning

### Options
1. Create C++ Complex Project
2. Create C++ EXE Project
3. Create C++ DLL Project
4. Create VCC Complex Project
5. Create VCC EXE Project
6. Create VCC DLL Project
7. Update VCC Project
8. Generate VCC Project

### Pre-Requirement
Must already install following before execute VCCProjectGenerator and VCCProjectGenerator VSCode Extension.
Or can download the template from git directly.
1. git
2. g++
3. make
4. gtest

### Procedcure of Extension
1. Download VCCProjectGenerator by Git to ~/Document/VCC folder.
2. Use make to build VCCProjectGenerator.
3. Download template by Git.
4. Add / Update based on Template.

If dont want to download extra project, please download following git manually and follow instruction listed in Makefile to adjust.
1. Versionin Common Codebase Simple C++ Template (https://github.com/s1155003185/VCCSimpleTemplateCPP)
2. Versionin Common Codebase Module - VCCModule (https://github.com/s1155003185/VCCModule)

## Build C++ project
Follow the instruction listed in Makefile

when compile enter following command in terminal:
To build debug
```
make debug -j10
```

To build unittest only
```
make unittest -j10
```

To build release
```
make release -j10
```

To clean project
```
make clean
```

Other command can be refer to Makefile

Debug program is built in bin/Debug
Release program is built in bin/Release

## Optional
-	Filter out *.o: File > Preferences > Setting => Files: Exclude => Add **/**.o
-	Remove unittest/External to skip running unit test of VCCModule.

## Execute C++ project in VScode
F5

## Compare to AI Code Generation
- Stable
- Reliable
- Faster
- No need to test existing code
- No need to study the code after generation
- No need to upload code to service provider

****
## VCCModule Feature
### Status:
-	Active: Keep changing. Not recommend to use.
-	Stable: Workable.
-	Inactive: Will not update in the future.
-	Pending: Coming soon.
### Core:
-	Action Manager (Pending)
-	Exception (Stable)
-	Helper (Stable, Keep update)
-	Log Service (Stable)
-   Process Service (Stable)
-	Property Accessor (Pending)
-   Terminal Service (Stable)
-   XML (Read only, Other are still pending)
### Common:
-   Git (Actives, basic function only)
### Module:
-	TextEditor (Pending)
-	Document (Pending)
-	Speedsheet (Pending)
### UI:
-	LoginService (Pending)
-	LicenseService (Pending)
-	PaymentService (Pending)

## Pending
- Use precompile header
- Update vcc.json also update Makefile etc.
- ProperyAccessor and java bridge, swift bridge.
- Auto generate ChangeLog
- Check style
- Auto generaste document for coding (like java)
- Enhance class generation for validation, trigger etc.
- Alert tag for Update
- OAuth

## Known Issue
- Compile unit test to slow because of linkage. No solution.
- Regex is too slow

****
## Versioning Common Codebase Project / Versioning Coding Cooperation Project Introduction
Document versioning are highly expected but seems no solution at the moment. Also, current existing document processors have straight behavior, such as auto creating style which make characters disappeared, saving whole document for each save, lagging for large document, etc. Unfortunately, seems that those behaviors are not bug. Enhancement cannot be expected. So, Versioning Coding Cooperation Project is introducted to develop a document versioning system.

In order to implement versioning document program, document processor and git manager are need to be implemented at the same time. To speed up, a generator is required to manage those projects. So, there are totally 3 programs need to be written at the same times.

When starting a new project, it is time consuming to create basic structure. And rewrite codebase is time consuming. In fact, all projects have similar structure and lots of similar functions. Versioning Common Codebase Project is raised to extract common factor to enhance development efficience.

## Versioning Common Codebase Project Members
VCC Module is a combination of Interface + C++ DLL. Most process are included in C++ DLL to minimize effort for cross-platform.
At the moment, only consider to support Java UI (Window, Linux and Android) and Swift UI (Mac OS and iOS).
For view object details, please visit corresponding Git response to check ReadMe.md

### Template
1. Versionin Common Codebase Simple C++ Template (https://github.com/s1155003185/VCCSimpleTemplateCPP)
=> Free, No License Project. C++ Template.
#### Features
- Just clone from git source and follow the instruction listed in the Header of Makefile to change.
- Can use VCC Project Generator to create c++ project.

2. Versionin Common Codebase Module - VCCModule (https://github.com/s1155003185/VCCModule)
Common and continous update template so that do not need to develop project from beginning. Also reduce rewrite codebase effort.
=> Free, No License Project. C++ Template with External Code.
#### Expected Features
- Just clone from git source and follow the instruction listed in the Header of Makefile to change.
- Include most of common component. e.g. macro, helper etc.
- Easy update. Just remove External/VCC under include, src and unittest folders and copy new version folder to same place.
- Do not need to find and import external library, eg. Git, etc.
- Easy customize.
- Recommand to use VCC Project Generator to create VCC project.

### Manager
1. Versionin Common Codebase Project Generator (https://github.com/s1155003185/VCCProjectGenerator)
=> Free, MIT License Project. Used to Create, Update, Generate Project. Mainly handle dll and bridge between c++ and interface.
#### Expected Features
- Create, Update, Generate C++, VCC Project.
- Create whole program structure via enum class like sql table.
- Create bridge between VCC dll and interface.
- Auto Generate Class. Auto Handle Action, Thread etc. Only need to implement logic.

2. Versioning Common Codebase Project Generator VSCode Extension (https://github.com/s1155003185/VCCProjectGeneratorVSCodeExtension)
=> Free, MIT License Project. VSCode Extension for VCCProject Generator.
#### Features
- VSCode Extension to execute Versioning Common Codebase Project Generator.

3. Versionin Common Codebase Project - Studio (Not Implement Yet)
=> Free Version: Java UI / Swift UI + c++ dll Simple Project Creation
=> Subscription Version: Web Service, Database linkage etc.
#### Expected Features
- Handle dynamic part that VCC Project Geneator not including. eg. webservice, database linkage, etc.
- Handle Form, Action, Virtual List, Virtual Table, Theme etc.

##  Versioning Coding Cooperation Project Members
1. Versioning Coding Cooperation - Office
=> Free Version: Document, SpreedSheet, Text Editor
=> Subscription Version: Git, Multi-user workspace, Plugins etc.

## Versioning Coding Cooperation Project / Versioning Common Codebase Project Common Members
1. VCCProject Manager (Not Implement Yet)
=> Free Version: VCC Project Generator and normal git operation. Fixed that inherited project is VCC Project Module.
=> Subscription Version: Self-define inherited project, Multi project handling, Document Versioning.

## Versionin Common Codebase Project - Project Generator Feature
1. Auto Create C++ Project
2. Auto Create, Update, Generate VCCModule (Any Interface + C++ DLL Moduel)

****
## Versionin Common Codebase Project Generator

### Pre-Requirement
1. git
2. g++
3. make
4. gtest

### Compile
1. Clone VCCProjectGenerator source code from https://github.com/s1155003185/VCCProjectGenerator
2. cd VCCProjectGenerator
3. make release -j10
4. program in bin/Release

### Procedure for Add or Update
1. Download template to ~/Document/VCC
2. For VCC, Check version of template equals to generator. If not switch to that tag.
3. Copy necessary files from template to workspace

### Command - Version
vpg -Version

Description:
    Get Current Version of Generator.

### Command - Add
vpg -Add -interface <Interface>
[-project-prefix <project-prefix>] [-project-name <project-name>] [-exe-name <exe-name>] [-dll-name <dll-name>] [-workspace-destination <workspace-destination>] [-plugins <plugins>] [--ExcludeUnitTest] [--ExcludeExternalUnitTest]


-project-prefix <project-prefix>: CPPDLL, CPPEXE, CPPCOMPLEX, VCCDLL, VCCEXE, VCCCOMPLEX
    CPPDLL: C++ DLL
    CPPEXE: C++ EXE
    CPPCOMPLEX: C++ DLL + EXE
    VCCDLL: VCC DLL
    VCCEXE: VCC EXE
    VCCCOMPLEX: VCC DLL + EXE

-project-name <project-name>
    Recommand. If not state, need to modify vcc.json and Makefile manually.

-exe-name <exe-name>
    Recommand. Only for EXE and Complex mode. If not state, need to modify vcc.json and Makefile manually.

-dll-name <dll-name>
    Recommand. Only for DLL and Complex mode. If not state, need to modify vcc.json and Makefile manually.

-workspace-destination <workspace-destination>
    Target workspace. If not state, then current workspace.

-plugins <plugins>
    Can state multiple times. It is the same as the list in vcc.json.

--ExcludeUnitTest
    Exclude gtest.

--ExcludeExternalUnitTest
    Exclude VCC unitest. If full trust VCC git source, then can skip to speed up. If need to customize, recommand to keep it.

Description:
    Generate project from template.

### Command - Update
vpg -Update [-workspace-destination <workspace-destination>]workspace

-workspace-destination <workspace-destination>
    Target workspace. If not state, then current workspace.

Description:
    Only for VCC Module. Other properties are overriden by vcc.json.
    First update template to the version that same as VCCProjectGenerator. Then compare different and update the files in workspace. Details: vcc.json, Update Rule.

### Command - Generate
vpg -Generate [-workspace-destination <workspace-destination>]workspace

Description:
    Only for VCC Module. Other properties are overriden by vcc.json. Details: vcc.json, Generate Rule.

### vcc.json
For VCCModule, there is vcc.json. Update Mode and Generate Mode fully depends on vcc.json. Can include this to .gitignore file.

#### Properies
Version
    Current VCCModule Version. Please manaully update while manually update Project from git source.

ProjectType
    VCCModule. Reserve for other project type, such as Java and Swift.

WorkspaceSourceGitUrl
    Git source.

WorkspaceDestination
    Current workspace. Just for referece.

ProjectPrefix
    In generation mode, generator only read file with that prefix and generate class with that prefix. It is used to distinguish different project if having multi dll.
    E.g. "ProjectPrefix": "VPG"
    Generator only read file have prefix vpg_* and generate class as VPGObject

ProjectName
    Project name.

ProjectNameDll
    DLL name. If does not compile dll, keep it empty. Will auto update Makefile and related files (Enhancement in next version).

ProjectNameExe
    EXE name. If does not compile exe, keep it empty. Will auto update Makefile and related files (Enhancement in next version).

ProjectNameGtest
    gtest folder name. Although can have rename to the other name, but keep it unittest is advise. If does not compile unittest, keep it empty. Will auto update Makefile and related files (Enhancement in next version).

IsGit
    If true, then generate .gitignore.

IsExcludeVCCUnitTest
    If true, then skip update unittest/External/VCC/

TypeWorkspace
    In Generation mode, generator will search file with suffix *_property.hpp to create Class, Property Accessor etc. Detail: Generation Rule.

ActionTypeDirectory, ExceptionTypeDirectory, ManagerTypeDirectory, ObjectTypeDirectory
    The location to export action_type.hpp, exception_type.hpp, manager_type.hpp, object_type.hpp
    All are used to export to interface. Cannot delete those files. action_type may be moved to External/VCC/Form/Action/ in the future such that action can be optional.

ModelDirectory
    In Generation mode, class files are generated here.

PropertyAccessorDirectoryHpp
    In Generation mode, property accessor hpp files are generated here. Not support in current version.

PropertyAccessorDirectoryCpp
    In Generation mode, property accessor cpp files are generated here. Not support in current version.

Plugins
    In Update mode, generator will copy folders under include/External, src/External, unittest/External to workspace.
    Option:
        VCC/Versioning/Git

### Update Rule
Procedure:
1. Download template.
2. Switch to corresponnding tag.
3. Synchronize files under External/VCC/Core and folder specified under Plugins in vcc.json
4. If file does not exist in workspace, then add files.
5. If file does not exist in template, then delete files.
6. If file both exists in workspace and template but not the same, then update files with following rules. (Samples in vpg_file_sync_service_test.cpp)
    6.1 Check file of workspace has following line at line 1.
        ```
        // <vcc:vccproj sync="FULL"/>
        ```
        Value of sync:
            FULL: Follow tag in file content.
            DEMAND: Replace all file content except tag has marked RESERVE.
            SKIP: No update.
            FORCE: Replace everything.

        Note: By default it is FULL mode.
        Note: current version only support sync only. Will add generation tag letter.

    6.2 In file content, if inherited project file and current project file both has that tag with same name, generateor will update with following rules.
        Inherited Project: 
            ```
            // <vcc:tagA sync="RESERVE">
            Code A
            // </vcc:tagA>
            ```

        Current Project: 
            // <vcc:tagA sync="REPLACE">
            Code B
            // </vcc:tagA>

        In the example, Inherited Project file and Current Project file both have tag with tagA. Then generator will update the code with following rules.
        Value of sync:
            RESERVE: No update.
            REPLACE: Replace current project file content by inherited project.
            ALERT: Show different it there is change. Not support at the moment.

### Generate Rule
The final objective is that user only need to handle enum class like sql table and logic in service and manager only. Other thing will be auto generate.
Sample in vpg_file_generation_manager_test.cpp

Definition of Program Structure
    Class: C++ Class. Data storage.
    Structure: C++ Structure. Used to support huge data process. Not support at the moment.
    PropertyAccessor: Bridge between Control and model. Provide thread safe access, validation, trigger etc. Not support at the moment.
    Service: Class with static function only. It is used to provide logic.
    Manager: Class with model. Call service to modify model.
    Form: Class with Manager. Call Manager to do action.

Definition of Enum Class
    Class Property file: file name with format projectPrefix_objectName_property.hpp
        If project prefix is defined in vcc.json, then all files and classes that not having project prefix will be skipped.
    Enum Class file: file name with format projectPrefix_enumName.hpp;
        If project prefix is defined in vcc.json, then all files and classes that not having project prefix will be skipped.
        All enum that used in class must be defined in TypeWorkspace. Otherwise will have compile error.


Restriction
- Field names should be long form and capital letter.
- Field names should having component with descending order. E.g. WorkspaceSource instead of SourceWorkspace
- All fields should use macro in class_macro.hpp.
- Not allow to use using namespace std, there are so many bugs. If want to use std library, please use std::wstring etc.
- The program is based on std::wstring. Should not use std::string.
- Only support basic type with std::wstring as Other language may not support the type defined in C++.
- Does not allow ClassA contains ClassB and ClassB contains ClassA. As there is Clone method in Macro which is not trivial.

Procedure
    1. Read all files in TypeWorkspace.
    2. For all Class Property file, will generate Class file etc.
    3. All Enum Class File will export to interface like java etc. (Not implement yet)

Example
    In vpg_object_property.hpp,
    
    ```
    #pragma once

    enum class VPGObjectProperty
    {
        EnumA, // Nothing
        EnumB, // GETSET(std::wstring, EnumB, "Default")
        EnumC, // GET(int64_t, EnumC, 0)
        EnumD, // GETSET(VPGEnumClass, EnumD, VPGEnumClass::OptionA)
        EnumE  // VECTOR(VPGEnumClass, EnumE)
    };
    ```

    In vpg_enum_class.hpp,
    ```
    #pragma once

    enum class VPGEnumClass
    {
        OptionA,
        OptionB,
        OptionC,
        OptionD
    };
    ```

    It will generate class file vpg_object.hpp with following content

    ```
    #pragma once

    #include <string>

    #include "base_object.hpp"
    #include "class_macro.hpp"
    #include "object_type.hpp"

    #include "vpg_enum_class.hpp"

    class VCCObject : public BaseObject<VCCObject>
    {
        GETSET(std::wstring, EnumB, L\"Default\")
        GET(int64_t, EnumC, 0)
        GETSET(VPGEnumClass, EnumD, VPGEnumClass::OptionA)
        VECTOR(EnumClass, EnumE)

        public:
            VCCObject() : BaseObject(ObjectType::Object) {}
            virtual ~VCCObject() {}
    };
    ```

****
## Versioning Common Codebase Project Generator VSCode Extension

### Pre-Requirement
Same as Versionin Common Codebase Project Generator

### Clone from git
After download from git
please enter following to terminal
```
npm install
```

### Procedure
1. Download VCCProject Generator sourcecode from GitHub to ~/Document/VCC folder if not exists.
2. Check the version of VCCProject Generator and Extension are the same. If not, switch to that version and Make. It will use some time to compile.
3. Execute the command listed in Options.

### Options
There are serveral options. (workspace means first workspace shown in project tree of VSCode)
Details refer to Versionin Common Codebase Project Generator.

1. Create C++ Complex Project
Execute: vpg -Add -interface CPPCOMPLEX -workspace-destination workspace

2. Create C++ EXE Project
Execute: vpg -Add -interface CPPEXE -workspace-destination workspace

3. Create C++ DLL Project
Execute: vpg -Add -interface CPPDLL -workspace-destination workspace

4. Create VCC Complex Project
Execute: vpg -Add -interface VCCCOMPLEX -workspace-destination workspace

5. Create VCC EXE Project
Execute: vpg -Add -interface VCCEXE -workspace-destination workspace

6. Create VCC DLL Project
Execute: vpg -Add -interface VCCDLL -workspace-destination workspace

7. Update VCC Project
Execute: vpg -Update -workspace-destination workspace

8. Generate VCC Project
Execute: vpg -Generate -workspace-destination workspace

****
## History
The project is start from 2022-06-22. As Document Versioning is needed but no solution can be found. Also, existing document processors have some interesting behaviour, such as

When save, will save from beginning.
Auto create new style which will make charactors hiden.
When press Enter at Title, the new line will have new style that make the content disappear.
When having outline then save, document cannot save correctly.
The Index cannot recognize item number. When save, item numbers disappear.
When handling thousand pages document, it lags. Some will hang and quit.
Docx cannot save as docx by default.
Cannot save in local storage by default.
Security Problem. etc.

As those problems have not been solve until 2022-06-22, I believe that they are properties instead of bugs. I better to write a document processor myself. When I try to write a document processor with versioning, I find that I should write a git application instead of document processor. But I need to write a document processor before creating a git application project. As most codes are mechanical work, I write a code generator to write those projects instead of writing one by one. So, I need to write 3 applications.

Firstly, I try to use SwiftUI + dynamic library first as I don’t need to create website and handle payment progess. But I can find no way to create virtual ListView.
Secondly, as SwiftUI seems difficult to me, I try to use C# + dynamic library as I am experienced in C#. But I found that Linux and MacOS does not support C# and Visual Studio does not support multi-platform, then I give up.
Thirdly, I try to use Java + dynamic library as Java is multiplatform. But I find that NetBeans has not supported C++ long time ago. 
Finally, I try to use VSCode with g++. Then I try about one month to setup the makefile. In fact, there are no templates for multiplatform with gtest. I need to study and write makefiles (both library and executable) from the beginning. In fact, I have used about 6 months to investigate. I think I better write a program to skip one year effort.

In fact, the first release is 2024-05-05, I have used one and a half year (although I can only write the code at weekend and after work) to write basic structure, which is not complete yet.

Hope everyone can skip at least 2 years effort and dont need to ask same question again and again.

## How to Contribute
It is my first git opensource project. In fact, I don't know what to do.
If you want to have new feature or report bugs, please tell me via GitHub or X(Twitter).
If you want to contribute, please create branch and tell me to merge when finished. I may need to modify the code to fit the style
Note that it is open source no license coorporation project, code will be modified without notification. 

## Contact
X(Twitter) @VCCProject

****
## Release Log
2024-05-19 v0.0.4
- Add make unittest in Makefile
- Initial Property Accessor
- Merge XML Reader to XML Builder
- Improve TRY CATCH macro

2024-05-06 v0.0.3
- Move vcc.json to .vcc/vcc.json
- Update Makefile to fix unittest name
- Trigger Update VCC Module will update Makefile according to vcc.json

2024-05-05 v0.0.2
- Fix cannot reexecute after switched to tag

2024-05-05 First Release: v0.0.1

2023-12-23 Combine VCCProjectGeneratorEXE and VCCProjectGeneratorDLL and rename to VCCProjectGenerator

2023-04-30 Initialization
