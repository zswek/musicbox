#include "mplayer.h"

QString settime(double ss) {
	QString time;
	if (ss < 60) {
		time.setNum(ss);
		return time + "s";
	}
	int ints = (int)ss;
	QString m,s;
	s.setNum(ints % 60);
	if (ss < 60*60) {
		m.setNum((int)(ints / 60));
		time = m + ":" + s;
	}
	else {
		QString h;
		m.setNum((int)((ints / 60)) % 60);
		h.setNum((int)(ints / (60 *60)));
		time = h + ":" + m + ":" + s;
	}
	return time;
}

Mplayer::Mplayer() {
	setting.load();
	process = new QProcess();
	process->setProcessChannelMode(QProcess::MergedChannels);
	connect(this->process,SIGNAL(readyReadStandardOutput()),this,SLOT(back_message_slots()));
	connect(this->process,SIGNAL(finished(int)),this,SLOT(playexit_slots(int)));
	lyricsmodel = new LyricsModel();
	playlistmodel = new PlayListModel();
	playlistlistmodel = new PlayListListModel();
	pathmodel = new PathModel();
	lyricspathmodel = new LyricsPathModel();
	status = CLOSE_STATUS;
	setPlayStatus(false);
	now_pos = 0;
	set_pos = 0;
	length_pos = 0;
	start_pos = 0;
	stop_pos = 0;
	setNowPos(0);
	setLenght(0);
	setPlayIndex(-1);
	setLyricsIndex(-1);
	pathmodel->openPath(setting.openpath,setting.musicmime,setting.showhiddenfile);
	lyricspathmodel->openPath(setting.lyricsopenpath,setting.showhiddenfile);
	setLyricsLine(0);
	setImagePath("");
	setListTag("");
	setLyricsTag("");
	setTrackTag("");
	setID3v1Tag("");
	setID3v2Tag("");
	setAPEv2Tag("");
	stopsig = false;
	nosig = false;
	timer = new QTimer(this);
	connect(timer,SIGNAL(timeout()),this,SLOT(get_time_slots()));
	timer->start(100);
	if (!QFileInfo(setting.setpath).isDir()) {
		QDir().mkdir(setting.setpath);
	}
	if (!QFileInfo(setting.playlistpath).isDir()) {
		QDir().mkdir(setting.playlistpath);
	}
	if (!QFileInfo(setting.coverpath).isDir()) {
		QDir().mkdir(setting.coverpath);
	}
	QStringList playlistlist = QDir(setting.playlistpath).entryList();
	int lastindex=-1;
	for (int n=0; n<playlistlist.count();n++ ) {
		if (QFileInfo(setting.playlistpath+"/"+playlistlist[n]).isFile()) {
			if (playlistlist[n]==setting.lastopenplaylist) {
				lastindex=playlistlistmodel->rowCount();
			}
			playlistlistmodel->addItem(PlayListListItem(playlistlist[n]));
		}
	}
	if (playlistlistmodel->rowCount() == 0 ) {
		setting.lastopenplaylist=setting.defaultplaylist;
		playlistlistmodel->addItem(PlayListListItem(setting.lastopenplaylist));
		setPlayListIndex(0);
	}
	else {
		if (setting.lastopenplaylist!="" && lastindex!=-1) {
			playlistmodel->openList(setting.playlistpath+"/"+setting.lastopenplaylist);
			setPlayListIndex(lastindex);
		}
		else {
			setting.lastopenplaylist=playlistlistmodel->getItem(0).name();
			playlistmodel->openList(setting.playlistpath+"/"+playlistlistmodel->getItem(0).name());
			setPlayListIndex(0);
		}
	}
}

int Mplayer::open(char *name) {
	if (status == CLOSE_STATUS) {
		path = name;
		status = OPEN_STATUS;
		setPlayStatus(false);
		now_pos = 0;
		set_pos = 0;
		return 0;
	}
	return -1;
}

int Mplayer::open(QString name) {
	if (status == CLOSE_STATUS) {
		path = name;
		status = OPEN_STATUS;
		setPlayStatus(false);
		now_pos = 0;
		set_pos = 0;
		setNowPos(0);
		setLenght(0);
		return 0;
	}
	return -1;
}

