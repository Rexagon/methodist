#include "SamplesTableModel.h"

SamplesTableModel::SamplesTableModel(QObject* parent) :
    QAbstractTableModel(parent)
{
}

SamplesTableModel::~SamplesTableModel()
{
}

int SamplesTableModel::rowCount(const QModelIndex& parent) const
{
    return m_samples.size();
}

int SamplesTableModel::columnCount(const QModelIndex& parent) const
{
    return 2;
}

QVariant SamplesTableModel::data(const QModelIndex& index, int role) const
{
    if (index.row() < m_samples.size()) {
        Test* test = m_samples[index.row()];

        if (role == Qt::DisplayRole) {
            switch(index.column()) {
            case 0:
                return test->getInputData();
    
            case 1:
                return test->getOutputData();
            }
        }
        else if (role == Qt::TextAlignmentRole) {
            return Qt::AlignLeft;
        }
    }

    return QVariant();
}

QVariant SamplesTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch(section) {
            case 0:
                return QString("Ввод");

            case 1:
                return QString("Вывод");
            }
        }
        else if (role == Qt::TextAlignmentRole) {
            return Qt::AlignLeft;
        }
    }

    return QVariant();
}

void SamplesTableModel::fill(Task* task)
{
    m_samples.clear();
    if (task == nullptr) {
        return;
    }
    
    for (size_t i = 0; i < task->getTestCount(); ++i) {
        Test* test = task->getTest(i);
        if (test->isSample()) {
            m_samples.push_back(test);
        }
    }
    
    emit layoutChanged();
}
