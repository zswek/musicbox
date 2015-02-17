#include "id3v2tag.h"

ID3v2Tag::ID3v2Tag(QString tid, qint64 tsize,char tflag[2], char tencode[4], QByteArray ttag) {
	id=tid;
	size=tsize;
	flag[0]=tflag[0];
	flag[1]=tflag[1];
	encode[0]=tencode[0];
	encode[1]=tencode[1];
	encode[2]=tencode[2];
	encode[3]=tencode[3];
	tag=ttag;
	startpos=0;
	stoppos=tag.size()-1;
}

QString ID3v2Tag::readtag() {
	if (id=="APIC"||id=="PIC") {
			return QString("");
	}
	else {
		return QTextCodec::codecForName(encodetag().toAscii().constData())->toUnicode(tag.mid(startpos,stoppos-startpos+1));
	}
}

QString ID3v2Tag::encodetag() {
	QString code="GBK";
	startpos=1;
	if (tag.at(0)==0) code="GBK";
	else if (tag.at(0)==1) code="UTF-16LE";
	else if (tag.at(0)==2) code="UTF-16BE";
	else if (tag.at(0)==3) code="UTF-8";
	else startpos=0;
	for (;startpos<tag.size();startpos++) {
		if (tag.at(startpos)==0&&tag.at(startpos+1)==0) {
			startpos++;
		}
		else if (tag.at(startpos)==0xff&&tag.at(startpos+1)==0xfe) {
			code="UTF-16LE";
			startpos++;
		}
		else if (tag.at(startpos)==0xfe&&tag.at(startpos+1)==0xff) {
			code="UTF-16BE";
			startpos++;
		}
		else if (QString(tag.mid(startpos,3))=="eng") {
			code="UTF-8";
			startpos+=2;
		}
		else if (QString(tag.mid(startpos,3))=="chi") {
			code="GBK";
			startpos+=2;
		}
		else break;
	}
	if (id.count()==3) {
		for (;stoppos>0;stoppos--) {
			if (tag.at(stoppos)!=0) break;
		}
	}
	return code;
}

bool ID3v2Tag::readpic(QString path) {
	if (id=="APIC") {
		int n;
		for (n=0;n<tag.count()-5;n++) {
			if (QString(tag.mid(n,5))=="image") {
				break;
			}
		}
		if (n>=tag.count()-5) return false;
		for (;n<tag.count();n++) {
			if (tag.at(n)==0) {
				break;
			}
		}
		if (n>=tag.count()) return false;
		if (tag.at(n+1)!=0||tag.at(n+2)!=0) {
			for (n+=3;n<tag.count();n++) {
				if (tag.at(n)==0) {
					break;
				}
			}
			if (n>=tag.count()) return false;
			n++;
		}
		else n+=3;
		for (;n<tag.count();n++) {
			if (tag.at(n)!=0) {
				break;
			}
		}
		QFile file(path);
		if (file.open(QIODevice::WriteOnly)) {
			file.write(tag.mid(n,tag.size()-n));
			file.close();
			return true;
		}
	}
	else if (id=="PIC") {
		QFile file(path);
		if (file.open(QIODevice::WriteOnly)) {
			file.write(tag.mid(6,tag.size()-6));
			file.close();
			return true;
		}
	}
	return false;
}

QStringList ID3v2Tag::readlt() {
	QStringList ltlist;
	if (id=="USLT") {
		QString code=encodetag();
		ltlist=QTextCodec::codecForName(code.toAscii().constData())->toUnicode(tag.mid(startpos,stoppos-startpos+1)).split("\r\n");
		if (ltlist.count()<=1) ltlist=QTextCodec::codecForName(code.toAscii().constData())->toUnicode(tag.mid(startpos,stoppos-startpos+1)).split("\n");
		//printf("%s\n",code.toAscii().data());
		//QFile file("a.lrc");
		//file.open(QIODevice::WriteOnly);
		//file.write(tag.mid(startpos,tag.size()-startpos));
		//file.close();
		//printf ("%d\n",ltlist.count());
		//int tp=0;
		//for (int p=startpos;p<tag.size();p++) {
		//	if(tag.at(p)==0xd&&tag.at(p+1)==0x0&&tag.at(p+2)==0xa&&tag.at(p+3)==0x0) {
		//		ltlist.append(QTextCodec::codecForName(code.toAscii().constData())->toUnicode(tag.mid(tp,p-tp)));
		//		tp=p+4;
		//		p+=3;
		//	}
		//}
	}
	return ltlist;
}

bool ID3v2::open(QString path) {
 QFile file(path);
 if (file.open(QIODevice::ReadOnly)) {
  if (QString(file.read(3))=="ID3") {
   version=(int)*file.read(1).constData();
   revision=(int)*file.read(1).constData();
   flag=*file.read(1).constData();
   char csize[4];
   csize[0]=*file.read(1).constData();
   csize[1]=*file.read(1).constData();
   csize[2]=*file.read(1).constData();
   csize[3]=*file.read(1).constData();
   size=csize[0]*0x200000+csize[1]*0x4000+csize[2]*0x80+csize[3];
   while (file.pos()<size+10) {
    QString id;
    if (version>2) id=QString(file.read(4));
    else id=QString(file.read(3));
    char fcsize[4];
    fcsize[0]=*file.read(1).constData();
    fcsize[1]=*file.read(1).constData();
    fcsize[2]=*file.read(1).constData();
    qint64 fsize;
    char flag[2];
    if (version>2) {
     fcsize[3]=*file.read(1).constData();
     fsize=fcsize[0]*0x1000000+fcsize[1]*0x10000+fcsize[2]*0x100+fcsize[3];
     flag[0]=*file.read(1).constData();
     flag[1]=*file.read(1).constData();
    }
    else fsize=fcsize[0]*0x10000+fcsize[1]*0x100+fcsize[2];
    char encode[4];
    encode[0]=*file.read(1).constData();
    encode[1]=*file.read(1).constData();
    encode[2]=*file.read(1).constData();
    encode[3]=*file.read(1).constData();
    file.seek(file.pos()-4);
    if (file.pos()+fsize<=size+10 && fsize!=0) {
     QByteArray tag=file.read(fsize);
     taglist.append(ID3v2Tag(id,fsize,flag,encode,tag));
     //printf("%s %s\n", id.toAscii().data(), tag.data());
    }
    else break;
   }
   return true;
  }
 }
 return false;
}