int Mplayer::close() {
	if (status == OPEN_STATUS) {
		status = CLOSE_STATUS;
		setPlayStatus(false);
		now_pos = 0;
		set_pos = 0;
		length_pos = 0;
		setNowPos(0);
		setLenght(0);
		lyricsmodel->clear();
		setImagePath("");
		return 0;
	}
	if (status == PLAY_STATUS || status == PAUSE_STATUS) {
		if (stop() == 0) return close();
		return -1;
	}
	return -1;
}

int Mplayer::play() {
	if (status == OPEN_STATUS) {
		QString start;
		start.setNum(start_pos);
		QString common = "mplayer -slave -quiet -vo null -fs -ss "+ start + " \"" + this->path + "\"";
		process->start(common);
		status = PLAY_STATUS;
		setPlayStatus(true);
		return 0;
	}
	if (status == PAUSE_STATUS) {
		status = PLAY_STATUS;
		setPlayStatus(true);
		process->write("pause\n");
		seek(set_pos);
		return 0;
	}
	return -1;
}

int Mplayer::stop() {
	if (status == PLAY_STATUS || status == PAUSE_STATUS) {
		stopsig = true;
		process->write("quit\n");
		status = OPEN_STATUS;
		setPlayStatus(false);
		now_pos = 0;
		set_pos = 0;
		setNowPos(0);
		close();
		return 0;
	}
	return -1;
}

int Mplayer::pause() {
	if (status == PLAY_STATUS) {
		process->write("pause\n");
		setNowPos(now_pos-start_pos);
		set_pos = now_pos;
		status = PAUSE_STATUS;
		setPlayStatus(false);
		return 0;
	}
	return -1;
}

int Mplayer::seek(double pos) {
	if (status == PLAY_STATUS) {
		QString spos;
		spos.setNum(pos);
		QString common = "seek " + spos + " 2 \n";
		process->write(common.toAscii().constData());
		now_pos = pos;
		setNowPos(pos-start_pos);
		return 0;
	}
	if (status == PAUSE_STATUS || status == OPEN_STATUS) {
		set_pos = pos;
		setNowPos(pos-start_pos);
		return 0;
	}
	return -1;
}

int Mplayer::get_pos() {
	return now_pos;
}

int Mplayer::get_length() {
	return length_pos;
}

void Mplayer::add_slots(QString name, QString path, QString lyrics, QString track, double start, double stop, QString artist, QString album) {
	playlistmodel->addItem(PlayListItem(name, path, lyrics, track, start, stop, artist, album));
}

void Mplayer::remove_slots(int i) {
	if (i == m_playindex) {
		playlistmodel->removeItem(i);
		int tmpindex = m_playindex;
		stopsig = true;
		process->close();
		lyricsmodel->clear();
		setImagePath("");
		if (playlistmodel->rowCount() == 0) {
			status = OPEN_STATUS;
			close();
			setPlayTitle("");
			setPlayIndex(-1);
		}
		else {
			if (tmpindex >= playlistmodel->rowCount()) {
				tmpindex = playlistmodel->rowCount()-1;
			}
			status = CLOSE_STATUS;
			setPlayIndex(tmpindex);
			open_slots(playlistmodel->getItem(tmpindex).name(), playlistmodel->getItem(tmpindex).path(), playlistmodel->getItem(tmpindex).lyrics(), playlistmodel->getItem(tmpindex).start(), playlistmodel->getItem(tmpindex).stop());
			play_slots();
		}
	}
	else {
		int tmpindex = m_playindex;
		playlistmodel->removeItem(i);
		if (i < m_playindex) {
			setPlayIndex(tmpindex-1);
		}
	}
}

void Mplayer::clear_slots() {
	playlistmodel->clear();
	lyricsmodel->clear();
	setPlayIndex(-1);
	process->close();
	status = CLOSE_STATUS;
	setPlayStatus(false);
	now_pos = 0;
	set_pos = 0;
	length_pos = 0;
	setNowPos(0);
	setLenght(0);
	setPlayTitle("");
	setImagePath("");
}

