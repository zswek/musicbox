#include "playlistmodel.h"

double stime(QString t) {
	QStringList tl=t.split(":");
	if (tl.count()==3) {
		return tl[0].toDouble()*60+tl[1].toDouble()+tl[2].toDouble()/75;
	}
	return 0;
}

PlayListItem::PlayListItem(const QString &name, const QString &path, const QString &lyrics, const QString &track, const double &start, const double &stop, const QString &artist, const QString &album)
    : m_name(name), m_path(path), m_lyrics(lyrics), m_track(track), m_start(start), m_stop(stop), m_artist(artist), m_album(album)
{
}

QString PlayListItem::name() const
{
    return m_name;
}

QString PlayListItem::path() const
{
    return m_path;
}

QString PlayListItem::lyrics() const
{
    return m_lyrics;
}

QString PlayListItem::track() const
{
    return m_track;
}

double PlayListItem::start() const
{
    return m_start;
}

double PlayListItem::stop() const
{
    return m_stop;
}

QString PlayListItem::artist() const
{
    return m_artist;
}

QString PlayListItem::album() const
{
    return m_album;
}

PlayListModel::PlayListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[PathRole] = "path";
    roles[LyricsRole] = "lyrics";
    roles[TrackRole] = "track";
    roles[StartRole] = "start";
    roles[StopRole] = "stop";
    roles[ArtistRole] = "artist";
    roles[AlbumRole] = "album";
    setRoleNames(roles);
}

