使用说明：
如果出现双击打开后无反应，则把MSCOMM32.OCX拷贝到64位系统需要把它放在C:\Windows\SysWOW64目录下
（32位系统则是C:\Windows\system32目录），然后右键管理员权限打开cmd窗口，进入对应目录输入
指令 regsvr32 MSCOMM32.OCX 即可。