import QtQuick 1.1
import com.nokia.meego 1.0

Item {
	id: lyricsbox
	anchors.fill: parent

	Label {
		visible:lyricslist.count==0
		anchors.horizontalCenter: parent.horizontalCenter
		anchors.verticalCenter: parent.verticalCenter
		color: theme.inverted?"white":"black"
		text: mplayer.playindex<0?"歌词":"未找到歌词"
	}
	Flickable {
		id: lyricsview
		anchors.fill: parent
		//currentIndex: mplayer.playindex
		//model: lyricsmodel
		//delegate: lyricsdelegate
		clip: true
		//contentWidth: lyricslist.width
		//contentHeight: lyricslist.height

		Component {
			id: lyricsdelegate

			Rectangle {
				id: lyricslinebox
				visible: true
				width: lyricslist.width
				height: lyricsline.height
				color: Qt.rgba(192, 192, 192, 0)
				Text {
					id: lyricsline
					//height: 25
					anchors.horizontalCenter: parent.horizontalCenter
					color: mplayer.set_lyricsfontcolor
					font.pointSize:lyricslinebox.ListView.isCurrentItem?mplayer.set_lyricshighlightfontsize:mplayer.set_lyricsfontsize
					//text: ((lyricsbox.height/2)>(lyricslist.y+lyricslinebox.y) && (lyricsbox.height/2)<(lyricslist.y+lyricslinebox.y+lyricslinebox.height))?"<b>"+line+"</b>":line
					text:lyricslinebox.ListView.isCurrentItem?"<b>"+line+"</b>":line
				}
				Rectangle {
					width: lyricsline.width
					anchors.horizontalCenter: parent.horizontalCenter
					color: Qt.rgba(192, 192, 192, 0)
				Text {
					visible: lyricslinebox.ListView.isCurrentItem
					width: lyricsline.width*mplayer.lyricsline
					//anchors.horizontalCenter: parent.horizontalCenter
					font.pointSize:mplayer.set_lyricshighlightfontsize
					color: mplayer.set_lyricshighlightfontcolor
					clip: true
					//text: ((lyricsbox.height/2)>(lyricslist.y+lyricslinebox.y) && (lyricsbox.height/2)<(lyricslist.y+lyricslinebox.y+lyricslinebox.height))?"<b>"+line+"</b>":line
					text:lyricslinebox.ListView.isCurrentItem?"<b>"+line+"</b>":line
				}
				}
			}
		}
		ListView {
			id: lyricslist
//			anchors.fill: parent
			x: 0
			y:lyricsmousearea.pressed ?lyricsmousearea.lyricslistY : (mplayer.lyricsindex>0?lyricsview.height/2-testtext.height*mplayer.lyricsindex-highlighttesttext.height*mplayer.lyricsline:lyricsview.height/2)
			currentIndex: mplayer.lyricsindex
			clip: false
			width: lyricsview.width
			height: testtext.height*lyricslist.count+highlighttesttext.height
			model: lyricsmodel
			delegate: lyricsdelegate
		}
		Text {
			id: testtext
			visible: false
			color: mplayer.set_lyricsfontcolor
			font.pointSize:mplayer.set_lyricsfontsize
			text: "test"
		}
//		Label {text:testtext.height+" "+highlighttesttext.height}
		Text {
			id: highlighttesttext
			visible: false
			color: mplayer.set_lyricshighlightfontcolor
			font.pointSize:mplayer.set_lyricshighlightfontsize
			text: "<b>test</b>"
		}
	}
	Rectangle {
		id: lyricsline
		visible: lyricsmousearea.pressed&&lyricslist.count!=0
		x: 0
		y: lyricsview.height/2
		color: theme.inverted?"white":"black"
		width: lyricsview.width
		height: 1
	}
	Text {
		id: lyricstimertext
		visible: lyricsmousearea.pressed&&lyricslist.count!=0
		color: theme.inverted?"white":"black"
		anchors.right: parent.right
		anchors.top: lyricsline.top
		font.pointSize:10
	}
	MouseArea {
		id: lyricsmousearea
		anchors.fill: parent
		visible: lyricslist.count!=0

		property int lastPressY
		property int lyricslistY
		onPressed: {
			lastPressY=mouseY
			lyricslistY=lyricslist.y
			if (lyricslistY>(lyricsview.height/2-testtext.height*mplayer.lyricsindex)) {
				lyricstimertext.text=mplayer.settime_slots(mplayer.lyricsseek1_slots((lyricsview.height/2-lyricslistY),testtext.height));
			}
			else if (lyricslistY>(lyricsview.height/2-testtext.height*mplayer.lyricsindex-highlighttesttext.height)) {
				lyricstimertext.text=mplayer.settime_slots(mplayer.lyricsseek2_slots((lyricsview.height/2-lyricslistY),testtext.height,highlighttesttext.height))
			}
			else {
				lyricstimertext.text=mplayer.settime_slots(mplayer.lyricsseek3_slots((lyricsview.height/2-lyricslistY),testtext.height,highlighttesttext.height));
			}
		}
		onPositionChanged: {
			lyricslistY=lyricslistY+mouseY-lastPressY
			lastPressY=mouseY
			if(lyricslistY>lyricsview.height/2) lyricslistY=lyricsview.height/2;
			else {
				if(mplayer.lyricsindex>=0) {
					if (lyricslistY<=(lyricsview.height/2-(testtext.height*(lyricslist.count-1)+highlighttesttext.height))) lyricslistY=lyricsview.height/2-testtext.height*(lyricslist.count-1)-highlighttesttext.height+1
				}
				else {
					if (lyricslistY<=(lyricsview.height/2-testtext.height*(lyricslist.count-1))) lyricslistY=lyricsview.height/2-testtext.height*(lyricslist.count-1)
				}
			}
			if (lyricslistY>(lyricsview.height/2-testtext.height*mplayer.lyricsindex)) {
				lyricstimertext.text=mplayer.settime_slots(mplayer.lyricsseek1_slots((lyricsview.height/2-lyricslistY),testtext.height));
			}
			else if (lyricslistY>(lyricsview.height/2-testtext.height*mplayer.lyricsindex-highlighttesttext.height)) {
				lyricstimertext.text=mplayer.settime_slots(mplayer.lyricsseek2_slots((lyricsview.height/2-lyricslistY),testtext.height,highlighttesttext.height))
			}
			else {
				lyricstimertext.text=mplayer.settime_slots(mplayer.lyricsseek3_slots((lyricsview.height/2-lyricslistY),testtext.height,highlighttesttext.height));
			}
		}
		onReleased: {
			if (lyricslistY>(lyricsview.height/2-testtext.height*mplayer.lyricsindex)) {
				mplayer.seek_slots(mplayer.lyricsseek1_slots((lyricsview.height/2-lyricslistY),testtext.height));
			}
			else if (lyricslistY>(lyricsview.height/2-testtext.height*mplayer.lyricsindex-highlighttesttext.height)) {
				mplayer.seek_slots(mplayer.lyricsseek2_slots((lyricsview.height/2-lyricslistY),testtext.height,highlighttesttext.height))
			}
			else {
				mplayer.seek_slots(mplayer.lyricsseek3_slots((lyricsview.height/2-lyricslistY),testtext.height,highlighttesttext.height));
			}
		}
	}
}
