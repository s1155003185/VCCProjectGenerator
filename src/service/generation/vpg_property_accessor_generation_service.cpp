#include "vpg_property_accessor_generation_service.hpp"

#include <set>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "vpg_class_helper.hpp"

using namespace vcc;

#define LOG_ID L"Property Accessor Generation"
const std::wstring containerToken = L"<Container>";
const std::wstring objectToken = L"<Object>";
const std::wstring enumToken = L"<Enum>";

const std::wstring generalTypeContentFooter = INDENT + INDENT + L"default:\r\n"
                + INDENT + INDENT + INDENT + L"assert(false);\r\n"
                + INDENT + INDENT + L"}\r\n";

std::wstring GetGeneralTypeContentHeader(const std::wstring &classPropertyName)
{
    const std::wstring className = GetClassNameFromPropertyClassName(classPropertyName);
    return INDENT + INDENT + L"auto obj = std::static_pointer_cast<" + className + L">(_Object);\r\n"
        + INDENT + INDENT + L"assert(obj != nullptr);\r\n"
        + INDENT + INDENT + L"switch(static_cast<" + classPropertyName + L">(objectProperty))\r\n"
        + INDENT + INDENT + L"{\r\n";
}

std::wstring GetGeneralTypeIndexContentHeader(const std::wstring &classPropertyName)
{
    const std::wstring className = GetClassNameFromPropertyClassName(classPropertyName);
    return INDENT + INDENT + L"assert(index >= -1);\r\n"
        + INDENT + INDENT + L"auto obj = std::static_pointer_cast<" + className + L">(_Object);\r\n"
        + INDENT + INDENT + L"assert(obj != nullptr);\r\n"
        + INDENT + INDENT + L"switch(static_cast<" + classPropertyName + L">(objectProperty))\r\n"
        + INDENT + INDENT + L"{\r\n";
}

std::wstring GetGeneralTypeMapContentHeader(const std::wstring &classPropertyName)
{
    const std::wstring className = GetClassNameFromPropertyClassName(classPropertyName);
    return INDENT + INDENT + L"auto obj = std::static_pointer_cast<" + className + L">(_Object);\r\n"
        + INDENT + INDENT + L"assert(obj != nullptr);\r\n"
        + INDENT + INDENT + L"assert(key != nullptr);\r\n"
        + INDENT + INDENT + L"switch(static_cast<" + classPropertyName + L">(objectProperty))\r\n"
        + INDENT + INDENT + L"{\r\n";
}

std::wstring GetGeneralContentHeader(const std::wstring &classPropertyName)
{
    const std::wstring className = GetClassNameFromPropertyClassName(classPropertyName);
    return INDENT + INDENT + L"auto obj = std::static_pointer_cast<" + className + L">(_Object);\r\n"
        + INDENT + INDENT + L"assert(obj != nullptr);\r\n"
        + INDENT + INDENT + L"switch(static_cast<" + classPropertyName + L">(objectProperty))\r\n"
        + INDENT + INDENT + L"{\r\n";
}

bool VPGPropertyAccessorGenerationService::IsIncludeString(const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList)
{
    bool result = false;
    TRY
        for (auto const &enumClass : enumClassList) {
            for (auto const &property : enumClass->GetProperties()) {
                if (Find(property->GetType1(), L"string") != std::wstring::npos
                    || Find(property->GetType2(), L"string") != std::wstring::npos) {
                    result = true;
                    break;
                }
            }
            if (result)
                break;
        }
    CATCH
    return result;
}

void VPGPropertyAccessorGenerationService::GetPropertyAccessorTypeName(const std::wstring &originalType, std::wstring &convertedType, std::wstring &convertedName, std::wstring &returnResult)
{        
    TRY
        if (originalType == objectToken) {
            convertedType = L"std::shared_ptr<IObject>";
            convertedName = L"Object";
            returnResult = L"nullptr";
        } else if (originalType == L"bool") {
            convertedType = L"bool";
            convertedName = L"Bool";
            returnResult = L"false";
        } else if (originalType == L"char"
            || originalType == L"byte") {
            convertedType = L"char";
            convertedName = L"Char";
            returnResult = L"'\0'";
        } else if (originalType == L"wchar_t") {
            convertedType = L"wchar_t";
            convertedName = L"Wchar";
            returnResult = L"L'\0'";
        } else if (originalType == L"int8_t") {
            convertedType = L"int8_t";
            convertedName = L"Int8";
            returnResult = L"0";
        } else if (originalType == L"uint8_t") {
            convertedType = L"uint8_t";
            convertedName = L"Uint8";
            returnResult = L"0";
        } else if (originalType == L"short"
            || originalType == L"int16_t") {
            convertedType = L"short";
            convertedName = L"Short";
            returnResult = L"0";
        } else if (originalType == L"uint16_t"
            || originalType == L"unsigned short"
            || originalType == L"unsigned int16_t") {
            convertedType = L"uint16_t";
            convertedName = L"UnsignedShort";
            returnResult = L"0";
        } else if (originalType == L"int"
            || originalType == L"unsigned int32_t") {
            convertedType = L"int";
            convertedName = L"Int";
            returnResult = L"0";
        } else if (originalType == L"uint32_t"
            || originalType == L"unsigned int"
            || originalType == L"unsigned int32_t") {
            convertedType = L"uint32_t";
            convertedName = L"UnsignedInt";
            returnResult = L"0";
        } else if (originalType == L"long"
            || originalType == L"int64_t"
            || originalType == L"time_t"
            || originalType == enumToken
            || (!originalType.empty() && IsCapital(originalType) /* enum */)) {
            convertedType = L"long";
            convertedName = L"Long";
            returnResult = L"0L";
        } else if (originalType == L"size_t"
            || originalType == L"unsigned long"
            || originalType == L"unsigned int64_t") {
            convertedType = L"size_t";
            convertedName = L"UnsignedLong";
            returnResult = L"0UL";
        } else if (originalType == L"float") {
            convertedType = L"float";
            convertedName = L"Float";
            returnResult = L"0";
        } else if (originalType == L"double") {
            convertedType = L"double";
            convertedName = L"Double";
            returnResult = L"0";
        } else if (originalType == L"std::string"
            || originalType == L"std::wstring") {
            convertedType = L"std::wstring";
            convertedName = L"String";
            returnResult = L"L\"\"";
        } else
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"Unknown type: " + originalType);
    CATCH
}

std::vector<std::wstring> VPGPropertyAccessorGenerationService::GetPropertyAccessorTypeOrder()
{
    std::vector<std::wstring> result;
    result.push_back(L"bool");
    result.push_back(L"char");
    result.push_back(L"wchar_t");
    result.push_back(L"int8_t");
    result.push_back(L"uint8_t");
    result.push_back(L"short");
    result.push_back(L"uint16_t");
    result.push_back(L"int");
    result.push_back(L"uint32_t");
    result.push_back(L"long");
    result.push_back(L"size_t");
    result.push_back(L"float");
    result.push_back(L"double");
    result.push_back(L"std::wstring");
    result.push_back(objectToken);
    result.push_back(containerToken);
    return result;
}

