#include "vpg_property_accessor_generation_service.hpp"

#include <set>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "vpg_class_helper.hpp"
#include "vpg_cpp_helper.hpp"

#define LOG_ID L"Property Accessor Generation"
const std::wstring containerToken = L"<Container>";
const std::wstring objectToken = L"<Object>";
const std::wstring enumToken = L"<Enum>";

const std::wstring generalTypeContentFooter = INDENT + INDENT + L"default:\r\n"
                + INDENT + INDENT + INDENT + L"assert(false);\r\n"
                + INDENT + INDENT + L"}\r\n";

std::wstring getGeneralTypeContentHeader(const std::wstring &classPropertyName)
{
    const std::wstring className = getClassNameFromPropertyClassName(classPropertyName);
    return INDENT + INDENT + L"auto obj = std::static_pointer_cast<" + className + L">(_Object);\r\n"
        + INDENT + INDENT + L"assert(obj != nullptr);\r\n"
        + INDENT + INDENT + L"switch(static_cast<" + classPropertyName + L">(objectProperty))\r\n"
        + INDENT + INDENT + L"{\r\n";
}

std::wstring getGeneralTypeIndexContentHeader(const std::wstring &classPropertyName)
{
    const std::wstring className = getClassNameFromPropertyClassName(classPropertyName);
    return INDENT + INDENT + L"assert(index >= -1);\r\n"
        + INDENT + INDENT + L"auto obj = std::static_pointer_cast<" + className + L">(_Object);\r\n"
        + INDENT + INDENT + L"assert(obj != nullptr);\r\n"
        + INDENT + INDENT + L"switch(static_cast<" + classPropertyName + L">(objectProperty))\r\n"
        + INDENT + INDENT + L"{\r\n";
}

std::wstring getGeneralTypeMapContentHeader(const std::wstring &classPropertyName)
{
    const std::wstring className = getClassNameFromPropertyClassName(classPropertyName);
    return INDENT + INDENT + L"auto obj = std::static_pointer_cast<" + className + L">(_Object);\r\n"
        + INDENT + INDENT + L"assert(obj != nullptr);\r\n"
        + INDENT + INDENT + L"assert(key != nullptr);\r\n"
        + INDENT + INDENT + L"switch(static_cast<" + classPropertyName + L">(objectProperty))\r\n"
        + INDENT + INDENT + L"{\r\n";
}

std::wstring getGeneralContentHeader(const std::wstring &classPropertyName)
{
    const std::wstring className = getClassNameFromPropertyClassName(classPropertyName);
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
            for (auto const &property : enumClass->getProperties()) {
                if (vcc::find(property->getType1(), L"string") != std::wstring::npos
                    || vcc::find(property->getType2(), L"string") != std::wstring::npos) {
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

void VPGPropertyAccessorGenerationService::getPropertyAccessorTypeName(const std::wstring &originalType, std::wstring &convertedType, std::wstring &convertedName, std::wstring &returnResult)
{        
    TRY
        if (originalType == objectToken) {
            convertedType = L"std::shared_ptr<vcc::IObject>";
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
            || (!originalType.empty() && vcc::isCapital(getTypeOrClassWithoutNamespace(originalType)) /* enum */)) {
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
        } else if (originalType == L"string"
            || originalType == L"std::string"
            || originalType == L"wstring"
            || originalType == L"std::wstring") {
            convertedType = L"std::wstring";
            convertedName = L"String";
            returnResult = L"L\"\"";
        } else
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"Unknown type: " + originalType);
    CATCH
}

std::vector<std::wstring> VPGPropertyAccessorGenerationService::getPropertyAccessorTypeOrder()
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
    result.push_back(L"wstring");
    result.push_back(L"std::wstring");
    result.push_back(objectToken);
    result.push_back(containerToken);
    return result;
}

std::wstring VPGPropertyAccessorGenerationService::getIncludeFile(const std::map<std::wstring, std::wstring> &projectClassIncludeFiles, const std::wstring &className)
{
    TRY
        if (vcc::isContain(projectClassIncludeFiles, className))
            return projectClassIncludeFiles.at(className);
        else
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Include file of Class " + className + L" NOT FOUND");
    CATCH
    return L"";
}

void VPGPropertyAccessorGenerationService::GenerateHpp(const vcc::LogConfig *logConfig, const std::wstring &projectPrefix,
    const std::wstring &filePathHpp, const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList)
{
    TRY
        vcc::LogService::logInfo(logConfig, LOG_ID, L"Generate property accessor hpp file: " + filePathHpp);

        std::wstring result = L"#pragma once\r\n\r\n";

        if (VPGPropertyAccessorGenerationService::IsIncludeString(enumClassList))
            result += L"#include <string>\r\n\r\n";

        result += L"#include \"base_property_accessor.hpp\"\r\n"
            "#include \"property_accessor_macro.hpp\"\r\n";

        std::map<std::wstring, std::vector<std::wstring>> namespaceClassMapping;
        for (auto const &enumClass : enumClassList) {
            std::wstring originalClassName = getTypeOrClassWithoutNamespace(enumClass->getName());
            if (!IsPropertyClass(originalClassName, projectPrefix))
                continue;
            std::vector<std::shared_ptr<VPGEnumClassAttribute>> readWrite;
            for (auto property : enumClass->getProperties()) {
                if (property->getAccessMode() != VPGEnumClassAttributeAccessMode::NoAccess)
                    readWrite.push_back(property);
            }

            std::wstring code = L"";
            std::wstring className = originalClassName + L"Accessor";
            code += L"\r\n"
                "class " + className + L" : public vcc::BasePropertyAccessor\r\n"
                "{\r\n";
            bool isCollection = false;
            bool isObject = false;
            std::map<std::wstring, std::wstring> types;
            for (auto const &property : readWrite) {
                if (property->getPropertyType() != VPGEnumClassAttributeType::Property)
                    continue;

                // property accessor not support set
                if (property->getIsSet()) {
                    vcc::LogService::LogWarning(logConfig, L"Property Accessor Generation Service", L"Property Accessor not support SET: class " + enumClass->getName() + L": " + property->getMacro());
                    continue;
                }
                
                isCollection |= property->getIsCollection();
                isObject |= property->getIsObject();
                if (!property->getIsObject()) {
                    std::wstring type = property->getIsMap() || property->getIsOrderedMap() ? property->getType2() : property->getType1();
                    if (type.empty())
                        THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"Unknow Macro found in " + enumClass->getName() + L": " + property->getMacro());
                    
                    std::wstring convertedType = L"";
                    std::wstring converttedName = L"";
                    std::wstring returnResult = L"";
                    VPGPropertyAccessorGenerationService::getPropertyAccessorTypeName(type, convertedType, converttedName, returnResult);
                    if (types.find(convertedType) == types.end())
                        types.insert(std::make_pair(convertedType, converttedName));
                }
            }

            for (auto const &type : getPropertyAccessorTypeOrder()) {
                if (types.find(type) != types.end())
                    code += INDENT + L"PROPERTY_ACCESSOR_HEADER(" + type + L", " + types[type] + L")\r\n";
            }

            if (isObject)
                code += INDENT + L"PROPERTY_ACCESSOR_OBJECT_HEADER(std::shared_ptr<vcc::IObject>, Object)\r\n";
            if (isCollection)
                code += L"\r\n" + INDENT + L"PROPERTY_ACCESSOR_CONTAINER_HEADER\r\n";
            
            code += L"\r\n"
                + INDENT + L"public:\r\n"
                + INDENT + INDENT + className + L"(std::shared_ptr<vcc::IObject> object) : vcc::BasePropertyAccessor(object) {}\r\n"
                + INDENT + INDENT + L"virtual ~" + className + L"() {}\r\n"
                "};\r\n";

            std::wstring namespaceName = getNamespaceFromClassName(enumClass->getName());
            if (namespaceClassMapping.find(namespaceName) != namespaceClassMapping.end()) {
                namespaceClassMapping[namespaceName].push_back(code);
            } else {
                namespaceClassMapping.insert(std::make_pair(namespaceName, std::vector<std::wstring>{ code }));
            }
        }
        result += generateCodeWithNamespace(namespaceClassMapping);

        vcc::writeFile(filePathHpp, result, true);
        vcc::LogService::logInfo(logConfig, LOG_ID, L"Generate property accessor hpp completed.");
    CATCH
}

