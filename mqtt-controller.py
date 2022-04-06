# 继续制造纯粹的学术以及代码垃圾

from ast import Pass
from prompt_toolkit.shortcuts import ProgressBar
from prompt_toolkit.shortcuts import input_dialog
from prompt_toolkit.formatted_text import HTML
from prompt_toolkit import prompt
from prompt_toolkit.completion import WordCompleter
from prompt_toolkit.validation import Validator, ValidationError
from prompt_toolkit import prompt
import time
import prompt_toolkit
import paho.mqtt.client as mqtt
from prompt_toolkit.formatted_text import HTML
from prompt_toolkit.shortcuts import ProgressBar
from prompt_toolkit.shortcuts.progress_bar import formatters
from prompt_toolkit.styles import Style
from prompt_toolkit.formatted_text import FormattedText
from prompt_toolkit.application import Application
from prompt_toolkit.application.current import get_app
from prompt_toolkit.key_binding import KeyBindings
from prompt_toolkit.key_binding.bindings.focus import focus_next, focus_previous
from prompt_toolkit.layout import HSplit, Layout, Float, VSplit
from prompt_toolkit.styles import Style
from prompt_toolkit.widgets import Box, Button, Frame, Label, TextArea, SearchToolbar, VerticalLine, RadioList,Box,MenuContainer,MenuItem
from prompt_toolkit.layout.menus import CompletionsMenu


def on_message(client, userdata, message):
    print("message received " ,str(message.payload.decode("utf-8")))
    print("message topic=",message.topic)
    print("message qos=",message.qos)
    print("message retain flag=",message.retain)
broker_address="h.kisonhe.xyz"

prompt_toolkit.shortcuts.clear()
title = HTML('系统启动中 <style bg="yellow" fg="black">加载文件...</style>')
label = FormattedText([
    ('#00aa00', '加载进度'),
])

# 用来看起来我的毕业设计工作量很大的完全没有用的进度条
# startProgressBarNum = 0
startProgressBarNum = 127

bootProgressBarStyle = Style.from_dict(
    {
        "progressbar title": "#ffffff",
        "item-title": "#ff4400 underline",
        "percentage": "#00ff00",
        "time-left": "#ffffff",
        "spinning-wheel": "bg:#06914c #000000",
    }
)

bootProgressBarFormatters = [
    formatters.Label(),
    formatters.Text(": "),
    formatters.SpinningWheel(),
    formatters.Text(" "),
    formatters.Text(HTML("<tildes>运行中</tildes>")),
    formatters.Bar(sym_a="=", sym_b=">", sym_c="."),
    formatters.Text(" 预计剩余时间: "),
    formatters.TimeLeft(),
]

with ProgressBar(title=title,style=bootProgressBarStyle,formatters=bootProgressBarFormatters) as pb:
    for i in pb(range(startProgressBarNum), label=label):
        if (i>(startProgressBarNum*2/3)):
            pb.title = HTML('系统启动中 <style bg="yellow" fg="black">配置系统设置...</style>')
            time.sleep(.01)
        elif (i>(startProgressBarNum/3)):
            pb.title = HTML('系统启动中 <style bg="yellow" fg="black">加载系统组件...</style>')
            time.sleep(.02)
        else:
            time.sleep(.04)
    
# 没有任何意义的登陆部分
prompt_toolkit.shortcuts.clear()
logInUsers = ['管理员', '陈工', '何工', '孙工']

class LogInValidator(Validator):
    def validate(self, document):
        text = document.text
        if not text in logInUsers:
            raise ValidationError(message='用户不存在！',cursor_position=0)

logIncompleter = WordCompleter(logInUsers)
text = prompt('输入用户名: ', completer=logIncompleter, validator=LogInValidator())


class PasswordValidator(Validator):
    def validate(self, document):
        text = document.text
        if not len(text) > 1:
            raise ValidationError(message='请输入密码！',cursor_position=0)