std::wstring VPGPropertyAccessorGenerationService::GetIncludeFile(const std::map<std::wstring, std::wstring> &projectClassIncludeFiles, const std::wstring &className)
{
    TRY
        if (IsContain(projectClassIncludeFiles, className))
            return projectClassIncludeFiles.at(className);
        else if (IsContain(projectClassIncludeFiles, L"vcc::" + className))
            return projectClassIncludeFiles.at(L"vcc::" + className);
    CATCH
    return L"";
}

void VPGPropertyAccessorGenerationService::GenerateHpp(const LogConfig *logConfig, const std::wstring &projectPrefix,
    const std::wstring &filePathHpp, const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList)
{
    TRY
        LogService::LogInfo(logConfig, LOG_ID, L"Generate property accessor hpp file: " + filePathHpp);

        std::wstring result = L"#pragma once\r\n\r\n";

        if (VPGPropertyAccessorGenerationService::IsIncludeString(enumClassList))
            result += L"#include <string>\r\n\r\n";

        result += L"#include \"base_property_accessor.hpp\"\r\n"
            "#include \"property_accessor_macro.hpp\"\r\n"
            "\r\n"
            "using namespace vcc;\r\n";

        for (auto const &enumClass : enumClassList) {
            if (!IsPropertyClass(enumClass->GetName(), projectPrefix))
                continue;
            std::vector<std::shared_ptr<VPGEnumClassProperty>> readWrite;
            for (auto property : enumClass->GetProperties()) {
                if (property->GetAccessMode() != VPGEnumClassPropertyAccessMode::NoAccess)
                    readWrite.push_back(property);
            }

            std::wstring className = enumClass->GetName() + L"Accessor";
            result += L"\r\n"
                "class " + className + L" : public BasePropertyAccessor\r\n"
                "{\r\n";
            bool isCollection = false;
            bool isObject = false;
            std::map<std::wstring, std::wstring> types;
            for (auto const &property : readWrite) {
                if (property->GetPropertyType() != VPGEnumClassPropertyType::Property)
                    continue;

                // property accessor not support set
                if (property->GetIsSet()) {
                    LogService::LogWarning(logConfig, L"Property Accessor Generation Service", L"Property Accessor not support SET: class " + enumClass->GetName() + L": " + property->GetMacro());
                    continue;
                }
                
                isCollection |= property->GetIsCollection();
                isObject |= property->GetIsObject();
                if (!property->GetIsObject()) {
                    std::wstring type = property->GetIsMap() || property->GetIsOrderedMap() ? property->GetType2() : property->GetType1();
                    if (type.empty())
                        THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"Unknow Macro found in " + enumClass->GetName() + L": " + property->GetMacro());
                    
                    std::wstring convertedType = L"";
                    std::wstring converttedName = L"";
                    std::wstring returnResult = L"";
                    VPGPropertyAccessorGenerationService::GetPropertyAccessorTypeName(type, convertedType, converttedName, returnResult);
                    if (types.find(convertedType) == types.end())
                        types.insert(std::make_pair(convertedType, converttedName));
                }
            }

            for (auto const &type : GetPropertyAccessorTypeOrder()) {
                if (types.find(type) != types.end())
                    result += INDENT + L"PROPERTY_ACCESSOR_HEADER(" + type + L", " + types[type] + L")\r\n";
            }

            if (isObject)
                result += INDENT + L"PROPERTY_ACCESSOR_OBJECT_HEADER(std::shared_ptr<IObject>, Object)\r\n";
            if (isCollection)
                result += L"\r\n" + INDENT + L"PROPERTY_ACCESSOR_CONTAINER_HEADER\r\n";
            
            result += L"\r\n"
                + INDENT + L"public:\r\n"
                + INDENT + INDENT + className + L"(std::shared_ptr<IObject> object) : BasePropertyAccessor(object) {}\r\n"
                + INDENT + INDENT + L"virtual ~" + className + L"() {}\r\n"
                "};\r\n";
        }

        WriteFile(filePathHpp, result, true);
        LogService::LogInfo(logConfig, LOG_ID, L"Generate property accessor hpp completed.");
    CATCH
}

