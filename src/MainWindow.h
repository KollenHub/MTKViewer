#pragma once

#include <QMainWindow>
#include "ui_MainWindow.h"
#include <vtkRenderer.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;
    vtkSmartPointer<vtkRenderer> m_Renderer;
    QTableView *m_PropsTable;

    void InitVTKWidget();

    void BindingMenus();

    void UnbindingMenus();

    void BindingAction(const QString &name,std::function<void()> func);

    QAction *FindAction(const QString &name);

    QAction *FindAction(const QList<QAction *> actions, const QString &name);

    void OpenDicom();

    void CloseDicom();

    void Exit();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
