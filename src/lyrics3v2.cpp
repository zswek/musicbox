#include "lyrics3v2.h"

bool Lyrics3v2::open(QString path) {
	QFile file(path);
	if (file.open(QIODevice::ReadOnly)) {
		qint64 sizepos=0;
		file.seek(file.size()-9);
		if (QString(file.read(9))=="LYRICS200") sizepos=file.size()-15;
		else {
			file.seek(file.size()-128-9);
			if (QString(file.read(9))=="LYRICS200") sizepos=file.size()-128-15;
		}
		if (sizepos!=0) {
		file.seek(sizepos);
		long sizeend=QString(file.read(6)).toLong();
		file.seek(sizepos-sizeend);
		if (QString(file.read(11))=="LYRICSBEGIN") {
		file.seek(sizepos-sizeend+25);
		size=QString(file.read(5)).toLong();
		lyrics=file.read(size);
		return true;
		}
		}
	}
	return false;
}

QStringList Lyrics3v2::lyricslist(QString code) {
	QString ls=QTextCodec::codecForName(code.toAscii().constData())->toUnicode(lyrics);
	QStringList ll=ls.split("\r\n");
	if (ll.count()<=1) ll=ls.split("\n");
	return ll;
}
