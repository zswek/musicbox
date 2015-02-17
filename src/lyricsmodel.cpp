#include "lyricsmodel.h"

LyricsLine::LyricsLine(const QString &line, const double &start)
    : m_line(line), m_start(start)
{
}

QString LyricsLine::line() const
{
    return m_line;
}

double LyricsLine::start() const
{
    return m_start;
}

LyricsModel::LyricsModel(QObject *parent)
    : QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[LineRole] = "line";
    roles[StartRole] = "start";
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

LyricsLine LyricsModel::getItem(int i)
{
    return m_lyricslist[i];
}

void LyricsModel::insertItem(int i,const LyricsLine &lyricsline) {
    beginInsertRows(QModelIndex(), i, i);
    m_lyricslist.insert(i,lyricsline);
    endInsertRows();
}

void LyricsModel::addItem(const LyricsLine &lyricsline)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_lyricslist << lyricsline;
    endInsertRows();
}

void LyricsModel::removeItem(int i)
{
    beginRemoveRows(QModelIndex(), i, i);
    m_lyricslist.removeAt(i);
    endRemoveRows();
}

void LyricsModel::clear()
{
    beginResetModel();
    m_lyricslist.clear();
    endResetModel();
}

int LyricsModel::rowCount(const QModelIndex & parent) const {
    return m_lyricslist.count();
}

QVariant LyricsModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() > m_lyricslist.count())
        return QVariant();

    const LyricsLine &lyricsline = m_lyricslist[index.row()];
    if (role == LineRole)
        return lyricsline.line();
    else if (role == StartRole)
        return lyricsline.start();
    return QVariant();
}

void LyricsModel::open(QString path,QString code) {
 clear();
 QFile file(path);
 if (file.open(QIODevice::ReadOnly)) {
  QString rcode=readcode(file.readAll());
  if (rcode=="") rcode=code;
  file.seek(0);
  QTextStream stream(&file);
  stream.setCodec(QTextCodec::codecForName(rcode.toAscii().constData()));
  //QList<LyricsLine> tlist;
  QString tline;
  while (!stream.atEnd()) {
   tline=stream.readLine();
   QStringList tlinelist=tline.split("]");
   for (int n=0; n<tlinelist.count()-1;n++) {
    if (tlinelist[n].indexOf("[") != -1) {
     double s=toTime(tlinelist[n].right(tlinelist[n].size()-tlinelist[n].indexOf("[")-1));
     if (s>=0) {
      int m;
      for (m=0; m<rowCount(); m++) {
       if (s<getItem(m).start()) {
        insertItem(m,LyricsLine(tlinelist[tlinelist.count()-1],s));
        //getItem(m-1).m_time=s-getItem(m-1).start();
        break;
       }
      }
      if (m==rowCount()) addItem(LyricsLine(tlinelist[tlinelist.count()-1],s));
      //if (m!=0) getItem(m-1).m_time=s-getItem(m-1).start();
     }
     //printf("%f %s\n",s,tlinelist[tlinelist.count()-1].toAscii().constData());
    }
   }
   //for (int n=0;n<tlist.rowCount()-1;n++) {
    //addItem(LyricsLine(tlist[n].line(),tlist[n].start(),tlist[n+1].start()-tlist[n].start());
   //}
  }
  if (rowCount()!=0&&getItem(0).start()!=0)
   insertItem(0,LyricsLine("",0));
  file.close();
 }
}

void LyricsModel::readlist(QStringList list) {
 clear();
 for (int i=0;i<list.count();i++) {
  QString tline=list[i];
  QStringList tlinelist=tline.split("]");
  for (int n=0; n<tlinelist.count()-1;n++) {
   if (tlinelist[n].indexOf("[") == 0) {
    double s=toTime(tlinelist[n].right(tlinelist[n].size()-tlinelist[n].indexOf("[")-1));
    if (s>=0) {
     int m;
     for (m=0; m<rowCount(); m++) {
      if (s<getItem(m).start()) {
       insertItem(m,LyricsLine(tlinelist[tlinelist.count()-1],s));
	//for (int c=0;c<tlinelist[tlinelist.count()-1].toAscii().size();c++) {
	//	printf("%d ",tlinelist[tlinelist.count()-1].toAscii().data()[c]);
	//}
	//printf("\n");
	//printf("b %s\n",tlinelist[tlinelist.count()-1].toAscii().data());
       //getItem(m-1).m_time=s-getItem(m-1).start();
       break;
      }
     }
     if (m==rowCount()) addItem(LyricsLine(tlinelist[tlinelist.count()-1],s));
	//printf("a %s\n",tlinelist[tlinelist.count()-1].toAscii().data());
     //if (m!=0) getItem(m-1).m_time=s-getItem(m-1).start();
    }
    //printf("%f %s\n",s,tlinelist[tlinelist.count()-1].toAscii().constData());
   }
  }
  //for (int n=0;n<tlist.rowCount()-1;n++) {
   //addItem(LyricsLine(tlist[n].line(),tlist[n].start(),tlist[n+1].start()-tlist[n].start());
  //}
 }
 if (rowCount()!=0&&getItem(0).start()!=0)
  insertItem(0,LyricsLine("",0));
}

QString LyricsModel::readcode(QByteArray t) {
 if (t.at(0)==0xff&&t.at(1)==0xfe) {
   return "UTF-16LE";
 }
 if (t.at(0)==0xfe&&t.at(1)==0xff) {
   return "UTF-16BE";
 }
 double u=Encoder::is_utf8_code(t);
 double g=Encoder::is_gbk_code(t);
 if (g>=u&&g>90) return "GBK";
 if (u>g&&u>90) return "UTF-8";
 if (t.at(0)==0) return "UTF-16BE";
 if (t.at(1)==0) return "UTF-16LE";
 return "";
}

double LyricsModel::toTime(QString tt) {
	QStringList tl=tt.split(":");
	for (int n=0;n<tl.count();n++) {
		for (int m=0;m<tl[n].size();m++) {
			if (tl[n].at(m)==47||tl[n].at(m)<46||tl[n].at(m)>57) return -1;
		}
	}
	if (tl.count()==1) return tl[0].toDouble();
	if (tl.count()==2) return tl[0].toDouble()*60+tl[1].toDouble();
	if (tl.count()==3) return tl[0].toDouble()*60*60+tl[1].toDouble()*60+tl[2].toDouble();
	return -1;
}
