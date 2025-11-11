#pragma once

#include <QMainWindow>
#include "ui_MainWindow.h"
#include "dicom/DicomData.h"
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <QTreeView.h>
#include "PatientItem.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;
    vtkSmartPointer<vtkRenderWindow> m_RendererWindow;
    QList<QMetaObject::Connection> m_Connections;
    std::vector<std::shared_ptr<PatientItem>> m_PatientItems;

    QTableView *m_PatientTagTable;
    QTableView *m_AllTagTable;
    QTreeView *m_ProjectTree;

private:
    // void UpdateProjectTree(const std::shared_ptr<PatientItem> &projectInfo,QString option);

    void AddProjectInfo(const std::shared_ptr<PatientItem> &patientInfo);

    // bool UpdateProjectInfo(const std::shared_ptr<ProjectItem> &projectInfo);

    // void DeleteProjectInfo(const std::shared_ptr<ProjectItem> &projectInfo);

    void SetTableViewData();

    void ResetImageData();

    void InitVTKWidget();

    void InitEventBindings();

    void OpenDicom();

    void CloseDicom();

    void Exit();

    void BindingMenus();

    vtkSmartPointer<vtkRenderer> GetRenderByIndex(int index);

    QMetaObject::Connection BindingAction(const QString &name, std::function<void()> func);

    QAction *FindAction(const QString &name);

    QAction *FindAction(const QList<QAction *> actions, const QString &name);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
