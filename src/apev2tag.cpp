#include "apev2tag.h"

APEv2Tag::APEv2Tag(qint64 tsize,char tflags[4],QString tid,QString tvalue) {
	size=tsize;
	flags[0]=tflags[0];
	flags[1]=tflags[1];
	flags[2]=tflags[2];
	flags[3]=tflags[3];
	id=tid;
	value=tvalue;
}

bool APEv2::open(QString path) {
 count=0;
 QFile file(path);
 if (file.open(QIODevice::ReadOnly)) {
  char csize[4];
  qint64 tagpos=0;
  qint64 tagend=file.size();
  file.seek(tagend-128);
  if (QString(file.read(3))=="TAG") {
   tagend-=128;
  }
  file.seek(tagend-9);
  if (QString(file.read(9))=="LYRICS200") {
   file.seek(tagend-9-6);
   long lsize=QString(file.read(6)).toLong();
   tagend-=9+6+lsize;
  }
  file.seek(tagend-32);
  if (QString(file.read(8))=="APETAGEX") {
   version[0]=*file.read(1).constData();
   version[1]=*file.read(1).constData();
   version[2]=*file.read(1).constData();
   version[3]=*file.read(1).constData();
   csize[0]=*file.read(1).constData();
   csize[1]=*file.read(1).constData();
   csize[2]=*file.read(1).constData();
   csize[3]=*file.read(1).constData();
   size=csize[0]+csize[1]*0x100+csize[2]*0x10000+csize[3]*0x1000000;
   char ccount[4];
   ccount[0]=*file.read(1).constData();
   ccount[1]=*file.read(1).constData();
   ccount[2]=*file.read(1).constData();
   ccount[3]=*file.read(1).constData();
   count=ccount[0]+ccount[1]*0x100+ccount[2]*0x10000+ccount[3]*0x1000000;
   flags[0]=*file.read(1).constData();
   flags[1]=*file.read(1).constData();
   flags[2]=*file.read(1).constData();
   flags[3]=*file.read(1).constData();
   if (tagend-size>=0) tagpos=tagend-size;
  }
  if (tagpos==0) {
   file.seek(tagpos);
   if (QString(file.read(3))=="ID3") {
    qint64 isize;
    char icsize[4];
    file.seek(tagpos+3+1+1+1);
    icsize[0]=*file.read(1).constData();
    icsize[1]=*file.read(1).constData();
    icsize[2]=*file.read(1).constData();
    icsize[3]=*file.read(1).constData();
    isize=icsize[0]*0x200000+icsize[1]*0x4000+icsize[2]*0x80+icsize[3];
    tagpos+=10+isize+32;
   }
  }
  if(tagpos!=0) file.seek(tagpos-32);
  if (QString(file.read(8))=="APETAGEX") {
   version[0]=*file.read(1).constData();
   version[1]=*file.read(1).constData();
   version[2]=*file.read(1).constData();
   version[3]=*file.read(1).constData();
   csize[0]=*file.read(1).constData();
   csize[1]=*file.read(1).constData();
   csize[2]=*file.read(1).constData();
   csize[3]=*file.read(1).constData();
   size=csize[0]+csize[1]*0x100+csize[2]*0x10000+csize[3]*0x1000000;
   char ccount[4];
   ccount[0]=*file.read(1).constData();
   ccount[1]=*file.read(1).constData();
   ccount[2]=*file.read(1).constData();
   ccount[3]=*file.read(1).constData();
   count=ccount[0]+ccount[1]*0x100+ccount[2]*0x10000+ccount[3]*0x1000000;
   flags[0]=*file.read(1).constData();
   flags[1]=*file.read(1).constData();
   flags[2]=*file.read(1).constData();
   flags[3]=*file.read(1).constData();
  }
  file.seek(tagpos);
  for (int n=0;n<count;n++) {
   if(file.pos()<=tagpos+size) {
    qint64 tagsize;
    char tagflags[4];
    QString tagid;
    QString tagvalue;
    char ttagsize[4];
    ttagsize[0]=*file.read(1).constData();
    ttagsize[1]=*file.read(1).constData();
    ttagsize[2]=*file.read(1).constData();
    ttagsize[3]=*file.read(1).constData();
    tagsize=ttagsize[0]+ttagsize[1]*0x100+ttagsize[2]*0x10000+ttagsize[3]*0x1000000;
    tagflags[0]=*file.read(1).constData();
    tagflags[1]=*file.read(1).constData();
    tagflags[2]=*file.read(1).constData();
    tagflags[3]=*file.read(1).constData();
    qint64 tmppos1=file.pos();
    while (!file.atEnd()) {
     if (*file.read(1).constData()==0){
      break;
     }
    }
    if (file.atEnd()) break;
    qint64 tmppos2=file.pos();
    file.seek(tmppos1);
    tagid=QString(file.read(tmppos2-tmppos1-1));
    file.seek(tmppos2);
    if (file.pos()+tagsize<=file.size()) {
     tagvalue=QTextCodec::codecForName("UTF-8")->toUnicode(file.read(tagsize));
     taglist.append(APEv2Tag(tagsize,tagflags,tagid,tagvalue));
    }
    else break;
   }
   else break;
  }
   return true;
  file.close();
 }
 return false;
}
