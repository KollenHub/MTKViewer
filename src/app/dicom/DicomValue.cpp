#include "DicomValue.h"
#include <dcmtk/dcmdata/dcsequen.h>
#include <core/Logger.h>

DicomValue::DicomValue() : m_valueType(UNKNOWN), m_vr("UN") {}

DicomValue::DicomValue(DcmElement *element)
{
    ExtractValue(element);
    m_ShowValue = QString::fromLocal8Bit(toString().c_str());
}

void DicomValue::ExtractValue(DcmElement *element)
{
    DcmVR vr = element->getVR();
    m_vr = vr.getVRName();

    switch (vr.getEVR())
    {
    // 字符串类型
    case EVR_AE:
    case EVR_AS:
    case EVR_CS:
    case EVR_DA:
    case EVR_DS:
    case EVR_DT:
    case EVR_IS:
    case EVR_LO:
    case EVR_LT:
    case EVR_PN:
    case EVR_SH:
    case EVR_ST:
    case EVR_TM:
    case EVR_UI:
    case EVR_UT:
    case EVR_UC:
    case EVR_UR:
        ExtractStringValue(element);
        break;

    // 数值类型
    case EVR_FL:
        ExtractFloatValue(element);
        break;
    case EVR_FD:
        ExtractDoubleValue(element);
        break;
    case EVR_SL:
        ExtractInt32Value(element);
        break;
    case EVR_SS:
        ExtractInt16Value(element);
        break;
    case EVR_UL:
        ExtractUint32Value(element);
        break;
    case EVR_US:
        ExtractUint16Value(element);
        break;

    // 二进制数据
    case EVR_OB:
    case EVR_OD:
    case EVR_OF:
    case EVR_OL:
    case EVR_OW:
        ExtractBinaryValue(element);
        break;

    // 特殊类型
    case EVR_AT:
        ExtractAttributeTagValue(element);
        break;
    case EVR_SQ:
        ExtractSequenceValue(element);
        break;
    case EVR_UN:
        ExtractUnknownValue(element);
        break;

    default:
        ExtractUnknownValue(element);
        break;
    }
}

bool DicomValue::updateValue(const QString &value)
{
    try
    {
        switch (m_valueType)
        {
        case STRING:
            m_value = value.toStdString();
            break;
        case INT16:
            m_value = value.toInt();
            break;
        case INT32:
            m_value = value.toInt();
            break;
        case UINT16:
            m_value = value.toUInt();
            break;
        case UINT32:
            m_value = value.toUInt();
            break;
        case FLOAT:
            m_value = value.toFloat();
            break;
        case DOUBLE:
            m_value = value.toDouble();
            break;
        case ATTRIBUTE_TAG:
            m_value = value.toStdString();
            break;
        default:
            return false;
        }
    }
    catch (const std::exception &e)
    {
        Logger::error("DicomValue::updateValue: {}", e.what());
        return false;
    }
}
std::string DicomValue::toString() const
{
    if (auto ptr = std::get_if<StringValue>(&m_value))
    {
        return *ptr;
    }
    else if (auto ptr = std::get_if<int16_t>(&m_value))
    {
        return std::to_string(*ptr);
    }
    else if (auto ptr = std::get_if<int32_t>(&m_value))
    {
        return std::to_string(*ptr);
    }
    else if (auto ptr = std::get_if<uint16_t>(&m_value))
    {
        return std::to_string(*ptr);
    }
    else if (auto ptr = std::get_if<uint32_t>(&m_value))
    {
        return std::to_string(*ptr);
    }
    else if (auto ptr = std::get_if<float>(&m_value))
    {
        return std::to_string(*ptr);
    }
    else if (auto ptr = std::get_if<double>(&m_value))
    {
        return std::to_string(*ptr);
    }
    return "";
}

void DicomValue::ExtractStringValue(DcmElement *element)
{
    OFString value;
    OFCondition condition = element->getOFStringArray(value);
    if (condition.good())
    {
        m_value = std::string(value.c_str());
        m_valueType = STRING;
    }
}

void DicomValue::ExtractFloatValue(DcmElement *element)
{
    Float32 value;
    if (element->getFloat32(value).good())
    {
        m_value = value;
        m_valueType = FLOAT;
    }
}

void DicomValue::ExtractDoubleValue(DcmElement *element)
{
    Float64 value;
    if (element->getFloat64(value).good())
    {
        m_value = value;
        m_valueType = DOUBLE;
    }
}