void VPGPropertyAccessorGenerationService::GenerateRead(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassProperty>> &enumClassPropertiesReadOnly, std::wstring &result)
{
    if (type == containerToken || enumClassPropertiesReadOnly.empty())
        return;
    TRY
        std::wstring convertedType = L"";
        std::wstring convertedName = L"";
        std::wstring returnResult = L"";
        VPGPropertyAccessorGenerationService::GetPropertyAccessorTypeName(type, convertedType, convertedName, returnResult);

        std::map<std::wstring, std::wstring> generalCases;
        std::map<std::wstring, std::wstring> vectorCases;
        std::map<std::wstring, std::wstring> mapCases;
        std::map<std::wstring, std::wstring> mapCasekeyTypes;

        for (auto const &property : enumClassPropertiesReadOnly) {
            bool isOrderedMap = property->GetIsOrderedMap();
            // General
            if (property->GetIsGeneralType()) {
                std::wstring returnStr = L"obj->Get" + property->GetPropertyName() + L"()";
                if (type == objectToken)
                    returnStr = L"std::static_pointer_cast<IObject>(" + returnStr + L")";
                else if (!property->GetType1().empty() && IsCapital(property->GetType1()))
                    returnStr = L"static_cast<long>(" + returnStr + L")";
                else if (property->GetType1() == L"std::string")
                    returnStr = L"str2wstr(" + returnStr + L")";
                generalCases.insert(std::make_pair(property->GetEnum(), returnStr));
            }
            // Vector
            if (property->GetIsVector() || property->GetIsOrderedMap()) {
                std::wstring returnStr = L"obj->Get" + property->GetPropertyName();
                std::wstring tmpConvertedType = L"";
                std::wstring tmpConvertedName = L"";
                std::wstring tmpReturnResult = L"";
                VPGPropertyAccessorGenerationService::GetPropertyAccessorTypeName(property->GetType1(), tmpConvertedType, tmpConvertedName, tmpReturnResult);
                
                if (type == objectToken)
                    returnStr = L"std::static_pointer_cast<IObject>(" + returnStr + L"AtIndex(index)" + (isOrderedMap ? L".second" : L"") + L")";
                else {
                    std::wstring type = isOrderedMap ? property->GetType2() : property->GetType1();
                    if (!type.empty() && IsCapital(type))
                        returnStr = L"static_cast<long>(" + returnStr + L"AtIndex(index)"+ (isOrderedMap ? L".second" : L"") + L")";
                    else if (type == L"std::string")
                        returnStr = L"str2wstr(" + returnStr + L"AtIndex(index)" + (isOrderedMap ? L".second" : L"") + L")";
                    else
                        returnStr = returnStr + L"AtIndex(index)" + (isOrderedMap ? L".second" : L"");
                }
                vectorCases.insert(std::make_pair(property->GetEnum(), returnStr));
            }
            // Map
            if (property->GetIsMap() || property->GetIsOrderedMap()) {
                // key
                std::wstring mapGetKey = property->GetType1() == L"std::wstring" ? L"keyPtr" : L"*keyPtr";
                std::wstring castType = property->GetType1() == L"std::wstring" ? L"wchar_t" : property->GetType1();
                mapCasekeyTypes.insert(std::make_pair(property->GetEnum(), castType));

                // value
                std::wstring returnStr = L"obj->Get" + property->GetPropertyName();
                if (type == objectToken)
                    returnStr = L"std::static_pointer_cast<IObject>(" + returnStr + L"AtKey(" + mapGetKey + L")" + (isOrderedMap ? L".second" : L"") + L")";
                else if (!property->GetType2().empty() && IsCapital(property->GetType2()))
                    returnStr = L"static_cast<long>(" + returnStr + L"AtKey(" + mapGetKey + L")" + (isOrderedMap ? L".second" : L"") + L")";
                else if (property->GetType2() == L"std::string")
                    returnStr = L"str2wstr(" + returnStr + L"AtKey(" + mapGetKey + L")"+ (isOrderedMap ? L".second" : L"") + L")";
                else
                    returnStr = returnStr + L"AtKey(" + mapGetKey + L")"+ (isOrderedMap ? L".second" : L"");
                mapCases.insert(std::make_pair(property->GetEnum(), returnStr));
            }
        }

        // General Type
        result += L"\r\n"
            + convertedType + L" " + propertyName + L"Accessor::_Read" + convertedName + L"(const int64_t &objectProperty) const\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n";
        if (!generalCases.empty()) {
            result += GetGeneralTypeContentHeader(propertyName);
            for (auto const &pair : generalCases)
                result += INDENT + INDENT + L"case " + propertyName + L"::" + pair.first + L":\r\n"
                    + INDENT + INDENT + INDENT + L"return " + pair.second + L";\r\n";
            result += generalTypeContentFooter;
        } else
            result += INDENT + INDENT + L"THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n";
        result += INDENT + L"CATCH\r\n"
            + INDENT + L"return " + returnResult + L";\r\n"
            "}\r\n";

        // Vector Type
        result += L"\r\n"
            + convertedType + L" " + propertyName + L"Accessor::_Read" + convertedName + L"AtIndex(const int64_t &objectProperty, const int64_t &" + (!vectorCases.empty() ? L"index" : L"/*index*/") + L") const\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n";
        if (!vectorCases.empty()) {
            result += GetGeneralTypeIndexContentHeader(propertyName);
            for (auto const &pair : vectorCases)
                result += INDENT + INDENT + L"case " + propertyName + L"::" + pair.first + L":\r\n"
                    + INDENT + INDENT + INDENT + L"return " + pair.second + L";\r\n";
            result += generalTypeContentFooter;
        } else
            result += INDENT + INDENT + L"THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n";
        result += INDENT + L"CATCH\r\n"
            + INDENT + L"return " + returnResult + L";\r\n"
            "}\r\n";

        // Map Type
        result += L"\r\n"
            + convertedType + L" " + propertyName + L"Accessor::_Read" + convertedName + L"AtKey(const int64_t &objectProperty, const void *" + (!mapCases.empty() ? L"key" : L"/*key*/") + L") const\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n";
        if (!mapCases.empty()) {
            result += GetGeneralTypeMapContentHeader(propertyName);
            for (auto const &pair : mapCases)
                result += INDENT + INDENT + L"case " + propertyName + L"::" + pair.first + L": {\r\n"
                    + INDENT + INDENT + INDENT + L"auto keyPtr = static_cast<const " + mapCasekeyTypes.at(pair.first) + L" *>(key);\r\n"
                    + INDENT + INDENT + INDENT + L"assert(keyPtr != nullptr);\r\n"
                    + INDENT + INDENT + INDENT + L"if (keyPtr == nullptr)\r\n"
                    + INDENT + INDENT + INDENT + INDENT + L"THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
                    + INDENT + INDENT + INDENT + L"return " + pair.second + L";\r\n"
                    + INDENT + INDENT + L"}\r\n";
            result += generalTypeContentFooter;
        } else
            result += INDENT + INDENT + L"THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n";
        result += INDENT + L"CATCH\r\n"
            + INDENT + L"return " + returnResult + L";\r\n"
            "}\r\n";
    CATCH
}

