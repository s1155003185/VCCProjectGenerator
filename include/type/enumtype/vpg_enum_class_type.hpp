#pragma once

enum class VPGEnumClassAttributeAccessMode
{
    ReadWrite,
    ReadOnly,
    WriteOnly,
    NoAccess
};

enum class VPGEnumClassType
{
    Object
    , Form
    , ActionArgument
    , Result
};

enum class VPGEnumClassAttributeType
{
    NA
    , Property
    , Manager
    , Action
};

enum class VPGEnumClassMacroType
{
    NA
    , Getcustom
    , Setcustom
    , GetcustomSptr
    , SetcustomSptr
    , Getset
    , GetsetValidate
    , GetsetSptr
    , GetsetSptrNull
    , GetsetValidateSptrNull
    , Vector
    , VectorValidate
    , VectorSptr
    , VectorValidateSptr
    , Set
    , SetValidate
    , SetSptr
    , SetValidateSptr
    , Map
    , MapValidate
    , MapSptrR
    , MapValidateSptrR
    , OrderedMap
    , OrderedMapValidate
    , OrderedMapSptrR
    , OrderedMapValidateSptrR
    , ManagerSptr
    , ManagerSptrNull
    , ManagerSptrParent
    , Action
    , ActionWithArgSptr
};
