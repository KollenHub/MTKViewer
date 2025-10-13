#include "MainWindow.h"
#include <vtkGenericOpenGLRenderWindow.h>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkRenderWindowInteractor.h>
#include "Logger.h"
void MainWindow::InitVTKWidget()
{
    QVTKOpenGLNativeWidget *widget = new QVTKOpenGLNativeWidget(this);

    ui->vtkViewerContainer->addWidget(widget);

    this->ui->vtkViewerContainer->setContentsMargins(0, 0, 0, 0);
    this->ui->vtkViewerContainer->setSpacing(0);

    // 关联VTK渲染窗口
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    widget->setRenderWindow(renderWindow);

    m_Renderer = vtkSmartPointer<vtkRenderer>::New();
    m_Renderer->SetBackground(0.117, 0.117, 0.117);

    // 添加到渲染窗口
    renderWindow->AddRenderer(m_Renderer);

    renderWindow->Render();
    this->resize(1000, 800);
}
void MainWindow::BindingMenus()
{
    BindingAction("打开", std::bind(&MainWindow::OpenDicom, this));
    BindingAction("关闭文件", std::bind(&MainWindow::CloseDicom, this));
    BindingAction("退出程序", std::bind(&MainWindow::Exit, this));
}
void MainWindow::UnbindingMenus()
{
}
void MainWindow::BindingAction(const QString &name, std::function<void()> func)
{
    QAction *action = FindAction(name);
    if (action != nullptr)
    {
        Logger::info("找到菜单项：{}", action->text().toStdString());
        connect(action, &QAction::triggered, this, func);
    }
}
QAction *MainWindow::FindAction(const QString &name)
{
    QList<QAction *> actions = ui->menubar->actions();
    return FindAction(actions, name);
}
QAction *MainWindow::FindAction(const QList<QAction *> actions, const QString &name)
{
    for (size_t i = 0; i < actions.size(); i++)
    {
        if (actions[i]->text() == name)
            return actions[i];

        if (actions[i]->menu() != nullptr && actions[i]->menu()->actions().size() > 0)
        {
            QAction *action = FindAction(actions[i]->menu()->actions(), name);
            if (action != nullptr)
                return action;
        }
        else if (actions[i]->actionGroup() != nullptr && actions[i]->actionGroup()->actions().size() > 0)
        {
            QAction *action = FindAction(actions[i]->actionGroup()->actions(), name);
            if (action != nullptr)
                return action;
        }
    }
    return nullptr;
}
void MainWindow::OpenDicom()
{
    Logger::info("打开DICOM文件");
}
void MainWindow::CloseDicom()
{
    Logger::info("关闭DICOM文件");
}
void MainWindow::Exit()
{
    Logger::info("退出程序");
}
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow())
{
    ui->setupUi(this);

    m_PropsTable = ui->propertiesTable;

    InitVTKWidget();

    // 绑定菜单
    BindingMenus();
}

MainWindow::~MainWindow()
{
}
