#pragma once
#include <QString>
#include "dicom/DicomData.h"
class DicomOperator
{
public:
    static std::shared_ptr<DicomData> OpenDicomFile(const QString &filePath);

    static bool SavaAsDiicomFile(const QString &filePath, const std::shared_ptr<DicomData> &dicomData);
};
