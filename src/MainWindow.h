#pragma once

#include <QMainWindow>
#include "ui_MainWindow.h"
#include "dicom/DicomData.h"
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;
    vtkSmartPointer<vtkRenderWindow> m_RendererWindow;
    QTreeView *m_PropsTree;
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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