void Mplayer::open_slots(QString name, QString path, QString lyrics, double start, double stop) {
	//close();
	if (status != CLOSE_STATUS) {
		stopsig = true;
		process->close();
		status = CLOSE_STATUS;
	}
	start_pos=start;
	stop_pos=stop;
	setPlayTitle(name);
	open(path);
	lyricsmodel->clear();
	setImagePath("");
	if (lyrics!=""&&QFileInfo(lyrics).isFile()) {
		lyricsmodel->open(lyrics,setting.lyricscode);
	}
	if (lyricsmodel->rowCount()<=0) {
		Lyrics3v2 lyrics3v2;
		if (lyrics3v2.open(path)) {
			QString code=lyricsmodel->readcode(lyrics3v2.lyrics);
			if (code=="") code=setting.lyrics3v2code;
			lyricsmodel->readlist(lyrics3v2.lyricslist(code.toAscii().constData()));
		}
	}
	ID3v2 id3v2;
	if (id3v2.open(path)) {
		for (int n=0;n<id3v2.taglist.count();n++) {
			if (id3v2.taglist[n].id=="USLT"&&lyricsmodel->rowCount()<=0) {
				lyricsmodel->readlist(id3v2.taglist[n].readlt());
			}
			if (id3v2.taglist[n].id=="APIC"||id3v2.taglist[n].id=="PIC") {
				id3v2.taglist[n].readpic(setting.coverpath+"/"+name);
				setImagePath(setting.coverpath+"/"+name);
			}
		}
	}
	if (lyricsmodel->rowCount()<=0&&start_pos==0&&stop_pos==0) {
		lyricsmodel->open(playlistmodel->lyrics_search_slots(path),setting.lyricscode);
	}
	if (lyricsmodel->rowCount()<=0) {
		lyricsmodel->open(path.left(path.lastIndexOf("/"))+"/"+name+".lrc",setting.lyricscode);
	}
}

void Mplayer::close_slots() {
	close();
}

void Mplayer::play_slots() {
	play();
}

void Mplayer::pause_slots() {
	pause();
}

void Mplayer::seek_slots(double pos) {
	nosig=true;
	setNowPos(pos);
	seek(pos+start_pos);
	updatelyrics_slots(pos);
	nosig=false;
}

void Mplayer::playexit_slots(int exitCode) {
	process->close();
	status = OPEN_STATUS;
	if (!stopsig && m_playindex >= 0) {
		if (setting.playmode == N_MODE) {
			if (m_playindex < playlistmodel->rowCount()-1) {
				close();
				open_slots(playlistmodel->getItem(m_playindex+1).name(), playlistmodel->getItem(m_playindex+1).path(), playlistmodel->getItem(m_playindex+1).lyrics(), playlistmodel->getItem(m_playindex+1).start(), playlistmodel->getItem(m_playindex+1).stop());
				play_slots();
				setPlayIndex(m_playindex+1);
			}
			else {
				close();
				setPlayTitle("");
				setPlayIndex(-1);
			}
		}
		if (setting.playmode == C_MODE) {
			if (m_playindex < playlistmodel->rowCount()-1) {
				close();
				open_slots(playlistmodel->getItem(m_playindex+1).name(), playlistmodel->getItem(m_playindex+1).path(), playlistmodel->getItem(m_playindex+1).lyrics(), playlistmodel->getItem(m_playindex+1).start(), playlistmodel->getItem(m_playindex+1).stop());
				play_slots();
				setPlayIndex(m_playindex+1);
			}
			else {
				close();
				open_slots(playlistmodel->getItem(0).name(), playlistmodel->getItem(0).path(), playlistmodel->getItem(0).lyrics(), playlistmodel->getItem(0).start(), playlistmodel->getItem(0).stop());
				play_slots();
				setPlayIndex(0);
			}
		}
		if (setting.playmode == I_MODE) {
			play_slots();
		}
	}
	else {
		stopsig = false;
	}
}

void Mplayer::backwards_slots() {
	if (now_pos-setting.forwardpos>start_pos) {
		seek(now_pos-setting.forwardpos);
	}
	else seek(start_pos);
}

void Mplayer::forward_slots() {
	if (stop_pos!=0&&(now_pos+setting.forwardpos)>=stop_pos) {
		seek(stop_pos);
	}
	else seek(now_pos+setting.forwardpos);
}

