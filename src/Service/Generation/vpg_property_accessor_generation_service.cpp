#include "vpg_property_accessor_generation_service.hpp"

#include <set>

#include "exception_macro.hpp"
#include "file_helper.hpp"

using namespace vcc;

#define LOG_ID L"Property Accessor Generation"
const std::wstring proeprtyClassNameSuffix = L"Property";
const std::wstring containerToken = L"<Container>";
const std::wstring objectToken = L"<Object>";
const std::wstring enumToken = L"<Enum>";

const std::wstring generalTypeContentFooter = INDENT + INDENT + L"default:\r\n"
                + INDENT + INDENT + INDENT + L"assert(false);\r\n"
                + INDENT + INDENT + L"}\r\n";

std::wstring GetClassNameFromClassPropertyName(const std::wstring &classPropertyName)
{
    return classPropertyName.substr(0, classPropertyName.size() - proeprtyClassNameSuffix.size());
}

std::wstring GetGeneralTypeIndexContentHeader(const std::wstring &classPropertyName)
{
    const std::wstring className = GetClassNameFromClassPropertyName(classPropertyName);
    return INDENT + INDENT + L"assert(index >= -1);\r\n"
        + INDENT + INDENT + L"std::shared_ptr<" + className + L"> obj = std::static_pointer_cast<" + className + L">(_Object);\r\n"
        + INDENT + INDENT + L"assert(obj != nullptr);\r\n"
        + INDENT + INDENT + L"switch(static_cast<" + classPropertyName + L">(objectProperty))\r\n"
        + INDENT + INDENT + L"{\r\n";
}

std::wstring GetGeneralTypeMapContentHeader(const std::wstring &classPropertyName)
{
    const std::wstring className = GetClassNameFromClassPropertyName(classPropertyName);
    return INDENT + INDENT + L"std::shared_ptr<" + className + L"> obj = std::static_pointer_cast<" + className + L">(_Object);\r\n"
        + INDENT + INDENT + L"assert(obj != nullptr);\r\n"
        + INDENT + INDENT + L"const PropertyAccessorKey *mapKey = dynamic_cast<const PropertyAccessorKey *>(key);\r\n"
        + INDENT + INDENT + L"assert(mapKey != nullptr);\r\n"
        + INDENT + INDENT + L"switch(static_cast<" + classPropertyName + L">(objectProperty))\r\n"
        + INDENT + INDENT + L"{\r\n";
}

std::wstring GetGeneralContentHeader(const std::wstring &classPropertyName)
{
    const std::wstring className = GetClassNameFromClassPropertyName(classPropertyName);
    return INDENT + INDENT + L"std::shared_ptr<" + className + L"> obj = std::static_pointer_cast<" + className + L">(_Object);\r\n"
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
            || (!originalType.empty() && std::iswupper(originalType[0]) /* enum */)) {
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
        } else if (originalType == L"std::string") {
            convertedType = L"std::string";
            convertedName = L"String";
            returnResult = L"\"\"";
        } else if (originalType == L"std::wstring") {
            convertedType = L"std::wstring";
            convertedName = L"Wstring";
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
    result.push_back(L"std::string");
    result.push_back(L"std::wstring");
    result.push_back(objectToken);
    result.push_back(containerToken);
    return result;
}

std::wstring VPGPropertyAccessorGenerationService::GetIncludeFile(const std::map<std::wstring, std::wstring> &projectClassIncludeFiles, const std::wstring &className)
{
    TRY
        if (projectClassIncludeFiles.contains(className))
            return projectClassIncludeFiles.at(className);
        else if (projectClassIncludeFiles.contains(L"vcc::" + className))
            return projectClassIncludeFiles.at(L"vcc::" + className);
    CATCH
    return L"";
}

