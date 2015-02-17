import QtQuick 1.1
import com.nokia.meego 1.0

Page {
	id: pathpage
	anchors.fill: parent
	orientationLock: mplayer.set_orientationlock=="automatic"?PageOrientation.Automatic:(mplayer.set_orientationlock=="locklandscape"?PageOrientation.LockLandscape:PageOrientation.LockPortrait)
	tools:pathtoolbar

	Component {
		id: pathdelegate

		Rectangle {
			id: filebutton
			width: pathpage.width
			height: 70
			color:  pathmousearea.pressed?Qt.rgba(192, 192, 192, 0.5):Qt.rgba(192, 192, 192, 0)

			Row {
				anchors.fill: parent

				Image {
					anchors.verticalCenter: parent.verticalCenter
					source: isdir?"image://theme/icon-m-common-directory":(theme.inverted?"image://theme/icon-m-content-audio-inverse":"image://theme/icon-m-content-audio")
				}
				Label {
					id: nametext
					anchors.verticalCenter: parent.verticalCenter
					font.pointSize: 30
//					color: mplayer.set_fontcolor
					text: name
				}
			}
			MouseArea {
				id: pathmousearea
				anchors.fill: parent
				onClicked: {
					if (isdir) {
						mplayer.pathin_slots(name)
					}
					else {
						filecheck.checked=(!filecheck.checked)
						mplayer.filestatus_slots(pathview.indexAt(mouseX,mouseY+filebutton.y),filecheck.checked);
					}
				}
				onPressAndHold: {
					if (isdir) {
						pathmenu.index=pathview.indexAt(mouseX,mouseY+filebutton.y)
						pathmenu.open()
					}
				}
			}
			CheckBox {
				id: filecheck
				visible: !isdir
				anchors.right: parent.right
				anchors.verticalCenter: parent.verticalCenter

				onClicked: {
					mplayer.filestatus_slots(pathview.indexAt(pathmousearea.mouseX,pathmousearea.mouseY+filebutton.y),checked);
				}
			}
                }
	}
	ListView{
		id: pathview
		anchors.fill: parent
		model: pathmodel
		delegate: pathdelegate
		
	}
	ScrollDecorator {
		flickableItem:pathview
	}
	ContextMenu {
		id: pathmenu
		property int index

		MenuLayout {
			MenuItem {
				text: "扫描文件夹"

				onClicked: {
					mplayer.seleted_folder_slots(pathmenu.index,false)
					pageStack.pop()
				}
			}
			MenuItem {
				text: "扫描文件夹和子文件夹"

				onClicked: {
					mplayer.seleted_folder_slots(pathmenu.index,true)
					pageStack.pop()
				}
			}
		}
	}
	ToolBarLayout {
		id: pathtoolbar
		visible: false

		ToolIcon { iconId: "toolbar-back"; onClicked: {pageStack.pop()} }
		ToolIcon { iconId: "toolbar-done"; onClicked: {mplayer.seleted_done_slots();pageStack.pop()}}
		ToolIcon { iconId: "toolbar-up"; onClicked:mplayer.pathup_slots()}
        }
}
