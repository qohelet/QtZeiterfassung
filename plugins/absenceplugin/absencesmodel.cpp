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
    Q_UNUSED(parent)

    return m_absences.count();
}

int AbsencesModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 9;
}

QVariant AbsencesModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(index.row() < m_absences.count());
    const auto &absence = m_absences.at(index.row());

    switch(role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        switch(index.column())
        {
        case 0: return absence.altRepresentative;
        case 1: return absence.compositeId;
        case 2: return absence.end;
        case 3: return absence.hourCategory;
        case 4: return absence.openMarking;
        case 5: return absence.persNr;
        case 6: return absence.representative;
        case 7: return absence.start;
        case 8: return absence.text;
        }
    }

    return QVariant();
}

QVariant AbsencesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch(orientation)
    {
    case Qt::Horizontal:
        switch(role)
        {
        case Qt::DisplayRole:
        case Qt::EditRole:
            switch(section)
            {
            case 0: return tr("altRepresentative");
            case 1: return tr("compositeId");
            case 2: return tr("end");
            case 3: return tr("hourCategory");
            case 4: return tr("openMarking");
            case 5: return tr("persNr");
            case 6: return tr("representative");
            case 7: return tr("start");
            case 8: return tr("text");
            }
        }
    default:
        qt_noop();
    }

    return QVariant();
}

void AbsencesModel::setDate(const QDate &date)
{
    m_date = date;

    auto oldEnabled = enabled();

    m_reply = m_erfassung.doGetAbsences(m_userId, m_date, m_date);
    connect(m_reply.get(), &ZeiterfassungReply::finished, this, &AbsencesModel::finished);

    if(oldEnabled != enabled())
        Q_EMIT enabledChanged(enabled());
}

void AbsencesModel::finished()
{
    if(!m_reply->success())
        Q_EMIT errorOccured(m_reply->message());

    beginResetModel();
    m_absences = m_reply->absences();
    endResetModel();

    auto oldEnabled = enabled();

    m_reply = Q_NULLPTR;

    if(oldEnabled != enabled())
        Q_EMIT enabledChanged(enabled());
}
