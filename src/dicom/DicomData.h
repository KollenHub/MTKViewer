#pragma once
#include <dcmtk/dcmdata/dctk.h>
#include <vector>
#include "DicomProperty.h"
class DicomData
{
private:
    DcmDataset *m_ImageData;

    std::vector<DicomProperty> m_MetaInfoCardinality;

    std::vector<DicomProperty> m_DataSetCardinality;

    std::vector<DicomProperty> m_OtherProperty;

public:
    DicomData();

    void AddMetaInfoCardinality(const DicomProperty &property)
    {
        m_MetaInfoCardinality.push_back(property);
    }

    void AddDataSetCardinality(const DicomProperty &property)
    {
        if (property.m_TagName == "PixelData")
            return;
        m_DataSetCardinality.push_back(property);
    }

    void AddOtherProperty(const DicomProperty &property)
    {
        m_OtherProperty.push_back(property);
    }

    void Print() const;

    ~DicomData();
};
