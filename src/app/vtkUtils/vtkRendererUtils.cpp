#include "vtkRendererUtils.h"
#include <vtkImageActor.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkImageFlip.h>

void vtkRendererUtils::AddImage(vtkSmartPointer<vtkRenderer> renderer, vtkSmartPointer<vtkImageData> image)
{
    //Y轴翻转
    vtkSmartPointer<vtkImageFlip> flip = vtkSmartPointer<vtkImageFlip>::New();
    flip->SetInputData(image);
    flip->SetFilteredAxis(1);
    flip->Update();

    vtkSmartPointer<vtkImageActor> actor = vtkSmartPointer<vtkImageActor>::New();
    actor->SetInputData(flip->GetOutput());

    renderer->AddActor(actor);
    renderer->ResetCamera();
    // interactor->Initialize();
    // interactor->Start();
}