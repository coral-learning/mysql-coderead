### 1.概述
    使用CLion Debug MySQL，编译mysql

### 2.获取源码
    git clone https://github.com/mysql/mysql-server

    brew install boost

### 3.编译安装初始化数据库
    cd mysql-server
    step.1
    cmake \
    -DCMAKE_INSTALL_PREFIX=/data/mysql/install \
    -DMYSQL_DATADIR=/data/mysql/data \
    -DSYSCONFDIR=/data/mysql/etc \
    -DMYSQL_UNIX_ADDR=/data/mysql/mysql.sock \
    -DWITH_DEBUG=1  \
    -DWITH_BOOST=/usr/local/Cellar/boost \
    -DDOWNLOAD_BOOST=1 -DWITH_BOOST=/Users/wuhao/Downloads/mysql-server-8.0 -DDOWNLOAD_BOOST_TIMEOUT=60000

    cmake \
    -DWITH_DEBUG=1 \
    -DDOWNLOAD_BOOST=1 \
    -DDOWNLOAD_BOOST_TIMEOUT=60000 \
    -DWITH_BOOST=/usr/local/Cellar/boost


    step.2(比较慢)
    make -j 4

    step.3
    make install -j 4

    step.4
    mysqld --initialize-insecure --user=root --datadir=/data/mysql/data



### 启动mysql
    启动MySQL，测试下是否安装成功 
/path/install/bin/mysqld --defaults-file=/path/mysql/etc/my.cnf

### 导入clion
使用CLion新建工程并打开源码目录之后，设置CLion
CMake Options和你编译安装时的选项一致

-DCMAKE_INSTALL_PREFIX=/data/mysql/install
-DMYSQL_DATADIR=/data/mysql/data
-DSYSCONFDIR=/data/mysql/etc
-DMYSQL_UNIX_ADDR=/data/mysql/mysql.sock
-DWITH_DEBUG=1
--initialize-insecure

然后在CLion里，Reload CMake Project



在Run/Debug列表里就可以看到很多选项了



找到mysqld配置下启动参数


mysqld --defaults-file=/path/mysql/etc/my.cnf
1
然后以Debug模式启动，看下成功的效果


学习MySQL源码的文档

https://dev.mysql.com/doc/internals/en/
1
可以找到想要学习的功能的源码位置，不至于没头苍蝇，比如主从同步功能（replication） 

https://dev.mysql.com/doc/internals/en/replication-source-code-files.html
1
更多架构、PHP、GO相关踩坑实践技巧请关注我的公众号：PHP架构师
————————————————
版权声明：本文为CSDN博主「安木1991」的原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/weixin_42900065/article/details/81775706