void DicomValue::ExtractInt16Value(DcmElement *element)
{
    Sint16 value;
    if (element->getSint16(value).good())
    {
        m_value = value;
        m_valueType = INT16;
    }
}

void DicomValue::ExtractInt32Value(DcmElement *element)
{
    Sint32 value;
    if (element->getSint32(value).good())
    {
        m_value = value;
        m_valueType = INT32;
    }
}

void DicomValue::ExtractUint16Value(DcmElement *element)
{
    Uint16 value;
    if (element->getUint16(value).good())
    {
        m_value = value;
        m_valueType = UINT16;
    }
}

void DicomValue::ExtractUint32Value(DcmElement *element)
{
    Uint32 value;
    if (element->getUint32(value).good())
    {
        m_value = value;
        m_valueType = UINT32;
    }
}

void DicomValue::ExtractBinaryValue(DcmElement *element)
{
    Uint8 *data = nullptr;
    unsigned long length = 0;
    if (element->getUint8Array(data).good() && data != nullptr)
    {
        length = element->getLength();
        m_value = std::vector<uint8_t>(data, data + length);
        m_valueType = BINARY;
    }
}

void DicomValue::ExtractSequenceValue(DcmElement *element)
{
    DcmSequenceOfItems *seq = dynamic_cast<DcmSequenceOfItems *>(element);
    if (seq)
    {
        SequenceValue sequenceData;
        // 递归处理序列中的每个项目
        for (unsigned long i = 0; i < seq->card(); i++)
        {
            DcmItem *item = seq->getItem(i);
            if (item)
            {
                // 这里可以创建新的DicomValue来处理序列项
                // 需要根据具体需求实现递归提取
                DicomValue seqValue;
                // 递归提取item中的元素...
                sequenceData.push_back(seqValue);
            }
        }
        m_value = sequenceData;
        m_valueType = SEQUENCE;
    }
}

void DicomValue::ExtractAttributeTagValue(DcmElement *element)
{
    DcmTagKey tagKey = element->getTag().getTagKey();
    m_value = std::string(tagKey.toString().c_str());
    m_valueType = ATTRIBUTE_TAG;
}

void DicomValue::ExtractUnknownValue(DcmElement *element)
{
    // 先尝试按二进制数据读取
    ExtractBinaryValue(element);
    if (m_valueType != BINARY)
    {
        // 如果失败，尝试按字符串读取
        ExtractStringValue(element);
    }
}

OFCondition DicomValue::PutToDataset(DcmDataset *dataset, const DcmTagKey &tag) const
{
    if (!dataset)
        return EC_IllegalParameter;

    // 根据VR类型调用不同的处理方法
    if (m_vr == "AE")
        return putStringToDataset(dataset, tag, m_vr);
    if (m_vr == "AS")
        return putStringToDataset(dataset, tag, m_vr);
    if (m_vr == "CS")
        return putStringToDataset(dataset, tag, m_vr);
    if (m_vr == "DA")
        return putStringToDataset(dataset, tag, m_vr);
    if (m_vr == "DS")
        return putStringToDataset(dataset, tag, m_vr);
    if (m_vr == "DT")
        return putStringToDataset(dataset, tag, m_vr);
    if (m_vr == "IS")
        return putStringToDataset(dataset, tag, m_vr);
    if (m_vr == "LO")
        return putStringToDataset(dataset, tag, m_vr);
    if (m_vr == "LT")
        return putStringToDataset(dataset, tag, m_vr);
    if (m_vr == "PN")
        return putStringToDataset(dataset, tag, m_vr);
    if (m_vr == "SH")
        return putStringToDataset(dataset, tag, m_vr);
    if (m_vr == "ST")
        return putStringToDataset(dataset, tag, m_vr);
    if (m_vr == "TM")
        return putStringToDataset(dataset, tag, m_vr);
    if (m_vr == "UI")
        return putStringToDataset(dataset, tag, m_vr);
    if (m_vr == "UT")
        return putStringToDataset(dataset, tag, m_vr);
    if (m_vr == "UC")
        return putStringToDataset(dataset, tag, m_vr);
    if (m_vr == "UR")
        return putStringToDataset(dataset, tag, m_vr);

    if (m_vr == "FL")
        return putFloatToDataset(dataset, tag);
    if (m_vr == "FD")
        return putDoubleToDataset(dataset, tag);
    if (m_vr == "SL")
        return putInt32ToDataset(dataset, tag);
    if (m_vr == "SS")
        return putInt16ToDataset(dataset, tag);
    if (m_vr == "UL")
        return putUint32ToDataset(dataset, tag);
    if (m_vr == "US")
        return putUint16ToDataset(dataset, tag);

    if (m_vr == "OB" || m_vr == "OD" || m_vr == "OF" || m_vr == "OL" || m_vr == "OW")
    {
        return putBinaryToDataset(dataset, tag, m_vr);
    }

    if (m_vr == "AT")
        return putAttributeTagToDataset(dataset, tag);
    if (m_vr == "SQ")
        return putSequenceToDataset(dataset, tag);

    // 未知类型尝试按字符串处理
    return putStringToDataset(dataset, tag, m_vr);
}

