#ifndef LYRICS3V2_H
#define LYRICS3V2_H

#include <QFile>
#include <QString>
#include <QStringList>
#include <QTextCodec>

class Lyrics3v2 {
	public:
		bool open(QString);
		QStringList lyricslist(QString);
		long size;
		QByteArray lyrics;
};

#endif // LYRICS3V2_H
