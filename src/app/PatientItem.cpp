#include "PatientItem.h"
#include <QUuid>
const QString &PatientItem::Id()
{
    return m_ID;
}

const QString &PatientItem::Name()
{
    return m_DicomDatas[0]->PatientName();
}

PatientItem::PatientItem(std::shared_ptr<DicomData> dicomData)
{
    m_ID = QUuid::createUuid().toString();
    m_DicomDatas.push_back(dicomData);
}

bool PatientItem::AddItem(std::shared_ptr<DicomData> dicomData)
{
    if (dicomData->PatientName() == m_DicomDatas[0]->PatientName())
    {
        m_DicomDatas.push_back(dicomData);
        return true;
    }
    return false;
}

QStandardItem *PatientItem::AddRecursion(QStandardItem *parent, std::shared_ptr<DicomData> dicomData, const std::vector<int> &fieldIndexs) const
{

    if (parent)
    {
        int index = GetItemLevel(parent);

        Logger::info("Item Level: {}", index);

        // 如果是最后一级，将数据添加到当前項
        if (index == fieldIndexs.size() - 1)
        {
            parent->setData(QVariant::fromValue(dicomData), DataType::ImageData);
        }

        QString fieldValue = GetFieldFunc(fieldIndexs[index])(dicomData);

        Logger::info("Field Value: {}  Parent:{}", fieldValue.toStdString(), parent->text().toStdString());

        // 如果当前父节点与当前的节点值获取的值一直，则判断是否一致
        if (parent->text() == fieldValue)
        {
            // 判断是否有子项
            // bool handled = false;
            for (size_t i = 0; i < parent->rowCount(); i++)
            {
                QStandardItem *child = parent->child(i);
                if (AddRecursion(child, dicomData, fieldIndexs))
                {
                    // handled = true;
                    // break;
                    return child;
                }
            }

            if (index < fieldIndexs.size() - 1)
            {
                QString childValue = GetFieldFunc(fieldIndexs[index + 1])(dicomData);
                QStandardItem *item = new QStandardItem(childValue);
                parent->appendRow(item);
                AddRecursion(item, dicomData, fieldIndexs);
                return item;
            }
        }
        else
        {
            return nullptr;
        }
    }
    else
    {
        QString fieldValue = GetFieldFunc(fieldIndexs[0])(dicomData);
        QStandardItem *item = new QStandardItem(fieldValue);

        AddRecursion(item, dicomData, fieldIndexs);

        return item;
    }

    return nullptr;
}

std::function<QString(const std::shared_ptr<DicomData> &)> PatientItem::GetFieldFunc(int fieldIndex) const
{
    switch (fieldIndex)
    {
    case ShowOption::Patient:
        return [](const std::shared_ptr<DicomData> &dicomData)
        { return dicomData->PatientName(); };
    case ShowOption::Studies:
        return [](const std::shared_ptr<DicomData> &dicomData)
        { return dicomData->StudyModality(); };
    case ShowOption::Series:
        return [](const std::shared_ptr<DicomData> &dicomData)
        {
            return dicomData->SeriesDescription();
        };
    case ShowOption::Imges:
        return [](const std::shared_ptr<DicomData> &dicomData)
        {
            return dicomData->FileName();
        };
    };
    return std::function<QString(const std::shared_ptr<DicomData> &)>();
}

int PatientItem::GetItemLevel(const QStandardItem *const item) const
{
    return item->parent() ? 1 + GetItemLevel(item->parent()) : 0;
}

std::vector<QStandardItem *> PatientItem::GenerateItems(QTreeView *treeView, int option) const
{
    std::vector<int> fields{ShowOption::Patient, ShowOption::Studies, ShowOption::Series, ShowOption::Imges};
    for (int i = fields.size() - 1; i >= 0; i--)
    {
        if (!(fields[i] & option))
        {
            fields.erase(fields.begin() + i);
        }
    }

    if (treeView == nullptr)
    {
        return std::vector<QStandardItem *>();
    }

    std::vector<QStandardItem *> rootItems;

    for (size_t i = 0; i < m_DicomDatas.size(); i++)
    {
        bool isHandled = false;
        // 如果已经有根节点了，尝试用根节点添加，如果没有的话则返回nullptr，新开一个新的根节点
        for (size_t j = 0; j < rootItems.size(); j++)
        {
            if (AddRecursion(rootItems[j], m_DicomDatas[i], fields))
            {
                isHandled = true;
                break;
            }
        }

        if (!isHandled)
        {
            QStandardItem *item = AddRecursion(nullptr, m_DicomDatas[i], fields);
            item->setData(QVariant::fromValue(m_ID), DataType::ID);
            rootItems.push_back(item);
        }
    }

    return rootItems;
}

PatientItem::~PatientItem()
{
}