void VPGPropertyAccessorGenerationService::GenerateWrite(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassProperty>> &enumClassPropertiesWriteOnly, std::wstring &result)
{
    if (type == containerToken || enumClassPropertiesWriteOnly.empty())
        return;
    TRY
        std::wstring convertedType = L"";
        std::wstring convertedName = L"";
        std::wstring returnResult = L"";
        VPGPropertyAccessorGenerationService::GetPropertyAccessorTypeName(type, convertedType, convertedName, returnResult);

        std::map<std::wstring, std::wstring> generalCases;
        std::map<std::wstring, std::wstring> vectorCases;
        std::map<std::wstring, std::wstring> mapCases;
        std::map<std::wstring, std::wstring> mapCasekeyTypes;

        for (auto const &property : enumClassPropertiesWriteOnly) {
            // General
            if (property->GetIsGeneralType()) {
                std::wstring returnStr = L"obj->Set" + property->GetPropertyName();

                if (type == objectToken)
                    returnStr += L"(std::static_pointer_cast<" + property->GetType1() + L">(value));\r\n";
                else if (!property->GetType1().empty() && IsCapital(property->GetType1()))
                    returnStr += L"(static_cast<" + property->GetType1() + L">(value));\r\n";
                else if (property->GetType1() == L"std::string")
                    returnStr += L"(wstr2str(value));\r\n";
                else
                    returnStr += L"(value);\r\n";
                generalCases.insert(std::make_pair(property->GetEnum(), returnStr));
            }
            // Vector
            if (property->GetIsVector() || property->GetIsOrderedMap()) {
                std::wstring returnStr = L"";

                bool isOrderedMap = property->GetIsOrderedMap();
                std::wstring type = isOrderedMap ? property->GetType2() : property->GetType1();
                if (property->GetIsObject()) {
                    returnStr = INDENT + INDENT + INDENT + L"if (index > -1)\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->Set" + property->GetPropertyName() + L"AtIndex(index, std::static_pointer_cast<" + type + L">(value));\r\n"
                        + INDENT + INDENT + INDENT + L"else\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->Insert" + property->GetPropertyName() + L"(std::static_pointer_cast<" + type + L">(value));\r\n";
                } else if (!type.empty() && IsCapital(type)) {
                    returnStr = INDENT + INDENT + INDENT + L"if (index > -1)\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->Set" + property->GetPropertyName() + L"AtIndex(index, static_cast<" + type + L">(value));\r\n"
                        + INDENT + INDENT + INDENT + L"else\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->Insert" + property->GetPropertyName() + L"(static_cast<" + type + L">(value));\r\n";
                } else if (type == L"std::string") {
                    returnStr = INDENT + INDENT + INDENT + L"if (index > -1)\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->Set" + property->GetPropertyName() + L"AtIndex(index, wstr2str(value));\r\n"
                        + INDENT + INDENT + INDENT + L"else\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->Insert" + property->GetPropertyName() + L"(wstr2str(value));\r\n";                    
                } else {
                    returnStr = INDENT + INDENT + INDENT + L"if (index > -1)\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->Set" + property->GetPropertyName() + L"AtIndex(index, value);\r\n"
                        + INDENT + INDENT + INDENT + L"else\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->Insert" + property->GetPropertyName() + L"(value);\r\n";
                };

                vectorCases.insert(std::make_pair(property->GetEnum(), returnStr));
            }
            // Map
            if (property->GetIsMap() || property->GetIsOrderedMap()) {
                // key
                std::wstring mapGetKey = property->GetType1() == L"std::wstring" ? L"keyPtr" : L"*keyPtr";
                std::wstring castType = property->GetType1() == L"std::wstring" ? L"wchar_t" : property->GetType1();
                mapCasekeyTypes.insert(std::make_pair(property->GetEnum(), castType));

                // value
                std::wstring returnStr = L"";
                if (type == objectToken) {
                    returnStr += INDENT + INDENT + INDENT + L"if (obj->Is" + property->GetPropertyName() + L"ContainKey(" + mapGetKey + L"))\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->Set" + property->GetPropertyName() + L"AtKey(" + mapGetKey + L", std::static_pointer_cast<" + property->GetType2() + L">(value));\r\n"
                        + INDENT + INDENT + INDENT + L"else\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->Insert" + property->GetPropertyName() + L"(" + mapGetKey + L", std::static_pointer_cast<" + property->GetType2() + L">(value));\r\n";
                } else {
                    std::wstring type = property->GetType2();
                    if (!type.empty() && IsCapital(type)) {
                        returnStr += INDENT + INDENT + INDENT + L"if (obj->Is" + property->GetPropertyName() + L"ContainKey(" + mapGetKey + L"))\r\n"
                            + INDENT + INDENT + INDENT + INDENT + L"obj->Set" + property->GetPropertyName() + L"AtKey(" + mapGetKey + L", static_cast<" + type + L">(value));\r\n"
                            + INDENT + INDENT + INDENT + L"else\r\n"
                            + INDENT + INDENT + INDENT + INDENT + L"obj->Insert" + property->GetPropertyName() + L"(" + mapGetKey + L", static_cast<" + type + L">(value));\r\n";
                    } else if (type == L"std::string") {
                        returnStr += INDENT + INDENT + INDENT + L"if (obj->Is" + property->GetPropertyName() + L"ContainKey(" + mapGetKey + L"))\r\n"
                            + INDENT + INDENT + INDENT + INDENT + L"obj->Set" + property->GetPropertyName() + L"AtKey(" + mapGetKey + L", str2wstr(value));\r\n"
                            + INDENT + INDENT + INDENT + L"else\r\n"
                            + INDENT + INDENT + INDENT + INDENT + L"obj->Insert" + property->GetPropertyName() + L"(" + mapGetKey + L", wstr2str(value));\r\n";
                    } else {
                        returnStr += INDENT + INDENT + INDENT + L"if (obj->Is" + property->GetPropertyName() + L"ContainKey(" + mapGetKey + L"))\r\n"
                            + INDENT + INDENT + INDENT + INDENT + L"obj->Set" + property->GetPropertyName() + L"AtKey(" + mapGetKey + L", value);\r\n"
                            + INDENT + INDENT + INDENT + L"else\r\n"
                            + INDENT + INDENT + INDENT + INDENT + L"obj->Insert" + property->GetPropertyName() + L"(" + mapGetKey + L", value);\r\n";
                    }                    
                }
                mapCases.insert(std::make_pair(property->GetEnum(), returnStr));
            }
        }

        // General Type
        result += L"\r\n"
            "void " + propertyName + L"Accessor::_Write" + convertedName + L"(const int64_t &objectProperty, " + (IsEqual(type, objectToken) ? L"" : L"const ")  + convertedType + L" " + (IsEqual(type, objectToken) ? L"" : L"&") + (!generalCases.empty() ? L"value" : L"/*value*/") + L") const\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n";
        if (!generalCases.empty()) {
            result += GetGeneralTypeContentHeader(propertyName);
            for (auto const &pair : generalCases)
                result += INDENT + INDENT + L"case " + propertyName + L"::" + pair.first + L":\r\n"
                    + INDENT + INDENT + INDENT + pair.second
                    + INDENT + INDENT + INDENT + L"break;\r\n";
            result += generalTypeContentFooter;
        } else
            result += INDENT + INDENT + L"THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n";
        result += INDENT + L"CATCH\r\n"
            "}\r\n";

        // vector
        result += L"\r\n"
            "void " + propertyName + L"Accessor::_Write" + convertedName + L"AtIndex(const int64_t &objectProperty, " + (IsEqual(type, objectToken) ? L"" : L"const ")  + convertedType + L" " + (IsEqual(type, objectToken) ? L"" : L"&") + (!vectorCases.empty() ? L"value" : L"/*value*/") + L", const int64_t &" + (!vectorCases.empty() ? L"index" : L"/*index*/") + L") const\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n";
        if (!vectorCases.empty()) {
            result += GetGeneralTypeIndexContentHeader(propertyName);
            for (auto const &pair : vectorCases)
                result += INDENT + INDENT + L"case " + propertyName + L"::" + pair.first + L":\r\n"
                    + pair.second
                    + INDENT + INDENT + INDENT + L"break;\r\n";
            result += generalTypeContentFooter;
        } else
            result += INDENT + INDENT + L"THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n";
        result += INDENT + L"CATCH\r\n"
            "}\r\n";

        // map
        result += L"\r\n"
            "void " + propertyName + L"Accessor::_Write" + convertedName + L"AtKey(const int64_t &objectProperty, " + (IsEqual(type, objectToken) ? L"" : L"const ")  + convertedType + L" " + (IsEqual(type, objectToken) ? L"" : L"&") + (!mapCases.empty() ? L"value" : L"/*value*/") + L", const void *" + (!mapCases.empty() ? L"key" : L"/*key*/") + L") const\r\n"
            "{\r\n"
                + INDENT + L"TRY\r\n";
        if (!mapCases.empty()) {
            result += GetGeneralTypeMapContentHeader(propertyName);
            for (auto const &pair : mapCases)
                result += INDENT + INDENT + L"case " + propertyName + L"::" + pair.first + L": {\r\n"
                    + INDENT + INDENT + INDENT + L"auto keyPtr = static_cast<const " + mapCasekeyTypes.at(pair.first) + L" *>(key);\r\n"
                    + INDENT + INDENT + INDENT + L"assert(keyPtr != nullptr);\r\n"
                    + INDENT + INDENT + INDENT + L"if (keyPtr == nullptr)\r\n"
                    + INDENT + INDENT + INDENT + INDENT + L"THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
                    + pair.second
                    + INDENT + INDENT + INDENT + L"break;\r\n"
                    + INDENT + INDENT + L"}\r\n";
            result += generalTypeContentFooter;
        } else
            result += INDENT + INDENT + L"THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n";        
        result += INDENT + L"CATCH\r\n"
                "}\r\n";
    CATCH
}

