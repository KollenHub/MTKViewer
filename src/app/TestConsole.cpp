#include <iostream>
#include <QCoreApplication>
#include "dicom/DicomOperator.h"
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    std::shared_ptr<DicomData> data = DicomOperator::OpenDicomFile("E:/DICOM/DCM/011958333339.dcm");

    bool result= DicomOperator::SavaAsDiicomFile("E:/DICOM/DCM/Test.dcm",data);

    std::cout << "Result: " << result << std::endl;

    return app.exec();
}