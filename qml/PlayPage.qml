import QtQuick 1.1
import com.nokia.meego 1.0

Page {
	id: playpage
	orientationLock: mplayer.set_orientationlock=="automatic"?PageOrientation.Automatic:(mplayer.set_orientationlock=="locklandscape"?PageOrientation.LockLandscape:PageOrientation.LockPortrait)
	tools: playtoolbar

	PathPage {
		id: pathpage
	}
	LyricsPathPage {
		id: lyricspathpage
	}
	Item {
		id: playbox
		anchors.fill: parent

		Column {
			Item {
				id: playnamebox
				width: playbox.width
				height: playname.height
				Label {
					id: playname
					anchors.horizontalCenter: playnamebox.horizontalCenter
//					color: mplayer.set_fontcolor
					text: mplayer.playtitle
				}
			}
			Item {
				id: playerbox
				width: playbox.width
				height: playbox.height-playname.height-playbuttonfill.height-sliderbox.height

				PlayList {
					id: playlist
				}
				LyricsView {
					id: lyricsview
					visible: false
				}
				Image {
					id: musicimage
					visible: false
					anchors.fill: parent
					source: mplayer.imagepath==""?"":"file://"+mplayer.imagepath

					Label {
						visible: mplayer.imagepath==""
						anchors.horizontalCenter: parent.horizontalCenter
						anchors.verticalCenter: parent.verticalCenter
						color: theme.inverted?"white":"black"
						text: mplayer.playindex<0?"专辑图":"无专辑图"
					}
				}
			}
			Item {
				id: playbuttonfill
				width: playbox.width
				height: playbuttonbox1.height+playbuttonbox2.height

				Column {
					width: playbox.width-50
					height: playbuttonbox1.height+playbuttonbox2.height
					anchors.horizontalCenter: playbuttonfill.horizontalCenter

				Item {
					id:playbuttonbox2
					width: playbox.width-50
					height: playmodebutton.height

					ToolIcon {
						id: viewbutton
						iconId: playlist.visible ? "toolbar-list" : (lyricsview.visible?"toolbar-grid":"toolbar-gallery")
						anchors.left: playbuttonbox2.left

						onClicked: {
							if (playlist.visible) {playlist.visible=!playlist.visible;lyricsview.visible=!lyricsview.visible}
							else if (lyricsview.visible) {lyricsview.visible=!lyricsview.visible;musicimage.visible=!musicimage.visible}
							else if (musicimage.visible) {musicimage.visible=!musicimage.visible;playlist.visible=!playlist.visible}
						}
					}
					ToolIcon {
						id: playmodebutton
						iconId: mplayer.set_playmode==0 ? "toolbar-shuffle" : (mplayer.set_playmode==1 ? "toolbar-repeat" : (theme.inverted?"toolbar-jump-to":"toolbar-jump-to-dimmed-white"))
						anchors.horizontalCenter: parent.horizontalCenter

						onClicked: {
							if (mplayer.set_playmode==2) mplayer.set_playmode=0
							else mplayer.set_playmode+=1
						}
					}
					ToolIcon {
						visible: (playlist.visible||mplayer.playindex>=0)&&!musicimage.visible
						iconId: playlist.visible?"toolbar-add":"toolbar-attachment"
						anchors.right: parent.right 
						onClicked: {
							if (playlist.visible) pageStack.push(pathpage);
							else pageStack.push(lyricspathpage);
						}
					}
				}
				Item {
					id: playbuttonbox1
					width: playbox.width-50
					height: playbutton.height
					//anchors.horizontalCenter: playbuttonfill.horizontalCenter

					ToolIcon {
						id: previousbutton
						iconId: "toolbar-mediacontrol-previous"
						anchors.left: playbuttonbox1.left

						onClicked: {
							mplayer.previous_slots()
						}
					}
					ToolIcon {
						id: backwardsbutton
						//visible: false
						iconId: "toolbar-mediacontrol-backwards"
						x: (playbutton.x+previousbutton.x)/2

						onClicked: {
							mplayer.backwards_slots()
						}
					}
					ToolIcon {
						id: playbutton
						iconId: mplayer.playstatus?"toolbar-mediacontrol-pause":"toolbar-mediacontrol-play"
						anchors.horizontalCenter: playbuttonbox1.horizontalCenter

						onClicked: {
							if(mplayer.playstatus) mplayer.pause_slots()
							else mplayer.play_slots()
						}
					}
					ToolIcon {
						id: forwardbutton
						//visible: false
						iconId: "toolbar-mediacontrol-forward"
						x: (playbutton.x+nextbutton.x)/2

						onClicked: {
							mplayer.forward_slots()
						}
					}
					ToolIcon {
						id: nextbutton
						iconId: "toolbar-mediacontrol-next"
						anchors.right: playbuttonbox1.right

						onClicked: {
							mplayer.next_slots()
						}
					}
				}
				}
			}
			Column {
				id: sliderbox
				width: playbox.width

				Slider {
					id: playslider
					width: sliderbox.width
					minimumValue: 0
					maximumValue: mplayer.lenght
					value: playslider.pressed?playslider.value:mplayer.nowpos
					valueIndicatorText: mplayer.settime_slots(playslider.value)
					valueIndicatorVisible: true

					onPressedChanged: {
					if (!pressed) mplayer.seek_slots(playslider.value)
					}
				}
				Item {
					id: slideritem
					width: sliderbox.width
					height: playtime.height

					Label {
						id: playnowtime
						anchors.left: slideritem.left
//						color: mplayer.set_fontcolor
						text: mplayer.settime_slots(mplayer.nowpos)
					}
					Label {
						id: playtime
						anchors.right: slideritem.right
//						color: mplayer.set_fontcolor
						text: mplayer.settime_slots(mplayer.lenght)
					}
				}
			}
		}
	}
	PlayListPage {
		id: playlistpage
	}
	ToolBarLayout {
		id: playtoolbar
		visible: false
//		ToolIcon { iconId: "toolbar-back"; onClicked: { mplayer.add_slots("a","/home/user/MyDocs/a.mp3");mplayer.add_slots("c","/home/user/MyDocs/Musics/a.ape");}}//mplayer.add_slots("d","");mplayer.add_slots("e","")} }
//		ToolIcon { iconId: "toolbar-back"; onClicked: { mplayer.open_playlist_slots("a");} }
		ToolIcon {
			iconId: "toolbar-tag"
			anchors.right: parent.right

			onClicked: {
				mplayer.update_playlist_slots()
				pageStack.push(playlistpage)
			}
		}
	}
}
