#include "NewWindow.h"
#include <vtkGenericOpenGLRenderWindow.h>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkRenderWindowInteractor.h>
#include "Logger.h"
#include "dicom/DicomOperator.h"
#include <QFileDialog>
#include <QDebug>
#include <QStandardItemModel>
#include "vtkUtils/vtkRendererUtils.h"
#include <vtkBMPReader.h>
#include <vtkInteractorStyleImage.h>
#include <QLayoutItem>
#include <vtkRendererCollection.h>

void NewWindow::SetTableViewData()
{
    // if (m_DicomData != nullptr)
    // {
    //     // 获取treeview的model
    //     QStandardItemModel *model = qobject_cast<QStandardItemModel *>(m_PropsTree->model());
    //     model->removeRows(0, model->rowCount());

    //     QStandardItem *datasetItem = new QStandardItem("DataSet");
    //     model->setItem(0, 0, datasetItem);
    //     for (auto &tag : m_DicomData->GetDataSetCardinality())
    //     {
    //         QStandardItem *item1 = new QStandardItem(tag.xTagName());
    //         QStandardItem *item2 = new QStandardItem(tag.tagName());
    //         // QStandardItem *item3 = new QStandardItem(tag.vrName());
    //         // QStandardItem *item4 = new QStandardItem(QString::number(tag.length()));
    //         QStandardItem *item3 = new QStandardItem(tag.value());
    //         datasetItem->appendRow(QList<QStandardItem *>{item1, item2, item3});
    //     }

    //     QStandardItem *metaInfoItem = new QStandardItem("MetaInfo");
    //     model->setItem(1, 0, metaInfoItem);
    //     for (auto &tag : m_DicomData->GetMetaInfoCardinality())
    //     {
    //         QStandardItem *item1 = new QStandardItem(tag.xTagName());
    //         QStandardItem *item2 = new QStandardItem(tag.tagName());
    //         // QStandardItem *item3 = new QStandardItem(tag.vrName());
    //         // QStandardItem *item4 = new QStandardItem(QString::number(tag.length()));
    //         QStandardItem *item3 = new QStandardItem(tag.value());
    //         metaInfoItem->appendRow(QList<QStandardItem *>{item1, item2, item3});
    //     }

    //     QStandardItem *otherItem = new QStandardItem("Other");
    //     model->setItem(1, 0, otherItem);
    //     for (auto &tag : m_DicomData->GetMetaInfoCardinality())
    //     {
    //         QStandardItem *item1 = new QStandardItem(tag.xTagName());
    //         QStandardItem *item2 = new QStandardItem(tag.tagName());
    //         // QStandardItem *item3 = new QStandardItem(tag.vrName());
    //         // QStandardItem *item4 = new QStandardItem(QString::number(tag.length()));
    //         QStandardItem *item3 = new QStandardItem(tag.value());
    //         otherItem->appendRow(QList<QStandardItem *>{item1, item2, item3});
    //     }

    //     m_PropsTree->expandAll();
    // }
}
void NewWindow::ResetImageData()
{
    vtkSmartPointer<vtkRenderer> firstRenderer = GetRenderByIndex(0);
    vtkRendererUtils::AddImage(firstRenderer, m_DicomData->GetImageData());
    firstRenderer->ResetCamera();
    m_RendererWindow->Render();

    // QLayoutItem *item = ui->vtkViewerContainer->itemAt(0);
    // if (item)
    // {
    //     QWidget *child = item->widget();
    //     QVTKOpenGLNativeWidget *vtkWidget = qobject_cast<QVTKOpenGLNativeWidget *>(child);
    //     if (vtkWidget)
    //     {
    //         vtkWidget->renderWindow()->Render();
    //     }
    // }
}
void NewWindow::InitVTKWidget()
{
    QVTKOpenGLNativeWidget *widget = new QVTKOpenGLNativeWidget(this);

    ui->vtkContainer->addWidget(widget);

    this->ui->vtkContainer->setContentsMargins(0, 0, 0, 0);
    this->ui->vtkContainer->setSpacing(0);

    // 关联VTK渲染窗口
    m_RendererWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    widget->setRenderWindow(m_RendererWindow);

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->SetBackground(0.177, 0.177, 0.177);
    // renderer->SetBackground(1, 1, 0);

    // Qt内部已经创建了一个vtkRenderWindowInteractor
    // 可以设置，但是不能调用自带的Initialize以及对应的Start
    vtkSmartPointer<vtkRenderWindowInteractor> interactor = m_RendererWindow->GetInteractor();
    vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
    interactor->SetInteractorStyle(style);
    m_RendererWindow->SetInteractor(interactor);
    // 添加到渲染窗口
    m_RendererWindow->AddRenderer(renderer);
    m_RendererWindow->Render();

    // interactor->Initialize();
    // interactor->Start();
}
void NewWindow::BindingMenus()
{
    auto openConnection = BindingAction("打开", std::bind(&NewWindow::OpenDicom, this));
    auto closeConnection = BindingAction("关闭文件", std::bind(&NewWindow::CloseDicom, this));
    auto exitConnection = BindingAction("退出程序", std::bind(&NewWindow::Exit, this));
    m_Connections.push_back(openConnection);
    m_Connections.push_back(closeConnection);
    m_Connections.push_back(exitConnection);
}

vtkSmartPointer<vtkRenderer> NewWindow::GetRenderByIndex(int index)
{
    return m_RendererWindow->GetRenderers()->GetNumberOfItems() > index ? dynamic_cast<vtkRenderer *>(m_RendererWindow->GetRenderers()->GetItemAsObject(index)) : nullptr;
}

QMetaObject::Connection NewWindow::BindingAction(const QString &name, std::function<void()> func)
{
    QAction *action = FindAction(name);
    if (action != nullptr)
    {
        Logger::info("找到菜单项：{}", action->text().toStdString());
        return connect(action, &QAction::triggered, this, func);
    }
    return QMetaObject::Connection();
}

QAction *NewWindow::FindAction(const QString &name)
{
    QList<QAction *> actions = ui->menubar->actions();
    return FindAction(actions, name);
}
QAction *NewWindow::FindAction(const QList<QAction *> actions, const QString &name)
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
void NewWindow::OpenDicom()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "选择Dicom文件",
        "",
        "DICOM文件 (*.dcm)");
    if (!fileName.isEmpty())
    {
        // 处理选中的文件
        qDebug() << "选中的文件:" << fileName;
        m_DicomData = DicomOperator::OpenDicomFile(fileName);
        SetTableViewData();
        // 添加图片
        ResetImageData();
        m_DicomData->Print();
    }
}
void NewWindow::CloseDicom()
{
    Logger::info("关闭DICOM文件");
}
void NewWindow::Exit()
{
    Logger::info("退出程序");
}
NewWindow::NewWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::NewWindow())
{
    ui->setupUi(this);

    // InitVTKWidget();

    // 设置表头
    // QStandardItemModel *model = new QStandardItemModel(this);

    // model->setHorizontalHeaderLabels(QStringList() << "XTagName" << "TagName" << "Value");
    // 设置表格内容
    // m_PropsTree->setModel(model);

    // m_PropsTree->expandAll();

    InitVTKWidget();

    // 绑定菜单
    BindingMenus();
}

NewWindow::~NewWindow()
{
    // 解除函数绑定

    for (auto &conn : m_Connections)
    {
        disconnect(conn);
    }
}
