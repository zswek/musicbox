#include "playlistlistmodel.h"

PlayListListItem::PlayListListItem(const QString &name)
    : m_name(name)//, m_path(path)
{
}

QString PlayListListItem::name() const
{
    return m_name;
}

/*
QString PlayListListItem::path() const
{
    return m_path;
}
*/

PlayListListModel::PlayListListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    //roles[PathRole] = "path";
    setRoleNames(roles);
}

/*
void PlayListListModel::openList(QString path)
{
	QFile file(path);
	if (file.open(QIODevice::ReadOnly)) {
		QTextStream stream(&file);
		QString list;
		while (!stream.atEnd()) {
			list=stream.readLine();
			if (list!="") {
				QStringList item = list.split("\t");
				if (item.count()==2) {
					addItem(PlayListItem(item[0], item[1]));
				}
			}
		}
		file.close();
	}
}

void PlayListModel::saveList(QString path)
{
	QFile file(path);
	if (file.open(QIODevice::WriteOnly)) {
		QTextStream stream(&file);
		for (int n=0; n<m_playlist.count(); n++) {
			stream << m_playlist[n].name() << "\t" << m_playlist[n].path() << "\n";
		}
		file.close();
	}
}
*/

PlayListListItem PlayListListModel::getItem(int i)
{
    return m_playlistlist[i];
}

void PlayListListModel::addItem(const PlayListListItem &playlistlistitem)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_playlistlist << playlistlistitem;
    endInsertRows();
}

void PlayListListModel::removeItem(int i)
{
    beginRemoveRows(QModelIndex(), i, i);
    m_playlistlist.removeAt(i);
    endRemoveRows();
}

void PlayListListModel::clear()
{
    beginResetModel();
    m_playlistlist.clear();
    endResetModel();
}

int PlayListListModel::rowCount(const QModelIndex & parent) const {
    return m_playlistlist.count();
}

QVariant PlayListListModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() > m_playlistlist.count())
        return QVariant();

    const PlayListListItem &playlistlistitem = m_playlistlist[index.row()];
    if (role == NameRole)
        return playlistlistitem.name();
    //else if (role == PathRole)
        //return playlistlistitem.path();
    return QVariant();
}

