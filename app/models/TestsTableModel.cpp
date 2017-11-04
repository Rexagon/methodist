#include "TestsTableModel.h"

TestsTableModel::TestsTableModel(QObject* parent) :
    QAbstractTableModel(parent), m_task(nullptr)
{
}

TestsTableModel::~TestsTableModel()
{
}

int TestsTableModel::rowCount(const QModelIndex& parent) const
{
    return m_task->getChildCount();
}

int TestsTableModel::columnCount(const QModelIndex& parent) const
{
    return 4;
}

QVariant TestsTableModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole) {
        Test* test = m_task->getTest(index.row());

        switch(index.column()) {
        case 0:
            return test->getInputData();

        case 1:
            return test->getOutputData();

        case 2:
            return test->isRequired();

        case 3:
            return test->getScore();
        }
    }

    return QVariant();
}

QVariant TestsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch(section) {
            case 0:
                return QString("Входные данные");

            case 1:
                return QString("Выходные данные");

            case 2:
                return QString("Обязателен");

            case 3:
                return QString("Баллы");
            }
        }
        else if (orientation == Qt::Vertical) {
            return section;
        }
    }

    return QVariant();
}

void TestsTableModel::setTask(Task* task)
{
    m_task = task;
}

Task* TestsTableModel::getTask() const
{
    return m_task;
}
