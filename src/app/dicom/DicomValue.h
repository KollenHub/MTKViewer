#ifndef DICOMVALUE_H
#define DICOMVALUE_H

#include <dcmtk/dcmdata/dcelem.h>
#include <string>
#include <vector>
#include <variant>
#include <memory>
#include <dcmtk/dcmdata/dcdatset.h>
#include <QString>

class DicomValue
{
public:
    enum ValueType
    {
        UNKNOWN,
        STRING,
        INT16,
        INT32,
        UINT16,
        UINT32,
        FLOAT,
        DOUBLE,
        BINARY,
        SEQUENCE,
        ATTRIBUTE_TAG
    };

    // 使用variant存储所有可能的值类型
    using StringValue = std::string;
    using BinaryValue = std::vector<uint8_t>;
    using SequenceValue = std::vector<DicomValue>;

    using Value = std::variant<
        std::monostate, // 空值
        int16_t,
        int32_t,
        uint16_t,
        uint32_t,
        float,
        double,
        StringValue,
        BinaryValue,
        SequenceValue>;

    // 构造函数
    DicomValue();

    DicomValue(DcmElement *element);

    // 添加
    OFCondition PutToDataset(DcmDataset *dataset, const DcmTagKey &tag) const;

    const QString &getShowValue() const { return m_ShowValue; }

    // Getter方法
    ValueType getValueType() const { return m_valueType; }
    const std::string &getVR() const { return m_vr; }
    const Value &getValue() const { return m_value; }

    // 类型安全的值获取
    template <typename T>
    bool getValue(T &value) const
    {
        if (auto ptr = std::get_if<T>(&m_value))
        {
            value = *ptr;
            return true;
        }
        return false;
    }

    bool updateValue(const QString &value);
    

    // 便捷方法
    std::string toString() const;
    bool isString() const { return m_valueType == STRING; }
    bool isNumeric() const
    {
        return m_valueType == INT16 || m_valueType == INT32 ||
               m_valueType == UINT16 || m_valueType == UINT32 ||
               m_valueType == FLOAT || m_valueType == DOUBLE;
    }
    bool isBinary() const { return m_valueType == BINARY; }
    bool isSequence() const { return m_valueType == SEQUENCE; }

private:
    ValueType m_valueType;
    std::string m_vr;
    QString m_ShowValue;
    Value m_value;

    // 主提取方法
    void ExtractValue(DcmElement *element);

    // 提取方法实现
    void ExtractStringValue(DcmElement *element);
    void ExtractFloatValue(DcmElement *element);
    void ExtractDoubleValue(DcmElement *element);
    void ExtractInt16Value(DcmElement *element);
    void ExtractInt32Value(DcmElement *element);
    void ExtractUint16Value(DcmElement *element);
    void ExtractUint32Value(DcmElement *element);
    void ExtractBinaryValue(DcmElement *element);
    void ExtractSequenceValue(DcmElement *element);
    void ExtractAttributeTagValue(DcmElement *element);
    void ExtractUnknownValue(DcmElement *element);

    // 按照VR类型分类的put方法
    OFCondition putStringToDataset(DcmDataset *dataset, const DcmTagKey &tag, const std::string &vr) const;
    OFCondition putInt16ToDataset(DcmDataset *dataset, const DcmTagKey &tag) const;
    OFCondition putUint16ToDataset(DcmDataset *dataset, const DcmTagKey &tag) const;
    OFCondition putInt32ToDataset(DcmDataset *dataset, const DcmTagKey &tag) const;
    OFCondition putUint32ToDataset(DcmDataset *dataset, const DcmTagKey &tag) const;
    OFCondition putFloatToDataset(DcmDataset *dataset, const DcmTagKey &tag) const;
    OFCondition putDoubleToDataset(DcmDataset *dataset, const DcmTagKey &tag) const;
    OFCondition putBinaryToDataset(DcmDataset *dataset, const DcmTagKey &tag, const std::string &vr) const;
    OFCondition putAttributeTagToDataset(DcmDataset *dataset, const DcmTagKey &tag) const;
    OFCondition putSequenceToDataset(DcmDataset *dataset, const DcmTagKey &tag) const;
};

#endif // DICOMVALUE_H