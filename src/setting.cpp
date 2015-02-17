#include "setting.h"

Setting::Setting() {
	appname = "musicbox";
	setpath = "/home/user/." + appname;
	setfile = setpath + "/config";
	playlistpath = setpath + "/list";
	coverpath = setpath + "/cover";
	defaultplaylist = QString::fromUtf8("默认列表");
	lastopenplaylist = "";
	playmode = N_MODE;
	forwardpos = 10;
	openpath = "/home/user/MyDocs";
	lyricsopenpath = "/home/user/MyDocs";
	musicmime
		<< "aac" << "ac3" << "aif" << "aifc" << "aiff" << "amr" << "ape" << "au"
		<< "cue"
		<< "flac"
		<< "gsm"
		<< "kar"
		<< "m3u" << "m4a" << "mid" << "midi" << "mp2" << "mp3" << "mp+" << "mpc" << "mpega" << "mpga" << "mpp"
		<< "oga" << "ogg"
		<< "pls"
		<< "ra" << "ram" << "rm"
		<< "sd2" << "sid" << "snd"
		<< "wax" << "wav" << "wma";
	style = false;
	showstatusbar = false;
	orientationlock = "lockportrait";
	showhiddenfile = false;
	listfontcolor = "black";
	listhighlightfontcolor = "red";
	lyricsfontcolor = "black";
	lyricshighlightfontcolor = "red";
	lyricsfontsize=20;
	lyricshighlightfontsize=20;
	lyricscode = "GBK";
	id3v1code = "GBK";
	lyrics3v2code = "GBK";
}

bool Setting::load() {
	QFile file(setfile);
	if (file.open(QIODevice::ReadOnly)) {
		QTextStream stream(&file);
		while (!stream.atEnd()) {
			QString line=stream.readLine();
			if (line.left(line.indexOf("=")).toLower()=="playlist") {
				lastopenplaylist = line.right(line.size()-line.indexOf("=")-1);
			}
			else if (line.left(line.indexOf("=")).toLower()=="playmode") {
				if (line.right(line.size()-line.indexOf("=")-1).toLower()=="n") playmode=N_MODE;
				else if (line.right(line.size()-line.indexOf("=")-1).toLower()=="c") playmode=C_MODE;
				else if (line.right(line.size()-line.indexOf("=")-1).toLower()=="i") playmode=I_MODE;
			}
			else if (line.left(line.indexOf("=")).toLower()=="forwardpos") {
				forwardpos = line.right(line.size()-line.indexOf("=")-1).toDouble();
			}
			else if (line.left(line.indexOf("=")).toLower()=="path") {
				openpath = line.right(line.size()-line.indexOf("=")-1);
			}
			else if (line.left(line.indexOf("=")).toLower()=="lyricspath") {
				lyricsopenpath = line.right(line.size()-line.indexOf("=")-1);
			}
			else if (line.left(line.indexOf("=")).toLower()=="style") {
				style = (line.right(line.size()-line.indexOf("=")-1).toLower()=="dark");
			}
			else if (line.left(line.indexOf("=")).toLower()=="musicmime") {
				musicmime = line.right(line.size()-line.indexOf("=")-1).split(" ");
			}
			else if (line.left(line.indexOf("=")).toLower()=="showstatusbar") {
				showstatusbar = (line.right(line.size()-line.indexOf("=")-1).toLower()=="yes");
			}
			else if (line.left(line.indexOf("=")).toLower()=="orientationlock") {
				orientationlock = line.right(line.size()-line.indexOf("=")-1).toLower();
			}
			else if (line.left(line.indexOf("=")).toLower()=="showhiddenfile") {
				showhiddenfile = (line.right(line.size()-line.indexOf("=")-1).toLower()=="yes");
			}
			else if (line.left(line.indexOf("=")).toLower()=="listfontcolor") {
				listfontcolor = line.right(line.size()-line.indexOf("=")-1);
			}
			else if (line.left(line.indexOf("=")).toLower()=="listhighlightfontcolor") {
				listhighlightfontcolor = line.right(line.size()-line.indexOf("=")-1);
			}
			else if (line.left(line.indexOf("=")).toLower()=="lyricsfontcolor") {
				lyricsfontcolor = line.right(line.size()-line.indexOf("=")-1);
			}
			else if (line.left(line.indexOf("=")).toLower()=="lyricshighlightfontcolor") {
				lyricshighlightfontcolor = line.right(line.size()-line.indexOf("=")-1);
			}
			else if (line.left(line.indexOf("=")).toLower()=="lyricsfontsize") {
				lyricsfontsize = line.right(line.size()-line.indexOf("=")-1).toInt();
			}
			else if (line.left(line.indexOf("=")).toLower()=="lyricshighlightfontsize") {
				lyricshighlightfontsize = line.right(line.size()-line.indexOf("=")-1).toInt();
			}
			else if (line.left(line.indexOf("=")).toLower()=="lyricscode") {
				lyricscode = line.right(line.size()-line.indexOf("=")-1);
			}
			else if (line.left(line.indexOf("=")).toLower()=="id3v1code") {
				id3v1code = line.right(line.size()-line.indexOf("=")-1);
			}
			else if (line.left(line.indexOf("=")).toLower()=="lyrics3v2code") {
				lyrics3v2code = line.right(line.size()-line.indexOf("=")-1);
			}
		}
		return true;
	}
	return false;
}

bool Setting::save() {
	QFile file(setfile);
	if (file.open(QIODevice::WriteOnly)) {
		QTextStream stream(&file);
		stream << "PlayList=" << lastopenplaylist << "\n";
		stream << "PlayMode=";
		if (playmode==N_MODE) stream << "n";
		else if (playmode==C_MODE) stream << "c";
		else if (playmode==I_MODE) stream << "i";
		stream << "\n";
		QString pos;
		pos.setNum(forwardpos);
		stream << "ForwardPos=" << pos << "\n";
		stream << "Path=" << openpath << "\n";
		stream << "LyricsPath=" << lyricsopenpath << "\n";
		stream << "MusicMime=";
		for (int n=0;n<musicmime.count();n++) {
			if (n!=0) stream << " ";
			stream << musicmime[n];
		}
		stream << "\n";
		stream << "Style=";
		if (style) stream << "dark";
		else stream << "light";
		stream << "\n";
		stream << "ShowStatusBar=";
		if (showstatusbar) stream << "yes";
		else stream << "no";
		stream << "\n";
		stream << "OrientationLock=" << orientationlock << "\n";
		stream << "ShowHiddenFile=";
		if (showhiddenfile) stream << "yes";
		else stream << "no";
		stream << "\n";
		stream << "ListFontColor=" << listfontcolor << "\n";
		stream << "ListHighlightFontColor=" << listhighlightfontcolor << "\n";
		stream << "LyricsFontColor=" << lyricsfontcolor << "\n";
		stream << "LyricsHighlightFontColor=" << lyricshighlightfontcolor << "\n";
		QString fontsize;
		fontsize.setNum(lyricsfontsize);
		stream << "LyricsFontSize=" << fontsize << "\n";
		fontsize.setNum(lyricshighlightfontsize);
		stream << "LyricsHighlightFontSize=" << fontsize << "\n";
		stream << "LyricsCode=" << lyricscode << "\n";
		stream << "ID3v1Code=" << id3v1code << "\n";
		stream << "Lyrics3v2Code=" << lyrics3v2code << "\n";
		file.close();
		return true;
	}
	return false;
}
