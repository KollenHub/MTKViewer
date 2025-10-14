#pragma once
#include <QString>
#include <dcmtk/dcmdata/dctk.h>
class DicomProperty
{
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
            m_Value = value.c_str();
        }
    }
};
