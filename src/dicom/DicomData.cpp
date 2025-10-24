#include "DicomData.h"
#include <Logger.h>
DicomData::DicomData()
{
}

void DicomData::Print() const
{
    Logger::warn("DicomData-->DataSet");
    for (auto &property : m_DataSetCardinality)
    {
        property.Print();
    }
    Logger::info("=============================================================");
    Logger::warn("DicomData-->MetaInfo");
    for (auto &property : m_MetaInfoCardinality)
    {
        property.Print();
    }
    Logger::info("=============================================================");
    Logger::warn("DicomData-->Other");

    for (auto &property : m_OtherProperty)
    {
        property.Print();
    }

    Logger::info("=============================================================");
}

DicomData::~DicomData()
{
    if (m_ImageData)
    {
        // delete m_ImageData;
        // m_ImageData = nullptr;
    }
}
