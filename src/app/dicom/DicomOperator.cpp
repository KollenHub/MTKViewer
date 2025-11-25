#include "DicomOperator.h"
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmjpeg/djdecode.h>
#include "core/Logger.h"
#include <dcmtk\dcmdata\dctk.h>
#include <dcmtk\dcmimgle\dcmimage.h>
#include <dcmtk\dcmjpeg\djdecode.h>
#include <dcmtk\dcmdata\libi2d\i2d.h>
#include <dcmtk\ofstd\ofcmdln.h>
#include <dcmtk\dcmdata\libi2d\i2djpgs.h>
#include <dcmtk\dcmdata\libi2d\i2dplnsc.h>
#include "I2DFromVtk.h"

std::shared_ptr<DicomData> DicomOperator::OpenDicomFile(const QString &filePath)
{
    DcmFileFormat fileFormat;
    auto fileLoadStatus = fileFormat.loadFile(filePath.toStdString().c_str());
    if (fileLoadStatus.bad())
    {
        Logger::error("file load fail, file path is : {}", filePath.toStdString());
        return nullptr;
    }

    auto metaInfo = fileFormat.getMetaInfo();
    if (!metaInfo)
    {
        Logger::error("meta info is empty");
        return nullptr;
    }

    auto dcmData = std::make_shared<DicomData>();

    auto dataSet = fileFormat.getDataset();
    if (!dataSet && !dataSet->isEmpty())
    {
        Logger::error("data set is empty");
        return nullptr;
    }

    if (dataSet->tagExists(DCM_PixelData))
    {
        Logger::info("Pixel data exists");
    }

    // 路径赋值
    dcmData->SetFilePath(filePath);

    Uint16 rows, cols;
    if (dataSet->findAndGetUint16(DCM_Rows, rows).good() &&
        dataSet->findAndGetUint16(DCM_Columns, cols).good())
    {
        Logger::info("Image size: {} x {}", rows, cols);
    }
    else
    {
        Logger::error("Can't get image size");
        return nullptr;
    }

    dcmData->setImage(dataSet);

    auto metaInfoCardinality = metaInfo->card();
    for (int i = 0; i < metaInfoCardinality; ++i)
    {
        auto element = metaInfo->getElement(i);
        dcmData->AddTag(DicomProperty(element));
    }

    auto dataSetCardinality = dataSet->card();
    for (int i = 0; i < dataSetCardinality; ++i)
    {
        auto element = dataSet->getElement(i);
        dcmData->AddTag(DicomProperty(element));
        // printDcmElement(element, "***********************");
        if (element->isLeaf())
        {
            continue;
        }

        DcmObject *top = nullptr;
        while (true)
        {
            top = element->nextInContainer(top);
            if (!top)
            {
                break;
            }
            dcmData->AddTag(DicomProperty(OFstatic_cast(DcmElement *, top)));
            // printDcmElement(OFstatic_cast(DcmElement *, top), "##################");

            DcmObject *down = nullptr;
            while (true)
            {
                down = top->nextInContainer(down);
                if (!down)
                {
                    break;
                }
                dcmData->AddTag(DicomProperty(OFstatic_cast(DcmElement *, down)));
                // printDcmElement(OFstatic_cast(DcmElement *, down), "$$$$$$$$$$$$$$");
            }
        }
    }
    return dcmData;
}

