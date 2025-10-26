#include "DicomData.h"
#include <Logger.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include "Logger.h"
#include <dcmtk/dcmjpeg/djdecode.h>
DicomData::DicomData()
{
}

void DicomData::setImage(DcmDataset *dataset)
{
    DcmDataset *copy = static_cast<DcmDataset *>(dataset->clone());
    m_ImageData = std::shared_ptr<DcmDataset>(copy);
}

const vtkSmartPointer<vtkImageData> DicomData::GetImageData() const
{

    DJDecoderRegistration::registerCodecs();

    E_TransferSyntax transferSyntax = m_ImageData->getOriginalXfer();
    DicomImage image(m_ImageData.get(), transferSyntax);

    if (image.getStatus() != EIS_Normal)
    {
        Logger::error("Error: cannot load image ({})", DicomImage::getString(image.getStatus()));
        return nullptr;
    }

    image.setMinMaxWindow(); // 自动窗口调整

    int width = image.getWidth();
    int height = image.getHeight();
    bool isColor = !image.isMonochrome();
    int numComponents = isColor ? 3 : 1;

    std::cout << "depth(bits): " << image.getDepth() << std::endl;
    std::cout << "channels: " << numComponents << std::endl;

    // 取 8bit 像素数据
    const void *pixelData = image.getOutputData(8 /* bits per sample */, 0 /* frame */);
    if (!pixelData)
    {
        Logger::error("Error: cannot get pixel data");
        return nullptr;
    }

    // 拷贝到 VTK
    vtkSmartPointer<vtkImageData> vtkImg = vtkSmartPointer<vtkImageData>::New();
    vtkImg->SetExtent(0, width - 1, 0, height - 1, 0, 0);
    vtkImg->AllocateScalars(VTK_UNSIGNED_CHAR, numComponents);

    unsigned char *vtkPtr = static_cast<unsigned char *>(vtkImg->GetScalarPointer());
    size_t bytes = static_cast<size_t>(width) * height * numComponents;
    memcpy(vtkPtr, pixelData, bytes);

    return vtkImg;
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
