#pragma once
#include <QString>
#include "dicom/DicomData.h"
class DicomOperator
{
public:
    static std::shared_ptr<DicomData> OpenDicomFile(const QString &filePath);
};
