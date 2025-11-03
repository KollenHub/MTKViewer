#pragma once

#include <QMainWindow>
#include "ui_NewWindow.h"
#include "dicom/DicomData.h"
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>

class NewWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::NewWindow *ui;
    vtkSmartPointer<vtkRenderWindow> m_RendererWindow;
    QList<QMetaObject::Connection> m_Connections;
    std::shared_ptr<DicomData> m_DicomData;
    

    void SetTableViewData();

    void ResetImageData();

    void InitVTKWidget();

    void BindingMenus();

    vtkSmartPointer<vtkRenderer> GetRenderByIndex(int index);

    QMetaObject::Connection BindingAction(const QString &name, std::function<void()> func);

    QAction *FindAction(const QString &name);

    QAction *FindAction(const QList<QAction *> actions, const QString &name);

    void OpenDicom();

    void CloseDicom();

    void Exit();

public:
    NewWindow(QWidget *parent = nullptr);
    ~NewWindow();
};
