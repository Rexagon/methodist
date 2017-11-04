#ifndef TESTSTABLEMODEL_H
#define TESTSTABLEMODEL_H

#include <QAbstractTableModel>

#include "../objects/Task.h"

class TestsTableModel : public QAbstractTableModel
{
public:
    TestsTableModel(QObject* parent = nullptr);
    ~TestsTableModel();

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void setTask(Task* task);
    Task* getTask() const;

public:
    Task* m_task;
};

#endif // TESTSTABLEMODEL_H
