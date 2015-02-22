# GameSpeechRecognition
编程纯语音打通超级玛丽，不用鼠标，不用键盘，语音控制上下左右移动，攻击，跳跃，打通超级玛丽！
功能
--
通过语音实现游戏控制
配置文件
--
er.xml
配置操作指令
操作
--
玛丽左移：
#
keybd_event(VK_LEFT,0,0,0);
#
Sleep(500);
#
keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
#
玛丽右移：
#
keybd_event(VK_RIGHT, 0, 0, 0);
#
Sleep(500);
#
keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
#
玛丽蹲下：
#
keybd_event(VK_DOWN, 0, 0, 0);
#
Sleep(500);
#
keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
#
玛丽上跳：
#
keybd_event(VK_UP, 0, 0, 0);
#
Sleep(100);
#
keybd_event(VK_UP, 0, KEYEVENTF_KEYUP, 0);
#
玛丽右上方移动：
#
keybd_event(VK_RIGHT, 0, 0, 0);
#
keybd_event(VK_UP, 0, 0, 0);
#
Sleep(500);
#
keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
#
keybd_event(VK_UP, 0, KEYEVENTF_KEYUP, 0);
#
玛丽左上方移动：
#
keybd_event(VK_LEFT, 0, 0, 0);
#
keybd_event(VK_UP, 0, 0, 0);
#
Sleep(500);
#
keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
#
keybd_event(VK_UP, 0, KEYEVENTF_KEYUP, 0);
#
攻击：
#
keybd_event(VK_SPACE, 0, 0, 0);
#
keybd_event(VK_SPACE, 0, KEYEVENTF_KEYUP, 0);
youku视频教程网址：
--
http://v.youku.com/v_show/id_XODk3NzQ3MjYw.html
语言
--
c/c++,xml
工具
--
vs2013 

