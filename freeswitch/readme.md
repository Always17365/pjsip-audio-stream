# centos7下安装freeswitch

## rpm安装（推荐）
[参考文档](https://freeswitch.org/confluence/display/FREESWITCH/CentOS+7+and+RHEL+7)

1. 安装
```
./install.sh
```

2. 启动
```
systemctl enable freeswitch.service
```

查看freeswitch是否启动

```
netstat -an | grep 5060
```

## 编译安装（不推荐）
[参考文档](https://wsonh.com/article/64.html)

# 常见问题

1. 如果运行freeswitch出现以下错误，是因为端口被占用了。
[root@localhost ~]# freeswitch
Cannot lock pid file /usr/local/freeswitch/run/freeswitch.pid.

解决办法：可以重启Linux或者关闭占用端口的进程。

2. 如果freeswitch启动成功，但是linphone注册不上。

解决办法：
```
# 查看防火墙状态
systemctl status firewalld.service
# 关闭防火墙
systemctl stop firewalld.service
# 取消防火墙开机启动
systemctl disable firewalld.service
```


3. centos默认网卡没有启动
```
vi /etc/sysconfig/network-scripts/ifcfg-ens33 # 修改ONBOOT=yes，并保存
sudo service network restart
ip addr # 可以看到ens33中的ip地址
```

4. 呼叫挂断都很慢
freeswitch没有修改用户默认密码的话会强制10s延迟，所以修改用户默认密码就好了
```
# 查找配置文件位置
find / -name vars.xml
```
我的vars.xml在这个位置/etc/freeswitch/vars.xml

在FreeSwitch系统中，所有用户的密码默认为 1234，该设置在 /etc/freeswitch/vars.xml 中，如下所示：
```
<X-PRE-PROCESS cmd=”set” data=”default_password=1234″/>
```
修改为
```
<X-PRE-PROCESS cmd=”set” data=”default_password=123456″/>
```

用户使用密码123456注册上就OK啦

5. freeswitch常用命令
```
# 查看帮助
freeswitch -help

# 启动freeswitch
freeswitch

# 关闭freeswitch
shutdown

# 后台启动
freeswitch -nc

# 关闭后台freeswitch
freeswitch -stop

# 查看freeswitch是否正常启动
netstat -anp | grep freeswitch
```

6. 无法主动挂断，30s后自己挂断

Wireshark抓包查看问题，发现sip消息有问题，地址ip被替换为外网ip导致消息发送不到FS；

解决：打开/etc/freeswitch/sip_profiles下的配置文件internal.xml，将外网配置注释掉，重启FS；
```
<!--<param name="ext-rtp-ip" value="$${external_rtp_ip}"> -->
<!--<param name="ext-sip-ip" value="$${external_rtp_ip}"> -->
```

# 客户端测试
FreeSWITCH使用make install安装完成后，默认就配置了1000~1019共20个账户，不用做任何其它配置，就可以开始我们的基本通话测试了，非常人性化。

- 查看freeswitch监听的ip地址，后面我们要用这个ip地址配置microsip
```
netstat -an | grep 5060
```

- 下载客户端

windows: [microsip](https://www.microsip.org/)

mac: [xlite](https://www.counterpath.com/x-lite-download/)

- 添加账户，Domain应该填写FreeSWITCH服务器的IP地址，Username可以填1000~1019其中一个，Password是1234，具体如图：

![pjsip](../doc/pjsip.png)

再不同机器上启动microsip登上不同账号可以互相呼叫