void Mplayer::previous_slots() {
	if (m_playindex >= 0) {
		if (m_playindex > 0 && playlistmodel->rowCount() > 0) {
			int tmpindex = m_playindex;
			stopsig = true;
			process->close();
			status = CLOSE_STATUS;
			setPlayIndex(tmpindex-1);
			open_slots(playlistmodel->getItem(tmpindex-1).name(), playlistmodel->getItem(tmpindex-1).path(), playlistmodel->getItem(tmpindex-1).lyrics(), playlistmodel->getItem(tmpindex-1).start(), playlistmodel->getItem(tmpindex-1).stop());
			play_slots();
		}
		else {//if (setting.playmode == C_MODE) {
			stopsig = true;
			process->close();
			status = CLOSE_STATUS;
			setPlayIndex(playlistmodel->rowCount()-1);
			open_slots(playlistmodel->getItem(playlistmodel->rowCount()-1).name(), playlistmodel->getItem(playlistmodel->rowCount()-1).path(), playlistmodel->getItem(playlistmodel->rowCount()-1).lyrics(), playlistmodel->getItem(playlistmodel->rowCount()-1).start(), playlistmodel->getItem(playlistmodel->rowCount()-1).stop());
			play_slots();
		}
	}
}

void Mplayer::next_slots() {
	if (m_playindex >= 0) {
		if (m_playindex >= 0 && m_playindex < playlistmodel->rowCount()-1) {
			int tmpindex = m_playindex;
			stopsig = true;
			process->close();
			status = CLOSE_STATUS;
			setPlayIndex(tmpindex+1);
			open_slots(playlistmodel->getItem(tmpindex+1).name(), playlistmodel->getItem(tmpindex+1).path(), playlistmodel->getItem(tmpindex+1).lyrics(), playlistmodel->getItem(tmpindex+1).start(), playlistmodel->getItem(tmpindex+1).stop());
			play_slots();
		}
		else {//if (setting.playmode == C_MODE) {
			stopsig = true;
			process->close();
			status = CLOSE_STATUS;
			setPlayIndex(0);
			open_slots(playlistmodel->getItem(0).name(), playlistmodel->getItem(0).path(), playlistmodel->getItem(0).lyrics(), playlistmodel->getItem(0).start(), playlistmodel->getItem(0).stop());
			play_slots();
		}
	}
}

void Mplayer::back_message_slots() {
	while(process->canReadLine()) {
		QString message(this->process->readLine());
		QStringList message_list = message.split("=");
		if(message_list[0] == "ANS_TIME_POSITION") {
			if (!nosig) {
			now_pos = message_list[1].toDouble();
			if (now_pos<start_pos) seek(start_pos);
			if (stop_pos==0||now_pos<stop_pos) {
			setNowPos(now_pos-start_pos);
			updatelyrics_slots(now_pos-start_pos);
			}
			else {
			stopsig = true;
			process->close();
			status = OPEN_STATUS;
		if (setting.playmode == N_MODE) {
			if (m_playindex < playlistmodel->rowCount()-1) {
				close();
				open_slots(playlistmodel->getItem(m_playindex+1).name(), playlistmodel->getItem(m_playindex+1).path(), playlistmodel->getItem(m_playindex+1).lyrics(), playlistmodel->getItem(m_playindex+1).start(), playlistmodel->getItem(m_playindex+1).stop());
				play_slots();
				setPlayIndex(m_playindex+1);
			}
			else {
				close();
				setPlayTitle("");
				setPlayIndex(-1);
			}
		}
		if (setting.playmode == C_MODE) {
			if (m_playindex < playlistmodel->rowCount()-1) {
				close();
				open_slots(playlistmodel->getItem(m_playindex+1).name(), playlistmodel->getItem(m_playindex+1).path(), playlistmodel->getItem(m_playindex+1).lyrics(), playlistmodel->getItem(m_playindex+1).start(), playlistmodel->getItem(m_playindex+1).stop());
				play_slots();
				setPlayIndex(m_playindex+1);
			}
			else {
				close();
				open_slots(playlistmodel->getItem(0).name(), playlistmodel->getItem(0).path(), playlistmodel->getItem(0).lyrics(), playlistmodel->getItem(0).start(), playlistmodel->getItem(0).stop());
				play_slots();
				setPlayIndex(0);
			}
		}
		if (setting.playmode == I_MODE) {
			play_slots();
			}
			}
			}
		}
		if(message_list[0] == "ANS_LENGTH") {
			length_pos = message_list[1].toDouble();
			if (stop_pos==0) {
				setLenght(length_pos-start_pos);
			}
			else {
				setLenght(stop_pos-start_pos);
			}
		}
	}
}

