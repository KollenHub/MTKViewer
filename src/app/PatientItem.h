#pragma once
#include "dicom/DicomData.h"
#include <vector>
#include <QStandardItem>
#include <QTreeView>
#include <QString>
enum ShowOption
{
    Patient = 1,
    Studies = 1 << 1,
    Series = 1 << 2,
    Imges = 1 << 3,
};

enum DataType
{
    ID = Qt::UserRole + 1,
    ImageData,
};

class PatientItem
{

private:
    std::vector<std::shared_ptr<DicomData>> m_DicomDatas;

    QString m_ID;

public:
    const QString &Id();

    const QString& Name();

    PatientItem(std::shared_ptr<DicomData> dicomData);

    bool AddItem(std::shared_ptr<DicomData> dicomData);

    QStandardItem *AddRecursion(QStandardItem *parent, std::shared_ptr<DicomData> dicomData, const std::vector<int> &fieldIndexs) const;

    std::function<QString(const std::shared_ptr<DicomData> &)> GetFieldFunc(int fieldIndex) const;

    int GetItemLevel(const QStandardItem *const item) const;

    std::vector<QStandardItem *> GenerateItems(QTreeView *treeView, int option) const;

    ~PatientItem();
};