void VPGPropertyAccessorGenerationService::GenerateInsert(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassProperty>> &enumClassPropertiesWriteOnly, std::wstring &result)
{
    if (type == containerToken || enumClassPropertiesWriteOnly.empty())
        return;
    TRY
        std::wstring convertedType = L"";
        std::wstring convertedName = L"";
        std::wstring returnResult = L"";
        VPGPropertyAccessorGenerationService::GetPropertyAccessorTypeName(type, convertedType, convertedName, returnResult);

        std::map<std::wstring, std::wstring> vectorCases;

        for (auto const &property : enumClassPropertiesWriteOnly) {
            if (property->GetIsVector() || property->GetIsOrderedMap()) {
                std::wstring returnStr = L"";

                bool isOrderedMap = property->GetIsOrderedMap();
                std::wstring type = isOrderedMap ? property->GetType2() : property->GetType1();
                if (property->GetIsObject()) {
                    returnStr += INDENT + INDENT + INDENT + L"if (index > -1)\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->Insert" + property->GetPropertyName() + L"AtIndex(index, std::static_pointer_cast<" + type + L">(value));\r\n"
                        + INDENT + INDENT + INDENT + L"else\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->Insert" + property->GetPropertyName() + L"(std::static_pointer_cast<" + type + L">(value));\r\n";
                } else if (!type.empty() && IsCapital(type)) {
                    returnStr += INDENT + INDENT + INDENT + L"if (index > -1)\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->Insert" + property->GetPropertyName() + L"AtIndex(index, static_cast<" + type + L">(value));\r\n"
                        + INDENT + INDENT + INDENT + L"else\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->Insert" + property->GetPropertyName() + L"(static_cast<" + type + L">(value));\r\n";
                } else if (type == L"std::string") {
                    returnStr += INDENT + INDENT + INDENT + L"if (index > -1)\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->Insert" + property->GetPropertyName() + L"AtIndex(index, wstr2str(value));\r\n"
                        + INDENT + INDENT + INDENT + L"else\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->Insert" + property->GetPropertyName() + L"(wstr2str(value));\r\n";
                } else {
                    returnStr += INDENT + INDENT + INDENT + L"if (index > -1)\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->Insert" + property->GetPropertyName() + L"AtIndex(index, value);\r\n"
                        + INDENT + INDENT + INDENT + L"else\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->Insert" + property->GetPropertyName() + L"(value);\r\n";
                }
                vectorCases.insert(std::make_pair(property->GetEnum(), returnStr));
            }
        }
        result += L"\r\n"
            "void " + propertyName + L"Accessor::_Insert" + convertedName + L"AtIndex(const int64_t &objectProperty, " + (IsEqual(type, objectToken) ? L"" : L"const ")  + convertedType + L" " + (IsEqual(type, objectToken) ? L"" : L"&") + (!vectorCases.empty() ? L"value" : L"/*value*/") + L", const int64_t &" + (!vectorCases.empty() ? L"index" : L"/*index*/") + L") const\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n";
        if (!vectorCases.empty()) {
            result += GetGeneralTypeIndexContentHeader(propertyName);
            for (auto const &pair : vectorCases)
                result += INDENT + INDENT + L"case " + propertyName + L"::" + pair.first + L":\r\n"
                    + pair.second
                    + INDENT + INDENT + INDENT + L"break;\r\n";
            result += generalTypeContentFooter;
        } else
            result += INDENT + INDENT + L"THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n";
        result += INDENT + L"CATCH\r\n"
            "}\r\n";
    CATCH
}

void VPGPropertyAccessorGenerationService::GenerateClone(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassProperty>> &enumClassPropertiesReadOnly, std::wstring &result)
{
    if (type == containerToken || type != objectToken || enumClassPropertiesReadOnly.empty())
        return;
    TRY
        std::wstring convertedType = L"";
        std::wstring convertedName = L"";
        std::wstring returnResult = L"";
        VPGPropertyAccessorGenerationService::GetPropertyAccessorTypeName(type, convertedType, convertedName, returnResult);
        
        std::map<std::wstring, std::wstring> generalCases;
        std::map<std::wstring, std::wstring> vectorCases;
        std::map<std::wstring, std::wstring> mapCases;

        for (auto const &property : enumClassPropertiesReadOnly) {
            if (!property->GetIsObject())
                continue;
            // General
            if (property->GetIsGeneralType()) {
                std::wstring returnStr = L"std::static_pointer_cast<IObject>(obj->Get" + property->GetPropertyName() + L"()->Clone())";
                generalCases.insert(std::make_pair(property->GetEnum(), returnStr));
            }
            // Vector
            if (property->GetIsVector() || property->GetIsOrderedMap()) {
                std::wstring returnStr = L"std::static_pointer_cast<IObject>(obj->Clone" + property->GetPropertyName() + L"AtIndex(index))";
                vectorCases.insert(std::make_pair(property->GetEnum(), returnStr));
            }
            // Map
            if (property->GetIsMap() || property->GetIsOrderedMap()) {
                std::wstring castType = property->GetType1() == L"std::wstring" ? L"wchar_t" : property->GetType1();
                std::wstring mapGetKey = property->GetType1() == L"std::wstring" ? L"keyPtr" : L"*keyPtr";
                std::wstring returnStr = INDENT + INDENT + INDENT + L"auto keyPtr = static_cast<const " + castType + L" *>(key);\r\n"
                    + INDENT + INDENT + INDENT + L"assert(keyPtr != nullptr);\r\n"
                    + INDENT + INDENT + INDENT + L"if (keyPtr == nullptr)\r\n"
                    + INDENT + INDENT + INDENT + INDENT + L"THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
                    + INDENT + INDENT + INDENT + L"return std::static_pointer_cast<IObject>(obj->Clone" + property->GetPropertyName() + L"AtKey(" + mapGetKey + L"));\r\n";
                mapCases.insert(std::make_pair(property->GetEnum(), returnStr));
            }
        }

        // General Type
        result += L"\r\n"
            + convertedType + L" " + propertyName + L"Accessor::_Clone" + convertedName + L"(const int64_t &objectProperty) const\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n";
        if (!generalCases.empty()) {
            result += GetGeneralTypeContentHeader(propertyName);
            for (auto const &pair : generalCases)
                result += INDENT + INDENT + L"case " + propertyName + L"::" + pair.first + L":\r\n"
                    + INDENT + INDENT + INDENT + L"return " + pair.second + L";\r\n";
            result += generalTypeContentFooter;
        } else
            result += INDENT + INDENT + L"THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n";
        result += INDENT + L"CATCH\r\n"
            + INDENT + L"return " + returnResult + L";\r\n"
            "}\r\n";
        
        // Vector
        result += L"\r\n"
            + convertedType + L" " + propertyName + L"Accessor::_Clone" + convertedName + L"AtIndex(const int64_t &objectProperty, const int64_t &" + (!vectorCases.empty() ? L"index" : L"/*index*/") + L") const\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n";
        if (!vectorCases.empty()) {
            result += GetGeneralTypeIndexContentHeader(propertyName);
            for (auto const &pair : vectorCases)
                result += INDENT + INDENT + L"case " + propertyName + L"::" + pair.first + L":\r\n"
                    + INDENT + INDENT + INDENT + L"return " + pair.second + L";\r\n";
            result += generalTypeContentFooter;
        } else
            result += INDENT + INDENT + L"THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n";
        result += INDENT + L"CATCH\r\n"
                + INDENT + L"return " + returnResult + L";\r\n"
                "}\r\n";
            
        // Map
        result += L"\r\n"
            + convertedType + L" " + propertyName + L"Accessor::_Clone" + convertedName + L"AtKey(const int64_t &objectProperty, const void *" + (!mapCases.empty() ? L"key" : L"/*key*/") + L") const\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n";
        if (!mapCases.empty()) {
            result += GetGeneralTypeMapContentHeader(propertyName);
            for (auto const &pair : mapCases)
                result += INDENT + INDENT + L"case " + propertyName + L"::" + pair.first + L": {\r\n"
                    + pair.second
                    + INDENT + INDENT + L"}\r\n";
            result += generalTypeContentFooter;
        } else
            result += INDENT + INDENT + L"THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n";
        result += INDENT + L"CATCH\r\n"
                + INDENT + L"return " + returnResult + L";\r\n"
                "}\r\n";
    CATCH
}

