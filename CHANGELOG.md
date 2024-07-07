# Change Log

## [Unreleased]

### [v1.0.0] - 2024-07-07: Java Bridge: Prepare for generate Java Bridge
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
