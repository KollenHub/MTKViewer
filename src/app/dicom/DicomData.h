#pragma once
#include <dcmtk/dcmdata/dctk.h>
#include <vector>
#include "DicomProperty.h"
#include <vtkImageData.h>
#include <vtkSmartPointer.h>
#include <QStringList>
#include <QMetaType>
class DicomData
{
private:
    const QStringList CPATIENTTAGS = {
        "PatientName",
        "PatientID",
        "PatientBirthDate",
        "PatientSex",
        "PatientAge",
        "PatientWeight",
        "PatientAddress",
        "StudyDate",
        "StudyTime",
        "StudyID",
        "Modality",
        "StudyDescription",
        "SeriesDate",
        "SeriesTime",
        "SeriesDescription",
    };

    vtkSmartPointer<vtkImageData>
        m_vtkImageData;

    std::vector<DicomProperty> m_AllTags;

    std::vector<DicomProperty> m_PatientTags;

    QString m_FilePath;

    QString m_FileName;

public:

    const QString& PatientName() const;

    const QString& StudyModality() const;

    const QString& SeriesDescription() const;

    const QString& PatientSex() const;

    const QString& FilePath() const;

    const QString& FileName() const;

    void SetFilePath(const QString &filePath);

    DicomData();

    void AddTag(const DicomProperty &property);

    void setImage(DcmDataset *dataset);

    const vtkSmartPointer<vtkImageData> GetImageData() const;

    const std::vector<DicomProperty> &GetPatientTags() const;

    const std::vector<DicomProperty> &GetAllTags() const;

    void Print() const;

    ~DicomData();
};


// 注册两种类型
Q_DECLARE_METATYPE(DicomData)
Q_DECLARE_METATYPE(std::shared_ptr<DicomData>)