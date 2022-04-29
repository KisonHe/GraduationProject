# 继续制造纯粹的学术以及代码垃圾
import re
from ast import Pass
from prompt_toolkit.shortcuts import ProgressBar
from prompt_toolkit.shortcuts import input_dialog
from prompt_toolkit.formatted_text import HTML
from prompt_toolkit import prompt
from prompt_toolkit.completion import WordCompleter
from prompt_toolkit.validation import Validator, ValidationError
from prompt_toolkit.completion import NestedCompleter
from prompt_toolkit import prompt
from prompt_toolkit.auto_suggest import AutoSuggestFromHistory
import time
import prompt_toolkit
import paho.mqtt.client as mqtt
import json
started = False
ID = "No128"
BottomBarText = ""
preText = ""
broker_address="www.kisonhe.xyz"
completer = NestedCompleter.from_nested_dict({
    'set': {
        'inPID': {
            'kP': None,
            'kI': None,
            'kD': None,
        },
        'outPID': {
            'kP': None,
            'kI': None,
            'kD': None,
        },
        'spd': None,
        'pos': None,
    },
    'exit': None,
})

# def on_connect(a,b,c,d):
#     print("yes connected")

# def on_disconnect(a,b,c):
#     print("no disconnected")

def on_message(client, userdata, message):
    global started, ID, BottomBarText
    started = True
    ID = message.topic.replace("gp/hb/","")
    data = json.loads(str(message.payload.decode("utf-8")))
    tmpBottomBarText =  ""
    if ("rpm" in data):
        tmpBottomBarText += "转速:"+str(data["rpm"])
    else:
        tmpBottomBarText += "转速:0"

    if ("angle" in data):
        tmpBottomBarText += " 路程:"+str(data["angle"])
    else:
        tmpBottomBarText += " 路程:0"
    
    if ("control" in data):
        if (data["control"]):
            tmp = "路程闭环"
        else:
            tmp = "速度闭环"
    tmpBottomBarText += " 运行态:"+tmp
    if ("sensorValueNum0" in data):
        tmpBottomBarText += " 自定义传感器1:"+str(data["sensorValueNum0"])

    if ("pid_out_p" in data)and(data["pid_out_p"]!=0):
        tmpBottomBarText += " 外环kP:"+str(data["pid_out_p"])
    if ("pid_out_i" in data)and(data["pid_out_p"]!=0):
        tmpBottomBarText += " 外环kI:"+str(data["pid_out_i"])
    if ("pid_out_d" in data)and(data["pid_out_p"]!=0):
        tmpBottomBarText += " 外环kD:"+str(data["pid_out_d"])

    if ("pid_in_p" in data)and(data["pid_in_p"]!=0):
        tmpBottomBarText += " 内环kP:"+str(data["pid_in_p"])
    if ("pid_in_i" in data)and(data["pid_in_p"]!=0):
        tmpBottomBarText += " 内环kI:"+str(data["pid_in_i"])
    if ("pid_in_d" in data)and(data["pid_in_p"]!=0):
        tmpBottomBarText += " 内环kD:"+str(data["pid_in_d"])

    BottomBarText = tmpBottomBarText
    # print(BottomBarText)

def bottom_toolbar():
    global ID, BottomBarText, preText
    return HTML('当前控制ID: ' + "<style bg=\"ansired\">"+ ID +"</style>" + " 参数详情->> " + BottomBarText)

def useless_fancy():
    prompt_toolkit.shortcuts.clear()
    title = HTML('系统启动中 <style bg="yellow" fg="black">加载文件...</style>')
    label = HTML('<seagreen>加载进度</seagreen>: ')

    # 用来看起来我的毕业设计工作量很大的完全没有用的进度条
    # startProgressBarNum = 0
    startProgressBarNum = 127
    with ProgressBar(title=title) as pb:
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


def is_valid(text):
    global BottomBarText
    if text.startswith("exit"):
        return True
    elif (re.match("^set (((in|out)PID k(P|I|D))|spd|pos) [+-]?([0-9]*[.])?[0-9]+$",text)!=None):
        return True
    return False

def main():
    global client
    useless_fancy()
    client = mqtt.Client() #create new instance, notice that https://github.com/knolleary/pubsubclient/issues/372
    # mqtt id should not be the same
    client.on_message=on_message #attach function to callback
    # client.on_connect=on_connect
    # client.on_disconnect=on_disconnect
    client.connect(broker_address,port=1883) #connect to broker
    client.loop_start() #start the loop
    client.subscribe("gp/hb/#")
    validator = Validator.from_callable(is_valid,error_message="输入命令格式错误",move_cursor_to_end=True)
    while 1:
        set_arg = False
        control_text = ""
        num = 0
        text = prompt('> ', bottom_toolbar=bottom_toolbar,completer=completer,validator=validator, validate_while_typing=True,auto_suggest=AutoSuggestFromHistory(),refresh_interval=0.5)
        if (text.startswith("exit")):
            break
        if (not is_valid(text)):
            continue
        num = re.search("[+-]?([0-9]*[.])?[0-9]+",text).group(0)
        if num.isdigit():
            num = int(num)
        else:
            num = float(num)
        if (re.match("^set ((in|out)PID k(P|I|D)) [+-]?([0-9]*[.])?[0-9]+$",text)):
            set_arg = True
            control_text += "pid_"+re.search("(in|out)",text).group(0)
            control_text += "_"
            control_text += re.search("k(P|I|D)",text).group(0)[1].lower()
        else:
            set_arg = False
            control_text = re.search("(spd|pos)",text).group(0)
            if re.search("(spd|pos)",text).group(0) == "spd":
                num = int(num)
        # pid_in_p
        cmd = {"commandType": "set"}
        if (set_arg):
            cmd[control_text] = num # do we need to convert to float?
            pass
        else:
            cmd["control"] = control_text
            cmd["controlValue"] = num
            pass
        payload = json.dumps(cmd)
        client.publish("gp/cmd/"+ID, payload)
        print (payload)
        

    print('退出系统')
    client.loop_stop() #stop the loop


main()