# VCCModuel / VCCProjectGenerator / VCCProjectGenerator VSCode Extension
### Versioning Common Codebase Project / Versioning Coding Cooperation Project
Cross plaform project to handle Any Interface with C++ dll.
Maintain those standard already stable long time ago. No reason to implement twice.

Current Stage Objective: Start VCC Project Manager (Multi Project Handling), including Java Interface, Thread, Form, Action, Git.

Note: Still in initialize version, will have full review when official release

Note: For Generator, if version not match, it will drop the template in Document/VCC/VCCModule and clone new one.
    But in Window version, Drop Create Method not work as Git still holds the folder a few while after executing Git Command.
    If find that generator have generated wrong file, it may because of outdated template. Please remove the template and try again.
    For example, if folder name has been changed from Uppercase to Lowercase in Git Response, Git may not be able to update because git is not case sensitive.

Please go to following session to see how to create VCC Project to generate c++ dll with Java Interface
- Tutorial for Create VCC DLL Project to generate dll with Java Interface

## What's new
Generate Child Class (allow inherit)

## What's next
Form, Action

## Features
- Easy update project to model version instead of rewrite codebase. Just Update Project Genertor to newest version, execute Update and Generation.
- Create program structure like sql, the remaining is just handle specific logic.
- Mainly handle those already stable and optimize features so that dont need to implement from the beginning
- Generate Java Class and Enum Automatcally after writting c++ enum class and executing Generate VCC Project.

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

C++
1. git
2. g++
3. make
4. gtest

Optional if using Java Interface
1. Maven Project
2. Java 21 or above

### Work flow for Extension
1. Download VCCProjectGenerator by Git to ~/Document/VCC folder.
2. Use make to build VCCProjectGenerator.
3. Download template by Git.
4. Add / Update based on Template.

