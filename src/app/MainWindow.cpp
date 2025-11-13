#include "MainWindow.h"
#include <vtkGenericOpenGLRenderWindow.h>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkRenderWindowInteractor.h>
#include "core/Logger.h"
#include "dicom/DicomOperator.h"
#include <QFileDialog>
#include <QDebug>
#include <QStandardItemModel>
#include "vtkUtils/vtkRendererUtils.h"
#include <vtkBMPReader.h>
#include <vtkInteractorStyleImage.h>
#include <QLayoutItem>
#include <vtkRendererCollection.h>
#include "extension/SplitterManager.h"
#include <QMessageBox>
#include <QCollator>

// void MainWindow::DeleteProjectInfo(const std::shared_ptr<ProjectItem> &projectInfo)
// {
//     auto it = std::find(m_Projects.begin(), m_Projects.end(), projectInfo);
//     if (it != m_Projects.end())
//     {
//         m_Projects.erase(it);
//         UpdateProjectTree(projectInfo, "delete");
//     }
// }

void MainWindow::SetTableViewData()
{
    // if (m_DicomData != nullptr)
    // {
    //     // 获取treeview的model
    //     QStandardItemModel *allTagModel = qobject_cast<QStandardItemModel *>(m_AllTagTable->model());
    //     allTagModel->removeRows(0, allTagModel->rowCount());

    //     QStandardItemModel *patientTagModel = qobject_cast<QStandardItemModel *>(m_PatientTagTable->model());
    //     patientTagModel->removeRows(0, patientTagModel->rowCount());

    //     for (auto &tag : m_DicomData->GetDataSetCardinality())
    //     {
    //         QStandardItem *item1 = new QStandardItem(tag.xTagName());
    //         QStandardItem *item2 = new QStandardItem(tag.tagName());
    //         QStandardItem *item3 = new QStandardItem(tag.value());
    //         allTagModel->appendRow(QList<QStandardItem *>{item1, item2, item3});
    //         if (tag.isPatientTag())
    //         {
    //             QStandardItem *item4 = new QStandardItem(tag.xTagName());
    //             QStandardItem *item5 = new QStandardItem(tag.tagName());
    //             QStandardItem *item6 = new QStandardItem(tag.value());
    //             patientTagModel->appendRow(QList<QStandardItem *>{item4, item5, item6});
    //         }
    //     }

    //     for (auto &tag : m_DicomData->GetMetaInfoCardinality())
    //     {
    //         QStandardItem *item1 = new QStandardItem(tag.xTagName());
    //         QStandardItem *item2 = new QStandardItem(tag.tagName());
    //         QStandardItem *item3 = new QStandardItem(tag.value());
    //         allTagModel->appendRow(QList<QStandardItem *>{item1, item2, item3});
    //         if (tag.isPatientTag())
    //         {
    //             QStandardItem *item4 = new QStandardItem(tag.xTagName());
    //             QStandardItem *item5 = new QStandardItem(tag.tagName());
    //             QStandardItem *item6 = new QStandardItem(tag.value());
    //             patientTagModel->appendRow(QList<QStandardItem *>{item4, item5, item6});
    //         }
    //     }

    //     for (auto &tag : m_DicomData->GetMetaInfoCardinality())
    //     {

    //         QStandardItem *item1 = new QStandardItem(tag.xTagName());
    //         QStandardItem *item2 = new QStandardItem(tag.tagName());
    //         QStandardItem *item3 = new QStandardItem(tag.value());
    //         allTagModel->appendRow(QList<QStandardItem *>{item1, item2, item3});
    //         if (tag.isPatientTag())
    //         {
    //             QStandardItem *item4 = new QStandardItem(tag.xTagName());
    //             QStandardItem *item5 = new QStandardItem(tag.tagName());
    //             QStandardItem *item6 = new QStandardItem(tag.value());
    //             patientTagModel->appendRow(QList<QStandardItem *>{item4, item5, item6});
    //         }
    //     }
    // }
}
void MainWindow::ResetImageData()
{
    // vtkSmartPointer<vtkRenderer> firstRenderer = GetRenderByIndex(0);
    // vtkRendererUtils::AddImage(firstRenderer, m_DicomData->GetImageData());
    // firstRenderer->ResetCamera();
    // m_RendererWindow->Render();
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
void MainWindow::InitStatusBar()
{
    auto statusBar = ui->statusBar;
    m_ProgressBar = new QProgressBar(statusBar);
    m_ProgressBar->setStyleSheet(
        "QProgressBar {"
        "    border: 1px solid #333333;"
        "    border-radius: 1px;"
        "    background-color: #1e1e1e;"
        "    text-align: center;"
        "    color: #ffffff;"
        "    font-weight: bold;"
        "}"
        "QProgressBar::chunk {"
        "    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, "
        "    stop:0 #4FC3F7, stop:0.5 #29B6F6, stop:1 #0288D1);"
        "    border-radius: 1px;"
        "}");
    m_ProgressBar->setRange(0, 100);
    m_ProgressBar->setVisible(false);
    m_ProgressBar->setTextVisible(true);
    m_ProgressBar->setFixedHeight(20);
    m_ProgressBar->setFixedWidth(200);
    m_ProgressBar->setFormat("%p%");
    statusBar->addWidget(m_ProgressBar);
}
void MainWindow::BindingMenus()
{
    auto openConnection = BindingAction("打开", std::bind(&MainWindow::OpenDicom, this));
    auto closeConnection = BindingAction("关闭文件", std::bind(&MainWindow::CloseDicom, this));
    auto exitConnection = BindingAction("退出程序", std::bind(&MainWindow::Exit, this));

    auto openFolderConnection = BindingAction("打开文件夹", std::bind(&MainWindow::OpenDicomFolder, this));
    m_Connections.push_back(openConnection);
    m_Connections.push_back(closeConnection);
    m_Connections.push_back(exitConnection);
    m_Connections.push_back(openFolderConnection);
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

// void MainWindow::UpdateProjectTree(const std::shared_ptr<ProjectItem> &projectInfo, QString option)
// {
//     // 找到当前是那个
//     QStandardItemModel *treeModel = qobject_cast<QStandardItemModel *>(m_ProjectTree->model());

//     if (option == "add")
//     {
//         if (projectInfo->getChildren().size() > 0)
//         {
//             QStandardItem *item = new QStandardItem(projectInfo->getName());
//             item->setData(projectInfo->getPath(), Qt::UserRole + 2);
//             for (auto &child : projectInfo->getChildren())
//             {
//                 QStandardItem *childItem = new QStandardItem(child->getName());
//                 childItem->setData(child->getPath(), Qt::UserRole + 2);
//                 item->appendRow(childItem);
//             }
//         }
//     }
//     else
//     {
//         auto items = treeModel->findItems(projectInfo->getName(), Qt::MatchRecursive);

//         if (option == "update")
//         {

//         }
//         else if (option == "delete")
//         {

//         }
//     }
// }

void MainWindow::AddProjectInfo(const std::shared_ptr<PatientItem> &patientInfo)
{
    if (patientInfo == nullptr)
        return;

    m_PatientItems.push_back(patientInfo);

    std::vector<QStandardItem *> items = patientInfo->GenerateItems(m_ProjectTree, defaultOption);

    auto model = qobject_cast<QStandardItemModel *>(m_ProjectTree->model());
    for (auto &item : items)
    {
        model->appendRow(item);
    }
}

void MainWindow::UpdateProjectInfo(const std::shared_ptr<PatientItem> &patientInfo)
{
    if (patientInfo == nullptr)
        return;

    auto model = qobject_cast<QStandardItemModel *>(m_ProjectTree->model());

    auto items = model->findItems(patientInfo->Name(), Qt::MatchRecursive);

    // 替换数据
    auto newItems = patientInfo->GenerateItems(m_ProjectTree, defaultOption);

    int insertRow = items[0]->row();

    model->removeRows(items[0]->row(), items.size());

    for (size_t i = 0; i < newItems.size(); i++)
    {
        model->insertRow(insertRow + i, newItems[i]);
    }
}

// bool MainWindow::UpdateProjectInfo(const std::shared_ptr<ProjectItem> &projectInfo)
// {
//     if (projectInfo == nullptr)
//         return;

//     for (size_t i = 0; i < m_Projects.size(); i++)
//     {
//         if (m_Projects[i]->getPath() == projectInfo->getPath())
//         {
//             // TODO:当这个值是当前选择項这里直接替换可能会引起数据的问题
//             m_Projects[i] = projectInfo;
//             UpdateProjectTree(projectInfo, "update");
//             return true;
//         }
//     }

//     return false;
// }

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
        "DICOM文件(*.*)");
    if (!fileName.isEmpty())
    {
        // 判断后缀名
        QFileInfo fileInfo(fileName);
        if (!(fileInfo.suffix().isEmpty() || !m_DicomFileExts.contains(fileInfo.suffix())))
        {
            QMessageBox::warning(this, "提示", "请选择DICOM文件");
            return;
        }

        // 处理选中的文件
        qDebug() << "选中的文件:" << fileName;

        std::shared_ptr<DicomData> dicomData = DicomOperator::OpenDicomFile(fileName);

        bool isHandled = false;
        for (size_t i = 0; i < m_PatientItems.size(); i++)
        {
            if (m_PatientItems[i]->AddItem(dicomData))
            {
                // 更新数据
                UpdateProjectInfo(m_PatientItems[i]);
                isHandled = true;
                break;
            }
        }

        if (!isHandled)
        {
            std::shared_ptr<PatientItem> patientInfo = std::make_shared<PatientItem>(dicomData);
            AddProjectInfo(patientInfo);
        }
    }
}
void MainWindow::OpenDicomFolder()
{
    Logger::info("打开DICOM文件夹");

    QDir dir = QFileDialog::getExistingDirectory(
        this,
        "选择Dicom文件夹",
        "",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (dir.isEmpty())
    {
        return;
    }

    Logger::info("打开文件夹{}", dir.absolutePath().toStdString());

    QStringList fileList = dir.entryList(QDir::Files | QDir::NoDotAndDotDot | QDir::Readable);

    // 使用 QCollator 进行自然排序
    QCollator collator;
    collator.setNumericMode(true); // 启用数字模式

    // 记录一次性更新
    std::vector<std::shared_ptr<PatientItem>> needUpdatePatientItems;

    std::sort(fileList.begin(), fileList.end(), collator);

    m_ProgressBar->setVisible(true);
    m_ProgressBar->setRange(0, fileList.size());
    m_ProgressBar->setValue(0);
    for (size_t i = 0; i < fileList.size(); i++)
    {
        Logger::info("文件名::{}", fileList[i].toStdString());

        QString fileName = dir.absoluteFilePath(fileList[i]);
        // 获取文件名
        QFileInfo fileInfo(fileName);

        // 获取文件后缀
        QString fileSuffix = fileInfo.suffix();

        Logger::info("文件后缀::{}", fileSuffix.toStdString());

        // 判断后缀名
        if (!(fileInfo.suffix().isEmpty() || m_DicomFileExts.contains(fileInfo.suffix())))
        {
            continue;
        }

        std::shared_ptr<DicomData> dicomData = DicomOperator::OpenDicomFile(fileName);

        if (dicomData == nullptr)
        {
            continue;
        }

        bool isHandled = false;
        for (size_t i = 0; i < m_PatientItems.size(); i++)
        {
            if (m_PatientItems[i]->AddItem(dicomData))
            {
                if (std::find(needUpdatePatientItems.begin(), needUpdatePatientItems.end(), m_PatientItems[i]) == needUpdatePatientItems.end())
                {
                    // 记录起来批量更新
                    needUpdatePatientItems.push_back(m_PatientItems[i]);
                }
                // 更新数据
                // UpdateProjectInfo(m_PatientItems[i]);
                isHandled = true;
                break;
            }
        }

        if (!isHandled)
        {
            // 新建的直接更新
            std::shared_ptr<PatientItem> patientInfo = std::make_shared<PatientItem>(dicomData);
            AddProjectInfo(patientInfo);
        }

        m_ProgressBar->setValue(i + 1);
    }

    for (auto updateItem : needUpdatePatientItems)
    {
        UpdateProjectInfo(updateItem);
    }
    m_ProgressBar->setVisible(false);
}
void MainWindow::CloseDicom()
{
    Logger::info("关闭DICOM文件");
}
void MainWindow::Exit()
{
    Logger::info("退出程序");
}

void SetTagTableAutoResize(QTableView *tableView)
{
    QHeaderView *header = tableView->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::ResizeToContents); // 前几列根据内容调整
    header->setStretchLastSection(true);                         // 最后一列自动拉伸填充
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow())
{
    ui->setupUi(this);

    SplitterManager::RestoreSplitterState(ui->splitter);

    m_PatientTagTable = ui->patientTagTable;
    m_AllTagTable = ui->allTagTable;
    m_ProjectTree = ui->ProjectTree;

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

    // 设置项目树
    QStandardItemModel *projectModel = new QStandardItemModel(0, 1, m_ProjectTree);
    m_ProjectTree->setModel(projectModel);

    InitVTKWidget();

    // 初始化状态栏
    InitStatusBar();

    // 绑定菜单
    BindingMenus();

    // 初始化其他事件绑定
    InitEventBindings();
}

MainWindow::~MainWindow()
{
    SplitterManager::SaveSplitterState(ui->splitter);
    // 解除函数绑定

    for (auto &conn : m_Connections)
    {
        disconnect(conn);
    }
}
