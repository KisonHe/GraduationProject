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
import json
started = False
def on_message(client, userdata, message):
    global started
    started = True
    ID = message.topic.replace("gp/hb/","")
    print("收到ID为:",ID,"的心跳报告包：" ,str(message.payload.decode("utf-8")))
    print("发送命令：命令其转速3000运行")
    cmd = {"commandType": "set","control": "spd","controlValue" : 3000}
    payload = json.dumps(cmd)
    print(payload)
    client.publish("gp/cmd/"+ID, payload)

broker_address="www.kisonhe.xyz"

# prompt_toolkit.shortcuts.clear()
# title = HTML('系统启动中 <style bg="yellow" fg="black">加载文件...</style>')
# label = HTML('<seagreen>加载进度</seagreen>: ')

# # 用来看起来我的毕业设计工作量很大的完全没有用的进度条
# # startProgressBarNum = 0
# startProgressBarNum = 127
# with ProgressBar(title=title) as pb:
#     for i in pb(range(startProgressBarNum), label=label):
#         if (i>(startProgressBarNum*2/3)):
#             pb.title = HTML('系统启动中 <style bg="yellow" fg="black">配置系统设置...</style>')
#             time.sleep(.01)
#         elif (i>(startProgressBarNum/3)):
#             pb.title = HTML('系统启动中 <style bg="yellow" fg="black">加载系统组件...</style>')
#             time.sleep(.02)
#         else:
#             time.sleep(.04)
    
# # 没有任何意义的登陆部分
# prompt_toolkit.shortcuts.clear()
# logInUsers = ['管理员', '陈工', '何工', '孙工']

# class LogInValidator(Validator):
#     def validate(self, document):
#         text = document.text
#         if not text in logInUsers:
#             raise ValidationError(message='用户不存在！',cursor_position=0)

# logIncompleter = WordCompleter(logInUsers)
# text = prompt('输入用户名: ', completer=logIncompleter, validator=LogInValidator())


# class PasswordValidator(Validator):
#     def validate(self, document):
#         text = document.text
#         if not len(text) > 1:
#             raise ValidationError(message='请输入密码！',cursor_position=0)

# passwordpass = False
# while(not passwordpass):
#     text = input_dialog(
#         title='用户验证',
#         text='输入密码:',password=True,validator=PasswordValidator()).run()
#     if len(text) > 0:
#         passwordpass = True


client = mqtt.Client("mqttInstance") #create new instance
client.on_message=on_message #attach function to callback
client.connect(broker_address,port=1883) #connect to broker
client.loop_start() #start the loop
client.subscribe("gp/hb/#")
# client.subscribe("gp/cmd/#")
time.sleep(10)
# while 1:
#     client.publish("gp/cmd/3508",payload)
#     time.sleep(1)

client.loop_stop() #stop the loop