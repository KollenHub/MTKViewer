#include "vtkWindowTest.h"
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkImageActor.h>
#include <vtkImageData.h>
#include "vtkUtils/vtkRendererUtils.h"
#include <chrono>
#include "Logger.h"
#include "Timer.h"
#include <dicom/DicomOperator.h>
#include <vtkImageFlip.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderWindowInteractor.h>
void vtkWindowTest::CreateVTKWindow()
{
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();

    vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
    renderWindowInteractor->SetInteractorStyle(style);

    renderWindow->AddRenderer(renderer);
    renderWindow->SetInteractor(renderWindowInteractor);

    vtkSmartPointer<vtkImageActor> actor = vtkSmartPointer<vtkImageActor>::New();
    {
        Timer timer("load dicom");

        auto dcmData = DicomOperator::OpenDicomFile("D:/DICOM/DCM/011958333339.dcm");

        vtkSmartPointer<vtkImageData> imageData = dcmData->GetImageData();

        // Y轴翻转
        vtkSmartPointer<vtkImageFlip> flip = vtkSmartPointer<vtkImageFlip>::New();
        flip->SetInputData(imageData);
        flip->SetFilteredAxis(1);
        flip->Update();

        actor->SetInputData(flip->GetOutput());
    }
    renderer->AddActor(actor);
    renderer->ResetCamera();

    renderWindowInteractor->Initialize();
    renderWindowInteractor->Start();
}