void PlayListModel::openList(QString path)
{
	QFile file(path);
	if (file.open(QIODevice::ReadOnly)) {
		QTextStream stream(&file);
		QString list;
		while (!stream.atEnd()) {
			list=stream.readLine();
			if (list!="") {
				QStringList item = list.split("\t");
				if (item.count()==8) {
					addItem(PlayListItem(item[0], item[1], item[2], item[3], item[4].toDouble(), item[5].toDouble(), item[6], item[7]));
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
			QString start,stop;
			start.setNum(m_playlist[n].start());
			stop.setNum(m_playlist[n].stop());
			stream << m_playlist[n].name() << "\t" << m_playlist[n].path() << "\t" << m_playlist[n].lyrics() <<  "\t" << m_playlist[n].track() << "\t" << start << "\t" << stop << "\t" << m_playlist[n].artist() << "\t" << m_playlist[n].album() << "\n";
		}
		file.close();
	}
}

PlayListItem PlayListModel::getItem(int i)
{
    return m_playlist[i];
}

void PlayListModel::addItem(const PlayListItem &playlistitem)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_playlist << playlistitem;
    endInsertRows();
}

void PlayListModel::insertItem(int n,const PlayListItem &playlistitem) {
    beginInsertRows(QModelIndex(), n, n);
    m_playlist.insert(n,playlistitem);
    endInsertRows();
}

void PlayListModel::removeItem(int i)
{
    beginRemoveRows(QModelIndex(), i, i);
    m_playlist.removeAt(i);
    endRemoveRows();
}

void PlayListModel::clear()
{
    beginResetModel();
    m_playlist.clear();
    endResetModel();
}

void PlayListModel::addCUE(QString path) {
	QFile file(path);
	if (file.open(QIODevice::ReadOnly)) {
		QTextStream stream(&file);
		stream.setCodec(QTextCodec::codecForName("GBK") );
		QString line;
		QString tfile="";
		int ttrack=0;
		int tindex=-1;
		QStringList ttitlelist;
		int tstart=0;
		int tstop=0;
		QString artist="";
		QString album="";
		while (!stream.atEnd()) {
			line=stream.readLine();
			QStringList ttl=line.split("\"");
			QStringList tl=ttl[0].trimmed().split(" ");
			if (ttl.count()>1) {
				tl.append(ttl[1]);
			}
			if (tl[0].toUpper()=="FILE") {
				tfile=tl[1];
				if (!tfile.startsWith("/")) {
				tfile=path.left(path.lastIndexOf("/"))+"/"+tfile;
				}
			}
			if (tl[0].toUpper()=="TRACK") {
				ttrack=tl[1].toInt();
			}
			if (tl[0].toUpper()=="TITLE") {
				if (ttrack>0) ttitlelist.append(tl[1]);
				else album=tl[1];
			}
			if (tl[0].toUpper()=="PERFORMER") {
				artist=tl[1];
			}
			if (tl[0].toUpper()=="INDEX") {
				if (tl[1].toInt()==0) {
					tstop=stime(tl[2]);
				}
				if (tl[1].toInt()==1) {
				if (ttrack>1) {
//				if (tindex==0) {
//					addItem(PlayListItem(ttitlelist[ttrack-2],tfile,tfile.left(tfile.lastIndexOf("/"))+"/"+ttitlelist[ttrack-2]+".lrc",path,tstart,tstop));
//				}
//				if (tindex==1) {
					addItem(PlayListItem(ttitlelist[ttrack-2],tfile,"",path,tstart,stime(tl[2]),artist,album));
//				}
				}
					tstart=stime(tl[2]);
				}
				tindex=tl[1].toInt();
			}
		}
		addItem(PlayListItem(ttitlelist[ttrack-1],tfile,"",path,tstart,0,artist,album));
	}
}

void PlayListModel::addMusic(QString path,QString id3v1code) {
	if (path.endsWith(".cue")) {
		addCUE(path);
	}
	else {
		QString ttitle="",tartist="",talbum="";
		APEv2 apev2;
		if (apev2.open(path)) {
			for (int n=0;n<apev2.taglist.count();n++) {
				if (apev2.taglist[n].id=="Title") {
					ttitle=apev2.taglist[n].value;
				}
				if (apev2.taglist[n].id=="Artist") {
					tartist=apev2.taglist[n].value;
				}
				if (apev2.taglist[n].id=="Album") {
					talbum=apev2.taglist[n].value;
				}
			}
		}
		ID3v2 id3v2;
		if (id3v2.open(path)) {
			for (int n=0;n<id3v2.taglist.count();n++) {
				if ((id3v2.taglist[n].id=="TIT2"||id3v2.taglist[n].id=="TT2")&&ttitle=="") {
					ttitle=id3v2.taglist[n].readtag();
				}
				if ((id3v2.taglist[n].id=="TPE1"||id3v2.taglist[n].id=="TP1")&&tartist=="") {
					tartist=id3v2.taglist[n].readtag();
				}
				if ((id3v2.taglist[n].id=="TALB"||id3v2.taglist[n].id=="TAL")&&talbum=="") {
					talbum=id3v2.taglist[n].readtag();
				}
			}
		}
		QStringList id3v1=readID3v1(path,id3v1code);
		if (id3v1.count()!=0) {
			if(ttitle=="") ttitle=id3v1[0];
			if(tartist=="") tartist=id3v1[1];
			if(talbum=="") talbum=id3v1[2];
		}
		if(ttitle=="") ttitle=path.mid(path.lastIndexOf("/")+1,path.lastIndexOf(".")-1-path.lastIndexOf("/"));
		addItem(PlayListItem(ttitle,path,"","",0,0,tartist,talbum));
	}
}

QStringList PlayListModel::readID3v1(QString path,QString code) {
	QFile file(path);
	QStringList tag;
	if (file.open(QIODevice::ReadOnly)) {
		file.seek(file.size()-128);
		if (QString(file.read(3))=="TAG") {
			tag.append(QTextCodec::codecForName(code.toAscii().constData())->toUnicode(file.read(30).constData()));
			tag.append(QTextCodec::codecForName(code.toAscii().constData())->toUnicode(file.read(30).constData()));
			tag.append(QTextCodec::codecForName(code.toAscii().constData())->toUnicode(file.read(30).constData()));
			tag.append(QTextCodec::codecForName(code.toAscii().constData())->toUnicode(file.read(4).constData()));
			file.seek(file.size()-3);
			if (*file.read(1).constData()=='\0') {
				file.seek(file.size()-31);
				tag.append(QTextCodec::codecForName(code.toAscii().constData())->toUnicode(file.read(28).constData()));
				file.seek(file.size()-2);
				QString track;
				track.setNum((int)*file.read(1).constData());
				tag.append(track);
				QString genre;
				genre.setNum((int)*file.read(1).constData());
				tag.append(genre);
			}
			else {
				file.seek(file.size()-31);
				tag.append(QTextCodec::codecForName(code.toAscii().constData())->toUnicode(file.read(30).constData()));
				QString genre;
				genre.setNum((int)*file.read(1).constData());
				tag.append(genre);
			}
		}
		file.close();
	}
	return tag;
}

QString PlayListModel::lyrics_search_slots(QString p) {
	if (QFileInfo(p.left(p.lastIndexOf("."))+".lrc").isFile()) {
		return p.left(p.lastIndexOf("."))+".lrc";
	}
	return "";
}

int PlayListModel::rowCount(const QModelIndex & parent) const {
    return m_playlist.count();
}

QVariant PlayListModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() > m_playlist.count())
        return QVariant();

    const PlayListItem &playlistitem = m_playlist[index.row()];
    if (role == NameRole)
        return playlistitem.name();
    else if (role == PathRole)
        return playlistitem.path();
    else if (role == LyricsRole)
        return playlistitem.lyrics();
    else if (role == TrackRole)
        return playlistitem.track();
    else if (role == StartRole)
        return playlistitem.start();
    else if (role == StopRole)
        return playlistitem.stop();
    else if (role == ArtistRole)
        return playlistitem.artist();
    else if (role == AlbumRole)
        return playlistitem.album();
    return QVariant();
}

