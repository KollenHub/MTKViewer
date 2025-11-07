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

    // 取 8bit 像素数据
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

}