// 字符串类型处理
OFCondition DicomValue::putStringToDataset(DcmDataset *dataset, const DcmTagKey &tag, const std::string &vr) const
{
    std::string value;
    if (getValue(value))
    {
        return dataset->putAndInsertString(tag, value.c_str());
    }
    return EC_IllegalCall;
}

// 数值类型处理
OFCondition DicomValue::putInt16ToDataset(DcmDataset *dataset, const DcmTagKey &tag) const
{
    int16_t value;
    if (getValue(value))
    {
        return dataset->putAndInsertSint16(tag, value);
    }
    return EC_IllegalCall;
}

OFCondition DicomValue::putUint16ToDataset(DcmDataset *dataset, const DcmTagKey &tag) const
{
    uint16_t value;
    if (getValue(value))
    {
        return dataset->putAndInsertUint16(tag, value);
    }
    return EC_IllegalCall;
}

OFCondition DicomValue::putInt32ToDataset(DcmDataset *dataset, const DcmTagKey &tag) const
{
    int32_t value;
    if (getValue(value))
    {
        return dataset->putAndInsertSint32(tag, value);
    }
    return EC_IllegalCall;
}

OFCondition DicomValue::putUint32ToDataset(DcmDataset *dataset, const DcmTagKey &tag) const
{
    uint32_t value;
    if (getValue(value))
    {
        return dataset->putAndInsertUint32(tag, value);
    }
    return EC_IllegalCall;
}

OFCondition DicomValue::putFloatToDataset(DcmDataset *dataset, const DcmTagKey &tag) const
{
    float value;
    if (getValue(value))
    {
        return dataset->putAndInsertFloat32(tag, value);
    }
    return EC_IllegalCall;
}

OFCondition DicomValue::putDoubleToDataset(DcmDataset *dataset, const DcmTagKey &tag) const
{
    double value;
    if (getValue(value))
    {
        return dataset->putAndInsertFloat64(tag, value);
    }
    return EC_IllegalCall;
}

// 二进制数据处理
OFCondition DicomValue::putBinaryToDataset(DcmDataset *dataset, const DcmTagKey &tag, const std::string &vr) const
{
    std::vector<uint8_t> data;
    if (getValue(data) && !data.empty())
    {
        DcmElement *element = nullptr;
        OFCondition result = DcmItem::newDicomElement(element, tag, vr.c_str());
        if (result.good() && element)
        {
            result = element->putUint8Array(data.data(), data.size());
            if (result.good())
            {
                result = dataset->insert(element);
            }
            if (result.bad())
            {
                delete element;
            }
        }
        return result;
    }
    return EC_IllegalCall;
}

// 属性标签处理
OFCondition DicomValue::putAttributeTagToDataset(DcmDataset *dataset, const DcmTagKey &tag) const
{
    std::string tagStr;
    if (getValue(tagStr))
    {
        // 解析字符串形式的tag，如 "0010,0010"
        unsigned int group, element;
        if (sscanf(tagStr.c_str(), "%04x,%04x", &group, &element) == 2)
        {
            DcmTagKey targetTag(group, element);
            return dataset->putAndInsertTagKey(tag, targetTag);
        }
    }
    return EC_IllegalCall;
}

// 序列处理（简化版，实际需要递归处理）
OFCondition DicomValue::putSequenceToDataset(DcmDataset *dataset, const DcmTagKey &tag) const
{
    // std::vector<DicomValue> sequence;
    // if (getValue(sequence))
    // {
    //     DcmSequenceOfItems *seqElement = new DcmSequenceOfItems(tag);

    //     for (const auto &itemValue : sequence)
    //     {
    //         DcmItem *item = new DcmItem();
    //         // 这里需要递归处理序列中的每个项目
    //         // 简化处理：假设序列项已经有自己的put逻辑
    //         // itemValue.putToDataset(dataset); // 这里需要调整，实际应该添加到item中
    //         // seqElement->append(item);
    //     }

    //     return dataset->insert(seqElement);
    // }
    return EC_IllegalCall;
}
