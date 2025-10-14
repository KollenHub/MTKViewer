#include "DicomData.h"

DicomData::DicomData()
{
}

DicomData::~DicomData()
{
    if (m_ImageData)
    {
        delete m_ImageData;
        m_ImageData = nullptr;
    }
}