void VPGPropertyAccessorGenerationService::GenerateContainerCount(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassProperty>> &enumClassProperties, std::wstring &result)
{
    if (type != containerToken || enumClassProperties.empty())
        return;
    TRY
        result += L"\r\n"
            "size_t " + propertyName + L"Accessor::_GetCount(const int64_t &objectProperty) const\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n" 
            + GetGeneralContentHeader(propertyName);
        for (auto const &property : enumClassProperties) {
            if (!property->GetIsCollection())
                continue;
            result += INDENT + INDENT + L"case " + propertyName + L"::" + property->GetEnum() + L":\r\n"
                + INDENT + INDENT + INDENT + L"return obj->Get" + property->GetPropertyName() + L"().size();\r\n";
        }
        result += generalTypeContentFooter
            + INDENT + L"CATCH\r\n"
            + INDENT + L"return 0;\r\n"
            "}\r\n";
    CATCH
}

void VPGPropertyAccessorGenerationService::GenerateContainerMapKey(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassProperty>> &enumClassProperties, std::wstring &result)
{
    if (type != containerToken || enumClassProperties.empty())
        return;
    TRY
        std::map<std::wstring, std::wstring> mapCases;
        for (auto const &property : enumClassProperties)
            if (property->GetIsMap() || property->GetIsOrderedMap())
                mapCases.insert(std::make_pair(property->GetEnum(), L"obj->Get" + property->GetPropertyName() + L"VoidKeys()"));

        result += L"\r\n"
            "std::set<void *> " + propertyName + L"Accessor::_GetMapKeys(const int64_t &objectProperty) const\r\n"
            "{\r\n"
            + INDENT + L"std::set<void *> result;\r\n"
            + INDENT + L"TRY\r\n";
        if (!mapCases.empty()) {
            result += GetGeneralContentHeader(propertyName);
            for (auto const &pair : mapCases)
                result += INDENT + INDENT + L"case " + propertyName + L"::" + pair.first + L":\r\n"
                        + INDENT + INDENT + INDENT + L"return " + pair.second + L";\r\n";
            result += generalTypeContentFooter;
        } else
            result += INDENT + INDENT + L"THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n";
        result += INDENT + L"CATCH\r\n"
            + INDENT + L"return result;\r\n"
            "}\r\n";
    CATCH
}

void VPGPropertyAccessorGenerationService::GenerateContainerIsContainKey(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassProperty>> &enumClassProperties, std::wstring &result)
{
    if (type != containerToken || enumClassProperties.empty())
        return;
    TRY
        std::map<std::wstring, std::wstring> mapCases;
        for (auto const &property : enumClassProperties)
            if (property->GetIsMap() || property->GetIsOrderedMap()) {
                std::wstring castType = property->GetType1() == L"std::wstring" ? L"wchar_t" : property->GetType1();
                std::wstring mapGetKey = property->GetType1() == L"std::wstring" ? L"keyPtr" : L"*keyPtr";
                mapCases.insert(std::make_pair(property->GetEnum(), 
                    INDENT + INDENT + INDENT + L"auto keyPtr = static_cast<const " + castType + L" *>(key);\r\n"
                    + INDENT + INDENT + INDENT + L"assert(keyPtr != nullptr);\r\n"
                    + INDENT + INDENT + INDENT + L"if (keyPtr == nullptr)\r\n"
                    + INDENT + INDENT + INDENT + INDENT + L"THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
                    + INDENT + INDENT + INDENT + L"return obj->Is" + property->GetPropertyName() + L"ContainKey(" + mapGetKey + L");\r\n"));
            }
                
        result += L"\r\n"
            "bool " + propertyName + L"Accessor::_IsContainKey(const int64_t &objectProperty, const void *" + (!mapCases.empty() ? L"key" : L"/*key*/") + L") const\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n";
        if (!mapCases.empty()) {
            result += GetGeneralTypeMapContentHeader(propertyName);
            for (auto const &pair : mapCases)
                result += INDENT + INDENT + L"case " + propertyName + L"::" + pair.first + L": {\r\n"
                    + pair.second
                    + INDENT + INDENT + L"}\r\n";
            result += generalTypeContentFooter;
        } else
            result += INDENT + INDENT + L"THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n";
        result += INDENT + L"CATCH\r\n"
            + INDENT + L"return false;\r\n"
            "}\r\n";
    CATCH
}

