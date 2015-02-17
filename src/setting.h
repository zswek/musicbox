#ifndef SETTING_H
#define SETTING_H

#include <QFile>
#include <QIODevice>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QTranslator>

#define N_MODE 0
#define C_MODE 1
#define I_MODE 2

class Setting {
	public:
		Setting();
		bool load();
		bool save();
		QString appname;
		QString setpath;
		QString setfile;
		QString playlistpath;
		QString coverpath;
		QString defaultplaylist;
		QString lastopenplaylist;
		QString openpath;
		QString lyricsopenpath;
		QStringList musicmime;
		bool style;
		bool showstatusbar;
		QString orientationlock;
		bool showhiddenfile;
		int playmode;
		double forwardpos;
		QString listfontcolor;
		QString listhighlightfontcolor;
		QString lyricsfontcolor;
		QString lyricshighlightfontcolor;
		int lyricsfontsize;
		int lyricshighlightfontsize;
		QString lyricscode;
		QString id3v1code;
		QString lyrics3v2code;
};

#endif // SETTING_H
