#pragma once
#include <QString>
#include <dcmtk/dcmdata/dctk.h>
#include "core/Logger.h"
#include "core/Timer.h"
#include "DicomValue.h"
#include <QStringList>
class DicomProperty
{
    friend class DicomData;

private:
    QString m_GroupName;

    QString m_TagName;

    DcmVR m_VR;

    QString m_XTagName;

    int m_Length;

    DicomValue m_Value;

    // bool isValid() const
    // {
    //     return !m_Value.isEmpty();
    // }

    DicomProperty() = default;

public:
    DicomProperty(DcmElement *element)
    {
        if (!element)
            return;

        auto tag = element->getTag();

        m_GroupName = tag.getGroup();

        if (strcmp(tag.getTagName(), "PixelData") == 0)
        {
            return;
        }

        DcmVR vr = element->getVR();

        m_Length = element->getLength();
        m_XTagName = tag.getXTag().toString().c_str();
        m_TagName = tag.getTagName();
        m_VR = vr;
        if (tag.isUnknownVR())
        {
            return;
        }
        m_Value = DicomValue(element);
        // OFString value;
        // auto con = element->getOFStringArray(value);
        // if (con.good())
        // {
        //     m_Value = QString::fromLocal8Bit(value.c_str());
        //     if (m_Value.isEmpty())
        //         m_Value = "empty value";
        // }
    }

    const QString &tagName() const
    {
        return m_TagName;
    }

    const QString vrName() const
    {
        return m_VR.getVRName();
    }

    const DcmVR &vr() const
    {
        return m_VR;
    }

    const QString &xTagName() const
    {
        return m_XTagName;
    }

    const QString &groupName() const
    {
        return m_GroupName;
    }

    const QString mergeGroupTagName() const
    {
        return QString("(%1,%2)").arg(m_GroupName).arg(m_XTagName);
    }

    int length() const
    {
        return m_Length;
    }

    const QString &value() const
    {
        return m_Value.getShowValue();
    }

    bool updateValue(const QString &value)
    {
        return m_Value.updateValue(value);
    }

    const DicomValue &originValue() const
    {
        return m_Value;
    }

    const bool AddTo(DcmDataset &dataset)
    {
        DcmTagKey key = GetDcmTagKey();
        if (key.getGroup() == 0x0000 && key.getElement() == 0x0000)
        {
            Logger::error("DicomProperty::AddTo: Invalid tag key {}", m_XTagName.toStdString());
            return false;
        }
        OFCondition con = m_Value.PutToDataset(&dataset, key);

        return con.good();
    }

    void Print() const
    {
        Logger::info(QString("TagName: %1, VRName: %2, XTagName: %3, Length: %4, Value: %5").arg(m_TagName).arg(m_Value.getVR().c_str()).arg(m_XTagName).arg(m_Length).arg(m_Value.getShowValue()).toStdString().c_str());
    }

    DcmTagKey GetDcmTagKey() const
    {
        if (m_XTagName.isEmpty())
        {
            return DcmTagKey(0, 0); // 返回无效标签
        }

        // XTagName 格式通常是 "(gggg,eeee) Description" 或类似
        // 例如: "(0010,0010) Patient's Name"
        QString xTagStr = m_XTagName;

        // 方法2: 简单字符串解析
        int start = xTagStr.indexOf('(');
        int end = xTagStr.indexOf(')');

        if (start != -1 && end != -1 && end > start)
        {
            QString tagPart = xTagStr.mid(start + 1, end - start - 1);
            QStringList parts = tagPart.split(',');

            if (parts.size() == 2)
            {
                bool ok1, ok2;
                uint16_t group = parts[0].toUShort(&ok1, 16);
                uint16_t element = parts[1].toUShort(&ok2, 16);

                if (ok1 && ok2)
                {
                    return DcmTagKey(group, element);
                }
            }
        }

        return DcmTagKey(0, 0); // 解析失败
    }
};
