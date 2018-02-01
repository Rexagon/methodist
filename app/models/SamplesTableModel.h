#ifndef SAMPLESTABLEMODEL_H
#define SAMPLESTABLEMODEL_H

#include <QAbstractTableModel>

#include "../objects/Task.h"

class SamplesTableModel : public QAbstractTableModel
{
public:
    SamplesTableModel(QObject* parent = nullptr);
    ~SamplesTableModel();
    
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void fill(Task* task);

private:
    std::vector<Test*> m_samples;    
};

#endif // SAMPLESTABLEMODEL_H
