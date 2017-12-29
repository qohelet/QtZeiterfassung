#include "absencesmodel.h"

#include "zeiterfassungapi.h"

AbsencesModel::AbsencesModel(int userId, const QDate &date, ZeiterfassungApi &erfassung, QObject *parent) :
    QAbstractListModel(parent),
    m_userId(userId),
    m_erfassung(erfassung)
{
    setDate(date);
}

bool AbsencesModel::enabled() const
{
    return m_reply == Q_NULLPTR;
}

int AbsencesModel::rowCount(const QModelIndex &parent) const
{

}

int AbsencesModel::columnCount(const QModelIndex &parent) const
{

}

QVariant AbsencesModel::data(const QModelIndex &index, int role) const
{

}

QVariant AbsencesModel::headerData(int section, Qt::Orientation orientation, int role) const
{

}

void AbsencesModel::setDate(const QDate &date)
{
    m_date = date;

    m_reply = m_erfassung.doGetAbsences(m_userId, m_date, m_date);
    connect(m_reply.get(), &ZeiterfassungReply::finished, this, &AbsencesModel::finished);
}

void AbsencesModel::finished()
{

}