void Mplayer::get_time_slots() {
	if (status == PLAY_STATUS) {
		process->write("get_time_pos\n");
		process->write("get_time_length\n");
	}
}

QString Mplayer::settime_slots(int ss) {
	return settime(ss);
}

void Mplayer::set_playmode_slots(int n) {
	setPlayMode(n);
}

void Mplayer::new_playlist_slots(QString name) {
	QFile file(setting.playlistpath+"/"+name);
	file.open(QIODevice::WriteOnly|QIODevice::Truncate);
	file.close();
}

void Mplayer::open_playlist_slots(QString path) {
	playlistmodel->openList(path);
}

void Mplayer::open_playlist_slots(int i) {
	playlistmodel->saveList(setting.playlistpath+"/"+setting.lastopenplaylist);
	if (i != m_playlistindex) {
		clear_slots();
		setting.lastopenplaylist=playlistlistmodel->getItem(i).name();
		playlistmodel->openList(setting.playlistpath+"/"+setting.lastopenplaylist);
		setPlayListIndex(i);
	}
}

void Mplayer::save_playlist_slots(QString path) {
	playlistmodel->saveList(path);
}

void Mplayer::remove_playlist_slots(int i) {
	if (m_playlistindex==i) {
		clear_slots();
		if (i < playlistlistmodel->rowCount()-1) {
			setting.lastopenplaylist=playlistlistmodel->getItem(i+1).name();
		}
		else if (playlistlistmodel->rowCount()==1) {
			setting.lastopenplaylist=setting.defaultplaylist;
		}
		else {
			setting.lastopenplaylist=playlistlistmodel->getItem(i-1).name();
		}
		lyricsmodel->clear();
		playlistmodel->openList(setting.playlistpath+"/"+setting.lastopenplaylist);
	}
	QFile::remove(setting.playlistpath+"/"+playlistlistmodel->getItem(i).name());
	update_playlist_slots();
}

void Mplayer::update_playlist_slots() {
	playlistmodel->saveList(setting.playlistpath+"/"+setting.lastopenplaylist);
	playlistlistmodel->clear();
	QStringList playlistlist = QDir(setting.playlistpath).entryList();
	int lastindex=-1;
	for (int n=0; n<playlistlist.count();n++ ) {
		if (QFileInfo(setting.playlistpath+"/"+playlistlist[n]).isFile()) {
			if (playlistlist[n]==setting.lastopenplaylist) {
				lastindex=playlistlistmodel->rowCount();
			}
			playlistlistmodel->addItem(PlayListListItem(playlistlist[n]));
		}
	}
	setPlayListIndex(lastindex);
}

void Mplayer::updatelyrics_slots(double ss) {
	int n;
	for (n=0; n<lyricsmodel->rowCount();n++) {
		if (ss<lyricsmodel->getItem(n).start()) {
			break;
		}
	}
	setLyricsIndex(n-1);
	if(n!=0) {
		if (n!=lyricsmodel->rowCount()) {
			setLyricsLine((ss-lyricsmodel->getItem(n-1).start())/(lyricsmodel->getItem(n).start()-lyricsmodel->getItem(n-1).start()));
		}
		else {
			setLyricsLine((ss-lyricsmodel->getItem(n-1).start())/(length_pos-lyricsmodel->getItem(n-1).start()));
		}
	}
}

double Mplayer::lyricsseek1_slots(double n,double t) {
	//nosig=true;
	if ((int)(n/t) < lyricsmodel->rowCount()-1) {
		return (lyricsmodel->getItem((int)(n/t)).start()+(lyricsmodel->getItem((int)(n/t)+1).start()-lyricsmodel->getItem((int)(n/t)).start())*((n/t)-(int)(n/t)));
	}
	else {
		return (lyricsmodel->getItem((int)(n/t)).start()+(length_pos-lyricsmodel->getItem((int)(n/t)).start())*((n/t)-(int)(n/t)));
	}
}