void VPGPropertyAccessorGenerationService::GenerateContainerRemove(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassProperty>> &enumClassProperties, std::wstring &result)
{
    if (type != containerToken || enumClassProperties.empty())
        return;
    TRY
        std::map<std::wstring, std::wstring> vectorCasesGeneral;
        std::map<std::wstring, std::wstring> vectorCasesObject;
        std::map<std::wstring, std::wstring> vectorCasesIndex;
        std::map<std::wstring, std::wstring> mapCases;

        for (auto const &property : enumClassProperties) {
            if (property->GetIsVector()) {
                if (property->GetIsObject()) {
                    std::wstring result = INDENT + INDENT + INDENT + L"obj->Remove" + property->GetPropertyName() + L"(value);\r\n";
                    vectorCasesObject.insert(std::make_pair(property->GetEnum(), result));
                } else {
                    std::wstring castType = property->GetType1() == L"std::wstring" ? L"wchar_t" : property->GetType1();
                    std::wstring valueAssign = property->GetType1() == L"std::wstring" ? L"valuePtr" : L"*valuePtr";
                    std::wstring result = INDENT + INDENT + INDENT + L"auto valuePtr = static_cast<const " + castType + L" *>(value);\r\n"
                        + INDENT + INDENT + INDENT + L"assert(valuePtr != nullptr);\r\n"
                        + INDENT + INDENT + INDENT + L"obj->Remove" + property->GetPropertyName() + L"(" + valueAssign + L");\r\n";
                    vectorCasesGeneral.insert(std::make_pair(property->GetEnum(), result));
                }
            }
            if (property->GetIsVector() || property->GetIsOrderedMap())
                vectorCasesIndex.insert(std::make_pair(property->GetEnum(), L"obj->Remove" + property->GetPropertyName() + L"AtIndex(index)"));
            if (property->GetIsMap() || property->GetIsOrderedMap()) {
                std::wstring castType = property->GetType1() == L"std::wstring" ? L"wchar_t" : property->GetType1();
                std::wstring mapGetKey = property->GetType1() == L"std::wstring" ? L"keyPtr" : L"*keyPtr";
                std::wstring result = INDENT + INDENT + INDENT + L"auto keyPtr = static_cast<const " + castType + L" *>(key);\r\n"
                    + INDENT + INDENT + INDENT + L"assert(keyPtr != nullptr);\r\n"
                    + INDENT + INDENT + INDENT + L"if (keyPtr == nullptr)\r\n"
                    + INDENT + INDENT + INDENT + INDENT + L"THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
                    + INDENT + INDENT + INDENT + L"obj->Remove" + property->GetPropertyName() + L"AtKey(" + mapGetKey + L");\r\n";
                mapCases.insert(std::make_pair(property->GetEnum(), result));
            }
        }

        const std::wstring className = GetClassNameFromPropertyClassName(propertyName);
        result += L"\r\n"
            "void " + propertyName + L"Accessor::_Remove(const int64_t &objectProperty, const void *" + (!vectorCasesGeneral.empty() ? L"value" : L"/*value*/") + L") const\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n";
        if (!vectorCasesGeneral.empty()) {
            result += INDENT + INDENT + L"assert(value != nullptr);\r\n"
                + INDENT + INDENT + L"auto obj = std::static_pointer_cast<" + className + L">(_Object);\r\n"
                + INDENT + INDENT + L"assert(obj != nullptr);\r\n"
                + INDENT + INDENT + L"switch(static_cast<" + propertyName + L">(objectProperty))\r\n"
                + INDENT + INDENT + L"{\r\n";

            for (auto const &pair : vectorCasesGeneral) {
                result += INDENT + INDENT + L"case " + propertyName + L"::" + pair.first + L": {\r\n"
                    + pair.second
                    + INDENT + INDENT + INDENT + L"break;\r\n"
                    + INDENT + INDENT + L"}\r\n";
            }
            result += generalTypeContentFooter;
        } else
            result += INDENT + INDENT + L"THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n";
        result += INDENT + L"CATCH\r\n"
            "}\r\n";
        
        result += L"\r\n"
            "void " + propertyName + L"Accessor::_RemoveObject(const int64_t &objectProperty, const IObject *" + (!vectorCasesObject.empty() ? L"value" : L"/*value*/") + L") const\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n";
        if (!vectorCasesObject.empty()) {
            result += INDENT + INDENT + L"assert(value != nullptr);\r\n"
                + INDENT + INDENT + L"auto obj = std::static_pointer_cast<" + className + L">(_Object);\r\n"
                + INDENT + INDENT + L"assert(obj != nullptr);\r\n"
                + INDENT + INDENT + L"switch(static_cast<" + propertyName + L">(objectProperty))\r\n"
                + INDENT + INDENT + L"{\r\n";
            for (auto const &pair : vectorCasesObject) {
                result += INDENT + INDENT + L"case " + propertyName + L"::" + pair.first + L":\r\n"
                    + pair.second
                    + INDENT + INDENT + INDENT + L"break;\r\n";
            }
            result += generalTypeContentFooter;
        } else
            result += INDENT + INDENT + L"THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n";
        result += INDENT + L"CATCH\r\n"
            "}\r\n";

        result += L"\r\n"
            "void " + propertyName + L"Accessor::_RemoveAtIndex(const int64_t &objectProperty, const int64_t &" + (!vectorCasesIndex.empty() ? L"index" : L"/*index*/" ) + L") const\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n";
        if (!vectorCasesIndex.empty()) {
            result += GetGeneralTypeIndexContentHeader(propertyName);
            for (auto const &pair : vectorCasesIndex)
                result += INDENT + INDENT + L"case " + propertyName + L"::" + pair.first + L":\r\n"
                    + INDENT + INDENT + INDENT + pair.second + L";\r\n"
                    + INDENT + INDENT + INDENT + L"break;\r\n";
            result += generalTypeContentFooter;
        } else
            result += INDENT + INDENT + L"THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n";
        result += INDENT + L"CATCH\r\n"
            "}\r\n";

        result += L"\r\n"
            "void " + propertyName + L"Accessor::_RemoveAtKey(const int64_t &objectProperty, const void *" + (!mapCases.empty()? L"key" : L"/*key*/") + L") const\r\n";
        result += L"{\r\n"
            + INDENT + L"TRY\r\n";
        if (!mapCases.empty()) {
            result += GetGeneralTypeMapContentHeader(propertyName);
            for (auto const &pair : mapCases)
                result += INDENT + INDENT + L"case " + propertyName + L"::" + pair.first + L": {\r\n"
                    + pair.second
                    + INDENT + INDENT + INDENT + L"break;\r\n"
                    + INDENT + INDENT + L"}\r\n";
            result += generalTypeContentFooter;
        } else
            result += INDENT + INDENT + L"THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n";
        result += INDENT + L"CATCH\r\n"
            "}\r\n";

        result += L"\r\n"
            "void " + propertyName + L"Accessor::_Clear(const int64_t &objectProperty) const\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n"
            + GetGeneralContentHeader(propertyName);
        for (auto const &property : enumClassProperties) {
            if (!property->GetIsCollection())
                continue;
            result += INDENT + INDENT + L"case " + propertyName + L"::" + property->GetEnum() + L":\r\n"
                + INDENT + INDENT + INDENT + L"obj->Clear" + property->GetPropertyName() + L"();\r\n"
                + INDENT + INDENT + INDENT + L"break;\r\n";
        }
        result += generalTypeContentFooter;
        result += INDENT + L"CATCH\r\n"
            "}\r\n";
    CATCH
}