If dont want to download extra project, please download following git manually and follow instruction listed in Makefile to adjust.
1. Versionin Common Codebase Simple C++ Template (https://github.com/s1155003185/VCCSimpleTemplateCPP)
2. Versionin Common Codebase Module - VCCModule (https://github.com/s1155003185/VCCModule)

## Sample Program
VCCProjectGenerator is self writting program.
Please download from github (https://github.com/s1155003185/VCCProjectGenerator).
Then follow the instruction listed in README.md to change.

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
1. Config Default F5 is to execute "Debug"
    or
    In launch.json, copy cwd and program from "Debug" to "C/C++ Runner: Debug Session"
2. If want to execute exe instead of unittest, change "unittest" in "program" to your program name
3. F5

## Compare to AI Code Generation
Suitable for long term project
- Stable
- Reliable
- No need to test existing code
- No need to study the code after generation
- No need to upload code to service provider

****
## VCCModule Feature
### Core
-	Action Manager (Pending)
-	Exception
-	Helper (Keep update)
-	Log Service
-   Process Service
-	Property Accessor (Not Support set at the moment)
-   Terminal Service
-   Thread Service
-   XML (Read only, Other are still pending)
### Common
-   Git (Basic function only, will complete in next stage)
### Module
-	TextEditor (Pending)
-	Document (Pending)
-	Speedsheet (Pending)
### UI
-	LoginService (Pending)
-	LicenseService (Pending)
-	PaymentService (Pending)
### Generation By Generator
-   Auto Update Makefile according to vcc.json
-   Auto Copy dll to Java dll folder after make release (Note: JNA cannot read debug mode dll)
-   Auto Create C++ Class Structure, Property Accessor, DLL Interface based on enum in TypeWorkspace of c++ in vcc.json
-   Auto Create Java Class and Enum based on enum in TypeWorkspace of c++ in vcc.json

## Pending
- Use precompile header
- Swift bridge.
- Check style
- Auto generaste document for coding (like java)
- Enhance class generation for validation, trigger etc.
- Alert tag for Update
- OAuth

## Known Issue
- For Generator, if version not match, it will drop the template in Document/VCC/VCCModule and clone new one.
    But in Window version, Drop Create Method not work as Git still holds the folder a few while after executing Git Command.
    If find that generator have generated wrong file, it may because of outdated template. Please remove the template and try again.
    For example, if folder name has been changed from Uppercase to Lowercase in Git Response, Git may not be able to update because git is not case sensitive.
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
1. Versioning Coding Cooperation - Office (Not Implement Yet)
=> Free Version: Document, SpreedSheet, Text Editor
=> Subscription Version: Git, Multi-user workspace, Plugins etc.

## Versioning Coding Cooperation Project / Versioning Common Codebase Project Common Members
1. VCCProject Manager (Not Implement Yet)
=> Free Version: VCC Project Generator and normal git operation. Fixed that inherited project is VCC Project Module.
=> Subscription Version: Self-define inherited project, Multi project handling, Document Versioning.

****
## Versionin Common Codebase Project Generator

C++
1. git
2. g++
3. make
4. gtest

If using Java Interface
1. Maven Project
2. Java 21 or above

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
vpg -Update [-workspace-destination <workspace-destination>]

-workspace-destination <workspace-destination>
    Target workspace. If not state, then current workspace.

Description:
    Only for VCC Module. Other properties are overriden by vcc.json.
    First update template to the version that same as VCCProjectGenerator. Then compare different and update the files in workspace. Details: vcc.json, Update Rule.

### Command - Generate
vpg -Generate [-workspace-destination <workspace-destination>]

Description:
    Only for VCC Module. Other properties are overriden by vcc.json. Details: vcc.json, Generate Rule.

### vcc.json
For VCCModule, there is vcc.json. Update Mode and Generate Mode fully depends on vcc.json. Can include this to .gitignore file.
Path in the project is recommaneded to be in Camel Case. So, when export to java, it will fit java naming style.

Sample
```
{
    "Version": "v0.2.1",
    "ProjectType": "VccModule",
    "TemplateGitUrl": "https://github.com/s1155003185/VCCModule.git",
    "TemplateWorkspace": "${userHome}/Documents/vcc/VCCModule",
    "ProjectPrefix": "VPG",
    "ProjectName": "VCCProjGenerator",
    "ProjectNameDll": "libvpg",
    "ProjectNameExe": "vpg",
    "IsGit": true,
    "IsExcludeUnittest": false,
    "IsExcludeVCCUnitTest": false,
    "TypeWorkspace": "include/type",
    "ActionTypeDirectory": "include/type",
    "ExceptionTypeDirectory": "include/type",
    "ManagerTypeDirectory": "include/type",
    "ObjectTypeDirectory": "include/type",
    "ObjectDirectoryHpp": "include/model",
    "ObjectDirectoryCpp": "src/model",
    "PropertyAccessorDirectoryHpp": "include/propertyAccessor",
    "PropertyAccessorDirectoryCpp": "src/propertyAccessor",
    "ObjectFactoryDirectoryHpp": "include/factory",
    "ObjectFactoryDirectoryCpp": "src/factory",
    "PropertyAccessorFactoryDirectoryHpp": "include/factory",
    "PropertyAccessorFactoryDirectoryCpp": "src/factory",
    "Plugins": [
        "vcc/versioning/git"
    ],
    "Exports": [
        {
            "Interface": "Java",
            "Workspace": "../VCCProjectManagerBasic",
            "IsExportExternalLib": true,
            "ExportDirectoryDll": "src/main/resources",
            "ExportDirectoryExe": "",
            "DllBridgeDirectory": "src/main/java/com/vcc",
            "ObjectDirectory": "src/main/java/com/vcc/model",
            "TypeDirectory": "src/main/java/com/vcc/type"
        }
    ]
}
```

#### Properies
Version
    ReadOnly. Current VCCModule Version. VCCProjectGenerator and VCCModel version must be the same.

ProjectType
    VCCModule. Reserve for other project type, such as Java and Swift.

TemplateGitUrl
    Template Git source.

TemplateWorkspace
    Template workspace. Update mode will update current resource from the source in Template workspace.
    By default, value is "${userHome}/Documents/vcc/VCCModule". Generator will auto update ${userHome} to user home path.

ProjectPrefix
    In generation mode, generator only read file with that prefix and generate class with that prefix. It is used to distinguish different project if having multi dll.
    E.g. "ProjectPrefix": "VPG"
    Generator only read file have prefix vpg_* and generate class as VPGObject

ProjectName
    Project name. Update mode will also update Makefile.


ProjectNameDll
    DLL name. If does not compile dll, keep it empty.
    Update mode will auto update Makefile.
    Generate mode will update DllFunction.h.

ProjectNameExe
    EXE name. If does not compile exe, keep it empty.
    Update mode will auto update Makefile.

IsGit
    If true, then generate .gitignore. (Pending to init git)

IsExcludeUnittest
    If true, then skip update unittest/

IsExcludeVCCUnitTest
    If true, then skip update unittest/External/VCC/

TypeWorkspace
    In Generation mode, Generator will search files with suffix *_property.hpp to create Class, Property Accessor etc. Detail: Generation Rule.

ActionTypeDirectory, ExceptionTypeDirectory, ManagerTypeDirectory, ObjectTypeDirectory
    The location to export action_type.hpp, exception_type.hpp, manager_type.hpp, object_type.hpp
    All are used to export to interface. Cannot delete those files. action_type may be moved to External/VCC/Form/Action/ in the future such that action can be optional.

ObjectDirectoryHpp, ObjectDirectoryCpp
    In Generation mode, class files are generated here.
    
PropertyAccessorDirectoryHpp, PropertyAccessorDirectoryCpp
    In Generation mode, Property Accessor files are generated here.

ObjectFactoryDirectoryHpp, ObjectFactoryDirectoryCpp
    In Generation mode, Object Factory file is generated here.

PropertyAccessorFactoryDirectoryHpp, PropertyAccessorFactoryDirectoryCpp
    In Generation mode, Property Accessor Factory file is generated here.

Plugins
    In Update mode, Generator will copy folders under include/External, src/External, unittest/External to workspace.
    Option:
        vcc/versioning/git

Exports
    In Update Mode, Generator will update Makefile so that dll, exe will be copied after make release. Note: JNA cannot read debug mode dll.
    In Generate Mode, Generator will generate class, enum, bridge according to TypeWorkspace stated above.

    Interface
        Only accept "Java" at the moment. Pending to have Swift and Cpp

    Workspace
        Destination of Target Project. Recommand to use relative path.

    IsExportExternalLib
        true or false
        If true, copy all files in directory lib to with main exe and dll.
        
    ExportDirectoryDll
        Relative path based on Workspace.
        After changed, please trigger Update Mode to update Makefile.
        When make, dll file will be copied to the path specified.
        For Java, it must have prefix src/main/resources

    ExportDirectoryExe
        Relative path based on Workspace.
        After changed, please trigger Update Mode to update Makefile.
        When make, dll file will be copied to the path specified.

    DllBridgeDirectory
        Relative path based on Workspace.
        In Generation Mode, Dll bridge will be generated in path specified.
        In Java, it must have prefix src/main/java

    ObjectDirectory
        Relative path based on Workspace.
        In Generation Mode, Java class file will be generated in path specified.
        In Java, it must have prefix src/main/java

    TypeDirectory
        Relative path based on Workspace.
        In Generation Mode, Java class file will be generated in path specified.
        In Java, it must have prefix src/main/java

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
Note: 
1. Set is not supported at the moment.
Note:
1. For any not support type / macro e.g. SET macro, std::complex, etc. User may need to use ReadLock, WriteLock of Property Accessor to handle manually.
Sample in 
1. vpg_file_generation_manager_test.cpp
2. vpg_property_accessor_generation_service_test.cpp

Definition of Program Structure
    Class: C++ Class. Data storage.
    Factory: Generate class based on Enum.
    Form: Class with Manager. Call Manager to do action.
    Manager: Class with model. Call service to modify model.
    PropertyAccessor: Bridge between Control and model with thread safe access. Plending to have validation, trigger etc.
    Service: Class with static function only. It is used to provide logic.
    Structure: C++ Structure. Used to support huge data process. Not support at the moment.

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
    3. All Enum Class File will export to interface like java etc. Only support Java at the moment.

Note
    Under DllFunctions.h, user can have custom function with C basic type within extern "C" { } but the function must start with DLLEXPORT. When export java interface, the custom function also will be generated. But the usage of PointerByReference will be review later.
    Please do not write any codes within tag dllInterfaceHeader and dllInterface. They will be overriden by Generator.

    Sample: in DllFunctions.h
```
// <vcc:vccproj gen="DEMAND"/>
#ifndef DLL_FUNCTIONS_H
#define DLL_FUNCTIONS_H

#include <string>

#ifdef _WIN32
#define DLLEXPORT __declspec (dllexport) 
#else
#define DLLEXPORT extern 
#endif

// <vcc:dllInterfaceHeader gen="FORCE">
#include "object_factory.hpp"
#include "property_accessor_factory.hpp"
#include "property_accessor_macro.hpp"

using namespace vcc;
// </vcc:dllInterfaceHeader>

extern "C"
{

DLLEXPORT int GetVersion(wchar_t **str);

DLLEXPORT void *CreateObject(int64_t objectType);

// <vcc:dllInterface gen="FORCE">
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(bool, Bool)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(char, Char)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(wchar_t, Wchar)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(int, Int8)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(int, Uint8)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(int, Short)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(int, UnsignedShort)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(long, Int)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(long, UnsignedInt)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(long, Long)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(long, UnsignedLong)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(float, Float)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(double, Double)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_STRING
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_OBJECT
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_CONTAINER
// </vcc:dllInterface>
}

#endif
```

in DllFunctions.cpp
```
// <vcc:vccproj gen="DEMAND"/>
#include "DllFunctions.h"

#include <locale.h>
#include <stdio.h>
#include <wchar.h>


// <vcc:dllInterfaceHeader gen="FORCE">
#include "exception_macro.hpp"
#include "i_object.hpp"
#include "lock_type.hpp"
#include "property_accessor_factory.hpp"
#include "property_accessor_macro.hpp"

using namespace vcc;
// </vcc:dllInterfaceHeader>

int GetVersion(wchar_t **str)
{
    std::wstring versionString = L"v0.0.1";
    size_t size = (versionString.length() + 1) * sizeof(wchar_t);
    *str = static_cast<wchar_t*>(malloc(size));
    if (*str == nullptr) {
        return -1;
    }
    wcscpy(*str, versionString.c_str());
    return 0;
}

#include "object_factory.hpp"
#include "object_type.hpp"
std::shared_ptr<IObject> obj = nullptr;
void *CreateObject(int64_t objectType)
{
    ObjectType tmpObjectType = static_cast<ObjectType>(objectType);
    obj = ObjectFactory::Create(tmpObjectType);
    return obj.get();
}

// <vcc:dllInterface gen="FORCE">
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(bool, Bool, false)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(char, Char, L'\0')
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(wchar_t, Wchar, L'\0')
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(int, Int8, 0)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(int, Uint8, 0)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(int, Short, 0)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(int, UnsignedShort, 0)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(long, Int, 0)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(long, UnsignedInt, 0)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(long, Long, 0)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(long, UnsignedLong, 0)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(float, Float, 0)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(double, Double, 0)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL_STRING
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL_OBJECT
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL_CONTAINER
// </vcc:dllInterface>

```


#### Documentation
// {Class Attribute}
enum class ##ClassName##Property {
    EnumA, // {Field Attribute}
    EnumB, // {Field Attribute}
    EnumC // {Field Attribute}
};

Note:
1.  Enum Class must be in file with ##prefix##_##class_name##_property.hpp
    where ##prefix## must be the Project Prefix defined in vcc.json
    the file name must be end with _property.hpp
    e.g. vpg_person_property.hpp
2.  Enum Class must be defined with ##PREFIX##ClassName##Property
    e.g. VPGPersionProperty

#### Class Attribute
// [@@Json { "Key.NamingStyle" : "PascalCase", "Value.DecimalPlaces":2 }] [@@Inherit { "Class": "ClassName" }]

[]: Optional
@@: Key for attributes. Need to state for attribute
{}: Json format to describe Json file

[@@Json { "Key.NamingStyle" : "PascalCase", "Value.DecimalPlaces":2 }]
    Generate Class as Json Object. Class will have attribute ToJson, SerializeJson and DeserializeJson
    Attribute:
        Key.NamingStyle
            Value can be following
            | Value | Result |
            | --- | --- |
            | CamelCase | camelCase |
            | ConstantCase | CONSTANT_CASE |
            | DotSeparatedLowercase | dot.seperated.lowercase |
            | KebabCase | kebab-case |
            | Lowercase | lowercase |
            | PascalCase | PascalCase |
            | ScreamingSnakeCase | SCREAMING_SNAKE_CASE |
            | SnakeCase | snake_case |
            | Uppercase | UPPERCASE |

        Value.DecimalPlaces
            value is number. Declare for decimal places for double and float. If not declare, Json number will be trim tailing 0s.

[@@Inherit { "Class": "ClassName" }]
    Generate Class that inherit ClassName
    Attribute:
        Class
            Value is Parent Class,

#### Field Attribute
Enum // {ClassMacro} [@@AccessMode] [@@Inherit]

{...}: Compulsory
[]: Optional
@@: Key for attributes. Need to state for attribute

{Enum}
    Enum class enum. Used in property Accessor

{ClassMacro}
    Getter, Setter stated in class_macro.hpp. If it is not match with any class macro in class_macro.hpp, the rest will be ignored

[@@AccessMode]
    Default is @@ReadWrite.
    Option:
        @@ReadWrite
            Allow Read and Write through Property Accessor
        @@ReadOnly
            Allow Read Only via Property Accessor
        @@WriteOnly
            Allow Write Only via Property Accessor
        @@NoAccess
            Cannot Access via Property Accessor

[@@Inherit]
    If stated, generate will not generate Getter and Setter.

#### Example
    There is TypeWorkspace in vcc.json
    "TypeWorkspace": "include/Type"

    Create .hpp file under include/Type with suffix _property.hpp
    e.g. vpg_generation_option_property.hpp

```
// @@json
enum class VPGGenerationOptionExportProperty
{
    InterfaceType // GETSET(VPGGenerationOptionInterfaceType, Interface, VPGGenerationOptionInterfaceType::Java)
};

// @@json { "Key.NamingStyle" : "PascalCase" }
enum class VPGGenerationOptionProperty
{
    Version, // GETSET(std::wstring, Version, L"v0.0.1");
    ProjectType,  // GETSET(VPGProjectType, ProjectType, VPGProjectType::VccModule);
    WorkspaceSourceGitUrl, // GETSET(std::wstring, WorkspaceSourceGitUrl, L"");
    WorkspaceSource, // GETSET(std::wstring, WorkspaceSource, L"");
    WorkspaceDestination, // GETSET(std::wstring, WorkspaceDestination, L"");

    // --------------------------------------------------
    // Config
    // --------------------------------------------------
    // Project
    ProjectPrefix, // GETSET(std::wstring, ProjectPrefix, L"");

    ProjectName, // GETSET(std::wstring, ProjectName, L"VCCModule"); @@Command Need to assign Default Name first to pass validation
    ProjectNameDll, // GETSET(std::wstring, ProjectNameDll, L"libVCCModule"); @@Command Need to assign Default Name first to pass validation
    ProjectNameExe, // GETSET(std::wstring, ProjectNameExe, L"VCCModule"); @@Command Need to assign Default Name first to pass validation
    IsGit, // GETSET(bool, IsGit, false);

    IsExcludeUnittest, // GETSET(bool, IsExcludeUnittest, false);
    IsExcludeVCCUnitTest, // GETSET(bool, IsExcludeVCCUnitTest, false);

    // Files
    TypeWorkspace, // GETSET(std::wstring, TypeWorkspace, L"include/Type");

    ActionTypeDirectory, // GETSET(std::wstring, ActionTypeDirectory, L"include/Type");
    ExceptionTypeDirectory, // GETSET(std::wstring, ExceptionTypeDirectory, L"include/Type");
    ManagerTypeDirectory, // GETSET(std::wstring, ManagerTypeDirectory, L"include/Type");
    ObjectTypeDirectory, // GETSET(std::wstring, ObjectTypeDirectory, L"include/Type");

    ObjectDirectoryHpp, // GETSET(std::wstring, ObjectDirectoryHpp, L"include/Model");
    ObjectDirectoryCpp, // GETSET(std::wstring, ObjectDirectoryCpp, L"src/Model");
    PropertyAccessorDirectoryHpp, // GETSET(std::wstring, PropertyAccessorDirectoryHpp, L"include/PropertyAccessor");
    PropertyAccessorDirectoryCpp, // GETSET(std::wstring, PropertyAccessorDirectoryCpp, L"src/PropertyAccessor");

    ObjectFactoryDirectoryHpp, // GETSET(std::wstring, ObjectFactoryDirectoryHpp, L"include/Factory");
    ObjectFactoryDirectoryCpp, // GETSET(std::wstring, ObjectFactoryDirectoryCpp, L"src/Factory");
    PropertyAccessorFactoryDirectoryHpp, // GETSET(std::wstring, PropertyAccessorFactoryDirectoryHpp, L"include/Factory");
    PropertyAccessorFactoryDirectoryCpp, // GETSET(std::wstring, PropertyAccessorFactoryDirectoryCpp, L"src/Factory");
    
    // Plugins
    Plugins, // VECTOR(std::wstring, Plugins);

    // Export
    Exports // VECTOR_SPTR(VPGGenerationOptionExport, Exports);
};
```
    Then use generation. It will auto create class, property, factory, property accessor


### Export to Java
Assume that using Maven Java Project. The library will be compiled to resources folder.
For MacOS, only test with M Chips.

Please put following code to Pom. Only test for version >= 5.14.0.
```
    <dependencies>
        <dependency>
            <groupId>net.java.dev.jna</groupId>
            <artifactId>jna</artifactId>
            <version>5.14.0</version>
        </dependency>
    </dependencies>
```

Note: NetBeans cannot find net.java.dev.jna, It can only find old version com.sun.jna. After verison 3.0.0, con.sun.jna has been renamed as net.java.dev.jna but the import files still called com.sun.jna.
For the com.sun.jna version, it does not support Mac M Chips and throw linkage error.

Note: When using jna, when we put "vpg" as dll name, it will auto searh with the libaray with name libvpg.dylib under src/main/resources
Please having lib name with libxxx.dylib format

```
import com.sun.jna.Library;
import com.sun.jna.Native;

interface DllFunctions extends Library {
    DllFunctions INSTANCE = (DllFunctions) Native.load("vpg", DllFunctions.class);

    int GetVersion();
}
public class Test {

    public static void main(String[] args) {
        System.out.println(DllFunctions.INSTANCE.GetVersion());
    }
}
```

#### Java Type Mapping
According to JNA (Java Native Access), here is the map of types between c++ and Java. Better to use below type only. If not using below type, generator will use static cast.
| C++ Type | Java Type |
| --- | --- |
| char | byte |
| short | short |
| wchart_t | char |
| int | int |
| bool | boolean |
| long | NativeLong |
| long long | long |
| float | float |
| double | double |
| char* | String |
| void* | pointer |

## Java UI
Here are some notes for Java UI.

Please ensure the program have following lines in main.

Set UI style be nature current platform style.
```
try {
    UIManager.setLookAndFeel(UIManager.getCrossPlatformLookAndFeelClassName());
} 
catch (ClassNotFoundException | IllegalAccessException | InstantiationException | UnsupportedLookAndFeelException e) {
    // handle exception
}
```

Set MacOS menu bar be screen menu bar.
```
System.setProperty("apple.laf.useScreenMenuBar", "true");
```

Sample
```
/**
* @param args the command line arguments
*/
public static void main(String args[]) {
    try {
        UIManager.setLookAndFeel(UIManager.getCrossPlatformLookAndFeelClassName());
    } 
    catch (ClassNotFoundException | IllegalAccessException | InstantiationException | UnsupportedLookAndFeelException ex) {
        java.util.logging.Logger.getLogger(Main.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
    }
    // Set the property to use the screen menu bar on macOS
    System.setProperty("apple.laf.useScreenMenuBar", "true");
    
    /* Create and display the form */
    java.awt.EventQueue.invokeLater(() -> {
        new Main().setVisible(true);
    });
}
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
## Tutorial for Create VCC DLL Project to generate dll with Java Interface

1. For C++, create a folder and use VSCode open it.
2. For Java, create a folder next to C++ folder and create a Maven Project within the folder
    |- CppProjectFolder
    |- JavaProjectFolder
3. Place following code to pom.xml.

    <dependencies>
        <dependency>
            <groupId>net.java.dev.jna</groupId>
            <artifactId>jna</artifactId>
            <version>5.14.0</version>
        </dependency>
    </dependencies>

4. In C++ Project VSCode bar, type "> Create VCC DLL Project" and execute. VSCode will download corresponding VCC Project Generator from GitHub to ~/Documents/VCC in local computer.
5. Open vcc.json file, update ProjectPrefix, ProjectName, ProjectNameDll. ProjectNameDll must be started with prefix "lib" for JNA.
    In this tutorial, use VPG as Project Prefix.

    Under Exports, write config like following
    "Exports": [
        {
            "Interface": "Java",
            "Workspace": "../JavaProjectFolder",
            "IsExportExternalLib": true,
            "ExportDirectoryDll": "src/main/resources",
            "ExportDirectoryExe": "",
            "DllBridgeDirectory": "src/main/java/company/name",
            "ObjectDirectory": "src/main/java/company/name/model",
            "TypeDirectory": "src/main/java/com/company/name/type"
        }
    ]

    Note: company/name is company / organization name, please replace to what you need.
6. In C++ Project VSCode bar, type "> Update VCC Project" and execute.
7. Go to VSCode Explorer, under "include/Type", create a file with name vpg_test_property.hpp
8. In vpg_test_property.hpp, write code
```
#pragma once

enum class VPGTestProperty
{
    PropertyA // GETSET(std::wstring, PropertyA, L"")
};
```
Note: If file name is not end with _property.hpp but in type workspace folder, generate will not generate the c++ class and property accessor. But will also export to Java interface.
9. In C++ Project VSCode bar, type "> Generate VCC Project" and execute.
10. In terminal, type "make release -j10" and execute.
11. Generator has already been generated whole C++ Project structure and Java Project Structure.
Note: As C++ is using shared_ptr when handle object, Need to find some place to store the pointer first. Otherwise, you will get freed pointer in Java.
Note: At the moment, there is no place to create object and return pointer to Java, user need to create object manually. Create pointer problem will be handled in next stage (Form Handling).

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

### [v0.2.3] - 2024-10-20: Form - Generate Inherit Class
- Initialize Application
- Generate Inherit Class
- Rename LogProperty to LogConfig as class suffix and Property and file suffix _property.hpp are reserved
- Merge IProperty and BaseProperty to IObject and BaseObject. Remove i_property.hpp and base_property.hpp
- Support Generate Form and inerted Class from Type Workspace in vcc.json
- Fix Win Version Generator not work

### [v0.2.2] - 2024-10-12: Form - Urgent Fix
- Fix Generator not work

### [v0.2.1] - 2024-10-12: Form - Initialize Thread
- Rename VCCModule Folder to Camel Case to fit java naming style
- VCCProjectGenerator Generation mode update Makefile according to vcc.json
- Drop and Create template in Document/VCC folder if VCCProjectGenerator version not match with template instead of Git Pull
- Remove file form_type.hpp, merge FormType to ObjectType

### [v0.2.0] - 2024-10-03: Form - Initialize Thread
- Initialize Thread, ThreadService, ThreadManager
- LogService add LogDebug and LogThread
- ActionType, ExceptionType, FormType, ManagerType, ObjectType add custom tag

### [v0.1.5] - 2024-09-17: Java Bridge: Generation - Auto Create Java Class and Enum according
- Use void * instead of ITypeUnion for Property Accessor Map Key.
- Generate Java Class according to C++ Enum Class under Type Workspace.
- Update vcc.json and Makefile to export dll, exe to Java project
- Update vcc.json TemplateWorkspace use ${userHome} instead of hardcode.
- Auto copy dll / exe to Java Project
- Fix Update Mode not working because of using // <vcc:exceptionType action:"RESERVE"> in exception_type.hpp instead of // <vcc:exceptionType action="RESERVE">

### [v0.1.4] - 2024-08-25: Java Bridge: Generation - Java Bridege for Enum
- Generate Java Enum according to C++ Enum Class under Type Workspace.

### [v0.1.3] - 2024-07-28: Generation - Dll Interface for Property Accessor
- Remove support of std::string, all change to use std::wstring
- Generate Dll Interface for Property Accessor
- vcc.json remove WorkspaceDestination, change WorkspaceSourceGitUrl to TemplateGitUrl, WorkspaceSource to TemplateWorkspace
Note: Generation mode need to change WorkspaceDestination if using different PC.

### [v0.1.2] - 2024-07-14: Java Bridge: Generation - Create Class as Json Object with 
- Update Generator: Create Table as Json Object

### [v0.1.1] - 2024-07-08: Java Bridge: Self Writing
- Fix Win Compile
- Update Generator be Self-writting project

### [v0.1.0] - 2024-07-07: Java Bridge: Prepare for generate Java Bridge
- Makefile MacOS extension changed from .so to .dylib
- vcc.json rename ModelDirectory to ObjectDirectory, ObjectDirectory to ObjectDirectoryHpp, added ObjectDirectoryCpp
- Object Factory, Property Accessor Factory
- Generate Class Object with Json Serialize and Deserialize

### [v0.0.5] - 2024-06-02: Basic Strucute: Property Accessor and Thread safe
- Generate Mode supports generate Property Accessor ReadWrite, ReadOnly, WriteOnly, NoAccess
- Class Macro remote STATIC_GET and GET, all class properties must support Get Set and the access mode is controlled by Property Accessor
- Generator support Map (Property Accessor cannot read key list )

### [v0.0.4] - 2024-05-19: Basic Strucute: Merge XML and Improve Try Catch
- Add make unittest in Makefile
- Initial Property Accessor
- Merge XML Reader to XML Builder
- Improve TRY CATCH macro

### [v0.0.3] 2024-05-06: Basic Strucute: Update project according to vcc.json
- Move vcc.json to .vcc/vcc.json
- Update Makefile to fix unittest name
- Trigger Update VCC Module will update Makefile according to vcc.json

### [v0.0.2] 2024-05-05: Basic Strucute: Urgent Fix
- Fix cannot reexecute after switched to tag

### [v0.0.1] 2024-05-05: Basic Strucute: Initialize
- Initialize