passwordpass = False
while(not passwordpass):
    text = input_dialog(
        title='用户验证',
        text='输入密码:',password=True,validator=PasswordValidator()).run()
    if len(text) > 0:
        passwordpass = True


motors = [
    ("motor1", "MotorOne"),
    ("motor2", "MotorTwo")
]
def motorListChange():
    global motors
    motors.append(("motor3", "MotorThree"))


motorRadioList = RadioList(values=motors)
motorListPickerContainer = HSplit([
    motorRadioList
])
motorInfoContainer = VSplit([Label("活动电机信息")])
cmdField = TextArea(
    height=1,
    prompt=">>> ",
    style="class:input-field",
    multiline=False,
    wrap_lines=True,
    search_field=SearchToolbar(),
)
root_container = Box(
    Frame(body=HSplit([
        VSplit([
            motorListPickerContainer,
            VerticalLine(),
            motorInfoContainer,
        ]),
        Frame(body=cmdField)
    ])
)
)

root_container = MenuContainer(
    body=root_container,
    menu_items=[
        MenuItem(
            "File",
            children=[
                MenuItem("New",handler=motorListChange),
                MenuItem(
                    "Open",
                    children=[
                        MenuItem("From file..."),
                        MenuItem("From URL..."),
                        MenuItem(
                            "Something else..",
                            children=[
                                MenuItem("A"),
                                MenuItem("B"),
                                MenuItem("C"),
                                MenuItem("D"),
                                MenuItem("E"),
                            ],
                        ),
                    ],
                ),
                MenuItem("Save"),
                MenuItem("Save as..."),
                MenuItem("-", disabled=True),
                MenuItem("Exit"),
            ],
        ),
        MenuItem(
            "Edit",
            children=[
                MenuItem("Undo"),
                MenuItem("Cut"),
                MenuItem("Copy"),
                MenuItem("Paste"),
                MenuItem("Delete"),
                MenuItem("-", disabled=True),
                MenuItem("Find"),
                MenuItem("Find next"),
                MenuItem("Replace"),
                MenuItem("Go To"),
                MenuItem("Select All"),
                MenuItem("Time/Date"),
            ],
        ),
        MenuItem("View", children=[MenuItem("Status Bar")]),
        MenuItem("Info", children=[MenuItem("About")]),
    ],
    floats=[
        Float(
            xcursor=True,
            ycursor=True,
            content=CompletionsMenu(max_height=16, scroll_offset=1),
        ),
    ],
)

layout = Layout(container=root_container)
# Key bindings.
kb = KeyBindings()
kb.add("tab")(focus_next)
kb.add("s-tab")(focus_previous)


@kb.add('c-q')
@kb.add('c-c')
def exit_(event):
    """
    Pressing Ctrl-Q/Ctrl-C will exit the user interface.

    Setting a return value means: quit the event loop that drives the user
    interface and return this value from the `Application.run()` call.
    """
    event.app.exit()


# Styling.
style = Style([
    ("left-pane", "bg:#888800 #000000"),
    ("right-pane", "bg:#00aa00 #000000"),
    ("button", "#000000"),
    ("button-arrow", "#000000"),
    ("button focused", "bg:#ff0000"),
    ("text-area focused", "bg:#ff0000"),
])

# Build a main application object.
application = Application(layout=layout,
                          key_bindings=kb,
                          style=style,
                          full_screen=True,
                          mouse_support=True)
application.run()

# client = mqtt.Client("mqttInstance") #create new instance
# client.on_message=on_message #attach function to callback
# client.connect(broker_address,port=1883) #connect to broker
# client.loop_start() #start the loop
# client.subscribe("gp/hb/3508")
# client.subscribe("gp/cmd/3508")
# # while 1:
# #     client.publish("gp/cmd/3508",payload)
# #     time.sleep(1)

# client.loop_stop() #stop the loop