double Mplayer::lyricsseek2_slots(double n,double t,double h) {
	//printf("2:%f %f %f %d\n",n,t,h,m_lyricsindex);
	//nosig=true;
	if (m_lyricsindex < lyricsmodel->rowCount()-1) {
		//printf ("%f\n",(lyricsmodel->getItem(m_lyricsindex).start()+(lyricsmodel->getItem(m_lyricsindex+1).start()-lyricsmodel->getItem(m_lyricsindex).start())*(((n-t*m_lyricsindex)/h))));
		return (lyricsmodel->getItem(m_lyricsindex).start()+(lyricsmodel->getItem(m_lyricsindex+1).start()-lyricsmodel->getItem(m_lyricsindex).start())*(((n-t*m_lyricsindex)/h)));
	}
	else {
		return (lyricsmodel->getItem(m_lyricsindex).start()+(length_pos-lyricsmodel->getItem(m_lyricsindex).start())*((n-t*m_lyricsindex)/h));
	}
	//printf("%d %f %f\n",(int)n,t,lyricsmodel->getItem((int)(n/t)).start());
}

double Mplayer::lyricsseek3_slots(double n,double t,double h) {
	//printf("3:%f %f %f\n",n,t,h);
	//nosig=true;
	if ((int)((n-h)/t)+1 < lyricsmodel->rowCount()-1) {
		return (lyricsmodel->getItem((int)((n-h)/t)+1).start()+(lyricsmodel->getItem((int)((n-h)/t)+2).start()-lyricsmodel->getItem((int)((n-h)/t)+1).start())*(((n-h)/t)-(int)((n-h)/t)));
	}
	else {
		return (lyricsmodel->getItem((int)((n-h)/t)+1).start()+(length_pos-lyricsmodel->getItem((int)((n-h)/t)+1).start())*(((n-h)/t)-(int)((n-h)/t)));
	}
	//printf("%d %f %f\n",(int)n,t,lyricsmodel->getItem((int)(n/t)).start());
}

void Mplayer::sleep_slots() {
	sleep(0.1);
}

void Mplayer::filestatus_slots(int n,bool v) {
	if (n>=pathmodel->filestart) {
		pathmodel->m_fileseleted[n-pathmodel->filestart]=v;
	}
}

void Mplayer::pathup_slots() {
	if (pathmodel->path!="/") {
		if (pathmodel->path.lastIndexOf("/")!=0) {
			pathmodel->openPath(pathmodel->path.left(pathmodel->path.lastIndexOf("/")),setting.musicmime,setting.showhiddenfile);
		}
		else {
			pathmodel->openPath("/",setting.musicmime,setting.showhiddenfile);
		}
	}
	setting.openpath=pathmodel->path;
}

void Mplayer::pathin_slots(QString name) {
	if (pathmodel->path=="/") {
		pathmodel->openPath("/"+name,setting.musicmime,setting.showhiddenfile);
	}
	else {
		pathmodel->openPath(pathmodel->path+"/"+name,setting.musicmime,setting.showhiddenfile);
	}
	setting.openpath=pathmodel->path;
}

void Mplayer::seleted_done_slots() {
	for (int n=pathmodel->filestart; n<pathmodel->rowCount(); n++) {
		if (pathmodel->m_fileseleted[n-pathmodel->filestart]) {
			QString name=pathmodel->getItem(n).name();
			playlistmodel->addMusic(pathmodel->path+"/"+name,setting.id3v1code);
		}
	}
	pathmodel->openPath(pathmodel->path,setting.musicmime,setting.showhiddenfile);
}

void Mplayer::seleted_folder_slots(int n,bool c) {
	seleted_folder_slots(pathmodel->path+"/"+pathmodel->getItem(n).name(),c);
}

void Mplayer::seleted_folder_slots(QString path,bool c) {
	QStringList filelist;
	if (setting.showhiddenfile) filelist=QDir(path).entryList(QDir::Dirs|QDir::Files|QDir::Hidden|QDir::Readable);
	else filelist=QDir(path).entryList(QDir::Dirs|QDir::Files|QDir::Readable);
	for (int n=0; n<filelist.count(); n++) {
		if (filelist[n]!="."&&filelist[n]!="..") {
			if (QFileInfo(path+"/"+filelist[n]).isDir()) {
				if (c) seleted_folder_slots(path+"/"+filelist[n],c);
			}
			else {
				if (pathmodel->isMime(filelist[n],setting.musicmime)) {
					playlistmodel->addMusic(path+"/"+filelist[n],setting.id3v1code);
				}
			}
		}
	}
}

