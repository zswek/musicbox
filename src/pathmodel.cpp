#include "pathmodel.h"

PathFile::PathFile(const QString &name,const bool &isdir)
    : m_name(name), m_isdir(isdir)
{
}

QString PathFile::name() const
{
    return m_name;
}


bool PathFile::isdir() const
{
    return m_isdir;
}


PathModel::PathModel(QObject *parent)
    : QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[IsDirRole] = "isdir";
    setRoleNames(roles);
}

void PathModel::openPath(QString p,QStringList mime,bool hidden) {
	path=p;
	clear();
	m_fileseleted.clear();
	filestart=0;
	QStringList filelist;
	if (hidden) filelist = QDir(p).entryList(QDir::Dirs|QDir::Files|QDir::Hidden|QDir::Readable);
	else filelist = QDir(p).entryList(QDir::Dirs|QDir::Files|QDir::Readable);
	for (int n=0;n<filelist.count();n++) {
		if (filelist[n]!="."&&filelist[n]!=".."&&QFileInfo(path+"/"+filelist[n]).isDir()) {
			addItem(PathFile(filelist[n],QFileInfo(path+"/"+filelist[n]).isDir()));
		}
	}
	filestart=m_pathlist.count();
	for (int n=0;n<filelist.count();n++) {
		if (filelist[n]!="."&&filelist[n]!=".."&&QFileInfo(path+"/"+filelist[n]).isFile()&&isMime(filelist[n],mime)) {
			addItem(PathFile(filelist[n],QFileInfo(path+"/"+filelist[n]).isDir()));
			m_fileseleted.append(false);
		}
	}
}

bool PathModel::isMime(QString name,QStringList mime) {
	for (int n=0;n<mime.count();n++) {
		if (name.toLower().endsWith("."+mime[n])) return true;
	}
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

PathFile PathModel::getItem(int i)
{
    return m_pathlist[i];
}

void PathModel::addItem(const PathFile &pathfile)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_pathlist << pathfile;
    endInsertRows();
}

void PathModel::removeItem(int i)
{
    beginRemoveRows(QModelIndex(), i, i);
    m_pathlist.removeAt(i);
    endRemoveRows();
}

void PathModel::clear()
{
    beginResetModel();
    m_pathlist.clear();
    endResetModel();
}

int PathModel::rowCount(const QModelIndex & parent) const {
    return m_pathlist.count();
}

QVariant PathModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() > m_pathlist.count())
        return QVariant();

    const PathFile &pathfile = m_pathlist[index.row()];
    if (role == NameRole)
        return pathfile.name();
    else if (role == IsDirRole)
        return pathfile.isdir();
    return QVariant();
}

