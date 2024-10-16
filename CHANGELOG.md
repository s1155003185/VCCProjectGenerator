# Change Log

## [Unreleased]

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

### [v0.1.3] - 2024-07-28: Java Bridge: Generation - Dll Interface for Property Accessor
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
- Generator support Map (Property Accessor still cannot read key list)
- Rename HasPrefix, HasSuffix in string_helper.hpp to IsStartWith, IsEndWith

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
