
#pragma once
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkImageData.h>
class vtkRendererUtils
{
public:
    static void AddImage(vtkSmartPointer<vtkRenderer> renderer, vtkSmartPointer<vtkImageData> image,bool isResetCamera=false);

    static void ClearImages(vtkSmartPointer<vtkRenderer> renderer);

};

