#include "MainWindow.h"
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

void MainWindow::SetTableViewData()
{
    if (m_DicomData != nullptr)
    {
        // 获取treeview的model
        QStandardItemModel *allTagModel = qobject_cast<QStandardItemModel *>(m_AllTagTable->model());
        allTagModel->removeRows(0, allTagModel->rowCount());

        QStandardItemModel *patientTagModel = qobject_cast<QStandardItemModel *>(m_PatientTagTable->model());
        patientTagModel->removeRows(0, patientTagModel->rowCount());

        for (auto &tag : m_DicomData->GetDataSetCardinality())
        {
            QStandardItem *item1 = new QStandardItem(tag.xTagName());
            QStandardItem *item2 = new QStandardItem(tag.tagName());
            QStandardItem *item3 = new QStandardItem(tag.value());
            allTagModel->appendRow(QList<QStandardItem *>{item1, item2, item3});
            if (tag.isPatientTag())
            {
                QStandardItem *item4 = new QStandardItem(tag.xTagName());
                QStandardItem *item5 = new QStandardItem(tag.tagName());
                QStandardItem *item6 = new QStandardItem(tag.value());
                patientTagModel->appendRow(QList<QStandardItem *>{item4, item5, item6});
            }
        }

        for (auto &tag : m_DicomData->GetMetaInfoCardinality())
        {
            QStandardItem *item1 = new QStandardItem(tag.xTagName());
            QStandardItem *item2 = new QStandardItem(tag.tagName());
            QStandardItem *item3 = new QStandardItem(tag.value());
            allTagModel->appendRow(QList<QStandardItem *>{item1, item2, item3});
            if (tag.isPatientTag())
            {
                QStandardItem *item4 = new QStandardItem(tag.xTagName());
                QStandardItem *item5 = new QStandardItem(tag.tagName());
                QStandardItem *item6 = new QStandardItem(tag.value());
                patientTagModel->appendRow(QList<QStandardItem *>{item4, item5, item6});
            }
        }

        for (auto &tag : m_DicomData->GetMetaInfoCardinality())
        {

            QStandardItem *item1 = new QStandardItem(tag.xTagName());
            QStandardItem *item2 = new QStandardItem(tag.tagName());
            QStandardItem *item3 = new QStandardItem(tag.value());
            allTagModel->appendRow(QList<QStandardItem *>{item1, item2, item3});
            if (tag.isPatientTag())
            {
                QStandardItem *item4 = new QStandardItem(tag.xTagName());
                QStandardItem *item5 = new QStandardItem(tag.tagName());
                QStandardItem *item6 = new QStandardItem(tag.value());
                patientTagModel->appendRow(QList<QStandardItem *>{item4, item5, item6});
            }
        }
    }
}
void MainWindow::ResetImageData()
{
    vtkSmartPointer<vtkRenderer> firstRenderer = GetRenderByIndex(0);
    vtkRendererUtils::AddImage(firstRenderer, m_DicomData->GetImageData());
    firstRenderer->ResetCamera();
    m_RendererWindow->Render();
}
void MainWindow::InitVTKWidget()
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
}
void MainWindow::BindingMenus()
{
    auto openConnection = BindingAction("打开", std::bind(&MainWindow::OpenDicom, this));
    auto closeConnection = BindingAction("关闭文件", std::bind(&MainWindow::CloseDicom, this));
    auto exitConnection = BindingAction("退出程序", std::bind(&MainWindow::Exit, this));
    m_Connections.push_back(openConnection);
    m_Connections.push_back(closeConnection);
    m_Connections.push_back(exitConnection);
}

void MainWindow::InitEventBindings()
{
    
}

vtkSmartPointer<vtkRenderer> MainWindow::GetRenderByIndex(int index)
{
    return m_RendererWindow->GetRenderers()->GetNumberOfItems() > index ? dynamic_cast<vtkRenderer *>(m_RendererWindow->GetRenderers()->GetItemAsObject(index)) : nullptr;
}

QMetaObject::Connection MainWindow::BindingAction(const QString &name, std::function<void()> func)
{
    QAction *action = FindAction(name);
    if (action != nullptr)
    {
        Logger::info("找到菜单项：{}", action->text().toStdString());
        return connect(action, &QAction::triggered, this, func);
    }
    return QMetaObject::Connection();
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
        // m_DicomData->Print();
    }
}
void MainWindow::CloseDicom()
{
    Logger::info("关闭DICOM文件");
}
void MainWindow::Exit()
{
    Logger::info("退出程序");
}

void SetTagTableAutoResize(QTableView * tableView)
{
    QHeaderView *header = tableView->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::ResizeToContents); // 前几列根据内容调整
    header->setStretchLastSection(true);                         // 最后一列自动拉伸填充

}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow())
{
    ui->setupUi(this);

    m_PatientTagTable = ui->patientTagTable;
    m_AllTagTable = ui->allTagTable;

    // 设置病人信息表头
    QStandardItemModel *patientModel = new QStandardItemModel(0, 3, m_PatientTagTable);
    patientModel->setHorizontalHeaderLabels(QStringList() << "XTagName" << "TagName" << "Value");
    // 设置表格内容
    m_PatientTagTable->setModel(patientModel);
    SetTagTableAutoResize(m_PatientTagTable);

    // 设置所有信息表头
    QStandardItemModel *allTagModel = new QStandardItemModel(0, 3, m_AllTagTable);
    allTagModel->setHorizontalHeaderLabels(QStringList() << "XTagName" << "TagName" << "Value");
    // 设置表格内容
    m_AllTagTable->setModel(allTagModel);
    SetTagTableAutoResize(m_AllTagTable);

    InitVTKWidget();

    // 绑定菜单
    BindingMenus();

    // 初始化其他事件绑定
    InitEventBindings();
}

MainWindow::~MainWindow()
{
    // 解除函数绑定

    for (auto &conn : m_Connections)
    {
        disconnect(conn);
    }
}
