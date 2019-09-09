# centos7下安装freeswitch版本:1.6。

1. 下载源代码：
进入目录 cd /usr/local/src

克隆原代码
```
git clone -b v1.6 https://freeswitch.org/stash/scm/fs/freeswitch.git freeswitch
```

2. 安装依赖库：
```
yum install -y git gcc-c++ wget alsa-lib-devel autoconf automake bison broadvoice-devel bzip2 curl-devel db-devel e2fsprogs-devel flite-devel g722_1-devel gdbm-devel gnutls-devel ilbc2-devel ldns-devel libcodec2-devel libcurl-devel libedit-devel libidn-devel libjpeg-devel libmemcached-devel libogg-devel libsilk-devel libsndfile-devel libtiff-devel libtheora-devel libtool libvorbis-devel libxml2-devel lua-devel lzo-devel mongo-c-driver-devel ncurses-devel net-snmp-devel openssl-devel opus-devel pcre-devel perl perl-ExtUtils-Embed pkgconfig portaudio-devel postgresql-devel python26-devel python-devel soundtouch-devel speex-devel sqlite-devel unbound-devel unixODBC-devel libuuid-devel which yasm zlib-devel
```

2. 开始编译
```
cd/ freeswitch
./bootstrap.sh
./configure
make
make install
```

3. 启动freeswitch
```
cd /usr/local/freeswitch/bin/
./freeswitch
```

4. 配置freeswitch在任何一个路径都可以运行
```
ln -sf /usr/local/freeswitch/bin/freeswitch /usr/bin/
ln -sf /usr/local/freeswitch/bin/fs_cli /usr/bin/
```

# 启动失败解决

1. 如果运行freeswitch出现以下错误，是因为端口被占用了。
[root@localhost ~]# freeswitch
Cannot lock pid file /usr/local/freeswitch/run/freeswitch.pid.

解决办法：可以重启Linux或者关闭占用端口的进程。

2. 如果freeswitch启动成功，但是linphone注册不上。

解决办法：systemctl stop firewalld.service （关闭防火墙）