void Mplayer::lyricspathup_slots() {
	if (lyricspathmodel->path!="/") {
		if (lyricspathmodel->path.lastIndexOf("/")!=0) {
			lyricspathmodel->openPath(lyricspathmodel->path.left(lyricspathmodel->path.lastIndexOf("/")),setting.showhiddenfile);
		}
		else {
			lyricspathmodel->openPath("/",setting.showhiddenfile);
		}
	}
	setting.lyricsopenpath=lyricspathmodel->path;
}

void Mplayer::lyricspathin_slots(QString name) {
	if (lyricspathmodel->path=="/") {
		lyricspathmodel->openPath("/"+name,setting.showhiddenfile);
	}
	else {
		lyricspathmodel->openPath(lyricspathmodel->path+"/"+name,setting.showhiddenfile);
	}
	setting.lyricsopenpath=lyricspathmodel->path;
}

void Mplayer::lyricsseleted_slots(QString name) {
	int index=m_playindex;
	PlayListItem item=playlistmodel->getItem(index);
	playlistmodel->removeItem(index);
	playlistmodel->insertItem(index,PlayListItem(item.name(), item.path(), lyricspathmodel->path+"/"+name, item.track(), item.start(), item.stop(), item.artist(), item.album()));
	setPlayIndex(index);
	lyricsmodel->clear();
	lyricsmodel->open(lyricspathmodel->path+"/"+name,setting.lyricscode);
	updatelyrics_slots(now_pos);
}