void VPGPropertyAccessorGenerationService::GenerateRead(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassAttribute>> &enumClassPropertiesReadOnly, std::wstring &result)
{
    if (type == containerToken || enumClassPropertiesReadOnly.empty())
        return;
    TRY
        std::wstring convertedType = L"";
        std::wstring convertedName = L"";
        std::wstring returnResult = L"";
        VPGPropertyAccessorGenerationService::getPropertyAccessorTypeName(type, convertedType, convertedName, returnResult);

        std::map<std::wstring, std::wstring> generalCases;
        std::map<std::wstring, std::wstring> vectorCases;
        std::map<std::wstring, std::wstring> mapCases;
        std::map<std::wstring, std::wstring> mapCasekeyTypes;

        for (auto const &property : enumClassPropertiesReadOnly) {
            bool isOrderedMap = property->getIsOrderedMap();
            // General
            if (property->getIsGeneralType()) {
                std::wstring returnStr = L"obj->get" + property->getPropertyName() + L"()";
                if (type == objectToken)
                    returnStr = L"std::static_pointer_cast<vcc::IObject>(" + returnStr + L")";
                else if (!property->getType1().empty() && vcc::isCapital(property->getType1()))
                    returnStr = L"static_cast<long>(" + returnStr + L")";
                else if ((property->getType1() == L"string" || property->getType1() == L"std::string"))
                    returnStr = L"vcc::str2wstr(" + returnStr + L")";
                generalCases.insert(std::make_pair(property->getEnum(), returnStr));
            }
            // Vector
            if (property->getIsVector() || property->getIsOrderedMap()) {
                std::wstring returnStr = L"obj->get" + property->getPropertyName();
                std::wstring tmpConvertedType = L"";
                std::wstring tmpConvertedName = L"";
                std::wstring tmpReturnResult = L"";
                VPGPropertyAccessorGenerationService::getPropertyAccessorTypeName(property->getType1(), tmpConvertedType, tmpConvertedName, tmpReturnResult);
                
                if (type == objectToken)
                    returnStr = L"std::static_pointer_cast<vcc::IObject>(" + returnStr + L"AtIndex(index)" + (isOrderedMap ? L".second" : L"") + L")";
                else {
                    std::wstring type = isOrderedMap ? property->getType2() : property->getType1();
                    if (!type.empty() && vcc::isCapital(type))
                        returnStr = L"static_cast<long>(" + returnStr + L"AtIndex(index)"+ (isOrderedMap ? L".second" : L"") + L")";
                    else if (type == L"string")
                        returnStr = L"vcc::str2wstr(" + returnStr + L"AtIndex(index)" + (isOrderedMap ? L".second" : L"") + L")";
                    else
                        returnStr = returnStr + L"AtIndex(index)" + (isOrderedMap ? L".second" : L"");
                }
                vectorCases.insert(std::make_pair(property->getEnum(), returnStr));
            }
            // Map
            if (property->getIsMap() || property->getIsOrderedMap()) {
                // key
                std::wstring mapGetKey = (property->getType1() == L"wstring" || property->getType1() == L"std::wstring") ? L"keyPtr" : L"*keyPtr";
                std::wstring castType = (property->getType1() == L"wstring" || property->getType1() == L"std::wstring") ? L"wchar_t" : property->getType1();
                mapCasekeyTypes.insert(std::make_pair(property->getEnum(), castType));

                // value
                std::wstring returnStr = L"obj->get" + property->getPropertyName();
                if (type == objectToken)
                    returnStr = L"std::static_pointer_cast<vcc::IObject>(" + returnStr + L"AtKey(" + mapGetKey + L")" + L")";
                else if (!property->getType2().empty() && vcc::isCapital(property->getType2()))
                    returnStr = L"static_cast<long>(" + returnStr + L"AtKey(" + mapGetKey + L")" + L")";
                else if (property->getType2() == L"string" || property->getType2() == L"std::string")
                    returnStr = L"vcc::str2wstr(" + returnStr + L"AtKey(" + mapGetKey + L")" + L")";
                else
                    returnStr = returnStr + L"AtKey(" + mapGetKey + L")";
                mapCases.insert(std::make_pair(property->getEnum(), returnStr));
            }
        }

        // General Type
        result += L"\r\n"
            + convertedType + L" " + propertyName + L"Accessor::_read" + convertedName + L"(const int64_t &objectProperty) const\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n";
        if (!generalCases.empty()) {
            result += getGeneralTypeContentHeader(propertyName);
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
            + convertedType + L" " + propertyName + L"Accessor::_read" + convertedName + L"AtIndex(const int64_t &objectProperty, const int64_t &" + (!vectorCases.empty() ? L"index" : L"/*index*/") + L") const\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n";
        if (!vectorCases.empty()) {
            result += getGeneralTypeIndexContentHeader(propertyName);
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
            + convertedType + L" " + propertyName + L"Accessor::_read" + convertedName + L"AtKey(const int64_t &objectProperty, const void *" + (!mapCases.empty() ? L"key" : L"/*key*/") + L") const\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n";
        if (!mapCases.empty()) {
            result += getGeneralTypeMapContentHeader(propertyName);
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

void VPGPropertyAccessorGenerationService::GenerateWrite(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassAttribute>> &enumClassPropertiesWriteOnly, std::wstring &result)
{
    if (type == containerToken || enumClassPropertiesWriteOnly.empty())
        return;
    TRY
        std::wstring convertedType = L"";
        std::wstring convertedName = L"";
        std::wstring returnResult = L"";
        VPGPropertyAccessorGenerationService::getPropertyAccessorTypeName(type, convertedType, convertedName, returnResult);

        std::map<std::wstring, std::wstring> generalCases;
        std::map<std::wstring, std::wstring> vectorCases;
        std::map<std::wstring, std::wstring> mapCases;
        std::map<std::wstring, std::wstring> mapCasekeyTypes;

        for (auto const &property : enumClassPropertiesWriteOnly) {
            // General
            if (property->getIsGeneralType()) {
                std::wstring returnStr = L"obj->set" + property->getPropertyName();

                if (type == objectToken)
                    returnStr += L"(std::static_pointer_cast<" + property->getType1() + L">(value));\r\n";
                else if (!property->getType1().empty() && vcc::isCapital(property->getType1()))
                    returnStr += L"(static_cast<" + property->getType1() + L">(value));\r\n";
                else if ((property->getType1() == L"string" || property->getType1() == L"std::string"))
                    returnStr += L"(wstr2str(value));\r\n";
                else
                    returnStr += L"(value);\r\n";
                generalCases.insert(std::make_pair(property->getEnum(), returnStr));
            }
            // Vector
            if (property->getIsVector()) {
                std::wstring returnStr = L"";

                bool isOrderedMap = property->getIsOrderedMap();
                std::wstring type = isOrderedMap ? property->getType2() : property->getType1();
                if (property->getIsObject()) {
                    returnStr = INDENT + INDENT + INDENT + L"if (index > -1)\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->set" + property->getPropertyName() + L"AtIndex(index, std::static_pointer_cast<" + type + L">(value));\r\n"
                        + INDENT + INDENT + INDENT + L"else\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->insert" + property->getPropertyName() + L"(std::static_pointer_cast<" + type + L">(value));\r\n";
                } else if (!type.empty() && vcc::isCapital(type)) {
                    returnStr = INDENT + INDENT + INDENT + L"if (index > -1)\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->set" + property->getPropertyName() + L"AtIndex(index, static_cast<" + type + L">(value));\r\n"
                        + INDENT + INDENT + INDENT + L"else\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->insert" + property->getPropertyName() + L"(static_cast<" + type + L">(value));\r\n";
                } else if (type == L"string") {
                    returnStr = INDENT + INDENT + INDENT + L"if (index > -1)\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->set" + property->getPropertyName() + L"AtIndex(index, wstr2str(value));\r\n"
                        + INDENT + INDENT + INDENT + L"else\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->insert" + property->getPropertyName() + L"(wstr2str(value));\r\n";                    
                } else {
                    returnStr = INDENT + INDENT + INDENT + L"if (index > -1)\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->set" + property->getPropertyName() + L"AtIndex(index, value);\r\n"
                        + INDENT + INDENT + INDENT + L"else\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->insert" + property->getPropertyName() + L"(value);\r\n";
                };

                vectorCases.insert(std::make_pair(property->getEnum(), returnStr));
            }
            // Map
            if (property->getIsMap() || property->getIsOrderedMap()) {
                // key
                std::wstring mapGetKey = (property->getType1() == L"wstring" || property->getType1() == L"std::wstring") ? L"keyPtr" : L"*keyPtr";
                std::wstring castType = (property->getType1() == L"wstring" || property->getType1() == L"std::wstring") ? L"wchar_t" : property->getType1();
                mapCasekeyTypes.insert(std::make_pair(property->getEnum(), castType));

                // value
                std::wstring returnStr = L"";
                if (type == objectToken) {
                    returnStr += INDENT + INDENT + INDENT + L"if (obj->is" + property->getPropertyName() + L"ContainKey(" + mapGetKey + L"))\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->set" + property->getPropertyName() + L"AtKey(" + mapGetKey + L", std::static_pointer_cast<" + property->getType2() + L">(value));\r\n"
                        + INDENT + INDENT + INDENT + L"else\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->insert" + property->getPropertyName() + L"AtKey(" + mapGetKey + L", std::static_pointer_cast<" + property->getType2() + L">(value));\r\n";
                } else {
                    std::wstring type = property->getType2();
                    if (!type.empty() && vcc::isCapital(type)) {
                        returnStr += INDENT + INDENT + INDENT + L"if (obj->is" + property->getPropertyName() + L"ContainKey(" + mapGetKey + L"))\r\n"
                            + INDENT + INDENT + INDENT + INDENT + L"obj->set" + property->getPropertyName() + L"AtKey(" + mapGetKey + L", static_cast<" + type + L">(value));\r\n"
                            + INDENT + INDENT + INDENT + L"else\r\n"
                            + INDENT + INDENT + INDENT + INDENT + L"obj->insert" + property->getPropertyName() + L"AtKey(" + mapGetKey + L", static_cast<" + type + L">(value));\r\n";
                    } else if (type == L"string") {
                        returnStr += INDENT + INDENT + INDENT + L"if (obj->is" + property->getPropertyName() + L"ContainKey(" + mapGetKey + L"))\r\n"
                            + INDENT + INDENT + INDENT + INDENT + L"obj->set" + property->getPropertyName() + L"AtKey(" + mapGetKey + L", vcc::str2wstr(value));\r\n"
                            + INDENT + INDENT + INDENT + L"else\r\n"
                            + INDENT + INDENT + INDENT + INDENT + L"obj->insert" + property->getPropertyName() + L"AtKey(" + mapGetKey + L", wstr2str(value));\r\n";
                    } else {
                        returnStr += INDENT + INDENT + INDENT + L"if (obj->is" + property->getPropertyName() + L"ContainKey(" + mapGetKey + L"))\r\n"
                            + INDENT + INDENT + INDENT + INDENT + L"obj->set" + property->getPropertyName() + L"AtKey(" + mapGetKey + L", value);\r\n"
                            + INDENT + INDENT + INDENT + L"else\r\n"
                            + INDENT + INDENT + INDENT + INDENT + L"obj->insert" + property->getPropertyName() + L"AtKey(" + mapGetKey + L", value);\r\n";
                    }                    
                }
                mapCases.insert(std::make_pair(property->getEnum(), returnStr));
            }
        }

        // General Type
        result += L"\r\n"
            "void " + propertyName + L"Accessor::_write" + convertedName + L"(const int64_t &objectProperty, " + (vcc::isEqual(type, objectToken) ? L"" : L"const ")  + convertedType + L" " + (vcc::isEqual(type, objectToken) ? L"" : L"&") + (!generalCases.empty() ? L"value" : L"/*value*/") + L")\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n";
        if (!generalCases.empty()) {
            result += getGeneralTypeContentHeader(propertyName);
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
            "void " + propertyName + L"Accessor::_write" + convertedName + L"AtIndex(const int64_t &objectProperty, " + (vcc::isEqual(type, objectToken) ? L"" : L"const ")  + convertedType + L" " + (vcc::isEqual(type, objectToken) ? L"" : L"&") + (!vectorCases.empty() ? L"value" : L"/*value*/") + L", const int64_t &" + (!vectorCases.empty() ? L"index" : L"/*index*/") + L")\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n";
        if (!vectorCases.empty()) {
            result += getGeneralTypeIndexContentHeader(propertyName);
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
            "void " + propertyName + L"Accessor::_write" + convertedName + L"AtKey(const int64_t &objectProperty, " + (vcc::isEqual(type, objectToken) ? L"" : L"const ")  + convertedType + L" " + (vcc::isEqual(type, objectToken) ? L"" : L"&") + (!mapCases.empty() ? L"value" : L"/*value*/") + L", const void *" + (!mapCases.empty() ? L"key" : L"/*key*/") + L")\r\n"
            "{\r\n"
                + INDENT + L"TRY\r\n";
        if (!mapCases.empty()) {
            result += getGeneralTypeMapContentHeader(propertyName);
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

void VPGPropertyAccessorGenerationService::GenerateInsert(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassAttribute>> &enumClassPropertiesWriteOnly, std::wstring &result)
{
    if (type == containerToken || enumClassPropertiesWriteOnly.empty())
        return;
    TRY
        std::wstring convertedType = L"";
        std::wstring convertedName = L"";
        std::wstring returnResult = L"";
        VPGPropertyAccessorGenerationService::getPropertyAccessorTypeName(type, convertedType, convertedName, returnResult);

        std::map<std::wstring, std::wstring> vectorCases;

        for (auto const &property : enumClassPropertiesWriteOnly) {
            if (property->getIsVector()) {
                std::wstring returnStr = L"";

                bool isOrderedMap = property->getIsOrderedMap();
                std::wstring type = isOrderedMap ? property->getType2() : property->getType1();
                if (property->getIsObject()) {
                    returnStr += INDENT + INDENT + INDENT + L"if (index > -1)\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->insert" + property->getPropertyName() + L"AtIndex(index, std::static_pointer_cast<" + type + L">(value));\r\n"
                        + INDENT + INDENT + INDENT + L"else\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->insert" + property->getPropertyName() + L"(std::static_pointer_cast<" + type + L">(value));\r\n";
                } else if (!type.empty() && vcc::isCapital(type)) {
                    returnStr += INDENT + INDENT + INDENT + L"if (index > -1)\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->insert" + property->getPropertyName() + L"AtIndex(index, static_cast<" + type + L">(value));\r\n"
                        + INDENT + INDENT + INDENT + L"else\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->insert" + property->getPropertyName() + L"(static_cast<" + type + L">(value));\r\n";
                } else if (type == L"string") {
                    returnStr += INDENT + INDENT + INDENT + L"if (index > -1)\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->insert" + property->getPropertyName() + L"AtIndex(index, wstr2str(value));\r\n"
                        + INDENT + INDENT + INDENT + L"else\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->insert" + property->getPropertyName() + L"(wstr2str(value));\r\n";
                } else {
                    returnStr += INDENT + INDENT + INDENT + L"if (index > -1)\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->insert" + property->getPropertyName() + L"AtIndex(index, value);\r\n"
                        + INDENT + INDENT + INDENT + L"else\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"obj->insert" + property->getPropertyName() + L"(value);\r\n";
                }
                vectorCases.insert(std::make_pair(property->getEnum(), returnStr));
            }
        }
        result += L"\r\n"
            "void " + propertyName + L"Accessor::_insert" + convertedName + L"AtIndex(const int64_t &objectProperty, " + (vcc::isEqual(type, objectToken) ? L"" : L"const ")  + convertedType + L" " + (vcc::isEqual(type, objectToken) ? L"" : L"&") + (!vectorCases.empty() ? L"value" : L"/*value*/") + L", const int64_t &" + (!vectorCases.empty() ? L"index" : L"/*index*/") + L")\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n";
        if (!vectorCases.empty()) {
            result += getGeneralTypeIndexContentHeader(propertyName);
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

void VPGPropertyAccessorGenerationService::GenerateClone(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassAttribute>> &enumClassPropertiesReadOnly, std::wstring &result)
{
    if (type == containerToken || type != objectToken || enumClassPropertiesReadOnly.empty())
        return;
    TRY
        std::wstring convertedType = L"";
        std::wstring convertedName = L"";
        std::wstring returnResult = L"";
        VPGPropertyAccessorGenerationService::getPropertyAccessorTypeName(type, convertedType, convertedName, returnResult);
        
        std::map<std::wstring, std::wstring> generalCases;
        std::map<std::wstring, std::wstring> vectorCases;
        std::map<std::wstring, std::wstring> mapCases;

        for (auto const &property : enumClassPropertiesReadOnly) {
            if (!property->getIsObject())
                continue;
            // General
            if (property->getIsGeneralType()) {
                std::wstring returnStr = L"std::static_pointer_cast<vcc::IObject>(obj->get" + property->getPropertyName() + L"()->clone())";
                generalCases.insert(std::make_pair(property->getEnum(), returnStr));
            }
            // Vector
            if (property->getIsVector() || property->getIsOrderedMap()) {
                std::wstring returnStr = L"std::static_pointer_cast<vcc::IObject>(obj->clone" + property->getPropertyName() + L"AtIndex(index))";
                vectorCases.insert(std::make_pair(property->getEnum(), returnStr));
            }
            // Map
            if (property->getIsMap() || property->getIsOrderedMap()) {
                std::wstring castType = (property->getType1() == L"wstring" || property->getType1() == L"std::wstring") ? L"wchar_t" : property->getType1();
                std::wstring mapGetKey = (property->getType1() == L"wstring" || property->getType1() == L"std::wstring") ? L"keyPtr" : L"*keyPtr";
                std::wstring returnStr = INDENT + INDENT + INDENT + L"auto keyPtr = static_cast<const " + castType + L" *>(key);\r\n"
                    + INDENT + INDENT + INDENT + L"assert(keyPtr != nullptr);\r\n"
                    + INDENT + INDENT + INDENT + L"if (keyPtr == nullptr)\r\n"
                    + INDENT + INDENT + INDENT + INDENT + L"THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
                    + INDENT + INDENT + INDENT + L"return std::static_pointer_cast<vcc::IObject>(obj->clone" + property->getPropertyName() + L"AtKey(" + mapGetKey + L"));\r\n";
                mapCases.insert(std::make_pair(property->getEnum(), returnStr));
            }
        }

        // General Type
        result += L"\r\n"
            + convertedType + L" " + propertyName + L"Accessor::_clone" + convertedName + L"(const int64_t &objectProperty) const\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n";
        if (!generalCases.empty()) {
            result += getGeneralTypeContentHeader(propertyName);
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
            + convertedType + L" " + propertyName + L"Accessor::_clone" + convertedName + L"AtIndex(const int64_t &objectProperty, const int64_t &" + (!vectorCases.empty() ? L"index" : L"/*index*/") + L") const\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n";
        if (!vectorCases.empty()) {
            result += getGeneralTypeIndexContentHeader(propertyName);
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
            + convertedType + L" " + propertyName + L"Accessor::_clone" + convertedName + L"AtKey(const int64_t &objectProperty, const void *" + (!mapCases.empty() ? L"key" : L"/*key*/") + L") const\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n";
        if (!mapCases.empty()) {
            result += getGeneralTypeMapContentHeader(propertyName);
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

void VPGPropertyAccessorGenerationService::GenerateContainerCount(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassAttribute>> &enumClassProperties, std::wstring &result)
{
    if (type != containerToken || enumClassProperties.empty())
        return;
    TRY
        result += L"\r\n"
            "size_t " + propertyName + L"Accessor::_getCount(const int64_t &objectProperty) const\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n" 
            + getGeneralContentHeader(propertyName);
        for (auto const &property : enumClassProperties) {
            if (!property->getIsCollection())
                continue;
            result += INDENT + INDENT + L"case " + propertyName + L"::" + property->getEnum() + L":\r\n"
                + INDENT + INDENT + INDENT + L"return obj->get" + property->getPropertyName() + L"().size();\r\n";
        }
        result += generalTypeContentFooter
            + INDENT + L"CATCH\r\n"
            + INDENT + L"return 0;\r\n"
            "}\r\n";
    CATCH
}

void VPGPropertyAccessorGenerationService::GenerateContainerMapKey(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassAttribute>> &enumClassProperties, std::wstring &result)
{
    if (type != containerToken || enumClassProperties.empty())
        return;
    TRY
        std::map<std::wstring, std::wstring> mapCases;
        for (auto const &property : enumClassProperties)
            if (property->getIsMap() || property->getIsOrderedMap())
                mapCases.insert(std::make_pair(property->getEnum(), L"obj->get" + property->getPropertyName() + L"VoidKeys()"));

        result += L"\r\n"
            "std::set<void *> " + propertyName + L"Accessor::_getMapKeys(const int64_t &objectProperty) const\r\n"
            "{\r\n"
            + INDENT + L"std::set<void *> result;\r\n"
            + INDENT + L"TRY\r\n";
        if (!mapCases.empty()) {
            result += getGeneralContentHeader(propertyName);
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

void VPGPropertyAccessorGenerationService::GenerateContainerisContainKey(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassAttribute>> &enumClassProperties, std::wstring &result)
{
    if (type != containerToken || enumClassProperties.empty())
        return;
    TRY
        std::map<std::wstring, std::wstring> mapCases;
        for (auto const &property : enumClassProperties)
            if (property->getIsMap() || property->getIsOrderedMap()) {
                std::wstring castType = (property->getType1() == L"wstring" || property->getType1() == L"std::wstring") ? L"wchar_t" : property->getType1();
                std::wstring mapGetKey = (property->getType1() == L"wstring" || property->getType1() == L"std::wstring") ? L"keyPtr" : L"*keyPtr";
                mapCases.insert(std::make_pair(property->getEnum(), 
                    INDENT + INDENT + INDENT + L"auto keyPtr = static_cast<const " + castType + L" *>(key);\r\n"
                    + INDENT + INDENT + INDENT + L"assert(keyPtr != nullptr);\r\n"
                    + INDENT + INDENT + INDENT + L"if (keyPtr == nullptr)\r\n"
                    + INDENT + INDENT + INDENT + INDENT + L"THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
                    + INDENT + INDENT + INDENT + L"return obj->is" + property->getPropertyName() + L"ContainKey(" + mapGetKey + L");\r\n"));
            }
                
        result += L"\r\n"
            "bool " + propertyName + L"Accessor::_isContainKey(const int64_t &objectProperty, const void *" + (!mapCases.empty() ? L"key" : L"/*key*/") + L") const\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n";
        if (!mapCases.empty()) {
            result += getGeneralTypeMapContentHeader(propertyName);
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

void VPGPropertyAccessorGenerationService::GenerateContainerRemove(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassAttribute>> &enumClassProperties, std::wstring &result)
{
    if (type != containerToken || enumClassProperties.empty())
        return;
    TRY
        std::map<std::wstring, std::wstring> vectorCasesGeneral;
        std::map<std::wstring, std::wstring> vectorCasesObject;
        std::map<std::wstring, std::wstring> vectorCasesIndex;
        std::map<std::wstring, std::wstring> mapCases;

        for (auto const &property : enumClassProperties) {
            if (property->getIsVector()) {
                if (property->getIsObject()) {
                    std::wstring result = INDENT + INDENT + INDENT + L"obj->remove" + property->getPropertyName() + L"(value);\r\n";
                    vectorCasesObject.insert(std::make_pair(property->getEnum(), result));
                } else {
                    std::wstring castType = (property->getType1() == L"wstring" || property->getType1() == L"std::wstring") ? L"wchar_t" : property->getType1();
                    std::wstring valueAssign = (property->getType1() == L"wstring" || property->getType1() == L"std::wstring") ? L"valuePtr" : L"*valuePtr";
                    std::wstring result = INDENT + INDENT + INDENT + L"auto valuePtr = static_cast<const " + castType + L" *>(value);\r\n"
                        + INDENT + INDENT + INDENT + L"assert(valuePtr != nullptr);\r\n"
                        + INDENT + INDENT + INDENT + L"obj->remove" + property->getPropertyName() + L"(" + valueAssign + L");\r\n";
                    vectorCasesGeneral.insert(std::make_pair(property->getEnum(), result));
                }
            }
            if (property->getIsVector() || property->getIsOrderedMap())
                vectorCasesIndex.insert(std::make_pair(property->getEnum(), L"obj->remove" + property->getPropertyName() + L"AtIndex(index)"));
            if (property->getIsMap() || property->getIsOrderedMap()) {
                std::wstring castType = (property->getType1() == L"wstring" || property->getType1() == L"std::wstring") ? L"wchar_t" : property->getType1();
                std::wstring mapGetKey = (property->getType1() == L"wstring" || property->getType1() == L"std::wstring") ? L"keyPtr" : L"*keyPtr";
                std::wstring result = INDENT + INDENT + INDENT + L"auto keyPtr = static_cast<const " + castType + L" *>(key);\r\n"
                    + INDENT + INDENT + INDENT + L"assert(keyPtr != nullptr);\r\n"
                    + INDENT + INDENT + INDENT + L"if (keyPtr == nullptr)\r\n"
                    + INDENT + INDENT + INDENT + INDENT + L"THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
                    + INDENT + INDENT + INDENT + L"obj->remove" + property->getPropertyName() + L"AtKey(" + mapGetKey + L");\r\n";
                mapCases.insert(std::make_pair(property->getEnum(), result));
            }
        }

        const std::wstring className = getClassNameFromPropertyClassName(propertyName);
        result += L"\r\n"
            "void " + propertyName + L"Accessor::_remove(const int64_t &objectProperty, const void *" + (!vectorCasesGeneral.empty() ? L"value" : L"/*value*/") + L")\r\n"
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
            "void " + propertyName + L"Accessor::_removeObject(const int64_t &objectProperty, const vcc::IObject *" + (!vectorCasesObject.empty() ? L"value" : L"/*value*/") + L")\r\n"
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
            "void " + propertyName + L"Accessor::_removeAtIndex(const int64_t &objectProperty, const int64_t &" + (!vectorCasesIndex.empty() ? L"index" : L"/*index*/" ) + L")\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n";
        if (!vectorCasesIndex.empty()) {
            result += getGeneralTypeIndexContentHeader(propertyName);
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
            "void " + propertyName + L"Accessor::_removeAtKey(const int64_t &objectProperty, const void *" + (!mapCases.empty()? L"key" : L"/*key*/") + L")\r\n";
        result += L"{\r\n"
            + INDENT + L"TRY\r\n";
        if (!mapCases.empty()) {
            result += getGeneralTypeMapContentHeader(propertyName);
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
            "void " + propertyName + L"Accessor::_clear(const int64_t &objectProperty)\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n"
            + getGeneralContentHeader(propertyName);
        for (auto const &property : enumClassProperties) {
            if (!property->getIsCollection())
                continue;
            result += INDENT + INDENT + L"case " + propertyName + L"::" + property->getEnum() + L":\r\n"
                + INDENT + INDENT + INDENT + L"obj->clear" + property->getPropertyName() + L"();\r\n"
                + INDENT + INDENT + INDENT + L"break;\r\n";
        }
        result += generalTypeContentFooter;
        result += INDENT + L"CATCH\r\n"
            "}\r\n";
    CATCH
}

void VPGPropertyAccessorGenerationService::GenerateCpp(const vcc::LogConfig *logConfig, const std::wstring &projectPrefix,
    const std::map<std::wstring, std::wstring> &projectClassIncludeFiles,
    const std::wstring &filePathCpp, const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList)
{
    TRY
        vcc::LogService::logInfo(logConfig, LOG_ID, L"Generate property accessor cpp file: " + filePathCpp);

        std::set<std::wstring> systemIncludeFiles;
        std::set<std::wstring> projectIncludeFiles;
        projectIncludeFiles.insert(L"exception_macro.hpp");
        projectIncludeFiles.insert(L"i_object.hpp");
        projectIncludeFiles.insert(L"property_accessor_macro.hpp");

        // class name, type, enum property
        std::map<std::wstring, std::map<std::wstring, std::vector<std::shared_ptr<VPGEnumClassAttribute>>>> typeMacroMap;
        // type macro
        for (auto const &enumClass : enumClassList) {
            if (!IsPropertyClass(enumClass->getName(), projectPrefix))
                continue;
            // include orginal class and enum class
            std::wstring className = getClassNameFromPropertyClassName(enumClass->getName());

            // include object class
            std::wstring includePath = VPGPropertyAccessorGenerationService::getIncludeFile(projectClassIncludeFiles, className);
            if (includePath.empty())
                THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Header file of class " + className + L" not found");
            projectIncludeFiles.insert(includePath);

            // include enum class
            includePath = VPGPropertyAccessorGenerationService::getIncludeFile(projectClassIncludeFiles, enumClass->getName());
            if (includePath.empty())
                THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Header file of enum class " + enumClass->getName() + L" not found");
            projectIncludeFiles.insert(includePath);

            std::map<std::wstring, std::vector<std::shared_ptr<VPGEnumClassAttribute>>> initMap;
            typeMacroMap.insert(std::make_pair(enumClass->getName(), initMap));
            for (auto const &property : enumClass->getProperties()) {
                if (property->getPropertyType() != VPGEnumClassAttributeType::Property)
                    continue;

                // not support set
                if (property->getIsSet())
                    continue;

                bool isContainer = property->getIsCollection();
                if (property->getIsVector())
                    systemIncludeFiles.insert(L"vector");
                if (property->getIsMap())
                    systemIncludeFiles.insert(L"map");
                if (property->getIsOrderedMap()) {
                    systemIncludeFiles.insert(L"vector");
                    systemIncludeFiles.insert(L"map");
                }
                if (property->getIsSet())
                    systemIncludeFiles.insert(L"set");

                // container
                if (isContainer) {
                    if (!vcc::isContain(typeMacroMap[enumClass->getName()], containerToken)) {
                        std::vector<std::shared_ptr<VPGEnumClassAttribute>> initVector;
                        typeMacroMap[enumClass->getName()].insert(std::make_pair(containerToken, initVector));
                    }
                    typeMacroMap[enumClass->getName()][containerToken].push_back(property);
                }

                std::wstring type1 = property->getType1();
                std::wstring type2 = property->getType2();
                if (!type1.empty()) {
                    if (vcc::isCapital(getTypeOrClassWithoutNamespace(type1))) {
                        includePath = VPGPropertyAccessorGenerationService::getIncludeFile(projectClassIncludeFiles, type1);
                        if (includePath.empty())
                            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Header file of class " + type1 + L" not found");
                        projectIncludeFiles.insert(includePath);
                    } else if (vcc::find(type1, L"string") != std::wstring::npos)
                        systemIncludeFiles.insert(L"string");
                }
                if (!type2.empty()) {
                    if (vcc::isCapital(getTypeOrClassWithoutNamespace(type2))) {
                        includePath = VPGPropertyAccessorGenerationService::getIncludeFile(projectClassIncludeFiles, type2);
                        if (includePath.empty())
                            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Header file of class " + type2 + L" not found");
                        projectIncludeFiles.insert(includePath);
                    } else if (vcc::find(type2, L"string") != std::wstring::npos)
                        systemIncludeFiles.insert(L"string");
                }

                // object
                if (property->getIsObject()) {
                    systemIncludeFiles.insert(L"memory");
                    if (!vcc::isContain(typeMacroMap[enumClass->getName()], (objectToken))) {
                        std::vector<std::shared_ptr<VPGEnumClassAttribute>> initVector;
                        typeMacroMap[enumClass->getName()].insert(std::make_pair(objectToken, initVector));
                    }
                    typeMacroMap[enumClass->getName()][objectToken].push_back(property);
                } else {
                    std::wstring type = property->getIsMap() || property->getIsOrderedMap() ? type2 : type1;
                    if (!type.empty() && vcc::isCapital(type))
                        type = enumToken;
                    std::wstring convertedType = L"";
                    std::wstring converttedName = L"";
                    std::wstring returnResult = L"";
                    VPGPropertyAccessorGenerationService::getPropertyAccessorTypeName(type, convertedType, converttedName, returnResult);
                    typeMacroMap[enumClass->getName()][convertedType].push_back(property);
                }
            }
        }

        std::wstring headerFileName = vcc::getFileName(filePathCpp);
        vcc::replace(headerFileName, L".cpp", L".hpp");
        std::wstring result = L"#include " + vcc::getEscapeStringWithQuote(vcc::EscapeStringType::DoubleQuote, headerFileName) + L"\r\n";
        // system include files
        if (!systemIncludeFiles.empty())
            result += L"\r\n";
        for (auto const &includeFiles : systemIncludeFiles)
            result += L"#include <" + includeFiles + L">\r\n";
        // project include files
        result += L"\r\n";
        for (auto const &includeFiles : projectIncludeFiles)
            result += L"#include " + vcc::getEscapeStringWithQuote(vcc::EscapeStringType::DoubleQuote, includeFiles) + L"\r\n";
        
        std::map<std::wstring, std::vector<std::wstring>> namespaceClassMapping;
        size_t count = 0;
        for (auto const &enumClass : typeMacroMap) {
            for (auto const &type : getPropertyAccessorTypeOrder()) {
                if (enumClass.second.find(type) != enumClass.second.end()) {
                    std::vector<std::shared_ptr<VPGEnumClassAttribute>> readOnly, writeOnly, readWrite;
                    for (auto property : enumClass.second.at(type)) {
                        switch (property->getAccessMode())
                        {
                        case VPGEnumClassAttributeAccessMode::NoAccess:
                            break;
                        case VPGEnumClassAttributeAccessMode::ReadOnly:
                            readOnly.push_back(property);
                            readWrite.push_back(property);
                            break;
                        case VPGEnumClassAttributeAccessMode::ReadWrite:
                            readOnly.push_back(property);
                            writeOnly.push_back(property);
                            readWrite.push_back(property);
                            break;
                        case VPGEnumClassAttributeAccessMode::WriteOnly:
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

                    std::wstring className = getTypeOrClassWithoutNamespace(enumClass.first);
                    std::wstring code = L"";
                    VPGPropertyAccessorGenerationService::GenerateRead(className, type, readOnly, code);
                    VPGPropertyAccessorGenerationService::GenerateWrite(className, type, writeOnly, code);
                    VPGPropertyAccessorGenerationService::GenerateInsert(className, type, writeOnly, code);
                    VPGPropertyAccessorGenerationService::GenerateClone(className, type, readOnly, code);
                    VPGPropertyAccessorGenerationService::GenerateContainerCount(className, type, readWrite, code);
                    VPGPropertyAccessorGenerationService::GenerateContainerMapKey(className, type, readWrite, code);
                    VPGPropertyAccessorGenerationService::GenerateContainerisContainKey(className, type, readOnly, code);
                    VPGPropertyAccessorGenerationService::GenerateContainerRemove(className, type, writeOnly, code);
                
                    std::wstring namespaceName = getNamespaceFromClassName(enumClass.first);
                    if (namespaceClassMapping.find(namespaceName) == namespaceClassMapping.end()) {
                        namespaceClassMapping.insert(std::make_pair(namespaceName, std::vector<std::wstring>()));
                    }
                    namespaceClassMapping[namespaceName].push_back(code);
                }
            }
        }
        if (count > 0) {
            vcc::writeFile(filePathCpp, result + generateCodeWithNamespace(namespaceClassMapping), true);
            vcc::LogService::logInfo(logConfig, LOG_ID, L"Generate property accessor cpp completed.");
        } else {
            if (vcc::isFilePresent(filePathCpp)) {
                vcc::removeFile(filePathCpp);
                vcc::LogService::logInfo(logConfig, LOG_ID, L"Removed property accessor cpp as no properties are acceesable.");
            } else
                vcc::LogService::logInfo(logConfig, LOG_ID, L"No property accessor cpp need to be generated.");
        }
    CATCH
}
