#include <iostream>
#include <QCoreApplication>
#include "dicom/DicomOperator.h"
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    std::shared_ptr<DicomData> data = DicomOperator::OpenDicomFile("D:/DICOM/DCM/011958333339.dcm");

    auto& datas = data->GetAllTags();

    for (size_t i = 0; i < datas.size(); i++)
    {
        auto& item = datas[i];
        if (item.tagName() == "Modality")
        {
            item.updateValue("CT");
        }
        else if (item.tagName() == "SeriesDescription")
        {
            item.updateValue("Test");
        }
        else if (item.tagName() == "PatientName")
        {
            item.updateValue("Test Patient");
        }
    }

    bool result = DicomOperator::SavaAsDiicomFile("D:/DICOM/DCM/Test.dcm", data);

    std::cout << "Result: " << result << std::endl;

    return app.exec();
}