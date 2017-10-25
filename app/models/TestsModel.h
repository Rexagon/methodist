#ifndef TESTSMODEL_H
#define TESTSMODEL_H

#include <QAbstractTableModel>

#include "Task.h"

class TestsModel : public QAbstractTableModel
{
public:
    TestsModel(QObject* parent = nullptr);
    ~TestsModel();

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void setTask(Task* task);
    Task* getTask() const;

public:
    Task* m_task;
};

#endif // TESTSMODEL_H