void VPGPropertyAccessorGenerationService::GenerateCpp(const LogConfig *logConfig, const std::wstring &projectPrefix,
    const std::map<std::wstring, std::wstring> &projectClassIncludeFiles,
    const std::wstring &filePathCpp, const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList)
{
    TRY
        LogService::LogInfo(logConfig, LOG_ID, L"Generate property accessor cpp file: " + filePathCpp);

        std::set<std::wstring> systemIncludeFiles;
        std::set<std::wstring> projectIncludeFiles;
        projectIncludeFiles.insert(L"exception_macro.hpp");
        projectIncludeFiles.insert(L"i_object.hpp");
        projectIncludeFiles.insert(L"property_accessor_macro.hpp");

        // class name, type, enum property
        std::map<std::wstring, std::map<std::wstring, std::vector<std::shared_ptr<VPGEnumClassProperty>>>> typeMacroMap;
        // type macro
        for (auto const &enumClass : enumClassList) {
            if (!IsPropertyClass(enumClass->GetName(), projectPrefix))
                continue;
            // include orginal class and enum class
            std::wstring className = GetClassNameFromPropertyClassName(enumClass->GetName());

            // include object class
            std::wstring includePath = VPGPropertyAccessorGenerationService::GetIncludeFile(projectClassIncludeFiles, className);
            if (includePath.empty())
                THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Header file of class " + className + L" not found");
            projectIncludeFiles.insert(includePath);

            // include enum class
            includePath = VPGPropertyAccessorGenerationService::GetIncludeFile(projectClassIncludeFiles, enumClass->GetName());
            if (includePath.empty())
                THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Header file of enum class " + enumClass->GetName() + L" not found");
            projectIncludeFiles.insert(includePath);

            std::map<std::wstring, std::vector<std::shared_ptr<VPGEnumClassProperty>>> initMap;
            typeMacroMap.insert(std::make_pair(enumClass->GetName(), initMap));
            for (auto const &property : enumClass->GetProperties()) {
                if (property->GetPropertyType() != VPGEnumClassPropertyType::Property)
                    continue;

                // not support set
                if (property->GetIsSet())
                    continue;

                bool isContainer = false;
                switch (property->GetGetSetType())
                {
                case VPGEnumClassGetSetType::Vector:
                    systemIncludeFiles.insert(L"vector");
                    isContainer = true;
                    break;
                case VPGEnumClassGetSetType::Map:
                    systemIncludeFiles.insert(L"map");
                    isContainer = true;
                    break;
                case VPGEnumClassGetSetType::OrderedMap:
                    systemIncludeFiles.insert(L"vector");
                    systemIncludeFiles.insert(L"map");
                    isContainer = true;
                    break;
                case VPGEnumClassGetSetType::Set:
                    systemIncludeFiles.insert(L"set");
                    isContainer = true;
                    break;
                default:
                    break;
                }

                // container
                if (isContainer) {
                    if (!IsContain(typeMacroMap[enumClass->GetName()], containerToken)) {
                        std::vector<std::shared_ptr<VPGEnumClassProperty>> initVector;
                        typeMacroMap[enumClass->GetName()].insert(std::make_pair(containerToken, initVector));
                    }
                    typeMacroMap[enumClass->GetName()][containerToken].push_back(property);
                }

                std::wstring type1 = property->GetType1();
                std::wstring type2 = property->GetType2();
                if (!type1.empty()) {
                    if (IsCapital(type1)) {
                        includePath = VPGPropertyAccessorGenerationService::GetIncludeFile(projectClassIncludeFiles, type1);
                        if (includePath.empty())
                            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Header file of class " + type1 + L" not found");
                        projectIncludeFiles.insert(includePath);
                    } else if (Find(type1, L"string") != std::wstring::npos)
                        systemIncludeFiles.insert(L"string");
                }
                if (!type2.empty()) {
                    if (IsCapital(type2)) {
                        includePath = VPGPropertyAccessorGenerationService::GetIncludeFile(projectClassIncludeFiles, type2);
                        if (includePath.empty())
                            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Header file of class " + type2 + L" not found");
                        projectIncludeFiles.insert(includePath);
                    } else if (Find(type2, L"string") != std::wstring::npos)
                        systemIncludeFiles.insert(L"string");
                }

                // object
                if (property->GetIsObject()) {
                    systemIncludeFiles.insert(L"memory");
                    if (!IsContain(typeMacroMap[enumClass->GetName()], (objectToken))) {
                        std::vector<std::shared_ptr<VPGEnumClassProperty>> initVector;
                        typeMacroMap[enumClass->GetName()].insert(std::make_pair(objectToken, initVector));
                    }
                    typeMacroMap[enumClass->GetName()][objectToken].push_back(property);
                } else {
                    std::wstring type = property->GetIsMap() || property->GetIsOrderedMap() ? type2 : type1;
                    if (!type.empty() && IsCapital(type))
                        type = enumToken;
                    std::wstring convertedType = L"";
                    std::wstring converttedName = L"";
                    std::wstring returnResult = L"";
                    VPGPropertyAccessorGenerationService::GetPropertyAccessorTypeName(type, convertedType, converttedName, returnResult);
                    typeMacroMap[enumClass->GetName()][convertedType].push_back(property);
                }
            }
        }

        std::wstring headerFileName = GetFileName(filePathCpp);
        Replace(headerFileName, L".cpp", L".hpp");
        std::wstring result = L"#include " + GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, headerFileName) + L"\r\n";
        // system include files
        if (!systemIncludeFiles.empty())
            result += L"\r\n";
        for (auto const &includeFiles : systemIncludeFiles)
            result += L"#include <" + includeFiles + L">\r\n";
        // project include files
        result += L"\r\n";
        for (auto const &includeFiles : projectIncludeFiles)
            result += L"#include " + GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, includeFiles) + L"\r\n";
        result += L"\r\n"
            "using namespace vcc;\r\n";
        
        size_t count = 0;
        for (auto const &enumClass : typeMacroMap) {
            for (auto const &type : GetPropertyAccessorTypeOrder()) {
                if (enumClass.second.find(type) != enumClass.second.end()) {
                    std::vector<std::shared_ptr<VPGEnumClassProperty>> readOnly, writeOnly, readWrite;
                    for (auto property : enumClass.second.at(type)) {
                        switch (property->GetAccessMode())
                        {
                        case VPGEnumClassPropertyAccessMode::NoAccess:
                            break;
                        case VPGEnumClassPropertyAccessMode::ReadOnly:
                            readOnly.push_back(property);
                            readWrite.push_back(property);
                            break;
                        case VPGEnumClassPropertyAccessMode::ReadWrite:
                            readOnly.push_back(property);
                            writeOnly.push_back(property);
                            readWrite.push_back(property);
                            break;
                        case VPGEnumClassPropertyAccessMode::WriteOnly:
                            writeOnly.push_back(property);
                            readWrite.push_back(property);
                            break;
                        default:
                            assert(false);
                            break;
                        }
                    }
                    if (readWrite.empty())
                        continue;
                    
                    count += readWrite.size();

                    VPGPropertyAccessorGenerationService::GenerateRead(enumClass.first, type, readOnly, result);
                    VPGPropertyAccessorGenerationService::GenerateWrite(enumClass.first, type, writeOnly, result);
                    VPGPropertyAccessorGenerationService::GenerateInsert(enumClass.first, type, writeOnly, result);
                    VPGPropertyAccessorGenerationService::GenerateClone(enumClass.first, type, readOnly, result);
                    VPGPropertyAccessorGenerationService::GenerateContainerCount(enumClass.first, type, readWrite, result);
                    VPGPropertyAccessorGenerationService::GenerateContainerMapKey(enumClass.first, type, readWrite, result);
                    VPGPropertyAccessorGenerationService::GenerateContainerIsContainKey(enumClass.first, type, readOnly, result);
                    VPGPropertyAccessorGenerationService::GenerateContainerRemove(enumClass.first, type, writeOnly, result);
                }
            }
        }
        if (count > 0) {
            WriteFile(filePathCpp, result, true);
            LogService::LogInfo(logConfig, LOG_ID, L"Generate property accessor cpp completed.");
        } else {
            if (IsFilePresent(filePathCpp)) {
                RemoveFile(filePathCpp);
                LogService::LogInfo(logConfig, LOG_ID, L"Removed property accessor cpp as no properties are acceesable.");
            } else
                LogService::LogInfo(logConfig, LOG_ID, L"No property accessor cpp need to be generated.");
        }
    CATCH
}
