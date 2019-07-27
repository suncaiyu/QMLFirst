import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import an.qt.ImageProcessor 1.0

Window {
    id:rootwindow;
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World");


    ImageProcessor{
        id:processor;
        onMyfinish:{
            switch(mtype){
            case ImageProcessor.HEIBAI:
                imageview.source = "file:///"+path;
                break;
            case ImageProcessor.HUIDU:
                imageview.source = "file:///"+path;
                break;
            default:
                break;
            }
        }
    }

    Component{
        id:buttonstyle;
        ButtonStyle {
            background: Rectangle {
                implicitWidth:(Window.width-23)/8;
                implicitHeight: toolbar.height/4*3;
                border.width: control.activeFocus ? 2 : 1
                border.color: "#888"
                radius: 4
                gradient: Gradient {
                    GradientStop { position: 0 ; color: control.pressed ? "#ccc" : "#eee" }
                    GradientStop { position: 1 ; color: control.pressed ? "#aaa" : "#ccc" }
                }
            }
        }
    }


//    ImageProcessor{
//        id:processor;
//    }

    ToolBar{
    id:toolbar;
    height:32;
    width: parent.width;
        RowLayout {
            spacing: 1;
                     ToolButton {
                         text: qsTr("open");
                         onClicked: filedialog.visible = true;
                         style:buttonstyle;
                     }
                     ToolButton{
                         text:"quit";
                         style: buttonstyle
                         onClicked: Qt.quit();
                     }
                     ToolButton{
                         text:"柔化";
                         style: buttonstyle
                     }
                     ToolButton{
                         text:"灰度";
                         style: buttonstyle
                         onClicked: {
                             processor.process(filedialog.fileUrl, ImageProcessor.HUIDU)
                         }
                     }
                     ToolButton{
                         text:"浮雕";
                         style: buttonstyle
                     }
                     ToolButton{
                         text:"黑白";
                         style: buttonstyle
                         onClicked: {
                             if(filedialog.fileUrl != ""){
                                 processor.process(filedialog.fileUrl, ImageProcessor.HEIBAI);
                             }
                         }
                     }
                     ToolButton{
                         text:"底片";
                         style: buttonstyle
                     }
                     ToolButton{
                         text:"锐化";
                         style: buttonstyle
                     }
                 }

    }

    FileDialog{
        id:filedialog;
        title:"选择图片";
        nameFilters: ["image file (*.jpg *.png *.gif *.bmp)"];
        onAccepted: {
            console.log(filedialog.fileUrl);
            imageview.source = filedialog.fileUrl;
        }
    }

    Rectangle{
        anchors.top:parent.top;
        anchors.topMargin: 32;
        color:"#DE3467";
        width: parent.width;
        height: parent.height-32;
        Image {
            width: parent.width;
            height: parent.height;
            fillMode:  Image.PreserveAspectFit;
            id: imageview;
            objectName: "imageview";
            asynchronous: true;
        anchors.centerIn: parent;
        }
    }
}