bool DicomOperator::SavaAsDiicomFile(const QString &filePath, const std::shared_ptr<DicomData> &dicomData)
{
#if 1
    // Main class for controlling conversion
    Image2Dcm i2d;
    // Output plugin to use (i.e. SOP class to write)
    I2DOutputPlug *outPlug = NULL;
    // Input plugin to use (i.e. file format to read)
    I2DImgSource *inputPlug = NULL;
    // Group length encoding mode for output DICOM file
    E_GrpLenEncoding grpLengthEnc = EGL_recalcGL;
    // Item and Sequence encoding mode for output DICOM file
    E_EncodingType lengthEnc = EET_ExplicitLength;
    // Padding mode for output DICOM file
    E_PaddingEncoding padEnc = EPD_noChange;
    // File pad length for output DICOM file
    OFCmdUnsignedInt filepad = 0;
    // Item pad length for output DICOM file
    OFCmdUnsignedInt itempad = 0;
    // Write file format (with meta header)
    E_FileWriteMode writeMode = EWM_updateMeta;

    // The transfer syntax proposed to be written by output plugin
    E_TransferSyntax writeXfer = EXS_LittleEndianExplicit;

    inputPlug = new I2DFromVtk(dicomData->GetImageData(), 0); // 第0层（或其他slice）
    outPlug = new I2DOutputPlugNewSC();

    DcmDataset *resultObject = nullptr;
    OFCondition cond = i2d.convertFirstFrame(inputPlug, outPlug, 1 /* total frames */, resultObject, writeXfer);

    if (cond.good())
    {
        // 插入记录的数据
        int tagSize = dicomData->GetAllTags().size();

        for (size_t i = 0; i < tagSize; i++)
        {
            auto tag = dicomData->GetAllTags().at(i);

            //读取时按照8位读取的，所以存的时候不能按照原来的数据存储进去
            if (tag.tagName() == "BitsAllocated" || tag.tagName() == "BitsStored" || tag.tagName() == "HighBit")
            {
                continue;
            }
            if (tag.tagName() == "PresentationLUTShape")
            {
                Logger::debug("tag:{}--->{}", tag.tagName().toStdString(), tag.originValue().toString());
                continue;
            }
            tag.AddTo(*resultObject);
        }

        DcmFileFormat dcmff(resultObject);
        cond = dcmff.saveFile(filePath.toStdString().c_str(), writeXfer, lengthEnc, grpLengthEnc, padEnc,
                              OFstatic_cast(Uint32, filepad), OFstatic_cast(Uint32, itempad), writeMode);
    }
    // update offset table if image type is encapsulated
    if (cond.good())
        cond = i2d.updateOffsetTable();

    // update attributes related to lossy compression
    // if (cond.good()) cond = i2d.updateLossyCompressionInfo(inputPlug, inputFiles.size(), resultObject);

    // resultObject->putAndInsertString(DCM_PatientName, "test");

    if (cond.good())
    {
        SPDLOG_INFO(": Saving output DICOM to file " + filePath.toStdString());
        DcmFileFormat dcmff(resultObject);

        cond = dcmff.saveFile(filePath.toStdString().c_str(), writeXfer, lengthEnc, grpLengthEnc, padEnc, OFstatic_cast(Uint32, filepad), OFstatic_cast(Uint32, itempad), writeMode);
    }

    // Cleanup and return
    delete outPlug;
    outPlug = NULL;
    delete inputPlug;
    inputPlug = NULL;
    delete resultObject;
    resultObject = NULL;

#else

    DcmFileFormat fileformat;
    DcmDataset *dataSet = fileformat.getDataset();

    // cv::Mat m_image1 = cv::imread("C:\\Users\\cobot\\Pictures\\test1.jpg");
    // cv::Mat m_image2 = cv::imread("C:\\Users\\cobot\\Pictures\\test2.jpg");
    // int size = m_image1.rows * m_image1.cols * 3;
    // unsigned char *data = (unsigned char *)malloc(size * 2);

    // memcpy(data, m_image1.data, size);
    // memcpy(data + size, m_image2.data, size);
    // dataSet->putAndInsertString(DCM_NumberOfFrames, "2");
    // dataSet->putAndInsertString(DCM_Rows, std::to_string(m_image1.rows).c_str());
    // dataSet->putAndInsertString(DCM_Columns, std::to_string(m_image1.cols).c_str());

    dataSet->putAndInsertString(DCM_SamplesPerPixel, "3");
    dataSet->putAndInsertString(DCM_PhotometricInterpretation, "RGB");
    dataSet->putAndInsertString(DCM_BitsAllocated, "8");
    dataSet->putAndInsertString(DCM_BitsStored, "8");
    dataSet->putAndInsertString(DCM_HighBit, "7");
    dataSet->putAndInsertString(DCM_PixelRepresentation, "0");
    dataSet->putAndInsertString(DCM_PlanarConfiguration, "0");

    dataSet->putAndInsertUint8Array(DCM_PixelData, data, size * 2);
    OFCondition status = fileformat.saveFile("test.dcm", EXS_LittleEndianExplicit);
#endif

    return false;
}