void VPGPropertyAccessorGenerationService::GenerateHpp(const LogProperty *logProperty, const std::wstring &hppFilePath, const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList)
{
    TRY
        LogService::LogInfo(logProperty, LOG_ID, L"Generate property accessor hpp file: " + hppFilePath);

        std::wstring result = L"#pragma once\r\n\r\n";

        if (VPGPropertyAccessorGenerationService::IsIncludeString(enumClassList))
            result += L"#include <string>\r\n\r\n";

        result += L"#include \"base_property_accessor.hpp\"\r\n"
            "#include \"property_accessor_macro.hpp\"\r\n"
            "\r\n"
            "using namespace vcc;\r\n";

        for (auto const &enumClass : enumClassList) {
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
                if (IsBlank(property->GetMacro()))
                    continue;

                // property accessor not support set
                if (IsStartWith(property->GetMacro(), L"SET")) {
                    LogService::LogWarning(logProperty, L"Property Accessor Generation Service", L"Property Accessor not support SET: class " + enumClass->GetName() + L": " + property->GetMacro());
                    continue;
                }
                
                size_t pos = Find(property->GetMacro(), L"(");
                std::wstring macro = pos != std::wstring::npos && pos > 0 ? property->GetMacro().substr(0, pos) : property->GetMacro();
                isCollection = isCollection
                                    || IsStartWith(macro, L"VECTOR")
                                    || IsStartWith(macro, L"MAP")
                                    || IsStartWith(macro, L"ORDERED_MAP")
                                    || IsStartWith(macro, L"SET");
                isObject = isObject || Find(macro, L"SPTR") != std::wstring::npos;
                if (Find(macro, L"SPTR") == std::wstring::npos) {
                    std::wstring type = IsStartWith(macro, L"MAP") || IsStartWith(macro, L"ORDERED_MAP") ? property->GetType2() : property->GetType1();
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

        WriteFile(hppFilePath, result, true);
        LogService::LogInfo(logProperty, LOG_ID, L"Generate property accessor hpp completed.");
    CATCH
}

void VPGPropertyAccessorGenerationService::GetIsHavingGenerateTypeMapType(const std::vector<std::shared_ptr<VPGEnumClassProperty>> &enumClassProperties, bool &isHavingGeneralType,  bool &isHavingVectorType, bool &isHavingMapType)
{
    TRY
        for (auto const &item : enumClassProperties) {
            if (IsStartWith(item->GetMacro(), L"MAP") || IsStartWith(item->GetMacro(), L"ORDERED_MAP"))
                isHavingMapType = true;
            else
                isHavingGeneralType = true;
            
            if (IsStartWith(item->GetMacro(), L"VECTOR") || IsStartWith(item->GetMacro(), L"ORDERED_MAP"))
                isHavingVectorType = true;
        }
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
        bool isHavingMapType = false;
        bool isHavingVectorType = false;
        bool isHavingGeneralType = false;
        GetIsHavingGenerateTypeMapType(enumClassPropertiesReadOnly, isHavingGeneralType, isHavingVectorType, isHavingMapType);

        // General Type
        // header
        result += L"\r\n"
            + convertedType + L" " + propertyName + L"Accessor::_Read" + convertedName;
        if (isHavingGeneralType) {
            result += L"(const size_t &objectProperty, const int64_t &index) const\r\n";
        } else {
            result += L"(const size_t &objectProperty, const int64_t & /*index*/) const\r\n";
        }
        // body
        result += L"{\r\n"
                + INDENT + L"TRY\r\n";
        if (isHavingGeneralType) {
            result += GetGeneralTypeIndexContentHeader(propertyName);
            for (auto const &property : enumClassPropertiesReadOnly) {
                if (IsStartWith(property->GetMacro(), L"MAP"))
                    continue;

                result += INDENT + INDENT + L"case " + propertyName + L"::" + property->GetEnum() + L":\r\n";
                if (IsStartWith(property->GetMacro(), L"VECTOR") || IsStartWith(property->GetMacro(), L"ORDERED_MAP")) {
                    std::wstring tmpConvertedType = L"";
                    std::wstring tmpConvertedName = L"";
                    std::wstring tmpReturnResult = L"";
                    VPGPropertyAccessorGenerationService::GetPropertyAccessorTypeName(property->GetType1(), tmpConvertedType, tmpConvertedName, tmpReturnResult);
                    
                    bool isOrderedMap = IsStartWith(property->GetMacro(), L"ORDERED_MAP");
                    if (type == objectToken)
                        result += INDENT + INDENT + INDENT + L"return std::static_pointer_cast<IObject>(obj->Get" + property->GetPropertyName() + (isOrderedMap ? L"ByIndex" : L"") + L"(index)" + (isOrderedMap ? L".second" : L"") + L");\r\n";
                    else {
                        std::wstring type = isOrderedMap ? property->GetType2() : property->GetType1();
                        if (!type.empty() && std::iswupper(type[0]))
                            result += INDENT + INDENT + INDENT + L"return static_cast<long>(obj->Get" + property->GetPropertyName() + (isOrderedMap ? L"ByIndex" : L"") + L"(index)"+ (isOrderedMap ? L".second" : L"") + L");\r\n";
                        else
                            result += INDENT + INDENT + INDENT + L"return obj->Get" + property->GetPropertyName() + (isOrderedMap ? L"ByIndex" : L"") + L"(index)" + (isOrderedMap ? L".second" : L"") + L";\r\n";
                    }
                } else if (type == objectToken)
                    result += INDENT + INDENT + INDENT + L"return std::static_pointer_cast<IObject>(obj->Get" + property->GetPropertyName() + L"());\r\n";
                else if (!property->GetType1().empty() && std::iswupper(property->GetType1()[0]))
                    result += INDENT + INDENT + INDENT + L"return static_cast<long>(obj->Get" + property->GetPropertyName() + L"());\r\n";
                else
                    result += INDENT + INDENT + INDENT + L"return obj->Get" + property->GetPropertyName() + L"();\r\n";
            }
            result += generalTypeContentFooter;
        } else
            result += INDENT + INDENT + L"THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n";
        result += INDENT + L"CATCH\r\n"
                + INDENT + L"return " + returnResult + L";\r\n"
                "}\r\n";
            
        // Map Type
        result += L"\r\n"
            + convertedType + L" " + propertyName + L"Accessor::_Read" + convertedName;
        if (isHavingMapType)
            result += L"(const size_t &objectProperty, const ITypeUnion *key) const\r\n";
        else
            result += L"(const size_t &objectProperty, const ITypeUnion * /*key*/) const\r\n";
        
        // body
        result += L"{\r\n"
                + INDENT + L"TRY\r\n";
        if (isHavingMapType) {
            result += GetGeneralTypeMapContentHeader(propertyName);
            for (auto const &property : enumClassPropertiesReadOnly) {
                if (IsStartWith(property->GetMacro(), L"VECTOR"))
                    continue;
                
                std::wstring tmpConvertedType = L"";
                std::wstring tmpConvertedName = L"";
                std::wstring tmpReturnResult = L"";
                VPGPropertyAccessorGenerationService::GetPropertyAccessorTypeName(property->GetType1(), tmpConvertedType, tmpConvertedName, tmpReturnResult);
                result += INDENT + INDENT + L"case " + propertyName + L"::" + property->GetEnum() + L":\r\n";
                
                bool isOrderedMap = IsStartWith(property->GetMacro(), L"ORDERED_MAP");
                if (type == objectToken)
                    result += INDENT + INDENT + INDENT + L"return std::static_pointer_cast<IObject>(obj->Get" + property->GetPropertyName() + (isOrderedMap ? L"ByKey" : L"") + L"(mapKey->GetKey" + tmpConvertedName + L"())" + (isOrderedMap ? L".second" : L"") + L");\r\n";
                else if (!property->GetType2().empty() && std::iswupper(property->GetType2()[0]))
                    result += INDENT + INDENT + INDENT + L"return static_cast<long>(obj->Get" + property->GetPropertyName() + (isOrderedMap ? L"ByKey" : L"") + L"(mapKey->GetKey" + tmpConvertedName + L"())" + (isOrderedMap ? L".second" : L"") + L");\r\n";
                else
                    result += INDENT + INDENT + INDENT + L"return obj->Get" + property->GetPropertyName() + (isOrderedMap ? L"ByKey" : L"") + L"(mapKey->GetKey" + tmpConvertedName + L"())"+ (isOrderedMap ? L".second" : L"") + L";\r\n";
            }
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
        bool isHavingMapType = false;
        bool isHavingVectorType = false;
        bool isHavingGeneralType = false;
        GetIsHavingGenerateTypeMapType(enumClassPropertiesWriteOnly, isHavingGeneralType, isHavingVectorType, isHavingMapType);

        // General Type
        // header
        result += L"\r\n"
            "void " + propertyName + L"Accessor::_Write" + convertedName;
        if (isHavingGeneralType) {
            if (type == objectToken)
                result += L"(const size_t &objectProperty, " + convertedType + L" value, const int64_t &index) const\r\n";
            else
                result += L"(const size_t &objectProperty, const " + convertedType + L" &value, const int64_t &index) const\r\n";
        } else {
            if (type == objectToken)
                result += L"(const size_t &objectProperty, " + convertedType + L" /*value*/, const int64_t & /*index*/) const\r\n";
            else
                result += L"(const size_t &objectProperty, const " + convertedType + L" & /*value*/, const int64_t & /*index*/) const\r\n";
        }
        // body
        result += L"{\r\n"
                + INDENT + L"TRY\r\n";
        if (isHavingGeneralType) {
            result += GetGeneralTypeIndexContentHeader(propertyName);
            for (auto const &property : enumClassPropertiesWriteOnly) {
                if (IsStartWith(property->GetMacro(), L"MAP"))
                    continue;

                result += INDENT + INDENT + L"case " + propertyName + L"::" + property->GetEnum() + L":\r\n";
                if (IsStartWith(property->GetMacro(), L"VECTOR") || IsStartWith(property->GetMacro(), L"ORDERED_MAP")) {
                    bool isOrderedMap = IsStartWith(property->GetMacro(), L"ORDERED_MAP");
                    std::wstring type = IsStartWith(property->GetMacro(), L"ORDERED_MAP") ? property->GetType2() : property->GetType1();
                    if (Find(property->GetMacro(), L"SPTR") != std::wstring::npos) {
                        result += INDENT + INDENT + INDENT + L"if (index > -1)\r\n"
                            + INDENT + INDENT + INDENT + INDENT + L"obj->Set" + property->GetPropertyName() + (isOrderedMap ? L"ByIndex" : L"") + L"(index, std::static_pointer_cast<" + type + L">(value));\r\n"
                            + INDENT + INDENT + INDENT + L"else\r\n"
                            + INDENT + INDENT + INDENT + INDENT + L"obj->Insert" + property->GetPropertyName() + L"(std::static_pointer_cast<" + type + L">(value));\r\n";
                    } else if (!type.empty() && std::iswupper(type[0])) {
                        result += INDENT + INDENT + INDENT + L"if (index > -1)\r\n"
                            + INDENT + INDENT + INDENT + INDENT + L"obj->Set" + property->GetPropertyName() + (isOrderedMap ? L"ByIndex" : L"") + L"(index, static_cast<" + type + L">(value));\r\n"
                            + INDENT + INDENT + INDENT + L"else\r\n"
                            + INDENT + INDENT + INDENT + INDENT + L"obj->Insert" + property->GetPropertyName() + L"(static_cast<" + type + L">(value));\r\n";
                    } else {
                        result += INDENT + INDENT + INDENT + L"if (index > -1)\r\n"
                            + INDENT + INDENT + INDENT + INDENT + L"obj->Set" + property->GetPropertyName() + (isOrderedMap ? L"ByIndex" : L"") + L"(index, value);\r\n"
                            + INDENT + INDENT + INDENT + L"else\r\n"
                            + INDENT + INDENT + INDENT + INDENT + L"obj->Insert" + property->GetPropertyName() + L"(value);\r\n";
                    }
                } else if (type == objectToken)
                    result += INDENT + INDENT + INDENT + L"obj->Set" + property->GetPropertyName() + L"(std::static_pointer_cast<" + property->GetType1() + L">(value));\r\n";
                else if (!property->GetType1().empty() && std::iswupper(property->GetType1()[0]))
                    result += INDENT + INDENT + INDENT + L"obj->Set" + property->GetPropertyName() + L"(static_cast<" + property->GetType1() + L">(value));\r\n";
                else
                    result += INDENT + INDENT + INDENT + L"obj->Set" + property->GetPropertyName() + L"(value);\r\n";
                result += INDENT + INDENT + INDENT + L"break;\r\n";
            }
            result += generalTypeContentFooter;
        } else
            result += INDENT + INDENT + L"THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n";
        result += INDENT + L"CATCH\r\n"
                "}\r\n";
            
        // Map Type
        result += L"\r\n"
            "void " + propertyName + L"Accessor::_Write" + convertedName;
        if (isHavingMapType) {
            if (type == objectToken)
                result += L"(const size_t &objectProperty, " + convertedType + L" value, const ITypeUnion *key) const\r\n";
            else
                result += L"(const size_t &objectProperty, const " + convertedType + L" &value, const ITypeUnion *key) const\r\n";
        } else {
            if (type == objectToken)
                result += L"(const size_t &objectProperty, " + convertedType + L" /*value*/, const ITypeUnion * /*key*/) const\r\n";
            else
                result += L"(const size_t &objectProperty, const " + convertedType + L" & /*value*/, const ITypeUnion * /*key*/) const\r\n";
        }
        // body
        result += L"{\r\n"
                + INDENT + L"TRY\r\n";
        if (isHavingMapType) {
            result += GetGeneralTypeMapContentHeader(propertyName);
            for (auto const &property : enumClassPropertiesWriteOnly) {
                if (IsStartWith(property->GetMacro(), L"VECTOR"))
                    continue;
                
                std::wstring tmpConvertedType = L"";
                std::wstring tmpConvertedName = L"";
                std::wstring tmpReturnResult = L"";
                VPGPropertyAccessorGenerationService::GetPropertyAccessorTypeName(property->GetType1(), tmpConvertedType, tmpConvertedName, tmpReturnResult);
                result += INDENT + INDENT + L"case " + propertyName + L"::" + property->GetEnum() + L":\r\n";
                
                bool isOrderedMap = IsStartWith(property->GetMacro(), L"ORDERED_MAP");
                if (type == objectToken) {
                    result += INDENT + INDENT + INDENT + L"if (obj->Is" + property->GetPropertyName() + L"ContainKey(mapKey->GetKey" + tmpConvertedName + L"()))\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->Set" + property->GetPropertyName() + (isOrderedMap ? L"ByKey" : L"") + L"(mapKey->GetKey" + tmpConvertedName + L"(), std::static_pointer_cast<" + property->GetType2() + L">(value));\r\n"
                        + INDENT + INDENT + INDENT + L"else\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->Insert" + property->GetPropertyName() + L"(mapKey->GetKey" + tmpConvertedName + L"(), std::static_pointer_cast<" + property->GetType2() + L">(value));\r\n";
                } else {
                    std::wstring type = property->GetType2();
                    if (!type.empty() && std::iswupper(type[0])) {
                        result += INDENT + INDENT + INDENT + L"if (obj->Is" + property->GetPropertyName() + L"ContainKey(mapKey->GetKey" + tmpConvertedName + L"()))\r\n"
                            + INDENT + INDENT + INDENT + INDENT + L"obj->Set" + property->GetPropertyName() + (isOrderedMap ? L"ByKey" : L"") + L"(mapKey->GetKey" + tmpConvertedName + L"(), static_cast<" + type + L">(value));\r\n"
                            + INDENT + INDENT + INDENT + L"else\r\n"
                            + INDENT + INDENT + INDENT + INDENT + L"obj->Insert" + property->GetPropertyName() + L"(mapKey->GetKey" + tmpConvertedName + L"(), static_cast<" + type + L">(value));\r\n";
                    } else {
                        result += INDENT + INDENT + INDENT + L"if (obj->Is" + property->GetPropertyName() + L"ContainKey(mapKey->GetKey" + tmpConvertedName + L"()))\r\n"
                            + INDENT + INDENT + INDENT + INDENT + L"obj->Set" + property->GetPropertyName() + (isOrderedMap ? L"ByKey" : L"") + L"(mapKey->GetKey" + tmpConvertedName + L"(), value);\r\n"
                            + INDENT + INDENT + INDENT + L"else\r\n"
                            + INDENT + INDENT + INDENT + INDENT + L"obj->Insert" + property->GetPropertyName() + L"(mapKey->GetKey" + tmpConvertedName + L"(), value);\r\n";
                    }
                    
                }
                result += INDENT + INDENT + INDENT + L"break;\r\n";
            }
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
        bool isHavingMapType = false;
        bool isHavingVectorType = false;
        bool isHavingGeneralType = false;
        GetIsHavingGenerateTypeMapType(enumClassPropertiesReadOnly, isHavingGeneralType, isHavingVectorType, isHavingMapType);

        // General Type
        // header
        result += L"\r\n"
            + convertedType + L" " + propertyName + L"Accessor::_Clone" + convertedName;
        if (isHavingGeneralType) {
            result += L"(const size_t &objectProperty, const int64_t &index) const\r\n";
        } else {
            result += L"(const size_t &objectProperty, const int64_t & /*index*/) const\r\n";
        }
        // body
        result += L"{\r\n"
                + INDENT + L"TRY\r\n";
        if (isHavingGeneralType) {
            result += GetGeneralTypeIndexContentHeader(propertyName);
            for (auto const &property : enumClassPropertiesReadOnly) {
                if (IsStartWith(property->GetMacro(), L"MAP"))
                    continue;
                
                bool isOrderedMap = IsStartWith(property->GetMacro(), L"ORDERED_MAP");
                bool isVector = IsStartWith(property->GetMacro(), L"VECTOR");
                result += INDENT + INDENT + L"case " + propertyName + L"::" + property->GetEnum() + L":\r\n";
                if (isOrderedMap || isVector)
                    result += INDENT + INDENT + INDENT + L"return std::static_pointer_cast<IObject>(obj->Clone" + property->GetPropertyName() + (isOrderedMap ? L"ByIndex" : L"") + L"(" + (isOrderedMap || isVector ? L"index" : L"") + L"));\r\n";
                else
                    result += INDENT + INDENT + INDENT + L"return std::static_pointer_cast<IObject>(obj->Get" + property->GetPropertyName() + L"()->Clone());\r\n";
            }
            result += generalTypeContentFooter;
        } else
            result += INDENT + INDENT + L"THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n";
        result += INDENT + L"CATCH\r\n"
                + INDENT + L"return " + returnResult + L";\r\n"
                "}\r\n";
            
        // Map Type
        result += L"\r\n"
            + convertedType + L" " + propertyName + L"Accessor::_Clone" + convertedName;
        if (isHavingMapType)
            result += L"(const size_t &objectProperty, const ITypeUnion *key) const\r\n";
        else
            result += L"(const size_t &objectProperty, const ITypeUnion * /*key*/) const\r\n";
        
        // body
        result += L"{\r\n"
                + INDENT + L"TRY\r\n";
        if (isHavingMapType) {
            result += GetGeneralTypeMapContentHeader(propertyName);
            for (auto const &property : enumClassPropertiesReadOnly) {
                if (IsStartWith(property->GetMacro(), L"VECTOR"))
                    continue;
                
                std::wstring tmpConvertedType = L"";
                std::wstring tmpConvertedName = L"";
                std::wstring tmpReturnResult = L"";
                VPGPropertyAccessorGenerationService::GetPropertyAccessorTypeName(property->GetType1(), tmpConvertedType, tmpConvertedName, tmpReturnResult);
                
                bool isOrderedMap = IsStartWith(property->GetMacro(), L"ORDERED_MAP");
                result += INDENT + INDENT + L"case " + propertyName + L"::" + property->GetEnum() + L":\r\n"
                    + INDENT + INDENT + INDENT + L"return std::static_pointer_cast<IObject>(obj->Clone" + property->GetPropertyName() + (isOrderedMap ? L"ByKey" : L"") + L"(mapKey->GetKey" + tmpConvertedName + L"()));\r\n";
            }
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
        bool isHavingMapType = false;
        bool isHavingVectorType = false;
        bool isHavingGeneralType = false;
        GetIsHavingGenerateTypeMapType(enumClassProperties, isHavingGeneralType, isHavingVectorType, isHavingMapType);

        result += L"\r\n"
            "size_t " + propertyName + L"Accessor::_GetContainerCount(const size_t &objectProperty) const\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n" 
            + GetGeneralContentHeader(propertyName);
        for (auto const &property : enumClassProperties) {
            result += INDENT + INDENT + L"case " + propertyName + L"::" + property->GetEnum() + L":\r\n"
                + INDENT + INDENT + INDENT + L"return obj->Get" + property->GetPropertyName() + L"().size();\r\n";
        }
        result += generalTypeContentFooter;
        result += INDENT + L"CATCH\r\n"
            + INDENT + L"return 0;\r\n"
            "}\r\n";
    CATCH
}


void VPGPropertyAccessorGenerationService::GenerateContainerIsContainKey(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassProperty>> &enumClassProperties, std::wstring &result)
{
    if (type != containerToken || enumClassProperties.empty())
        return;
    TRY
        bool isHavingMapType = false;
        bool isHavingVectorType = false;
        bool isHavingGeneralType = false;
        GetIsHavingGenerateTypeMapType(enumClassProperties, isHavingGeneralType, isHavingVectorType, isHavingMapType);

        result += L"\r\n"
            "bool " + propertyName + L"Accessor::_IsContainKey";
        if (isHavingMapType)
            result += L"(const size_t &objectProperty, const ITypeUnion *key) const\r\n";
        else
            result += L"(const size_t &objectProperty, const ITypeUnion * /*key*/) const\r\n";
        result += L"{\r\n"
            + INDENT + L"TRY\r\n";
        if (isHavingMapType) {
            result += GetGeneralTypeMapContentHeader(propertyName);
            for (auto const &property : enumClassProperties) {
                if (IsStartWith(property->GetMacro(), L"MAP") || IsStartWith(property->GetMacro(), L"ORDERED_MAP")) {
                    std::wstring tmpConvertedType = L"";
                    std::wstring tmpConvertedName = L"";
                    std::wstring tmpReturnResult = L"";
                    VPGPropertyAccessorGenerationService::GetPropertyAccessorTypeName(property->GetType1(), tmpConvertedType, tmpConvertedName, tmpReturnResult);
                    result += INDENT + INDENT + L"case " + propertyName + L"::" + property->GetEnum() + L":\r\n"
                        + INDENT + INDENT + INDENT + L"return obj->Is" + property->GetPropertyName() + L"ContainKey(mapKey->GetKey" + tmpConvertedName + L"());\r\n";
                }
            }
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
        bool isHavingMapType = false;
        bool isHavingVectorType = false;
        bool isHavingGeneralType = false;
        GetIsHavingGenerateTypeMapType(enumClassProperties, isHavingGeneralType, isHavingVectorType, isHavingMapType);

        result += L"\r\n"
            "void " + propertyName + L"Accessor::_RemoveContainerElement";
        if (isHavingVectorType)
            result += L"(const size_t &objectProperty, const int64_t &index) const\r\n";
        else
            result += L"(const size_t &objectProperty, const int64_t & /*index*/) const\r\n";
        result += L"{\r\n"
            + INDENT + L"TRY\r\n";
        if (isHavingVectorType) {
            result += GetGeneralTypeIndexContentHeader(propertyName);
            for (auto const &property : enumClassProperties) {
                if (IsStartWith(property->GetMacro(), L"VECTOR") || IsStartWith(property->GetMacro(), L"ORDERED_MAP")) {
                    std::wstring tmpConvertedType = L"";
                    std::wstring tmpConvertedName = L"";
                    std::wstring tmpReturnResult = L"";
                    VPGPropertyAccessorGenerationService::GetPropertyAccessorTypeName(property->GetType1(), tmpConvertedType, tmpConvertedName, tmpReturnResult);
                    bool isOrderedMap = IsStartWith(property->GetMacro(), L"ORDERED_MAP");
                    result += INDENT + INDENT + L"case " + propertyName + L"::" + property->GetEnum() + L":\r\n"
                        + INDENT + INDENT + INDENT + L"obj->Remove" + property->GetPropertyName() + (isOrderedMap ? L"ByIndex" : L"") + L"(index);\r\n"
                        + INDENT + INDENT + INDENT + L"break;\r\n";
                }
            }
            result += generalTypeContentFooter;
        } else
            result += INDENT + INDENT + L"THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n";
        result += INDENT + L"CATCH\r\n"
            "}\r\n";

        result += L"\r\n"
            "void " + propertyName + L"Accessor::_RemoveContainerElement";
        if (isHavingMapType)
            result += L"(const size_t &objectProperty, const ITypeUnion *key) const\r\n";
        else
            result += L"(const size_t &objectProperty, const ITypeUnion * /*key*/) const\r\n";
        result += L"{\r\n"
            + INDENT + L"TRY\r\n";
        if (isHavingMapType) {
            result += GetGeneralTypeMapContentHeader(propertyName);
            for (auto const &property : enumClassProperties) {
                if (IsStartWith(property->GetMacro(), L"MAP") || IsStartWith(property->GetMacro(), L"ORDERED_MAP")) {
                    std::wstring tmpConvertedType = L"";
                    std::wstring tmpConvertedName = L"";
                    std::wstring tmpReturnResult = L"";
                    VPGPropertyAccessorGenerationService::GetPropertyAccessorTypeName(property->GetType1(), tmpConvertedType, tmpConvertedName, tmpReturnResult);
                    bool isOrderedMap = IsStartWith(property->GetMacro(), L"ORDERED_MAP");
                    result += INDENT + INDENT + L"case " + propertyName + L"::" + property->GetEnum() + L":\r\n"
                        + INDENT + INDENT + INDENT + L"obj->Remove" + property->GetPropertyName() + (isOrderedMap ? L"ByKey" : L"") + L"(mapKey->GetKey" + tmpConvertedName + L"());\r\n"
                        + INDENT + INDENT + INDENT + L"break;\r\n";
                }
            }
            result += generalTypeContentFooter;
        } else
            result += INDENT + INDENT + L"THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n";
        result += INDENT + L"CATCH\r\n"
            "}\r\n";

        result += L"\r\n"
            "void " + propertyName + L"Accessor::_ClearContainer(const size_t &objectProperty) const\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n"
            + GetGeneralContentHeader(propertyName);
        for (auto const &property : enumClassProperties) {
            result += INDENT + INDENT + L"case " + propertyName + L"::" + property->GetEnum() + L":\r\n"
                + INDENT + INDENT + INDENT + L"obj->Clear" + property->GetPropertyName() + L"();\r\n"
                + INDENT + INDENT + INDENT + L"break;\r\n";
        }
        result += generalTypeContentFooter;
        result += INDENT + L"CATCH\r\n"
            "}\r\n";
    CATCH
}

void VPGPropertyAccessorGenerationService::GenerateCpp(const LogProperty *logProperty, const std::map<std::wstring, std::wstring> &projectClassIncludeFiles,
            const std::wstring &cppFilePath, const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList)
{
    TRY
        LogService::LogInfo(logProperty, LOG_ID, L"Generate property accessor cpp file: " + cppFilePath);

        std::set<std::wstring> systemIncludeFiles;
        std::set<std::wstring> projectIncludeFiles;
        projectIncludeFiles.insert(L"exception_macro.hpp");
        projectIncludeFiles.insert(L"i_object.hpp");
        projectIncludeFiles.insert(L"i_type_union.hpp");
        projectIncludeFiles.insert(L"property_accessor_key.hpp");
        projectIncludeFiles.insert(L"property_accessor_macro.hpp");

        // class name, type, enum property
        std::map<std::wstring, std::map<std::wstring, std::vector<std::shared_ptr<VPGEnumClassProperty>>>> typeMacroMap;
        // type macro
        for (auto const &enumClass : enumClassList) {
            // include orginal class and enum class
            std::wstring className = enumClass->GetName().substr(0, enumClass->GetName().size() - proeprtyClassNameSuffix.size());

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
                if (property->GetMacro().empty())
                    continue;

                // not support set
                if (IsStartWith(property->GetMacro(), L"SET")) {
                    continue;
                }

                bool isContainer = false;
                if (IsStartWith(property->GetMacro(), L"MAP")
                    || IsStartWith(property->GetMacro(), L"ORDERED_MAP")) {
                    systemIncludeFiles.insert(L"map");
                    isContainer = true;
                }
                if (IsStartWith(property->GetMacro(), L"SET")) {
                    systemIncludeFiles.insert(L"set");
                    isContainer = true;
                }
                if (IsStartWith(property->GetMacro(), L"VECTOR")
                    || IsStartWith(property->GetMacro(), L"ORDERED_MAP")) {
                    systemIncludeFiles.insert(L"vector");
                    isContainer = true;
                }
                // container
                if (isContainer) {
                    if (typeMacroMap[enumClass->GetName()].find(containerToken) == typeMacroMap[enumClass->GetName()].end()) {
                        std::vector<std::shared_ptr<VPGEnumClassProperty>> initVector;
                        typeMacroMap[enumClass->GetName()].insert(std::make_pair(containerToken, initVector));
                    }
                    typeMacroMap[enumClass->GetName()][containerToken].push_back(property);
                }

                std::wstring type1 = property->GetType1();
                std::wstring type2 = property->GetType2();
                if (!type1.empty()) {
                    if (std::iswupper(type1[0])) {
                        includePath = VPGPropertyAccessorGenerationService::GetIncludeFile(projectClassIncludeFiles, type1);
                        if (includePath.empty())
                            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Header file of class " + type1 + L" not found");
                        projectIncludeFiles.insert(includePath);
                    } else if (Find(type1, L"string") != std::wstring::npos)
                        systemIncludeFiles.insert(L"string");
                }
                if (!type2.empty()) {
                    if (std::iswupper(type2[0])) {
                        includePath = VPGPropertyAccessorGenerationService::GetIncludeFile(projectClassIncludeFiles, type2);
                        if (includePath.empty())
                            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Header file of class " + type2 + L" not found");
                        projectIncludeFiles.insert(includePath);
                    } else if (Find(type2, L"string") != std::wstring::npos)
                        systemIncludeFiles.insert(L"string");
                }

                // object
                if (Find(property->GetMacro(), L"SPTR") != std::wstring::npos) {
                    systemIncludeFiles.insert(L"memory");
                    if (typeMacroMap[enumClass->GetName()].find(objectToken) == typeMacroMap[enumClass->GetName()].end()) {
                        std::vector<std::shared_ptr<VPGEnumClassProperty>> initVector;
                        typeMacroMap[enumClass->GetName()].insert(std::make_pair(objectToken, initVector));
                    }
                    typeMacroMap[enumClass->GetName()][objectToken].push_back(property);
                } else {
                    std::wstring type = IsStartWith(property->GetMacro(), L"MAP") || IsStartWith(property->GetMacro(), L"ORDERED_MAP") ? type2 : type1;
                    if (!type.empty() && std::iswupper(type[0]))
                        type = enumToken;
                    std::wstring convertedType = L"";
                    std::wstring converttedName = L"";
                    std::wstring returnResult = L"";
                    VPGPropertyAccessorGenerationService::GetPropertyAccessorTypeName(type, convertedType, converttedName, returnResult);
                    typeMacroMap[enumClass->GetName()][convertedType].push_back(property);
                }

            }
        }


        std::wstring headerFileName = GetFileName(cppFilePath);
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
                    VPGPropertyAccessorGenerationService::GenerateClone(enumClass.first, type, readOnly, result);
                    VPGPropertyAccessorGenerationService::GenerateContainerCount(enumClass.first, type, readWrite, result);
                    VPGPropertyAccessorGenerationService::GenerateContainerIsContainKey(enumClass.first, type, readOnly, result);
                    VPGPropertyAccessorGenerationService::GenerateContainerRemove(enumClass.first, type, writeOnly, result);
                }
            }
        }
        if (count > 0) {
            WriteFile(cppFilePath, result, true);
            LogService::LogInfo(logProperty, LOG_ID, L"Generate property accessor cpp completed.");
        } else {
            if (IsFileExists(cppFilePath)) {
                RemoveFile(cppFilePath);
                LogService::LogInfo(logProperty, LOG_ID, L"Removed property accessor cpp as no properties are acceesable.");
            } else
                LogService::LogInfo(logProperty, LOG_ID, L"No property accessor cpp need to be generated.");
        }
    CATCH
}
