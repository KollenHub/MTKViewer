#pragma once
#include <vector>
#include <QString>

class DicomSuffix
{
private:
    static std::vector<QString> m_Suffixes;

    static std::vector<QString> m_SuffixesFilter;

public:
    std::vector<QString> GetSuffixes() const
    {
        return m_Suffixes;
    }

    static bool IsDicomFile(const QString &suffix)
    {
        return std::find(m_Suffixes.begin(), m_Suffixes.end(), suffix) != m_Suffixes.end();
    }
};

std::vector<QString> DicomSuffix::m_Suffixes = {"dcm", "DCM", "dicom", "DICOM", "ima", "img"};

std::vector<QString> DicomSuffix::m_SuffixesFilter = {"*.dcm", "*.DCM", "*.dicom", "*.DICOM", "*.ima", "*.img"};