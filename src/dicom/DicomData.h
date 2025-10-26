#pragma once
#include <dcmtk/dcmdata/dctk.h>
#include <vector>
#include "DicomProperty.h"
#include <vtkImageData.h>
#include <vtkSmartPointer.h>
class DicomData
{
private:
    std::shared_ptr<DcmDataset> m_ImageData;

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
        {
            return;
        }
        m_DataSetCardinality.push_back(property);
    }

    void AddOtherProperty(const DicomProperty &property)
    {
        m_OtherProperty.push_back(property);
    }

    void setImage(DcmDataset *dataset);
   
    const vtkSmartPointer<vtkImageData> GetImageData() const;

    const std::vector<DicomProperty> &GetMetaInfoCardinality() const
    {
        return m_MetaInfoCardinality;
    }

    const std::vector<DicomProperty> &GetDataSetCardinality() const
    {
        return m_DataSetCardinality;
    }

    const std::vector<DicomProperty> &GetOtherProperty() const
    {
        return m_OtherProperty;
    }

    void Print() const;

    ~DicomData();
};
