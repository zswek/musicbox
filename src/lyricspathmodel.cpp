#include "lyricspathmodel.h"

LyricsPathFile::LyricsPathFile(const QString &name,const bool &isdir)
    : m_name(name), m_isdir(isdir)
{
}

QString LyricsPathFile::name() const
{
    return m_name;
}


bool LyricsPathFile::isdir() const
{
    return m_isdir;
}


LyricsPathModel::LyricsPathModel(QObject *parent)
    : QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[IsDirRole] = "isdir";
    setRoleNames(roles);
}

void LyricsPathModel::openPath(QString p,bool hidden) {
	path=p;
	clear();
	QStringList filelist;
	if (hidden) filelist = QDir(p).entryList(QDir::Dirs|QDir::Files|QDir::Hidden|QDir::Readable);
	else filelist = QDir(p).entryList(QDir::Dirs|QDir::Files|QDir::Readable);
	for (int n=0;n<filelist.count();n++) {
		if (filelist[n]!="."&&filelist[n]!=".."&&QFileInfo(path+"/"+filelist[n]).isDir()) {
			addItem(LyricsPathFile(filelist[n],QFileInfo(path+"/"+filelist[n]).isDir()));
		}
	}
	for (int n=0;n<filelist.count();n++) {
		if (filelist[n]!="."&&filelist[n]!=".."&&QFileInfo(path+"/"+filelist[n]).isFile()&&isLyrics(filelist[n])) {
			addItem(LyricsPathFile(filelist[n],QFileInfo(path+"/"+filelist[n]).isDir()));
		}
	}
}

bool LyricsPathModel::isLyrics(QString name) {
	if (name.toLower().endsWith(".lrc")) return true;
	return false;
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

LyricsPathFile LyricsPathModel::getItem(int i)
{
    return m_pathlist[i];
}

void LyricsPathModel::addItem(const LyricsPathFile &pathfile)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_pathlist << pathfile;
    endInsertRows();
}

void LyricsPathModel::removeItem(int i)
{
    beginRemoveRows(QModelIndex(), i, i);
    m_pathlist.removeAt(i);
    endRemoveRows();
}

void LyricsPathModel::clear()
{
    beginResetModel();
    m_pathlist.clear();
    endResetModel();
}

int LyricsPathModel::rowCount(const QModelIndex & parent) const {
    return m_pathlist.count();
}

QVariant LyricsPathModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() > m_pathlist.count())
        return QVariant();

    const LyricsPathFile &pathfile = m_pathlist[index.row()];
    if (role == NameRole)
        return pathfile.name();
    else if (role == IsDirRole)
        return pathfile.isdir();
    return QVariant();
}