void Mplayer::show_tag_slots(int n) {
	setListTag("");
	setLyricsTag("");
	setTrackTag("");
	setID3v1Tag("");
	setID3v2Tag("");
	QString listtext="";
	listtext += QString::fromUtf8("标题: ") + playlistmodel->getItem(n).name() + "\n";
	if (playlistmodel->getItem(n).artist()!="") {
		listtext += QString::fromUtf8("歌手: ") + playlistmodel->getItem(n).artist() + "\n";
	}
	if (playlistmodel->getItem(n).album()!="") {
		listtext += QString::fromUtf8("专辑: ") + playlistmodel->getItem(n).album() + "\n";
	}
	if (playlistmodel->getItem(n).start()!=0||playlistmodel->getItem(n).stop()!=0) {
		listtext += QString::fromUtf8("开始时间: ") + settime(playlistmodel->getItem(n).start()) + "\n";
	}
	if (playlistmodel->getItem(n).stop()!=0) {
		listtext += QString::fromUtf8("结束时间: ") + settime(playlistmodel->getItem(n).stop()) + "\n";
	}
	listtext += QString::fromUtf8("路径: ") + playlistmodel->getItem(n).path() + "\n";
	if (QFileInfo(playlistmodel->getItem(n).lyrics()).isFile()) {
		listtext += QString::fromUtf8("歌词路径: ") + playlistmodel->getItem(n).lyrics() + "\n";
		QString lyricstext="";
		QFile lyricsfile(playlistmodel->getItem(n).lyrics());
		if (lyricsfile.open(QIODevice::ReadOnly)) {
			QTextStream lyricsstream(&lyricsfile);
			lyricsstream.setCodec(QTextCodec::codecForName("GBK"));
			QString lyricsline;
			while (!lyricsstream.atEnd()) {
				lyricsline=lyricsstream.readLine();
				QStringList lyricslinelist=lyricsline.split("]");
				if (lyricslinelist[0].startsWith("[ti:")) {
					lyricstext += QString::fromUtf8("标题: ") + lyricslinelist[0].mid(4,lyricslinelist[0].count()-4) + "\n";
				}
				if (lyricslinelist[0].startsWith("[ar:")) {
					lyricstext += QString::fromUtf8("歌手: ") + lyricslinelist[0].mid(4,lyricslinelist[0].count()-4) + "\n";
				}
				if (lyricslinelist[0].startsWith("[al:")) {
					lyricstext += QString::fromUtf8("专辑: ") + lyricslinelist[0].mid(4,lyricslinelist[0].count()-4) + "\n";
				}
				if (lyricslinelist[0].startsWith("[by:")) {
					lyricstext += QString::fromUtf8("来自: ") + lyricslinelist[0].mid(4,lyricslinelist[0].count()-4) + "\n";
				}
			}
			lyricsfile.close();
			setLyricsTag(lyricstext);
		}
	}
	if (QFileInfo(playlistmodel->getItem(n).track()).isFile()) {
		listtext += QString::fromUtf8("音轨路径: ") + playlistmodel->getItem(n).track() + "\n";
		QString tracktext="";
		QFile trackfile(playlistmodel->getItem(n).track());
		if (trackfile.open(QIODevice::ReadOnly)) {
			QTextStream trackstream(&trackfile);
			trackstream.setCodec(QTextCodec::codecForName("GBK") );
			QString trackline;
			while (!trackstream.atEnd()) {
				trackline=trackstream.readLine();
				QStringList ttracklinelist=trackline.split("\"");
				QStringList tracklinelist=ttracklinelist[0].trimmed().split(" ");
				if (ttracklinelist.count()>1) {
					tracklinelist.append(ttracklinelist[1]);
				}
				if (tracklinelist[0].toUpper()=="FILE") {
					tracktext += QString::fromUtf8("歌曲文件: ") + tracklinelist[1] + "\n";
				}
				if (tracklinelist[0].toUpper()=="TITLE") {
					tracktext += QString::fromUtf8("标题: ") + tracklinelist[1] + "\n";
				}
				if (tracklinelist[0].toUpper()=="PERFORMER") {
					tracktext += QString::fromUtf8("歌手: ") + tracklinelist[1] + "\n";
				}
				if (tracklinelist[0].toUpper()=="TRACK") {
					break;
				}
			}
			trackfile.close();
			setTrackTag(tracktext);
		}
	}
	setListTag(listtext);
	QStringList id3v1=playlistmodel->readID3v1(playlistmodel->getItem(n).path(),setting.id3v1code);
	if(id3v1.count()!=0) {
		QString id3v1text="";
		id3v1text += QString::fromUtf8("歌曲名: ") + id3v1[0] + "\n";
		id3v1text += QString::fromUtf8("歌手名: ") + id3v1[1] + "\n";
		id3v1text += QString::fromUtf8("专辑名: ") + id3v1[2] + "\n";
		id3v1text += QString::fromUtf8("年份: ") + id3v1[3] + "\n";
		id3v1text += QString::fromUtf8("注释: ") + id3v1[4] + "\n";
		if (id3v1.count()==7) {
			id3v1text += QString::fromUtf8("音轨: ") + id3v1[5] + "\n";
		}
		id3v1text += QString::fromUtf8("风格: ") + id3v1[id3v1.count()-1] + "\n";
		setID3v1Tag(id3v1text);
	}
	ID3v2 id3v2;
	if (id3v2.open(playlistmodel->getItem(n).path())) {
		QString id3v2text="";
		for (int i=0;i<id3v2.taglist.count();i++) {
			if (id3v2.taglist[i].id!="") {
				if (id3v2.taglist[i].id=="APIC"||id3v2.taglist[i].id=="PIC") {
					id3v2.taglist[i].readpic(setting.coverpath+"/"+playlistmodel->getItem(n).name());
					id3v2text += id3v2.taglist[i].id + ": <br><img src=\"file://" + setting.coverpath+"/"+playlistmodel->getItem(n).name() + "\"/><br>";
				}
				else {
					id3v2text += id3v2.taglist[i].id + ": " + id3v2.taglist[i].readtag() + "<br>";
				}
			}
		}
		setID3v2Tag(id3v2text);
	}
	APEv2 apev2;
	if (apev2.open(playlistmodel->getItem(n).path())) {
		QString apev2text="";
		for (int i=0;i<apev2.taglist.count();i++) {
			if (apev2.taglist[i].id!="") {
				apev2text += apev2.taglist[i].id + ": " + apev2.taglist[i].value + "\n";
			}
		}
		setAPEv2Tag(apev2text);
	}
}
