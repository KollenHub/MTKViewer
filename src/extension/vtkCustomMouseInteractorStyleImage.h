#pragma once
#include <QObject>
#include <vtkInteractorStyleImage.h>
#include <vtkCommand.h>
#include <vtkRenderWindowInteractor.h>

class vtkCustomMouseInteractorStyleImage : public vtkInteractorStyleImage
{
public:
    static vtkCustomMouseInteractorStyleImage *New();
    vtkTypeMacro(vtkCustomMouseInteractorStyleImage, vtkInteractorStyleImage);

protected:
    void OnMouseWheelForward() override
    {
        if (m_OnMouseWheel)
        {
            m_OnMouseWheel(1);
        }
    }

    void OnMouseWheelBackward() override
    {
        if (m_OnMouseWheel)
        {
            m_OnMouseWheel(-1);
        }
    }

private:
    std::function<void(int)> m_OnMouseWheel;

public:
    void SetOnMouseWheel(std::function<void(int)> func)
    {
        m_OnMouseWheel = func;
    }

public:
    vtkCustomMouseInteractorStyleImage(/* args */);
    ~vtkCustomMouseInteractorStyleImage();
};
