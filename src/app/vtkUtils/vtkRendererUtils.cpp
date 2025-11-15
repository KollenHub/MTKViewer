#include "vtkRendererUtils.h"
#include <vtkImageActor.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkImageFlip.h>

void vtkRendererUtils::AddImage(vtkSmartPointer<vtkRenderer> renderer, vtkSmartPointer<vtkImageData> image, bool isResetCamera)
{
    // Y轴翻转
    vtkSmartPointer<vtkImageFlip> flip = vtkSmartPointer<vtkImageFlip>::New();
    flip->SetInputData(image);
    flip->SetFilteredAxis(1);
    flip->Update();

    vtkSmartPointer<vtkImageActor> actor = vtkSmartPointer<vtkImageActor>::New();
    actor->SetInputData(flip->GetOutput());

    renderer->AddActor(actor);
    if (isResetCamera)
    {
        renderer->ResetCamera();
    }
    // interactor->Initialize();
    // interactor->Start();
}

void vtkRendererUtils::ClearImages(vtkSmartPointer<vtkRenderer> renderer)
{
    // 获取所有Actor的集合
    // vtkActorCollection *actors = renderer->GetActors();
    // actors->InitTraversal(); // 初始化遍历

    // // 遍历集合并移除每一个Actor
    // while (vtkActor *actor = actors->GetNextActor())
    // {
    //     renderer->RemoveActor(actor);
    // }
    renderer->RemoveAllViewProps();
}
