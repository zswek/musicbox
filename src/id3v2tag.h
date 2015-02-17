#ifndef ID3V2TAG_H
#define ID3V2TAG_H

#include <QDir>
#include <QFile>
#include <QTemporaryFile>
#include <QList>
#include <QString>
#include <QTextCodec>

class ID3v2Tag {
	public:
		ID3v2Tag(QString, qint64, char[], char[], QByteArray);
		QString readtag();
		QString encodetag();
		bool readpic(QString);
		QStringList readlt();
		QString id;
		qint64 size;
		char flag[2];
		char encode[4];
		QByteArray tag;
		qint64 startpos;
		qint64 stoppos;
};

class ID3v2 {
	public:
		bool open(QString);
		int version;
		int revision;
		char flag;
		qint64 size;
		QList<ID3v2Tag> taglist;
};

#endif // ID3V2TAG_H
