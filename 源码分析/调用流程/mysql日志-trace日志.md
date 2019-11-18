### 1.MySql Trace Log
#### 1.1 my.cnf配置
```
在MySql5.1.12之前，在my.cnf或者my.ini中的[mysqld]节下配置：
log = /path/for/file/live-query.log
#而在MySql 5.1.12中，你需要在上述配置文件中的[mysqld]中配置：
general_log=1
#MySql 5.1.29中，包括之后版本，你就需要再加上一句：
general_log_file = /path/for/file/live-query.log
```
#### 1.2 在线配置
```cassandraql
-- 查看MySQL的版本号
select version();
-- 查询关于log的设置 （若版本号为 ‘5.1.29’以上版本 like 'general_log%'）
SHOW VARIABLES LIKE 'general_log%';    
-- 开启日志
SET GLOBAL general_log ='ON';
-- 设置日志的文件路径：本例在Windows下
SET GLOBAL general_log_file='D:/MySql/Sql.log';
-- 设置结束，执行的sql语句会保存在‘D:/MySql/Sql.log’，可通过notepad++查看

```
#### 1.3操作结果
通过插入工具显示
```cassandraql
191024 22:32:05	    1 Query	INSERT INTO `test` (`id`, `bane`) VALUES (NULL, NULL)
		    1 Query	SELECT * FROM `test` LIMIT 0,1000
191024 22:32:06	    1 Query	SHOW COLUMNS FROM `test`
191024 22:32:11	    1 Query	UPDATE `test` SET `bane` = '1' WHERE `id` = '3'
		    1 Query	SELECT * FROM `test` LIMIT 0,1000
		    1 Query	SET NAMES 'utf8'
		    1 Query	SHOW TABLE STATUS LIKE 'test'
		    1 Query	SET NAMES 'utf8mb4'
		    1 Query	SET NAMES 'utf8'
		    1 Query	SHOW CREATE TABLE `test`
		    1 Query	SET NAMES 'utf8mb4'
		    1 Query	SELECT * FROM `test` LIMIT 0,1000
```

### 2.debug日志
#### 