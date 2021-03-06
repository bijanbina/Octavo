import QtQuick 2.5
import QtQuick.Window 2.0
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4
import Qt.labs.settings 1.0

Window {
    id:page
    visible: true
    property alias miheight:page.width
    color:"#fff"
    minimumHeight: 600
    minimumWidth: 360
    //minimumHeight: 565
    //minimumWidth: 360

    //Properties:
    property string ipAddress: "192.168.1.101"
    property string message
    property int  lamp_con_id
    property int  lamp_count : 0
    property real scale_x : width/minimumWidth
    property real scale_y : height/620
    property bool isConnected : false

    property real setting_red_value
    property real setting_green_value
    property real setting_blue_value

    Settings {
        property alias setting_red_value: settings_redSlider.value
        property alias setting_green_value: settings_greenSlider.value
        property alias setting_blue_value: settings_blueSlider.value
    }

    signal set_lamp(int id);
    signal change_color(int id,int value);
    signal change_color2(string value);
    signal music_random;
    signal music_play;
    signal music_stop;
    signal music_forward;
    signal music_prev;
    signal light_off;
    signal light_on;
    signal rainbow;



    ///////////////////////////////////////////////////
    //----------------MAIN WINDOW--------------------//
    ///////////////////////////////////////////////////
    Rectangle
    {
        anchors.fill: parent
        id: main_window
        Image {
            id: back_image;
            anchors.fill: parent
            source: "qrc:Resources/Screens/start.jpg"
        }

        MouseArea
        {
            onClicked: {
                setting_window.visible = true;
                main_window.visible = false;
                setting_window.forceActiveFocus()
            }
            id: setting_button
            x: 0 * scale_x
            y: 95 * scale_y
            width: 80 * scale_x
            height: 265 * scale_y
        }


        /*Rectangle
        {
            color: "#00000000"
            border.color: "#00f"
            id: setting_rec
            x: 0 * scale_x
            y: 95 * scale_y
            width: 80 * scale_x
            height: 265 * scale_y
        }*/

        MouseArea
        {
            onClicked: {
                info_window.visible = true;
                main_window.visible = false;
                info_window.forceActiveFocus()
            }
            id: info_button
            x: 85 * scale_x
            y: 165 * scale_y
            width: 95 * scale_x
            height: 105 * scale_y
        }

        MouseArea
        {
            onClicked: {
                lamp_window.visible = true;
                main_window.visible = false;
                lamp_window.forceActiveFocus()
            }
            id: lamp_button
            x: 185 * scale_x
            y: 165 * scale_y
            width: 100 * scale_x
            height: 105 * scale_y

        }

        MouseArea
        {
            onClicked: {
                screen_window.visible = true;
                main_window.visible = false;
                screen_window.forceActiveFocus()
            }
            id: screen_button
            x: 85 * scale_x
            y: 270 * scale_y
            width: 95 * scale_x
            height: 100 * scale_y

        }

        MouseArea
        {
            id: puzzle_button
            x: 185 * scale_x
            y: 270 * scale_y
            width: 100 * scale_x
            height: 100 * scale_y
            onClicked: {
                puzzle_window.visible = true;
                main_window.visible = false;
                puzzle_window.forceActiveFocus()
            }

        }

        MouseArea
        {
            onClicked: page.close()
            id: quit_button
            x: 85 * scale_x
            y: 375 * scale_y
            width: 195 * scale_x
            height: 80 * scale_y
        }

        Keys.onReleased:
        {
            if (event.key === Qt.Key_Back || event.key=== Qt.Key_Backspace)
            {
                event.accepted=true;
                page.close();
            }
        }
    }

    ///////////////////////////////////////////////////
    //----------------LAMP WINDOW--------------------//
    ///////////////////////////////////////////////////
    Rectangle
    {
        visible: false;
        anchors.fill: parent
        id: lamp_window
        Image {
            id: back_image_lamp;
            anchors.fill: parent
            source: "qrc:Resources/Screens/lamp-window.png"
        }

        MouseArea
        {
            id: select_button
            x: 100 * scale_x
            y: 385 * scale_y
            width: 215 * scale_x
            height: 30 * scale_y
            //border.color: "#00f"
            //color: "#00000000"

            //onClicked: music_play()
        }

        MouseArea
        {
            id: stop_button
            x: 100 * scale_y
            y: 425 * scale_y
            width: 34 * scale_y
            height: 45 * scale_y
            onClicked: music_stop()
        }

        MouseArea
        {
            id: prev_button
            x: 134 * scale_y
            y: 425 * scale_y
            width: 34 * scale_y
            height: 45 * scale_y
            onClicked: music_prev()
        }

        MouseArea
        {
            id: play_button
            x: 168 * scale_y
            y: 425 * scale_y
            width: 33 * scale_y
            height: 45 * scale_y
            onClicked: music_play()
        }

        MouseArea
        {
            id: forward_button
            x: 201 * scale_y
            y: 425 * scale_y
            width: 37 * scale_y
            height: 45 * scale_y
            onClicked: music_forward()
        }

        MouseArea
        {
            id: random_button
            x: 238 * scale_y
            y: 425 * scale_y
            width: 38 * scale_y
            height: 45 * scale_y
            //border.color: "#00f"
            //color: "#00000000"
            onClicked: music_random()
        }

        MouseArea
        {
            id: lamp1_button
            x: 0 * scale_x
            y: 120 * scale_y
            width: 125 * scale_x
            height: 105 * scale_y
            onClicked: set_lamp(0)
        }

        MouseArea
        {
            id: lamp2_button
            x: 245 * scale_x
            y: 100 * scale_y
            width: 115 * scale_x
            height: 105 * scale_y
            onClicked: set_lamp(1)
        }

        MouseArea
        {
            id: lamp3_button
            x: 0 * scale_x
            y: 450 * scale_y
            width: 125 * scale_x
            height: 110 * scale_y
            onClicked: set_lamp(2)
        }

        MouseArea
        {
            id: lamp4_button
            x: 245 * scale_x
            y: 495 * scale_y
            width: 115 * scale_x
            height: 100 * scale_y
            onClicked: set_lamp(3)
        }

        MouseArea
        {

            //color: "#00000000"
            //border.color: "#00f"
            x: 120 * scale_x
            y: 0 * scale_y
            width: 120 * scale_x
            height: 100 * scale_y
            id: rainbow_button
            onClicked: rainbow();
        }

        Image
        {
            id: lamp1_on
            anchors.fill: parent
            source: "qrc:Resources/Lamp/on-tl.png"
            z:2
            visible: true
        }

        Image
        {
            id: lamp2_on
            anchors.fill: parent
            source: "qrc:Resources/Lamp/on-tr.png"
            z:2
            visible: false
        }

        Image
        {
            id: lamp3_on
            anchors.fill: parent
            source: "qrc:Resources/Lamp/on-bl.png"
            z:2
            visible: false
        }

        Image
        {
            id: lamp4_on
            anchors.fill: parent
            source: "qrc:Resources/Lamp/on-br.png"
            z:2
            visible: false
        }

        Image
        {
            id: lamp1_off
            anchors.fill: parent
            source: "qrc:Resources/Lamp/off-tl.png"
            visible: true
        }

        Image
        {
            id: lamp2_off
            anchors.fill: parent
            source: "qrc:Resources/Lamp/off-tr.png"
            visible: true
        }

        Image
        {
            id: lamp3_off
            anchors.fill: parent
            source: "qrc:Resources/Lamp/off-bl.png"
            visible: true
        }

        Image
        {
            id: lamp4_off
            anchors.fill: parent
            source: "qrc:Resources/Lamp/off-br.png"
            visible: false
        }


        Image
        {
            id: color_button
            x: 40 * scale_x
            y: 270 * scale_y
            width: 45 * scale_x
            height: 45 * scale_y
            source: "qrc:/Resources/Button/color-off.png"
            MouseArea
            {
                id: colorbtn_mouseArea
                anchors.fill: parent
                onClicked:
                {
                    if (isConnected)
                    {
                        if (color_button.source == "qrc:/Resources/Button/color-off.png")
                        {
                            color_button.source = "qrc:/Resources/Button/color-on.png";
                            light_on();

                        }
                        else
                        {
                            color_button.source = "qrc:/Resources/Button/color-off.png";
                            light_off();
                        }
                    }
                }
            }
        }


        Image
        {
            id: music_button
            x: 40 * scale_x
            y: 375 * scale_y
            width: 45 * scale_x
            height: 45 * scale_y
            source: "qrc:/Resources/Button/sound-off.png"
            MouseArea
            {
                id: musicbtn_mouseArea
                anchors.fill: parent
                onClicked:
                {
                    if (isConnected)
                    {
                        if (music_button.source == "qrc:/Resources/Button/sound-off.png")
                        {
                            music_button.source = "qrc:/Resources/Button/sound-on.png";
                            music_play();
                        }
                        else
                        {
                            music_button.source = "qrc:/Resources/Button/sound-off.png";
                            music_stop();
                        }
                    }
                }
            }
        }


        Text
        {
            id: debug_label
            y: 10;
            x: 10;
            text: "Warning: Debug mode is enabled"
            color: "#f00"
            font.pixelSize: 9 * scale_x

        }
        Keys.onReleased:
        {
            if (event.key === Qt.Key_Back || event.key=== Qt.Key_Backspace)
            {
                event.accepted=true;
                main_window.visible = true;
                lamp_window.visible = false;
                main_window.forceActiveFocus();
            }
        }
        ColorCell
        {
            id: cell1
            x: 110 * page.scale_x
            y: 210 * page.scale_y
            color1: "#F73E2A"
            color2: "#EA1361"
            color3: "#9C1AB2"
            color4: "#4C288D"
            color5: "#3C4DB7"
            onCell_clicked: {
                cell2.checkedNum=0
                cell3.checkedNum=0
                cell4.checkedNum=0
                change_color2 (cell.color)
            }
        }
        ColorCell
        {
            id: cell2
            x: 110 * page.scale_x
            y: 250 * page.scale_y
            color1: "#47AF4C"
            color2: "#009687"
            color3: "#00B9D4"
            color4: "#00A6F4"
            color5: "#0F8FEC"
            onCell_clicked: {
                cell1.checkedNum=0
                cell3.checkedNum=0
                cell4.checkedNum=0
                change_color2 (cell.color)
            }
        }
        ColorCell
        {
            id: cell3
            x: 110 * page.scale_x
            y: 290 * page.scale_y
            color1: "#88C440"
            color2: "#CDDB1F"
            color3: "#FEEA17"
            color4: "#FEC000"
            color5: "#3C4DB7"
            onCell_clicked: {
                cell1.checkedNum=0
                cell2.checkedNum=0
                cell4.checkedNum=0
                change_color2 (cell.color)
            }
        }
        ColorCell
        {
            id: cell4
            x: 110 * page.scale_x
            y: 330 * page.scale_y
            color1: "#000000"
            color2: "#5E7B8B"
            color3: "#9C9C9C"
            color4: "#7A5449"
            color5: "#FF5405"
            onCell_clicked: {
                cell1.checkedNum=0
                cell2.checkedNum=0
                cell3.checkedNum=0
                change_color2 (cell.color)
            }
        }
    }

    ///////////////////////////////////////////////////
    //---------------PUZZLE WINDOW-------------------//
    ///////////////////////////////////////////////////
    Rectangle
    {
        visible: false;
        anchors.fill: parent
        id: puzzle_window
        Image {
            id: back_image_puzzle;
            anchors.fill: parent
            source: "qrc:Resources/Screens/rahnema.jpg"
        }
        Keys.onReleased:
        {
            if (event.key === Qt.Key_Back || event.key=== Qt.Key_Backspace)
            {
                event.accepted=true;
                main_window.visible = true;
                puzzle_window.visible = false;
                main_window.forceActiveFocus();
            }
        }
    }


    ///////////////////////////////////////////////////
    //----------------INFO WINDOW--------------------//
    ///////////////////////////////////////////////////
    Rectangle
    {
        visible: false;
        anchors.fill: parent
        id: info_window
        Image {
            id: back_image_info;
            anchors.fill: parent
            source: "qrc:Resources/Screens/about.jpg"
        }
        Keys.onReleased:
        {
            if (event.key === Qt.Key_Back || event.key=== Qt.Key_Backspace)
            {
                event.accepted=true;
                main_window.visible = true;
                info_window.visible = false;
                main_window.forceActiveFocus();
            }
        }
    }


    ///////////////////////////////////////////////////
    //---------------SCREEN WINDOW-------------------//
    ///////////////////////////////////////////////////
    Rectangle
    {
        visible: false;
        anchors.fill: parent
        id: screen_window
        Image {
            id: back_image_screen;
            anchors.fill: parent
            source: "qrc:Resources/Screens/aboutAutism.jpg"
        }
        Keys.onReleased:
        {
            if (event.key === Qt.Key_Back || event.key=== Qt.Key_Backspace)
            {
                event.accepted=true;
                main_window.visible = true;
                screen_window.visible = false;
                main_window.forceActiveFocus();
            }
        }
    }


    ///////////////////////////////////////////////////
    //---------------SETTING WINDOW------------------//
    ///////////////////////////////////////////////////
    Rectangle
    {
        id: setting_window
        visible: false
        anchors.fill: parent
        Image {
            id: back_image_setting;
            anchors.fill: parent
            source: "qrc:Resources/Screens/settings.png"
        }
        Keys.onReleased:
        {
            if (event.key === Qt.Key_Back || event.key=== Qt.Key_Backspace)
            {
                event.accepted=true;
                main_window.visible = true;
                setting_window.visible = false;
                main_window.forceActiveFocus();
            }
        }
        AndroidSlider{
            id: settings_greenSlider
            anchors.bottom: settings_green_button.top
            anchors.left: settings_green_button.left
            anchors.leftMargin: -7
            handle_image: "qrc:Resources/greenGrip.png"
            //onValueChanged: change_color(1,value*255)
            width: 153 * scale_x
            height: 15 * scale_y
        }

        MouseArea
        {
            id: settings_green_button
            x: 100 * scale_x
            y: 225 * scale_y
            width: 215 * scale_x
            height: 30 * scale_y
            //onClicked: change_color(0)
        }

        AndroidSlider{
            id: settings_blueSlider
            anchors.bottom: settings_blue_button.top
            anchors.left: settings_blue_button.left
            anchors.leftMargin: -7
            handle_image: "qrc:Resources/blueGrip.png"
            //onValueChanged: change_color(2,value*255)
            width: 153 * scale_x
            height: 15 * scale_y
        }

        MouseArea
        {
            id: settings_blue_button
            x: 100 * scale_x
            y: 280 * scale_y
            width: 215 * scale_x
            height: 30 * scale_y
            //onClicked: change_color(1)
        }

        AndroidSlider{
            id: settings_redSlider
            anchors.bottom: settings_red_button.top
            anchors.left: settings_red_button.left
            anchors.leftMargin: -7
            anchors.bottomMargin: 2
            //onValueChanged: change_color(0,value*255)
            width: 153 * scale_x
            height: 15 * scale_y
        }

        MouseArea
        {
            id: settings_red_button
            x: 100 * scale_x
            y: 335 * scale_y
            width: 215 * scale_x
            height: 30 * scale_y
            //onClicked: change_color(2)
        }
    }

    function tof_on_screen () {
        debug_label.text += message;
    }

    function lamp_connected () {
        switch(lamp_con_id)
        {
            case 0:
                lamp1_on.visible = true;
                break;
            case 1:
                lamp2_on.visible = true;
                break;
            case 2:
                lamp3_on.visible = true;
                break;
            case 3:
                lamp4_on.visible = true;
                break;
        }
        isConnected = true;
    }

    function lamp_disconnected () {
        switch(lamp_con_id)
        {
            case 0:
                lamp1_on.visible = false;
                break;
            case 1:
                lamp2_on.visible = false;
                break;
            case 2:
                lamp3_on.visible = false;
                break;
            case 3:
                lamp4_on.visible = false;
                break;
        }
        isConnected = false;
    }

    function lamp_enable () {
        if (lamp_count > 0)
        {
            lamp1_off.visible = true;
        }
        if (lamp_count > 1)
        {
            lamp2_off.visible = true;
        }
        if (lamp_count > 2)
        {
            lamp3_off.visible = true;
        }
        if (lamp_count > 3)
        {
            lamp4_off.visible = true;
        }

        isConnected = false;
    }


}

