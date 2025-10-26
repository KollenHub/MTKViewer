#pragma once
#include <QString>
#include <dcmtk/dcmdata/dctk.h>
#include "Logger.h"
class DicomProperty
{
    friend class DicomData;
private:
    QString m_TagName;

    QString m_VRName;

    QString m_XTagName;

    int m_Length;

    QString m_Value;

    bool isValid() const
    {
        return !m_Value.isEmpty();
    }

public:
    DicomProperty(DcmElement *element)
    {
        if (!element)
            return;

        auto tag = element->getTag();

        auto vr = DcmVR(element->getVR());

        m_Length = element->getLength();
        m_XTagName = tag.getXTag().toString().c_str();
        m_TagName = tag.getTagName();
        m_VRName = vr.getVRName();

        if (tag.isUnknownVR())
        {
            return;
        }
        OFString value;
        auto con = element->getOFStringArray(value);
        if (con.good())
        {
            m_Value =QString::fromLocal8Bit(value.c_str());
        }
    }

    const QString &tagName() const
    {
        return m_TagName;
    }

    const QString &vrName() const
    {
        return m_VRName;
    }

    const QString &xTagName() const
    {
        return m_XTagName;
    }

    int length() const
    {
        return m_Length;
    }

    const QString &value() const
    {
        return m_Value;
    }

    void Print() const
    {
        Logger::info(QString("TagName: %1, VRName: %2, XTagName: %3, Length: %4, Value: %5").arg(m_TagName).arg(m_VRName).arg(m_XTagName).arg(m_Length).arg(m_Value).toStdString().c_str());
    }
};
