#include <QFileInfo>
#include "DicomData.h"
#include <dcmtk/dcmimgle/dcmimage.h>
#include "core/Logger.h"
#include <dcmtk/dcmjpeg/djdecode.h>
const QString &DicomData::PatientName() const
{
    return m_PatientTags[0].value();
}
const QString &DicomData::StudyModality() const
{
    return m_PatientTags[10].value();
}
const QString &DicomData::SeriesDescription() const
{
    return m_PatientTags[11].value();
}
const QString &DicomData::PatientSex() const
{
    return m_PatientTags[3].value();
}
const QString &DicomData::FilePath() const
{
    return m_FilePath;
}
const QString &DicomData::FileName() const
{
    return m_FileName;
}
void DicomData::SetFilePath(const QString &filePath)
{
    m_FileName = QFileInfo(filePath).completeBaseName();
    m_FilePath = filePath;
}
DicomData::DicomData()
{
    m_PatientTags.resize(CPATIENTTAGS.length(), DicomProperty());
}

void DicomData::AddTag(const DicomProperty &property)
{
    m_AllTags.push_back(property);
    auto index = CPATIENTTAGS.indexOf(property.tagName());
    // Logger::debug("Tag: {} Value:{}", property.tagName().toStdString(), property.value().toStdString());
    if (index != -1)
    {
        m_PatientTags[index] = property;
    }
}

void DicomData::setImage(DcmDataset *dataset)
{
    DJDecoderRegistration::registerCodecs();

    E_TransferSyntax transferSyntax = dataset->getOriginalXfer();
    DicomImage image(dataset, transferSyntax);

    if (image.getStatus() != EIS_Normal)
    {
        Logger::error("Error: cannot load image ({})", DicomImage::getString(image.getStatus()));
        return;
    }

    image.setMinMaxWindow(); // 自动窗口调整

    int width = image.getWidth();
    int height = image.getHeight();
    bool isColor = !image.isMonochrome();
    int numComponents = isColor ? 3 : 1;

    std::cout << "depth(bits): " << image.getDepth() << std::endl;
    std::cout << "channels: " << numComponents << std::endl;

    // 取 8bit 像素数据(这个有可能是高位来的，取出效果觉得不好，暂时用8位)
    const void *pixelData = image.getOutputData(8 /* bits per sample */, 0 /* frame */);
    if (!pixelData)
    {
        Logger::error("Error: cannot get pixel data");
        return;
    }

    // 拷贝到 VTK
    m_vtkImageData = vtkSmartPointer<vtkImageData>::New();
    m_vtkImageData->SetExtent(0, width - 1, 0, height - 1, 0, 0);
    m_vtkImageData->AllocateScalars(VTK_UNSIGNED_CHAR, numComponents);

    unsigned char *vtkPtr = static_cast<unsigned char *>(m_vtkImageData->GetScalarPointer());
    size_t bytes = static_cast<size_t>(width) * height * numComponents;
    memcpy(vtkPtr, pixelData, bytes);
}

const vtkSmartPointer<vtkImageData> DicomData::GetImageData() const
{
    return m_vtkImageData;
}

const std::vector<DicomProperty> &DicomData::GetPatientTags() const
{
    return m_PatientTags;
}

std::vector<DicomProperty> &DicomData::GetAllTags() 
{
    return m_AllTags;
}

void DicomData::Print() const
{
    for (auto &property : m_AllTags)
    {
        property.Print();
    }
}

DicomData::~DicomData()
{
}
