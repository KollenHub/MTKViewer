#include "DicomOperator.h"
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmjpeg/djdecode.h>
#include "Logger.h"

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

    auto metaInfoCardinality = metaInfo->card();
    for (int i = 0; i < metaInfoCardinality; ++i)
    {
        auto element = metaInfo->getElement(i);
        dcmData->AddMetaInfoCardinality(DicomProperty(element));
    }

    auto dataSet = fileFormat.getDataset();
    if (!dataSet)
    {
        Logger::error("data set is empty");
        return nullptr;
    }

    auto dataSetCardinality = dataSet->card();
    for (int i = 0; i < dataSetCardinality; ++i)
    {
        auto element = dataSet->getElement(i);
        dcmData->AddDataSetCardinality(DicomProperty(element));
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
            dcmData->AddOtherProperty(DicomProperty(OFstatic_cast(DcmElement *, top)));
            // printDcmElement(OFstatic_cast(DcmElement *, top), "##################");

            DcmObject *down = nullptr;
            while (true)
            {
                down = top->nextInContainer(down);
                if (!down)
                {
                    break;
                }
                dcmData->AddOtherProperty(DicomProperty(OFstatic_cast(DcmElement *, down)));
                // printDcmElement(OFstatic_cast(DcmElement *, down), "$$$$$$$$$$$$$$");
            }
        }
    }
}
