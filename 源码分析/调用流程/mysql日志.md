<div id="mainContent">
<div class="forFlow">
<div id="post_detail">
<div id="topics">
<div class="post">
<h1 class="postTitle">

<a id="cb_post_title_url" class="postTitle2" href="https://www.cnblogs.com/f-ck-need-u/p/9001061.html">详细分析MySQL的日志(一)</a>

</h1>
<div class="clear"></div>
<div class="post-categoty-tags"><div class="post-categoty">
分类: 
<a href="https://www.cnblogs.com/f-ck-need-u/category/1085743.html" target="_blank">数据库系列</a></div><div class="post-tags">undefined</div></div><div class="postBody">

<div id="cnblogs_post_body" class="blogpost-body "><p><strong><span style="font-size: 18px;">转载请务必在文章<span style="color: red;">最开头</span>标明原文地址<br>本文原创地址：骏马金龙<a href="https://www.cnblogs.com/f-ck-need-u/p/9001061.html">https://www.cnblogs.com/f-ck-need-u/p/9001061.html</a></span></strong><br><strong><span style="font-size: 18px;">本人博客搬家：骏马金龙<a href="https://www.junmajinlong.com">www.junmajinlong.com</a></span></strong></p><p>&nbsp;</p>
<p class="01">官方手册:<a href="https://dev.mysql.com/doc/refman/5.7/en/server-logs.html" target="_blank"><span style="color: #0000ff;">https://dev.mysql.com/doc/refman/5.7/en/server-logs.html</span></a></p>
<p>不管是哪个数据库产品，一定会有日志文件。在MariaDB/MySQL中，主要有5种日志文件：<br>
1.错误日志(error log)：记录mysql服务的启停时正确和错误的信息，还记录启动、停止、运行过程中的错误信息。<br>
2.查询日志(general log)：记录建立的客户端连接和执行的语句。<br>
3.二进制日志(bin log)：记录所有更改数据的语句，可用于数据复制。<br>
4.慢查询日志(slow log)：记录所有执行时间超过long_query_time的所有查询或不使用索引的查询。<br>
5.中继日志(relay log)：主从复制时使用的日志。</p>
<p>除了这5种日志，在需要的时候还会创建DDL日志。本文暂先讨论错误日志、一般查询日志、慢查询日志和二进制日志，中继日志和主从复制有关，将在复制的章节中介绍。下一篇文章将介绍innodb事务日志，见：<span style="color: #0000ff;"><a id="cb_post_title_url" href="http://www.cnblogs.com/f-ck-need-u/p/9010872.html" target="_blank"><span style="color: #0000ff;">MySQL的事务日志</span></a></span>。</p>
<p><a name="blog1"></a></p>
<h1 id="auto_id_0">1.日志刷新操作</h1>
<p class="01">以下操作会刷新日志文件，刷新日志文件时会关闭旧的日志文件并重新打开日志文件。对于有些日志类型，如二进制日志，刷新日志会滚动日志文件，而不仅仅是关闭并重新打开。</p>
<div class="cnblogs_code">
<pre>mysql<span style="color: #808080;">&gt;</span><span style="color: #000000;"> FLUSH LOGS;
shell</span><span style="color: #808080;">&gt;</span> mysqladmin flush<span style="color: #808080;">-</span><span style="color: #000000;">logs
shell</span><span style="color: #808080;">&gt;</span> mysqladmin refresh</pre>
</div>
<p><a name="blog2"></a></p>
<h1 id="auto_id_1">2.错误日志</h1>
<p class="01">错误日志是最重要的日志之一，它记录了MariaDB/MySQL服务启动和停止正确和错误的信息，还记录了mysqld实例运行过程中发生的错误事件信息。</p>
<p class="01">可以使用"&nbsp;<span class="cnblogs_code"><span style="color: #008080;">--</span><span style="color: #008080;">log-error=[file_name]</span></span>&nbsp;"来指定mysqld记录的错误日志文件，如果没有指定file_name，则默认的错误日志文件为datadir目录下的&nbsp;<span class="cnblogs_code">`hostname`.err</span>&nbsp;，hostname表示当前的主机名。</p>
<p class="01">也可以在MariaDB/MySQL配置文件中的mysqld配置部分，使用log-error指定错误日志的路径。</p>
<p class="01">如果不知道错误日志的位置，可以查看变量log_error来查看。</p>
<div>
<div class="cnblogs_code">
<pre>mysql<span style="color: #808080;">&gt;</span> show variables <span style="color: #808080;">like</span> <span style="color: #ff0000;">'</span><span style="color: #ff0000;">log_error</span><span style="color: #ff0000;">'</span><span style="color: #000000;">;
</span><span style="color: #808080;">+</span><span style="color: #008080;">--</span><span style="color: #008080;">-------------+----------------------------------------+</span>
<span style="color: #808080;">|</span> Variable_name <span style="color: #808080;">|</span> Value                                  <span style="color: #808080;">|</span>
<span style="color: #808080;">+</span><span style="color: #008080;">--</span><span style="color: #008080;">-------------+----------------------------------------+</span>
<span style="color: #808080;">|</span> log_error     <span style="color: #808080;">|</span> <span style="color: #808080;">/</span><span style="color: #ff00ff;">var</span><span style="color: #808080;">/</span>lib<span style="color: #808080;">/</span>mysql<span style="color: #808080;">/</span>node1.longshuai.com.err <span style="color: #808080;">|</span>
<span style="color: #808080;">+</span><span style="color: #008080;">--</span><span style="color: #008080;">-------------+----------------------------------------+</span></pre>
</div>
</div>
<p class="01">在MySQL 5.5.7之前，刷新日志操作(如flush logs)会备份旧的错误日志(以_old结尾)，并创建一个新的错误日志文件并打开，在MySQL 5.5.7之后，执行刷新日志的操作时，错误日志会关闭并重新打开，如果错误日志不存在，则会先创建。</p>
<p class="01">在MariaDB/MySQL正在运行状态下删除错误日志后，不会自动创建错误日志，只有在刷新日志的时候才会创建一个新的错误日志文件。</p>
<p class="01">以下是MySQL 5.6.35启动的日志信息。</p>
<div class="cnblogs_code"><div class="cnblogs_code_toolbar"><span class="cnblogs_code_copy"><a href="javascript:void(0);" onclick="copyCnblogsCode(this)" title="复制代码"><img src="//common.cnblogs.com/images/copycode.gif" alt="复制代码"></a></span></div>
<pre><span style="color: #888888;"><span style="font-weight: bold;">2017</span>-<span style="font-weight: bold;">03</span>-<span style="font-weight: bold;">29</span> <span style="font-weight: bold;">01</span>:<span style="font-weight: bold;">15</span>:<span style="font-weight: bold;">14</span> <span style="font-weight: bold;">2362</span> [Note] Plugin 'FEDERATED' is disabled.
<span style="font-weight: bold;">2017</span>-<span style="font-weight: bold;">03</span>-<span style="font-weight: bold;">29</span> <span style="font-weight: bold;">01</span>:<span style="font-weight: bold;">15</span>:<span style="font-weight: bold;">14</span> <span style="font-weight: bold;">2362</span> [Note] InnoDB: Using atomics to ref count buffer pool pages
<span style="font-weight: bold;">2017</span>-<span style="font-weight: bold;">03</span>-<span style="font-weight: bold;">29</span> <span style="font-weight: bold;">01</span>:<span style="font-weight: bold;">15</span>:<span style="font-weight: bold;">14</span> <span style="font-weight: bold;">2362</span> [Note] InnoDB: The InnoDB memory heap is disabled
<span style="font-weight: bold;">2017</span>-<span style="font-weight: bold;">03</span>-<span style="font-weight: bold;">29</span> <span style="font-weight: bold;">01</span>:<span style="font-weight: bold;">15</span>:<span style="font-weight: bold;">14</span> <span style="font-weight: bold;">2362</span> [Note] InnoDB: Mutexes and rw_locks use GCC atomic builtins
<span style="font-weight: bold;">2017</span>-<span style="font-weight: bold;">03</span>-<span style="font-weight: bold;">29</span> <span style="font-weight: bold;">01</span>:<span style="font-weight: bold;">15</span>:<span style="font-weight: bold;">14</span> <span style="font-weight: bold;">2362</span> [Note] InnoDB: Memory barrier is not used
<span style="font-weight: bold;">2017</span>-<span style="font-weight: bold;">03</span>-<span style="font-weight: bold;">29</span> <span style="font-weight: bold;">01</span>:<span style="font-weight: bold;">15</span>:<span style="font-weight: bold;">14</span> <span style="font-weight: bold;">2362</span> [Note] InnoDB: Compressed tables use zlib <span style="font-weight: bold;">1.2</span>.<span style="font-weight: bold;">3</span>
<span style="font-weight: bold;">2017</span>-<span style="font-weight: bold;">03</span>-<span style="font-weight: bold;">29</span> <span style="font-weight: bold;">01</span>:<span style="font-weight: bold;">15</span>:<span style="font-weight: bold;">14</span> <span style="font-weight: bold;">2362</span> [Note] InnoDB: Using Linux native AIO
<span style="font-weight: bold;">2017</span>-<span style="font-weight: bold;">03</span>-<span style="font-weight: bold;">29</span> <span style="font-weight: bold;">01</span>:<span style="font-weight: bold;">15</span>:<span style="font-weight: bold;">14</span> <span style="font-weight: bold;">2362</span> [Note] InnoDB: Using CPU crc32 instructions
<span style="font-weight: bold;">2017</span>-<span style="font-weight: bold;">03</span>-<span style="font-weight: bold;">29</span> <span style="font-weight: bold;">01</span>:<span style="font-weight: bold;">15</span>:<span style="font-weight: bold;">14</span> <span style="font-weight: bold;">2362</span> [Note] <span style="color: #ff0000;"><em><strong>InnoDB: Initializing buffer pool, size = 128.0M</strong></em></span>
<span style="font-weight: bold;">2017</span>-<span style="font-weight: bold;">03</span>-<span style="font-weight: bold;">29</span> <span style="font-weight: bold;">01</span>:<span style="font-weight: bold;">15</span>:<span style="font-weight: bold;">14</span> <span style="font-weight: bold;">2362</span> [Note] InnoDB: Completed initialization of buffer pool
<span style="font-weight: bold;">2017</span>-<span style="font-weight: bold;">03</span>-<span style="font-weight: bold;">29</span> <span style="font-weight: bold;">01</span>:<span style="font-weight: bold;">15</span>:<span style="font-weight: bold;">14</span> <span style="font-weight: bold;">2362</span> [Note] InnoDB: Highest supported file format is Barracuda.
<span style="font-weight: bold;">2017</span>-<span style="font-weight: bold;">03</span>-<span style="font-weight: bold;">29</span> <span style="font-weight: bold;">01</span>:<span style="font-weight: bold;">15</span>:<span style="font-weight: bold;">14</span> <span style="font-weight: bold;">2362</span> [Note] <em><strong><span style="color: #ff0000;">InnoDB: 128 rollback segment(s) are active.</span></strong></em>
<span style="font-weight: bold;">2017</span>-<span style="font-weight: bold;">03</span>-<span style="font-weight: bold;">29</span> <span style="font-weight: bold;">01</span>:<span style="font-weight: bold;">15</span>:<span style="font-weight: bold;">14</span> <span style="font-weight: bold;">2362</span> [Note] InnoDB: Waiting for purge to start
<span style="font-weight: bold;">2017</span>-<span style="font-weight: bold;">03</span>-<span style="font-weight: bold;">29</span> <span style="font-weight: bold;">01</span>:<span style="font-weight: bold;">15</span>:<span style="font-weight: bold;">14</span> <span style="font-weight: bold;">2362</span> [Note] InnoDB: <span style="font-weight: bold;">5.6</span>.<span style="font-weight: bold;">35</span> started;<em><strong><span style="color: #ff0000;"> log sequence number 3911610</span></strong></em>
<span style="font-weight: bold;">2017</span>-<span style="font-weight: bold;">03</span>-<span style="font-weight: bold;">29</span> <span style="font-weight: bold;">01</span>:<span style="font-weight: bold;">15</span>:<span style="font-weight: bold;">14</span> <span style="font-weight: bold;">2362</span> [Note] Server hostname (bind-address): '*'; port: <span style="font-weight: bold;">3306</span>
<span style="font-weight: bold;">2017</span>-<span style="font-weight: bold;">03</span>-<span style="font-weight: bold;">29</span> <span style="font-weight: bold;">01</span>:<span style="font-weight: bold;">15</span>:<span style="font-weight: bold;">14</span> <span style="font-weight: bold;">2362</span> [Note] IPv6 is available.
<span style="font-weight: bold;">2017</span>-<span style="font-weight: bold;">03</span>-<span style="font-weight: bold;">29</span> <span style="font-weight: bold;">01</span>:<span style="font-weight: bold;">15</span>:<span style="font-weight: bold;">14</span> <span style="font-weight: bold;">2362</span> [Note]   - '::' resolves to '::';
<span style="font-weight: bold;">2017</span>-<span style="font-weight: bold;">03</span>-<span style="font-weight: bold;">29</span> <span style="font-weight: bold;">01</span>:<span style="font-weight: bold;">15</span>:<span style="font-weight: bold;">14</span> <span style="font-weight: bold;">2362</span> [Note] Server socket created on IP: '::'.
<span style="font-weight: bold;">2017</span>-<span style="font-weight: bold;">03</span>-<span style="font-weight: bold;">29</span> <span style="font-weight: bold;">01</span>:<span style="font-weight: bold;">15</span>:<span style="font-weight: bold;">14</span> <span style="font-weight: bold;">2362</span> [Warning] 'proxies_priv' entry '@ root@xuexi.longshuai.com' ignored in --skip-name-resolve mode.
<span style="font-weight: bold;">2017</span>-<span style="font-weight: bold;">03</span>-<span style="font-weight: bold;">29</span> <span style="font-weight: bold;">01</span>:<span style="font-weight: bold;">15</span>:<span style="font-weight: bold;">14</span> <span style="font-weight: bold;">2362</span> [Note] Event Scheduler: Loaded <span style="font-weight: bold;">0</span> events
<span style="font-weight: bold;">2017</span>-<span style="font-weight: bold;">03</span>-<span style="font-weight: bold;">29</span> <span style="font-weight: bold;">01</span>:<span style="font-weight: bold;">15</span>:<span style="font-weight: bold;">14</span> <span style="font-weight: bold;">2362</span> [Note] /usr/local/mysql/bin/mysqld: ready for connections.
Version: '5.6.35'  socket: '/mydata/data/mysql.sock'  port: <span style="font-weight: bold;">3306</span>  MySQL Community Server (GPL)</span></pre>
<div class="cnblogs_code_toolbar"><span class="cnblogs_code_copy"><a href="javascript:void(0);" onclick="copyCnblogsCode(this)" title="复制代码"><img src="//common.cnblogs.com/images/copycode.gif" alt="复制代码"></a></span></div></div>
<p><a name="blog3"></a></p>
<h1 id="auto_id_2">3.一般查询日志</h1>
<p class="01">查询日志分为一般查询日志和慢查询日志，它们是通过查询是否超出变量&nbsp;<span class="cnblogs_code">long_query_time</span>&nbsp;指定时间的值来判定的。在超时时间内完成的查询是一般查询，可以将其记录到一般查询日志中，<span style="color: #ff0000;"><strong>但是建议关闭这种日志（默认是关闭的）</strong></span>，超出时间的查询是慢查询，可以将其记录到慢查询日志中。</p>
<p class="01">使用"&nbsp;<span class="cnblogs_code"><span style="color: #008080;">--</span><span style="color: #008080;">general_log={0|1}</span></span>&nbsp;"来决定是否启用一般查询日志，使用"&nbsp;<span class="cnblogs_code"><span style="color: #008080;">--</span><span style="color: #008080;">general_log_file=file_name</span></span>&nbsp;"来指定查询日志的路径。不给定路径时默认的文件名以&nbsp;<span class="cnblogs_code">`hostname`.<span style="color: #ff00ff;">log</span></span>&nbsp;命名。</p>
<p class="01">和查询日志有关的变量有：</p>
<div class="cnblogs_Highlighter sh-gutter">
<div><div id="highlighter_149656" class="syntaxhighlighter  bash"><div class="toolbar"><span><a href="#" class="toolbar_item command_help help">?</a></span></div><table border="0" cellpadding="0" cellspacing="0"><tbody><tr><td class="gutter"><div class="line number1 index0 alt2">1</div><div class="line number2 index1 alt1">2</div></td><td class="code"><div class="container"><div class="line number1 index0 alt2"><code class="bash plain">long_query_time = 10 </code><code class="bash comments"># 指定慢查询超时时长，超出此时长的属于慢查询，会记录到慢查询日志中</code></div><div class="line number2 index1 alt1"><code class="bash plain">log_output={TABLE|FILE|NONE}&nbsp; </code><code class="bash comments"># 定义一般查询日志和慢查询日志的输出格式，不指定时默认为file</code></div></div></td></tr></tbody></table></div></div>
</div>
<p class="01">TABLE表示记录日志到表中，FILE表示记录日志到文件中，NONE表示不记录日志。只要这里指定为NONE，即使开启了一般查询日志和慢查询日志，也都不会有任何记录。</p>
<p class="01">和一般查询日志相关的变量有：</p>
<div class="cnblogs_Highlighter sh-gutter">
<div><div id="highlighter_378607" class="syntaxhighlighter  bash"><div class="toolbar"><span><a href="#" class="toolbar_item command_help help">?</a></span></div><table border="0" cellpadding="0" cellspacing="0"><tbody><tr><td class="gutter"><div class="line number1 index0 alt2">1</div><div class="line number2 index1 alt1">2</div><div class="line number3 index2 alt2">3</div></td><td class="code"><div class="container"><div class="line number1 index0 alt2"><code class="bash plain">general_log=off </code><code class="bash comments"># 是否启用一般查询日志，为全局变量，必须在global上修改。</code></div><div class="line number2 index1 alt1"><code class="bash plain">sql_log_off=off </code><code class="bash comments"># 在session级别控制是否启用一般查询日志，默认为off，即启用</code></div><div class="line number3 index2 alt2"><code class="bash plain">general_log_file=</code><code class="bash plain">/mydata/data/hostname</code><code class="bash plain">.log&nbsp; </code><code class="bash comments"># 默认是库文件路径下主机名加上.log</code></div></div></td></tr></tbody></table></div></div>
</div>
<p class="01">在MySQL 5.6以前的版本还有一个"log"变量也是决定是否开启一般查询日志的。在5.6版本开始已经废弃了该选项。</p>
<p class="01">默认没有开启一般查询日志，也不建议开启一般查询日志。此处打开该类型的日志，看看是如何记录一般查询日志的。</p>
<p class="01">首先开启一般查询日志。</p>
<div>
<div class="cnblogs_code">
<pre>mysql<span style="color: #808080;">&gt;</span> <span style="color: #0000ff;">set</span> <span style="color: #008000; font-weight: bold;">@@global</span>.general_log<span style="color: #808080;">=</span><span style="color: #800000; font-weight: bold;">1</span><span style="color: #000000;">;

</span><span style="color: #ff0000;">[</span><span style="color: #ff0000;">root@xuexi data</span><span style="color: #ff0000;">]</span># ll <span style="color: #808080;">*</span>.<span style="color: #ff00ff;">log</span>
<span style="color: #808080;">-</span>rw<span style="color: #808080;">-</span>rw<span style="color: #008080;">--</span><span style="color: #008080;">-- 1 mysql mysql 5423 Mar 20 16:29 mysqld.log</span>
<span style="color: #808080;">-</span>rw<span style="color: #808080;">-</span>rw<span style="color: #008080;">--</span><span style="color: #008080;">-- 1 mysql mysql  262 Mar 29 09:31 xuexi.log</span></pre>
</div>
</div>
<p class="01">执行几个语句。</p>
<div class="cnblogs_code">
<pre>mysql<span style="color: #808080;">&gt;</span> <span style="color: #0000ff;">select</span> host,<span style="color: #ff00ff;">user</span> <span style="color: #0000ff;">from</span> mysql.<span style="color: #ff00ff;">user</span><span style="color: #000000;">;
mysql</span><span style="color: #808080;">&gt;</span> show variables <span style="color: #808080;">like</span> "<span style="color: #808080;">%</span>error<span style="color: #808080;">%</span><span style="color: #000000;">";
mysql</span><span style="color: #808080;">&gt;</span> <span style="color: #0000ff;">insert</span> <span style="color: #0000ff;">into</span> ttt <span style="color: #0000ff;">values</span>(<span style="color: #800000; font-weight: bold;">233</span><span style="color: #000000;">);
mysql</span><span style="color: #808080;">&gt;</span> <span style="color: #0000ff;">create</span> <span style="color: #0000ff;">table</span> tt(id <span style="color: #0000ff;">int</span><span style="color: #000000;">);
mysql</span><span style="color: #808080;">&gt;</span> <span style="color: #0000ff;">set</span> <span style="color: #008000;">@a</span>:<span style="color: #808080;">=</span><span style="color: #800000; font-weight: bold;">3</span>;</pre>
</div>
<p class="01">查看一般查询日志的内容。</p>
<div class="cnblogs_code"><div class="cnblogs_code_toolbar"><span class="cnblogs_code_copy"><a href="javascript:void(0);" onclick="copyCnblogsCode(this)" title="复制代码"><img src="//common.cnblogs.com/images/copycode.gif" alt="复制代码"></a></span></div>
<pre><span style="color: #ff0000;">[</span><span style="color: #ff0000;">root@xuexi data</span><span style="color: #ff0000;">]</span># cat xuexi.<span style="color: #ff00ff;">log</span> 
<span style="color: #808080;">/</span>usr<span style="color: #808080;">/</span>local<span style="color: #808080;">/</span>mysql<span style="color: #808080;">/</span>bin<span style="color: #808080;">/</span>mysqld, Version: <span style="color: #800000; font-weight: bold;">5.6</span>.<span style="color: #800000; font-weight: bold;">35</span><span style="color: #808080;">-</span><span style="color: #ff00ff;">log</span> (MySQL Community Server (GPL)). started <span style="color: #0000ff;">with</span><span style="color: #000000;">:
Tcp port: </span><span style="color: #800000; font-weight: bold;">3306</span>  Unix socket: <span style="color: #808080;">/</span>mydata<span style="color: #808080;">/</span>data<span style="color: #808080;">/</span><span style="color: #000000;">mysql.sock
Time                Id Command    Argument
</span><span style="color: #800000; font-weight: bold;">180421</span> <span style="color: #800000; font-weight: bold;">20</span>:<span style="color: #800000; font-weight: bold;">04</span>:<span style="color: #800000; font-weight: bold;">41</span>     <span style="color: #800000; font-weight: bold;">13</span> Query      <span style="color: #0000ff;">select</span> <span style="color: #ff00ff;">user</span>,host <span style="color: #0000ff;">from</span> mysql.<span style="color: #ff00ff;">user</span>
<span style="color: #800000; font-weight: bold;">180421</span> <span style="color: #800000; font-weight: bold;">20</span>:<span style="color: #800000; font-weight: bold;">06</span>:<span style="color: #800000; font-weight: bold;">06</span>     <span style="color: #800000; font-weight: bold;">13</span> Query      show variables <span style="color: #808080;">like</span> "<span style="color: #808080;">%</span>error<span style="color: #808080;">%</span><span style="color: #000000;">"
</span><span style="color: #800000; font-weight: bold;">180421</span> <span style="color: #800000; font-weight: bold;">20</span>:<span style="color: #800000; font-weight: bold;">07</span>:<span style="color: #800000; font-weight: bold;">28</span>     <span style="color: #800000; font-weight: bold;">13</span> Query      <span style="color: #0000ff;">insert</span> <span style="color: #0000ff;">into</span> ttt <span style="color: #0000ff;">values</span>(<span style="color: #800000; font-weight: bold;">233</span><span style="color: #000000;">)
</span><span style="color: #800000; font-weight: bold;">180421</span> <span style="color: #800000; font-weight: bold;">20</span>:<span style="color: #800000; font-weight: bold;">11</span>:<span style="color: #800000; font-weight: bold;">47</span>     <span style="color: #800000; font-weight: bold;">13</span> Query      <span style="color: #0000ff;">create</span> <span style="color: #0000ff;">table</span> tt(id <span style="color: #0000ff;">int</span><span style="color: #000000;">)
</span><span style="color: #800000; font-weight: bold;">180421</span> <span style="color: #800000; font-weight: bold;">20</span>:<span style="color: #800000; font-weight: bold;">12</span>:<span style="color: #800000; font-weight: bold;">29</span>     <span style="color: #800000; font-weight: bold;">13</span> Query      <span style="color: #0000ff;">set</span> <span style="color: #008000;">@a</span>:<span style="color: #808080;">=</span><span style="color: #800000; font-weight: bold;">3</span></pre>
<div class="cnblogs_code_toolbar"><span class="cnblogs_code_copy"><a href="javascript:void(0);" onclick="copyCnblogsCode(this)" title="复制代码"><img src="//common.cnblogs.com/images/copycode.gif" alt="复制代码"></a></span></div></div>
<p class="01">由此可知，一般查询日志查询的不止是select语句，几乎所有的语句都会记录。</p>
<p><a name="blog4"></a></p>
<h1 id="auto_id_3">4.慢查询日志</h1>
<p class="01">查询超出变量&nbsp;<span class="cnblogs_code">long_query_time</span>&nbsp;指定时间值的为慢查询。但是查询获取锁(包括锁等待)的时间不计入查询时间内。</p>
<p class="01">mysql记录慢查询日志是在查询执行完毕且已经完全释放锁之后才记录的，因此慢查询日志记录的顺序和执行的SQL查询语句顺序可能会不一致(例如语句1先执行，查询速度慢，语句2后执行，但查询速度快，则语句2先记录)。</p>
<p class="01">注意，MySQL 5.1之后就支持微秒级的慢查询超时时长，对于DBA来说，一个查询运行0.5秒和运行0.05秒是非常不同的，前者可能索引使用错误或者走了表扫描，后者可能索引使用正确。</p>
<p class="01">另外，指定的慢查询超时时长表示的是超出这个时间的才算是慢查询，等于这个时间的不会记录。</p>
<p>和慢查询有关的变量：</p>
<div class="cnblogs_Highlighter sh-gutter">
<div><div id="highlighter_159854" class="syntaxhighlighter  bash"><div class="toolbar"><span><a href="#" class="toolbar_item command_help help">?</a></span></div><table border="0" cellpadding="0" cellspacing="0"><tbody><tr><td class="gutter"><div class="line number1 index0 alt2">1</div><div class="line number2 index1 alt1">2</div><div class="line number3 index2 alt2">3</div><div class="line number4 index3 alt1">4</div><div class="line number5 index4 alt2">5</div><div class="line number6 index5 alt1">6</div></td><td class="code"><div class="container"><div class="line number1 index0 alt2"><code class="bash plain">long_query_time=10 </code><code class="bash comments"># 指定慢查询超时时长(默认10秒)，超出此时长的属于慢查询</code></div><div class="line number2 index1 alt1"><code class="bash plain">log_output={TABLE|FILE|NONE} </code><code class="bash comments"># 定义一般查询日志和慢查询日志的输出格式，默认为file</code></div><div class="line number3 index2 alt2"><code class="bash plain">log_slow_queries={</code><code class="bash functions">yes</code><code class="bash plain">|no}&nbsp;&nbsp;&nbsp; </code><code class="bash comments"># 是否启用慢查询日志，默认不启用</code></div><div class="line number4 index3 alt1"><code class="bash plain">slow_query_log={1|ON|0|OFF}&nbsp; </code><code class="bash comments"># 也是是否启用慢查询日志，此变量和log_slow_queries修改一个另一个同时变化</code></div><div class="line number5 index4 alt2"><code class="bash plain">slow_query_log_file=</code><code class="bash plain">/mydata/data/hostname-slow</code><code class="bash plain">.log&nbsp; </code><code class="bash comments">#默认路径为库文件目录下主机名加上-slow.log</code></div><div class="line number6 index5 alt1"><code class="bash plain">log_queries_not_using_indexes=OFF </code><code class="bash comments"># 查询没有使用索引的时候是否也记入慢查询日志</code></div></div></td></tr></tbody></table></div></div>
</div>
<p class="01">现在启用慢查询日志。</p>
<div>
<div class="cnblogs_code">
<pre>mysql<span style="color: #808080;">&gt;</span> <span style="color: #0000ff;">set</span> <span style="color: #008000; font-weight: bold;">@@global</span>.slow_query_log<span style="color: #808080;">=</span><span style="color: #0000ff;">on</span>;</pre>
</div>
</div>
<p class="01">因为默认超时时长为10秒，所以进行一个10秒的查询。</p>
<div>
<div class="cnblogs_code">
<pre>mysql<span style="color: #808080;">&gt;</span> <span style="color: #0000ff;">select</span> sleep(<span style="color: #800000; font-weight: bold;">10</span>);</pre>
</div>
</div>
<p class="01">查看慢查询日志文件。这里看到虽然sleep了10秒，但是最后查询时间超出了847微秒，因此这里也记录了该查询。</p>
<div class="cnblogs_code"><div class="cnblogs_code_toolbar"><span class="cnblogs_code_copy"><a href="javascript:void(0);" onclick="copyCnblogsCode(this)" title="复制代码"><img src="//common.cnblogs.com/images/copycode.gif" alt="复制代码"></a></span></div>
<pre><span style="color: #ff0000;">[</span><span style="color: #ff0000;">root@xuexi data</span><span style="color: #ff0000;">]</span># cat xuexi<span style="color: #808080;">-</span>slow.<span style="color: #ff00ff;">log</span> 
<span style="color: #888888;">/usr/local/mysql/bin/mysqld, Version: <span style="font-weight: bold;">5.6</span>.<span style="font-weight: bold;">35</span>-log (MySQL Community Server (GPL)). started with:
Tcp port: <span style="font-weight: bold;">3306</span>  Unix socket: /mydata/data/mysql.sock
Time                 Id Command    Argument
# Time: <span style="font-weight: bold;">170329</span>  <span style="font-weight: bold;">9</span>:<span style="font-weight: bold;">55</span>:<span style="font-weight: bold;">58</span>
# User@Host: root[root] @ localhost []  Id:     <span style="font-weight: bold;">1</span>
# <span style="color: #ff0000;"><em><strong>Query_time: 10.000847</strong></em></span>  Lock_time: <span style="font-weight: bold;">0.000000</span> Rows_sent: <span style="font-weight: bold;">1</span>  Rows_examined: <span style="font-weight: bold;">0</span>
<strong><span style="color: #ff0000;">use test;
SET timestamp=1490752558;
select sleep(10);</span></strong></span></pre>
<div class="cnblogs_code_toolbar"><span class="cnblogs_code_copy"><a href="javascript:void(0);" onclick="copyCnblogsCode(this)" title="复制代码"><img src="//common.cnblogs.com/images/copycode.gif" alt="复制代码"></a></span></div></div>
<p>随着时间的推移，慢查询日志文件中的记录可能会变得非常多，这对于分析查询来说是非常困难的。好在提供了一个专门归类慢查询日志的工具mysqldumpslow。</p>
<div class="cnblogs_Highlighter sh-gutter">
<div><div id="highlighter_565702" class="syntaxhighlighter  bash"><div class="toolbar"><span><a href="#" class="toolbar_item command_help help">?</a></span></div><table border="0" cellpadding="0" cellspacing="0"><tbody><tr><td class="gutter"><div class="line number1 index0 alt2">1</div><div class="line number2 index1 alt1">2</div><div class="line number3 index2 alt2">3</div><div class="line number4 index3 alt1">4</div><div class="line number5 index4 alt2">5</div><div class="line number6 index5 alt1">6</div></td><td class="code"><div class="container"><div class="line number1 index0 alt2"><code class="bash plain">[root@xuexi data]</code><code class="bash comments"># mysqldumpslow --help</code></div><div class="line number2 index1 alt1"><code class="bash spaces">&nbsp;&nbsp;</code><code class="bash plain">-d&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; debug&nbsp; </code></div><div class="line number3 index2 alt2"><code class="bash spaces">&nbsp;&nbsp;</code><code class="bash plain">-</code><code class="bash functions">v</code>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <code class="bash plain">verbose：显示详细信息</code></div><div class="line number4 index3 alt1"><code class="bash spaces">&nbsp;&nbsp;</code><code class="bash plain">-t NUM&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; just show the </code><code class="bash functions">top</code> <code class="bash plain">n queries：仅显示前n条查询</code></div><div class="line number5 index4 alt2"><code class="bash spaces">&nbsp;&nbsp;</code><code class="bash plain">-a&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; don</code><code class="bash string">'t abstract all numbers to N and strings to '</code><code class="bash plain">S'：归类时不要使用N替换数字，S替换字符串</code></div><div class="line number6 index5 alt1"><code class="bash spaces">&nbsp;&nbsp;</code><code class="bash plain">-g PATTERN&nbsp;&nbsp; </code><code class="bash functions">grep</code><code class="bash plain">: only consider stmts that include this string：通过</code><code class="bash functions">grep</code><code class="bash plain">来筛选</code><code class="bash functions">select</code><code class="bash plain">语句。</code></div></div></td></tr></tbody></table></div></div>
</div>
<p class="01">该工具归类的时候，默认会将<strong>同文本但变量值不同的查询语句视为同一类，并使用N代替其中的数值变量，使用S代替其中的字符串变量</strong>。可以使用-a来禁用这种替换。如：</p>
<div class="cnblogs_code"><div class="cnblogs_code_toolbar"><span class="cnblogs_code_copy"><a href="javascript:void(0);" onclick="copyCnblogsCode(this)" title="复制代码"><img src="//common.cnblogs.com/images/copycode.gif" alt="复制代码"></a></span></div>
<pre><span style="color: #ff0000;">[</span><span style="color: #ff0000;">root@xuexi data</span><span style="color: #ff0000;">]</span># mysqldumpslow xuexi<span style="color: #808080;">-</span>slow.<span style="color: #ff00ff;">log</span> 
Reading mysql slow query <span style="color: #ff00ff;">log</span> <span style="color: #0000ff;">from</span> xuexi<span style="color: #808080;">-</span>slow.<span style="color: #ff00ff;">log</span>
<span style="color: #ff00ff;">Count</span>: <span style="color: #800000; font-weight: bold;">1</span>  Time<span style="color: #808080;">=</span><span style="color: #800000; font-weight: bold;">10</span>.00s (10s)  Lock<span style="color: #808080;">=</span><span style="color: #800000; font-weight: bold;">0</span>.00s (0s)  Rows<span style="color: #808080;">=</span><span style="color: #800000; font-weight: bold;">1.0</span> (<span style="color: #800000; font-weight: bold;">1</span>), root<span style="color: #ff0000;">[</span><span style="color: #ff0000;">root</span><span style="color: #ff0000;">]</span><span style="color: #008000;">@localhost</span>
<span style="color: #0000ff;">select</span> sleep<span style="color: #ff0000; background-color: #008080;"><strong>(N)</strong></span>

<span style="color: #ff0000;">[</span><span style="color: #ff0000;">root@xuexi data</span><span style="color: #ff0000;">]</span>#  mysqldumpslow <span style="color: #808080;">-</span>a xuexi<span style="color: #808080;">-</span>slow.<span style="color: #ff00ff;">log</span>   
<span style="color: #000000;">Reading mysql slow query log from xuexi-slow.log
Count: <span style="font-weight: bold;">1</span>  Time=<span style="font-weight: bold;">10</span>.00s (10s)  Lock=<span style="font-weight: bold;">0</span>.00s (0s)  Rows=<span style="font-weight: bold;">1.0</span> (<span style="font-weight: bold;">1</span>), root[root]@localhost
select sleep<span style="background-color: #008080; color: #ff0000;"><strong>(10)</strong></span></span></pre>
<div class="cnblogs_code_toolbar"><span class="cnblogs_code_copy"><a href="javascript:void(0);" onclick="copyCnblogsCode(this)" title="复制代码"><img src="//common.cnblogs.com/images/copycode.gif" alt="复制代码"></a></span></div></div>
<p>显然，这里归类后的结果只是精确到0.01秒的，如果想要显示及其精确的秒数，则使用-d选项启用调试功能。</p>
<div class="cnblogs_code"><div class="cnblogs_code_toolbar"><span class="cnblogs_code_copy"><a href="javascript:void(0);" onclick="copyCnblogsCode(this)" title="复制代码"><img src="//common.cnblogs.com/images/copycode.gif" alt="复制代码"></a></span></div>
<pre><span style="color: #ff0000;">[</span><span style="color: #ff0000;">root@xuexi data</span><span style="color: #ff0000;">]</span>#  mysqldumpslow <span style="color: #808080;">-</span>d xuexi<span style="color: #808080;">-</span>slow.<span style="color: #ff00ff;">log</span><span style="color: #000000;">   
Reading mysql slow query </span><span style="color: #ff00ff;">log</span> <span style="color: #0000ff;">from</span> xuexi<span style="color: #808080;">-</span>slow.<span style="color: #ff00ff;">log</span>
<span style="color: #ff0000;">[</span><span style="color: #ff0000;">[/usr/local/mysql/bin/mysqld, Version: 5.6.35-log (MySQL Community Server (GPL)). started with:
Tcp port: 3306  Unix socket: /mydata/data/mysql.sock
Time                 Id Command    Argument
# Time: 170329  9:55:58
# User@Host: root[root</span><span style="color: #ff0000;">]</span> @ localhost <span style="color: #ff0000;">[]</span>  Id:     <span style="color: #800000; font-weight: bold;">1</span><span style="color: #000000;">
# Query_time: </span><span style="color: #800000; font-weight: bold;">10.000847</span>  Lock_time: <span style="color: #800000; font-weight: bold;">0.000000</span> Rows_sent: <span style="color: #800000; font-weight: bold;">1</span>  Rows_examined: <span style="color: #800000; font-weight: bold;">0</span>
<span style="color: #0000ff;">use</span><span style="color: #000000;"> test;
</span><span style="color: #0000ff;">SET</span> <span style="color: #0000ff;">timestamp</span><span style="color: #808080;">=</span><span style="color: #800000; font-weight: bold;">1490752558</span><span style="color: #000000;">;
</span><span style="color: #0000ff;">select</span> sleep(<span style="color: #800000; font-weight: bold;">10</span><span style="color: #000000;">);
]]
</span><span style="color: #808080;">&lt;&lt;&gt;&gt;</span>
<span style="color: #808080;">&lt;&lt;</span># Time: <span style="color: #800000; font-weight: bold;">170329</span>  <span style="color: #800000; font-weight: bold;">9</span>:<span style="color: #800000; font-weight: bold;">55</span>:<span style="color: #800000; font-weight: bold;">58</span><span style="color: #000000;">
# </span><span style="color: #ff00ff;">User</span><span style="color: #008000;">@Host</span>: root<span style="color: #ff0000;">[</span><span style="color: #ff0000;">root</span><span style="color: #ff0000;">]</span> @ localhost <span style="color: #ff0000;">[]</span>  Id:     <span style="color: #800000; font-weight: bold;">1</span><span style="color: #000000;">
# Query_time: </span><span style="color: #800000; font-weight: bold;">10.000847</span>  Lock_time: <span style="color: #800000; font-weight: bold;">0.000000</span> Rows_sent: <span style="color: #800000; font-weight: bold;">1</span>  Rows_examined: <span style="color: #800000; font-weight: bold;">0</span>
<span style="color: #0000ff;">use</span><span style="color: #000000;"> test;
</span><span style="color: #0000ff;">SET</span> <span style="color: #0000ff;">timestamp</span><span style="color: #808080;">=</span><span style="color: #800000; font-weight: bold;">1490752558</span><span style="color: #000000;">;
</span><span style="color: #0000ff;">select</span> sleep(<span style="color: #800000; font-weight: bold;">10</span><span style="color: #000000;">);
</span><span style="color: #808080;">&gt;&gt;</span> at <span style="color: #808080;">/</span>usr<span style="color: #808080;">/</span>local<span style="color: #808080;">/</span>mysql<span style="color: #808080;">/</span>bin<span style="color: #808080;">/</span>mysqldumpslow line <span style="color: #800000; font-weight: bold;">97</span>, <span style="color: #808080;">&lt;&gt;</span> chunk <span style="color: #800000; font-weight: bold;">1</span><span style="color: #000000;">.
</span><span style="color: #ff0000;">[</span><span style="color: #ff0000;">[# Time: 170329  9:55:58
# User@Host: root[root</span><span style="color: #ff0000;">]</span> @ localhost <span style="color: #ff0000;">[]</span>  Id:     <span style="color: #800000; font-weight: bold;">1</span><span style="color: #000000;">
# Query_time: </span><span style="color: #800000; font-weight: bold;">10.000847</span>  Lock_time: <span style="color: #800000; font-weight: bold;">0.000000</span> Rows_sent: <span style="color: #800000; font-weight: bold;">1</span>  Rows_examined: <span style="color: #800000; font-weight: bold;">0</span>
<span style="color: #0000ff;">use</span><span style="color: #000000;"> test;
</span><span style="color: #0000ff;">SET</span> <span style="color: #0000ff;">timestamp</span><span style="color: #808080;">=</span><span style="color: #800000; font-weight: bold;">1490752558</span><span style="color: #000000;">;
</span><span style="color: #0000ff;">select</span> sleep(<span style="color: #800000; font-weight: bold;">10</span><span style="color: #000000;">);
]]
{{  </span><span style="color: #0000ff;">select</span><span style="color: #000000;"> sleep(N)}}

</span><span style="color: #ff00ff;">Count</span>: <span style="color: #800000; font-weight: bold;">1</span>  Time<span style="color: #808080;">=</span><span style="color: #800000; font-weight: bold;">10</span>.00s (10s)  Lock<span style="color: #808080;">=</span><span style="color: #800000; font-weight: bold;">0</span>.00s (0s)  Rows<span style="color: #808080;">=</span><span style="color: #800000; font-weight: bold;">1.0</span> (<span style="color: #800000; font-weight: bold;">1</span>), root<span style="color: #ff0000;">[</span><span style="color: #ff0000;">root</span><span style="color: #ff0000;">]</span><span style="color: #008000;">@localhost</span>
<span style="color: #0000ff;">select</span> sleep(N)</pre>
<div class="cnblogs_code_toolbar"><span class="cnblogs_code_copy"><a href="javascript:void(0);" onclick="copyCnblogsCode(this)" title="复制代码"><img src="//common.cnblogs.com/images/copycode.gif" alt="复制代码"></a></span></div></div>
<p class="01">慢查询在SQL语句调优的时候非常有用，应该将它启用起来，且应该让慢查询阈值尽量小，例如1秒甚至低于1秒。就像一天执行上千次的1秒语句，和一天执行几次的20秒语句，显然更值得去优化这个1秒的语句。</p>
<p><a name="blog5"></a></p>
<h1 id="auto_id_4">5.二进制日志</h1>
<p><a name="blog5.1"></a></p>
<h2 id="auto_id_5">5.1 二进制日志文件</h2>
<p class="01">二进制日志包含了<span style="color: #ff0000;"><strong>引起或可能引起数据库改变</strong></span>(如delete语句但没有匹配行)的事件信息，但绝不会包括select和show这样的查询语句。语句以"事件"的形式保存，所以包含了时间、事件开始和结束位置等信息。</p>
<p class="01">二进制日志是<span style="color: #ff0000;"><strong>以事件形式记录的，不是事务日志</strong><strong>(</strong><strong>但可能是基于事务来记录二进制日志)</strong></span>，不代表它只记录innodb日志，myisam表也一样有二进制日志。</p>
<p class="01">对于事务表的操作，二进制日志<span style="color: #ff0000;"><strong>只在事务提交的时候一次性写入</strong><strong>(</strong><strong>基于事务的innodb</strong><strong>二进制日志)，提交前的每个二进制日志记录都先cache，提交时写入</strong></span>。对于非事务表的操作，每次执行完语句就直接写入。</p>
<p>MariaDB/MySQL默认没有启动二进制日志，要启用二进制日志使用&nbsp;<span class="cnblogs_code"><span style="color: #008080;">--</span><span style="color: #008080;">log-bin=[on|off|file_name]</span></span>&nbsp;选项指定，如果没有给定file_name，则默认为datadir下的主机名加"-bin"，并在后面跟上一串数字表示日志序列号，如果给定的日志文件中包含了后缀(logname.suffix)将忽略后缀部分。</p>
<p><img src="https://images2018.cnblogs.com/blog/733013/201805/733013-20180507084125816-1681048114.png" alt=""></p>
<p class="01">或者在配置文件中的[mysqld]部分设置log-bin也可以。注意：对于mysql 5.7，直接启动binlog可能会导致mysql服务启动失败，这时需要在配置文件中的mysqld为mysql实例分配server_id。</p>
<div class="cnblogs_Highlighter sh-gutter">
<div><div id="highlighter_484446" class="syntaxhighlighter  bash"><div class="toolbar"><span><a href="#" class="toolbar_item command_help help">?</a></span></div><table border="0" cellpadding="0" cellspacing="0"><tbody><tr><td class="gutter"><div class="line number1 index0 alt2">1</div><div class="line number2 index1 alt1">2</div><div class="line number3 index2 alt2">3</div></td><td class="code"><div class="container"><div class="line number1 index0 alt2"><code class="bash plain">[mysqld]</code></div><div class="line number2 index1 alt1"><code class="bash comments"># server_id=1234</code></div><div class="line number3 index2 alt2"><code class="bash plain">log-bin=[on|filename]</code></div></div></td></tr></tbody></table></div></div>
</div>
<p class="01">mysqld还<span style="color: #ff0000;"><strong>创建一个二进制日志索引文件</strong></span>，当二进制日志文件滚动的时候会向该文件中写入对应的信息。所以该文件包含所有使用的二进制日志文件的文件名。默认情况下该文件与二进制日志文件的文件名相同，扩展名为'.index'。要指定该文件的文件名使用&nbsp;<span class="cnblogs_code"><span style="color: #008080;">--</span><span style="color: #008080;">log-bin-index[=file_name]</span></span>&nbsp;选项。当mysqld在运行时不应手动编辑该文件，免得mysqld变得混乱。</p>
<p class="01">当重启mysql服务或刷新日志或者达到日志最大值时，将滚动二进制日志文件，滚动日志时只修改日志文件名的数字序列部分。</p>
<p class="01">二进制日志文件的最大值通过变量&nbsp;<span class="cnblogs_code">max_binlog_size</span>&nbsp;设置(默认值为1G)。但由于二进制日志可能是基于事务来记录的(如innodb表类型)，而事务是绝对不可能也不应该跨文件记录的，如果正好二进制日志文件达到了最大值但事务还没有提交则不会滚动日志，而是继续增大日志，所以&nbsp;<span class="cnblogs_code">max_binlog_size</span>&nbsp;指定的值和实际的二进制日志大小不一定相等。</p>
<p class="01">因为二进制日志文件增长迅速，但官方说明因此而损耗的性能小于1%，且二进制目的是为了恢复定点数据库和主从复制，所以出于安全和功能考虑，<span style="color: #ff0000;"><strong>极不建议将二进制日志和</strong><strong>datadir</strong><strong>放在同一磁盘上</strong></span>。</p>
<p><a name="blog5.2"></a></p>
<h2 id="auto_id_6">5.2 查看二进制日志</h2>
<p class="01">MySQL中查看二进制日志的方法主要有几种。</p>
<p class="01">1.使用mysqlbinlog工具。</p>
<p class="01">2.使用show显示对应的信息。</p>
<div class="cnblogs_Highlighter sh-gutter">
<div><div id="highlighter_964981" class="syntaxhighlighter  bash"><div class="toolbar"><span><a href="#" class="toolbar_item command_help help">?</a></span></div><table border="0" cellpadding="0" cellspacing="0"><tbody><tr><td class="gutter"><div class="line number1 index0 alt2">1</div><div class="line number2 index1 alt1">2</div><div class="line number3 index2 alt2">3</div></td><td class="code"><div class="container"><div class="line number1 index0 alt2"><code class="bash plain">SHOW {BINARY | MASTER} LOGS&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </code><code class="bash comments"># 查看使用了哪些日志文件</code></div><div class="line number2 index1 alt1"><code class="bash plain">SHOW BINLOG EVENTS [IN </code><code class="bash string">'log_name'</code><code class="bash plain">] [FROM pos]&nbsp;&nbsp; </code><code class="bash comments"># 查看日志中进行了哪些操作</code></div><div class="line number3 index2 alt2"><code class="bash plain">SHOW MASTER STATUS&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </code><code class="bash comments"># 显式主服务器中的二进制日志信息</code></div></div></td></tr></tbody></table></div></div>
</div>
<p><a name="blog5.2.1"></a></p>
<h3 id="auto_id_7">5.2.1 mysqlbinlog</h3>
<p class="01">二进制日志可以使用mysqlbinlog命令查看。</p>
<div class="cnblogs_Highlighter sh-gutter">
<div><div id="highlighter_18591" class="syntaxhighlighter  bash"><div class="toolbar"><span><a href="#" class="toolbar_item command_help help">?</a></span></div><table border="0" cellpadding="0" cellspacing="0"><tbody><tr><td class="gutter"><div class="line number1 index0 alt2">1</div></td><td class="code"><div class="container"><div class="line number1 index0 alt2"><code class="bash plain">mysqlbinlog [option] log-file1 log-file2...</code></div></div></td></tr></tbody></table></div></div>
</div>
<p>以下是常用的几个选项：</p>
<div class="cnblogs_Highlighter sh-gutter">
<div><div id="highlighter_527941" class="syntaxhighlighter  bash"><div class="toolbar"><span><a href="#" class="toolbar_item command_help help">?</a></span></div><table border="0" cellpadding="0" cellspacing="0"><tbody><tr><td class="gutter"><div class="line number1 index0 alt2">1</div><div class="line number2 index1 alt1">2</div><div class="line number3 index2 alt2">3</div><div class="line number4 index3 alt1">4</div><div class="line number5 index4 alt2">5</div><div class="line number6 index5 alt1">6</div><div class="line number7 index6 alt2">7</div><div class="line number8 index7 alt1">8</div></td><td class="code"><div class="container"><div class="line number1 index0 alt2"><code class="bash plain">-d,--database=name：只查看指定数据库的日志操作</code></div><div class="line number2 index1 alt1"><code class="bash plain">-o,--offset=</code><code class="bash comments">#：忽略掉日志中的前n个操作命令</code></div><div class="line number3 index2 alt2"><code class="bash plain">-r,--result-</code><code class="bash functions">file</code><code class="bash plain">=name：将输出的日志信息输出到指定的文件中，使用重定向也一样可以。</code></div><div class="line number4 index3 alt1"><code class="bash plain">-s,--short-form：显示简单格式的日志，只记录一些普通的语句，会省略掉一些额外的信息如位置信息和时间信息以及基于行的日志。可以用来调试，生产环境千万不可使用</code></div><div class="line number5 index4 alt2"><code class="bash plain">--</code><code class="bash functions">set</code><code class="bash plain">-charset=char_name：在输出日志信息到文件中时，在文件第一行加上</code><code class="bash functions">set</code> <code class="bash plain">names char_name</code></div><div class="line number6 index5 alt1"><code class="bash plain">--start-datetime,--stop-datetime：指定输出开始时间和结束时间内的所有日志信息</code></div><div class="line number7 index6 alt2"><code class="bash plain">--start-position=</code><code class="bash comments">#,--stop-position=#：指定输出开始位置和结束位置内的所有日志信息</code></div><div class="line number8 index7 alt1"><code class="bash plain">-</code><code class="bash functions">v</code><code class="bash plain">,-vv：显示更详细信息，基于row的日志默认不会显示出来，此时使用-</code><code class="bash functions">v</code><code class="bash plain">或-vv可以查看</code></div></div></td></tr></tbody></table></div></div>
</div>
<p class="01">在进行测试之前，先对日志进行一次刷新，以方便解释二进制日志的信息。</p>
<div>
<div class="cnblogs_code">
<pre>shell<span style="color: #808080;">&gt;</span> mysqladmin <span style="color: #808080;">-</span>uroot <span style="color: #808080;">-</span>p refresh</pre>
</div>
</div>
<p class="01">假设现在的日志文件是mysql-bin.000001，里面暂时只有一些初始信息，没有记录任何操作过的记录。</p>
<p>下面是每个二进制日志文件的初始信息。可以看到记录了时间和位置信息(at 4)。</p>
<div class="cnblogs_code"><div class="cnblogs_code_toolbar"><span class="cnblogs_code_copy"><a href="javascript:void(0);" onclick="copyCnblogsCode(this)" title="复制代码"><img src="//common.cnblogs.com/images/copycode.gif" alt="复制代码"></a></span></div>
<pre><span style="color: #ff0000;">[</span><span style="color: #ff0000;">root@xuexi data</span><span style="color: #ff0000;">]</span># mysqlbinlog mysql<span style="color: #808080;">-</span>bin.<span style="color: #800000; font-weight: bold;">000001</span> 
<span style="color: #000000;">/*!50530 SET @@SESSION.PSEUDO_SLAVE_MODE=1*/;
/*!40019 SET @@session.max_insert_delayed_threads=0*/;
/*!50003 SET @OLD_COMPLETION_TYPE=@@COMPLETION_TYPE,COMPLETION_TYPE=0*/;
DELIMITER /*!*/;
<span style="color: #ff0000;"><em><strong># at 4</strong></em></span>
#<span style="font-weight: bold;">170329</span>  <span style="font-weight: bold;">2</span>:<span style="font-weight: bold;">18</span>:<span style="font-weight: bold;">10</span> server id <span style="font-weight: bold;">1</span>  end_log_pos <span style="font-weight: bold;">120</span> CRC32 <span style="font-weight: bold;">0x40f62523</span>  Start: binlog v <span style="font-weight: bold;">4</span>, server v <span style="font-weight: bold;">5.6</span>.<span style="font-weight: bold;">35</span>-log created <span style="font-weight: bold;">170329</span>  <span style="font-weight: bold;">2</span>:<span style="font-weight: bold;">18</span>:<span style="font-weight: bold;">10</span> at startup
# Warning: this binlog is either in use or was not closed properly.
ROLLBACK/*!*/;
BINLOG '
4qjaWA8BAAAAdAAAAHgAAAABAAQANS42LjM1LWxvZwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAAAAAADiqNpYEzgNAAgAEgAEBAQEEgAAXAAEGggAAAAICAgCAAAACgoKGRkAASMl
9kA=
'/*!*/;
DELIMITER ;
# End of log file
ROLLBACK /* added by mysqlbinlog */;
/*!50003 SET COMPLETION_TYPE=@OLD_COMPLETION_TYPE*/;
/*!50530 SET @@SESSION.PSEUDO_SLAVE_MODE=0*/;</span></pre>
<div class="cnblogs_code_toolbar"><span class="cnblogs_code_copy"><a href="javascript:void(0);" onclick="copyCnblogsCode(this)" title="复制代码"><img src="//common.cnblogs.com/images/copycode.gif" alt="复制代码"></a></span></div></div>
<p class="01">现在在数据库中执行下面的操作：</p>
<div class="cnblogs_code">
<pre><span style="color: #0000ff;">use</span><span style="color: #000000;"> test;
</span><span style="color: #0000ff;">create</span> <span style="color: #0000ff;">table</span> student(studentid <span style="color: #0000ff;">int</span> <span style="color: #808080;">not</span> <span style="color: #0000ff;">null</span> <span style="color: #0000ff;">primary</span> <span style="color: #0000ff;">key</span>,name <span style="color: #0000ff;">varchar</span>(<span style="color: #800000; font-weight: bold;">30</span>) <span style="color: #808080;">not</span> <span style="color: #0000ff;">null</span>,gender enum(<span style="color: #ff0000;">'</span><span style="color: #ff0000;">female</span><span style="color: #ff0000;">'</span>,<span style="color: #ff0000;">'</span><span style="color: #ff0000;">mail</span><span style="color: #ff0000;">'</span><span style="color: #000000;">));
</span><span style="color: #0000ff;">alter</span> <span style="color: #0000ff;">table</span> student change gender gender enum(<span style="color: #ff0000;">'</span><span style="color: #ff0000;">female</span><span style="color: #ff0000;">'</span>,<span style="color: #ff0000;">'</span><span style="color: #ff0000;">male</span><span style="color: #ff0000;">'</span><span style="color: #000000;">);
</span><span style="color: #0000ff;">insert</span> <span style="color: #0000ff;">into</span> student <span style="color: #0000ff;">values</span>(<span style="color: #800000; font-weight: bold;">1</span>,<span style="color: #ff0000;">'</span><span style="color: #ff0000;">malongshuai</span><span style="color: #ff0000;">'</span>,<span style="color: #ff0000;">'</span><span style="color: #ff0000;">male</span><span style="color: #ff0000;">'</span>),(<span style="color: #800000; font-weight: bold;">2</span>,<span style="color: #ff0000;">'</span><span style="color: #ff0000;">gaoxiaofang</span><span style="color: #ff0000;">'</span>,<span style="color: #ff0000;">'</span><span style="color: #ff0000;">female</span><span style="color: #ff0000;">'</span>);</pre>
</div>
<p class="01">再查看二进制日志信息。</p>
<div class="cnblogs_code"><div class="cnblogs_code_toolbar"><span class="cnblogs_code_copy"><a href="javascript:void(0);" onclick="copyCnblogsCode(this)" title="复制代码"><img src="//common.cnblogs.com/images/copycode.gif" alt="复制代码"></a></span></div>
<pre><span style="color: #ff0000;">[</span><span style="color: #ff0000;">root@xuexi data</span><span style="color: #ff0000;">]</span># mysqlbinlog mysql<span style="color: #808080;">-</span>bin.<span style="color: #800000; font-weight: bold;">000001</span> 
<span style="color: #000000;">/*!50530 SET @@SESSION.PSEUDO_SLAVE_MODE=1*/;
/*!40019 SET @@session.max_insert_delayed_threads=0*/;
/*!50003 SET @OLD_COMPLETION_TYPE=@@COMPLETION_TYPE,COMPLETION_TYPE=0*/;
DELIMITER /*!*/;
# at <span style="font-weight: bold;">4</span>
#<span style="font-weight: bold;">170329</span>  <span style="font-weight: bold;">2</span>:<span style="font-weight: bold;">18</span>:<span style="font-weight: bold;">10</span> server id <span style="font-weight: bold;">1</span>  end_log_pos <span style="font-weight: bold;">120</span> CRC32 <span style="font-weight: bold;">0x40f62523</span>  Start: binlog v <span style="font-weight: bold;">4</span>, server v <span style="font-weight: bold;">5.6</span>.<span style="font-weight: bold;">35</span>-log created <span style="font-weight: bold;">170329</span>  <span style="font-weight: bold;">2</span>:<span style="font-weight: bold;">18</span>:<span style="font-weight: bold;">10</span> at startup
# Warning: this binlog is either in use or was not closed properly.
ROLLBACK/*!*/;
BINLOG '
4qjaWA8BAAAAdAAAAHgAAAABAAQANS42LjM1LWxvZwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAAAAAADiqNpYEzgNAAgAEgAEBAQEEgAAXAAEGggAAAAICAgCAAAACgoKGRkAASMl
9kA=
'/*!*/;
<em><strong><span style="color: #ff0000;"># at 120</span></strong></em>
#<span style="font-weight: bold;">170329</span> <span style="color: #ff0000;"> <span style="font-weight: bold;">5</span>:<span style="font-weight: bold;">20</span>:<span style="font-weight: bold;">00</span></span> server id <span style="font-weight: bold;">1</span>  <span style="color: #ff0000;"><strong>end_log_pos 305</strong></span> CRC32 <span style="font-weight: bold;">0xbac43912</span>  Query   thread_id=<span style="font-weight: bold;">1</span>     exec_time=<span style="font-weight: bold;">0</span>     error_code=<span style="font-weight: bold;">0</span>
use `test`/*!*/;
SET TIMESTAMP=<span style="font-weight: bold;">1490736000</span>/*!*/;
SET <span style="font-weight: bold;">@@session</span>.pseudo_thread_id=<span style="font-weight: bold;">1</span>/*!*/;
SET <span style="font-weight: bold;">@@session</span>.foreign_key_checks=<span style="font-weight: bold;">1</span>, <span style="font-weight: bold;">@@session</span>.sql_auto_is_null=<span style="font-weight: bold;">0</span>, <span style="font-weight: bold;">@@session</span>.unique_checks=<span style="font-weight: bold;">1</span>, <span style="font-weight: bold;">@@session</span>.autocommit=<span style="font-weight: bold;">1</span>/*!*/;
SET <span style="font-weight: bold;">@@session</span>.sql_mode=<span style="font-weight: bold;">1075838976</span>/*!*/;
SET <span style="font-weight: bold;">@@session</span>.auto_increment_increment=<span style="font-weight: bold;">1</span>, <span style="font-weight: bold;">@@session</span>.auto_increment_offset=<span style="font-weight: bold;">1</span>/*!*/;
/*!\C utf8 *//*!*/;
SET <span style="font-weight: bold;">@@session</span>.character_set_client=<span style="font-weight: bold;">33</span>,<span style="font-weight: bold;">@@session</span>.collation_connection=<span style="font-weight: bold;">33</span>,<span style="font-weight: bold;">@@session</span>.collation_server=<span style="font-weight: bold;">8</span>/*!*/;
SET <span style="font-weight: bold;">@@session</span>.lc_time_names=<span style="font-weight: bold;">0</span>/*!*/;
SET <span style="font-weight: bold;">@@session</span>.collation_database=DEFAULT/*!*/;
create table student(studentid int not null primary key,name varchar(<span style="font-weight: bold;">30</span>) not null,gender enum('female','mail'))
/*!*/;
<strong><span style="color: #ff0000;"># at 305</span></strong>
#<span style="font-weight: bold;">170329</span>  <span style="color: #ff0000;"><span style="font-weight: bold;">5</span>:<span style="font-weight: bold;">21</span>:<span style="font-weight: bold;">21</span></span> server id <span style="font-weight: bold;">1</span>  <strong><span style="color: #ff0000;">end_log_pos 441</span></strong> CRC32 <span style="font-weight: bold;">0xde67f702</span>  Query   thread_id=<span style="font-weight: bold;">1</span>     exec_time=<span style="font-weight: bold;">0</span>     error_code=<span style="font-weight: bold;">0</span>
SET TIMESTAMP=<span style="font-weight: bold;">1490736081</span>/*!*/;
alter table student change gender gender enum('female','male')
/*!*/;
<strong><span style="color: #ff0000;"># at 441</span></strong>
#<span style="font-weight: bold;">170329</span>  <span style="color: #ff0000;"><span style="font-weight: bold;">5</span>:<span style="font-weight: bold;">21</span>:<span style="font-weight: bold;">33</span></span> server id <span style="font-weight: bold;">1</span>  <span style="color: #ff0000;"><strong>end_log_pos 520</strong></span> CRC32 <span style="font-weight: bold;">0x05a9c5a1</span>  Query   thread_id=<span style="font-weight: bold;">1</span>     exec_time=<span style="font-weight: bold;">0</span>     error_code=<span style="font-weight: bold;">0</span>
SET TIMESTAMP=<span style="font-weight: bold;">1490736093</span>/*!*/;
BEGIN
/*!*/;
<strong><span style="color: #ff0000;"># at 520</span></strong>
#<span style="font-weight: bold;">170329</span>  <span style="color: #ff0000;"><span style="font-weight: bold;">5</span>:<span style="font-weight: bold;">21</span>:<span style="font-weight: bold;">33</span></span> server id <span style="font-weight: bold;">1</span>  <span style="color: #ff0000;"><strong>end_log_pos 671</strong></span> CRC32 <span style="font-weight: bold;">0xad9e7dc8</span>  Query   thread_id=<span style="font-weight: bold;">1</span>     exec_time=<span style="font-weight: bold;">0</span>     error_code=<span style="font-weight: bold;">0</span>
SET TIMESTAMP=<span style="font-weight: bold;">1490736093</span>/*!*/;
insert into student values(<span style="font-weight: bold;">1</span>,'malongshuai','male'),(<span style="font-weight: bold;">2</span>,'gaoxiaofang','female')
/*!*/;
<strong><span style="color: #ff0000;"># at 671</span></strong>
#<span style="font-weight: bold;">170329</span> <span style="color: #ff0000;"> <span style="font-weight: bold;">5</span>:<span style="font-weight: bold;">21</span>:<span style="font-weight: bold;">33</span></span> server id <span style="font-weight: bold;">1</span>  <strong><span style="color: #ff0000;">end_log_pos 702</span></strong> CRC32 <span style="font-weight: bold;">0xb69b0f7d</span>  Xid = <span style="font-weight: bold;">32</span>
COMMIT/*!*/;
DELIMITER ;
# End of log file
ROLLBACK /* added by mysqlbinlog */;
/*!50003 SET COMPLETION_TYPE=@OLD_COMPLETION_TYPE*/;
/*!50530 SET @@SESSION.PSEUDO_SLAVE_MODE=0*/;</span> </pre>
<div class="cnblogs_code_toolbar"><span class="cnblogs_code_copy"><a href="javascript:void(0);" onclick="copyCnblogsCode(this)" title="复制代码"><img src="//common.cnblogs.com/images/copycode.gif" alt="复制代码"></a></span></div></div>
<p class="01">将上述信息整理为下图：其中timestamp记录的是从1970-01-01到现在的总秒数时间戳，可以使用&nbsp;<span class="cnblogs_code"><span style="color: #0000ff;">date</span> -d <span style="color: #800000;">'</span><span style="color: #800000;">@1490736093</span><span style="color: #800000;">'</span></span>&nbsp;转换。</p>
<p><img src="https://images2018.cnblogs.com/blog/733013/201805/733013-20180507085958196-1633846460.png" alt=""></p>
<ul>
<li class="01">位置0-120记录的是二进制日志的一些固定信息。</li>
<li class="01">位置120-305记录的是use和create table语句，语句的记录时间为5:20:00。但注意，这里的use不是执行的use语句，而是MySQL发现要操作的数据库为test，而自动进行的操作并记录下来。人为的use语句是不会记录的。</li>
<li class="01">位置305-441记录的是alter table语句，语句的记录时间为5:20:21。</li>
<li>位置441-702记录的是insert操作，因为该操作是DML语句，因此记录了事务的开始BEGIN和提交COMMIT。
<ul>
<li>begin的起止位置为441-520；</li>
<li>insert into语句的起止位置为520-671，记录的时间和自动开启事务的begin时间是一样的；</li>
<li>commit的起止位置为671-702。</li>
</ul>
</li>
</ul>
<p class="01">使用-r命令将日志文件导入到指定文件中，使用重定向也可以实现同样的结果。并使用-s查看简化的日志文件。</p>
<div class="cnblogs_code">
<pre>[root@xuexi data]# mysqlbinlog mysql-bin.<span style="color: #800080;">000001</span> -r /tmp/binlog.<span style="color: #800080;">000001</span><span style="color: #000000;">
[root@xuexi data]# mysqlbinlog mysql</span>-bin.<span style="color: #800080;">000001</span> -s&gt;/tmp/binlog.sample</pre>
</div>
<p class="01">比较这两个文件，看看简化的日志文件简化了哪些东西。</p>
<p><img src="https://images2018.cnblogs.com/blog/733013/201805/733013-20180507090446176-118990478.png" alt=""></p>
<p class="01">从上图中可以看出，使用-s后，少了基于行的日志信息，也少了记录的位置和时间信息。</p>
<p>使用-o可以忽略前N个条目，例如上面的操作涉及了6个操作。忽略掉前3个后的日志显示如下：可以看到直接从位置441开始显示了。</p>
<div class="cnblogs_code"><div class="cnblogs_code_toolbar"><span class="cnblogs_code_copy"><a href="javascript:void(0);" onclick="copyCnblogsCode(this)" title="复制代码"><img src="//common.cnblogs.com/images/copycode.gif" alt="复制代码"></a></span></div>
<pre><span style="color: #ff0000;">[</span><span style="color: #ff0000;">root@xuexi data</span><span style="color: #ff0000;">]</span># mysqlbinlog mysql<span style="color: #808080;">-</span>bin.<span style="color: #800000; font-weight: bold;">000001</span> <span style="color: #808080;">-</span>o <span style="color: #800000; font-weight: bold;">3</span><span style="color: #000000;">
...前面固定部分省略...<br>
</span><span style="color: #000000;">'/*!*/;
# at 441
#170329  5:21:33 server id 1  end_log_pos 520 CRC32 0x05a9c5a1  Query   thread_id=1     exec_time=0     error_code=0
SET TIMESTAMP=1490736093/*!*/;
SET @@session.pseudo_thread_id=1/*!*/;
SET @@session.foreign_key_checks=1, @@session.sql_auto_is_null=0, @@session.unique_checks=1, @@session.autocommit=1/*!*/;
SET @@session.sql_mode=1075838976/*!*/;
SET @@session.auto_increment_increment=1, @@session.auto_increment_offset=1/*!*/;
/*!\C utf8 *//*!*/;
SET @@session.character_set_client=33,@@session.collation_connection=33,@@session.collation_server=8/*!*/;
SET @@session.lc_time_names=0/*!*/;
SET @@session.collation_database=DEFAULT/*!*/;
BEGIN
/*!*/;
# at 520
#170329  5:21:33 server id 1  end_log_pos 671 CRC32 0xad9e7dc8  Query   thread_id=1     exec_time=0     error_code=0
use `test`/*!*/;
SET TIMESTAMP=1490736093/*!*/;
insert into student values(1,'malongshuai','male'),(2,'gaoxiaofang','female'</span><span style="color: #ff0000;"><span style="color: #000000;">)
/*!*/;
# at 671
#170329  5:21:33 server id 1  end_log_pos 702 CRC32 0xb69b0f7d  Xid = 32
COMMIT/*!*/;
DELIMITER ;<br>
...后面固定部分省略...</span> </span></pre>
<div class="cnblogs_code_toolbar"><span class="cnblogs_code_copy"><a href="javascript:void(0);" onclick="copyCnblogsCode(this)" title="复制代码"><img src="//common.cnblogs.com/images/copycode.gif" alt="复制代码"></a></span></div></div>
<p class="01">使用-d可以只显示指定数据库相关的操作。例如先切换到其他数据库进行一番操作，然后再使用-d查看日志。</p>
<div class="cnblogs_code"><div class="cnblogs_code_toolbar"><span class="cnblogs_code_copy"><a href="javascript:void(0);" onclick="copyCnblogsCode(this)" title="复制代码"><img src="//common.cnblogs.com/images/copycode.gif" alt="复制代码"></a></span></div>
<pre>mysql<span style="color: #808080;">&gt;</span> <span style="color: #0000ff;">use</span><span style="color: #000000;"> mysql;
mysql</span><span style="color: #808080;">&gt;</span> <span style="color: #0000ff;">create</span> <span style="color: #0000ff;">table</span> mytest(id <span style="color: #0000ff;">int</span><span style="color: #000000;">);

</span><span style="color: #ff0000;">[</span><span style="color: #ff0000;">root@xuexi data</span><span style="color: #ff0000;">]</span># mysqlbinlog mysql<span style="color: #808080;">-</span>bin.<span style="color: #800000; font-weight: bold;">000001</span> <span style="color: #808080;">-</span><span style="color: #000000;">d mysql
...前固定部分省略...</span><span style="color: #000000;">'</span><span style="color: #ff0000;"><span style="color: #000000;">/*!*/;
# at 120
# at 305
# at 441
#170329  5:21:33 server id 1  end_log_pos 520 CRC32 0x05a9c5a1  Query   thread_id=1     exec_time=0     error_code=0
SET TIMESTAMP=1490736093/*!*/;
SET @@session.pseudo_thread_id=1/*!*/;
SET @@session.foreign_key_checks=1, @@session.sql_auto_is_null=0, @@session.unique_checks=1, @@session.autocommit=1/*!*/;
SET @@session.sql_mode=1075838976/*!*/;
SET @@session.auto_increment_increment=1, @@session.auto_increment_offset=1/*!*/;
/*!\C utf8 *//*!*/;
SET @@session.character_set_client=33,@@session.collation_connection=33,@@session.collation_server=8/*!*/;
SET @@session.lc_time_names=0/*!*/;
SET @@session.collation_database=DEFAULT/*!*/;
BEGIN
/*!*/;
# at 520
# at 671
#170329  5:21:33 server id 1  end_log_pos 702 CRC32 0xb69b0f7d  Xid = 32
COMMIT/*!*/;
# at 702
#170329  6:27:12 server id 1  end_log_pos 805 CRC32 0x491529ff  Query   thread_id=1     exec_time=0     error_code=0
use `mysql`/*!*/;
SET TIMESTAMP=1490740032/*!*/;
create table mytest(id int)
/*!*/;
DELIMITER ;
...后面固定部分省略...</span> </span></pre>
<div class="cnblogs_code_toolbar"><span class="cnblogs_code_copy"><a href="javascript:void(0);" onclick="copyCnblogsCode(this)" title="复制代码"><img src="//common.cnblogs.com/images/copycode.gif" alt="复制代码"></a></span></div></div>
<p class="01">可以看到，除了指定的mysql数据库的信息输出了，还非常简化的输出了其他数据库的信息。</p>
<p class="01">mysqlbinlog最有用的两个选项就是指定时间和位置来输出日志。</p>
<p class="01">指定时间时，将输出指定时间范围内的日志。指定的时间可以不和日志中记录的日志相同。</p>
<div class="cnblogs_code"><div class="cnblogs_code_toolbar"><span class="cnblogs_code_copy"><a href="javascript:void(0);" onclick="copyCnblogsCode(this)" title="复制代码"><img src="//common.cnblogs.com/images/copycode.gif" alt="复制代码"></a></span></div>
<pre><span style="color: #ff0000;">[</span><span style="color: #ff0000;">root@xuexi data</span><span style="color: #ff0000;">]</span># mysqlbinlog mysql<span style="color: #808080;">-</span>bin.<span style="color: #800000; font-weight: bold;">000001</span> <span style="color: #008080;">--</span><span style="color: #008080;">start-datetime='2017-03-28 00:00:01' --stop-datetime='2017-03-29 05:21:23'</span>
<span style="color: #000000;">...前面固定部分省略...
</span><span style="color: #000000;">'/*!*/;
# at 120
#170329  5:20:00 server id 1  end_log_pos 305 CRC32 0xbac43912  Query   thread_id=1     exec_time=0     error_code=0
use `test`/*!*/;
SET TIMESTAMP=1490736000/*!*/;
SET @@session.pseudo_thread_id=1/*!*/;
SET @@session.foreign_key_checks=1, @@session.sql_auto_is_null=0, @@session.unique_checks=1, @@session.autocommit=1/*!*/;
SET @@session.sql_mode=1075838976/*!*/;
SET @@session.auto_increment_increment=1, @@session.auto_increment_offset=1/*!*/;
/*!\C utf8 *//*!*/;
SET @@session.character_set_client=33,@@session.collation_connection=33,@@session.collation_server=8/*!*/;
SET @@session.lc_time_names=0/*!*/;
SET @@session.collation_database=DEFAULT/*!*/;
create table student(studentid int not null primary key,name varchar(30) not null,gender enum('female','mail'))
/*!*/;
# at 305
#170329  5:21:21 server id 1  end_log_pos 441 CRC32 0xde67f702  Query   thread_id=1     exec_time=0     error_code=0
SET TIMESTAMP=1490736081/*!*/;
alter table student change gender gender enum('female','male')
/*!*/;
DELIMITER ;
...后面固定部分省略...</span></pre>
<div class="cnblogs_code_toolbar"><span class="cnblogs_code_copy"><a href="javascript:void(0);" onclick="copyCnblogsCode(this)" title="复制代码"><img src="//common.cnblogs.com/images/copycode.gif" alt="复制代码"></a></span></div></div>
<p>同理指定位置也一样，但是指定位置时有个要求是如果指定起始位置，则必须指定日志文件中明确的起始位置。例如，日志文件中有位置120、305、441，可以指定起始和结束位置为120、500，但是不可以指定起止位置为150、500，因为日志文件中不存在150这个位置。</p>
<div class="cnblogs_code"><div class="cnblogs_code_toolbar"><span class="cnblogs_code_copy"><a href="javascript:void(0);" onclick="copyCnblogsCode(this)" title="复制代码"><img src="//common.cnblogs.com/images/copycode.gif" alt="复制代码"></a></span></div>
<pre><span style="color: #ff0000;">[</span><span style="color: #ff0000;">root@xuexi data</span><span style="color: #ff0000;">]</span># mysqlbinlog mysql<span style="color: #808080;">-</span>bin.<span style="color: #800000; font-weight: bold;">000001</span> <span style="color: #008080;">--</span><span style="color: #008080;">start-position=150 --stop-position=441</span>
<span style="color: #000000;">...前面固定部分省略...
</span><span style="color: #000000;">'/*!*/;
ERROR: Error in Log_event::read_log_event(): 'read error', data_len: 4202496, event_type: 0
...后面固定部分省略... 

[root@xuexi data]# mysqlbinlog mysql-bin.000001 --start-position=305 --stop-position=500
...前面固定部分省略... 
'/*!*/;
# at <span style="font-weight: bold;">305</span>
#<span style="font-weight: bold;">170329</span>  <span style="font-weight: bold;">5</span>:<span style="font-weight: bold;">21</span>:<span style="font-weight: bold;">21</span> server id <span style="font-weight: bold;">1</span>  end_log_pos <span style="font-weight: bold;">441</span> CRC32 <span style="font-weight: bold;">0xde67f702</span>  Query   thread_id=<span style="font-weight: bold;">1</span>     exec_time=<span style="font-weight: bold;">0</span>     error_code=<span style="font-weight: bold;">0</span>
use `test`/*!*/;
SET TIMESTAMP=<span style="font-weight: bold;">1490736081</span>/*!*/;
SET <span style="font-weight: bold;">@@session</span>.pseudo_thread_id=<span style="font-weight: bold;">1</span>/*!*/;
SET <span style="font-weight: bold;">@@session</span>.foreign_key_checks=<span style="font-weight: bold;">1</span>, <span style="font-weight: bold;">@@session</span>.sql_auto_is_null=<span style="font-weight: bold;">0</span>, <span style="font-weight: bold;">@@session</span>.unique_checks=<span style="font-weight: bold;">1</span>, <span style="font-weight: bold;">@@session</span>.autocommit=<span style="font-weight: bold;">1</span>/*!*/;
SET <span style="font-weight: bold;">@@session</span>.sql_mode=<span style="font-weight: bold;">1075838976</span>/*!*/;
SET <span style="font-weight: bold;">@@session</span>.auto_increment_increment=<span style="font-weight: bold;">1</span>, <span style="font-weight: bold;">@@session</span>.auto_increment_offset=<span style="font-weight: bold;">1</span>/*!*/;
/*!\C utf8 *//*!*/;
SET <span style="font-weight: bold;">@@session</span>.character_set_client=<span style="font-weight: bold;">33</span>,<span style="font-weight: bold;">@@session</span>.collation_connection=<span style="font-weight: bold;">33</span>,<span style="font-weight: bold;">@@session</span>.collation_server=<span style="font-weight: bold;">8</span>/*!*/;
SET <span style="font-weight: bold;">@@session</span>.lc_time_names=<span style="font-weight: bold;">0</span>/*!*/;
SET <span style="font-weight: bold;">@@session</span>.collation_database=DEFAULT/*!*/;
alter table student change gender gender enum('female','male')
/*!*/;
# at <span style="font-weight: bold;">441</span>
#<span style="font-weight: bold;">170329</span>  <span style="font-weight: bold;">5</span>:<span style="font-weight: bold;">21</span>:<span style="font-weight: bold;">33</span> server id <span style="font-weight: bold;">1</span>  end_log_pos <span style="font-weight: bold;">520</span> CRC32 <span style="font-weight: bold;">0x05a9c5a1</span>  Query   thread_id=<span style="font-weight: bold;">1</span>     exec_time=<span style="font-weight: bold;">0</span>     error_code=<span style="font-weight: bold;">0</span>
SET TIMESTAMP=<span style="font-weight: bold;">1490736093</span>/*!*/;
BEGIN
/*!*/;
DELIMITER ;</span>
...后面固定部分省略...</pre>
<div class="cnblogs_code_toolbar"><span class="cnblogs_code_copy"><a href="javascript:void(0);" onclick="copyCnblogsCode(this)" title="复制代码"><img src="//common.cnblogs.com/images/copycode.gif" alt="复制代码"></a></span></div></div>
<p><a name="blog5.2.2"></a></p>
<h3 id="auto_id_8">5.2.2 show binary logs</h3>
<p>该语句用于查看当前使用了哪些二进制日志文件。</p>
<p class="01">可以通过查看二进制的index文件来查看当前正在使用哪些二进制日志。</p>
<div class="cnblogs_code">
<pre>[root@xuexi data]# <span style="color: #0000ff;">cat</span> mysql-<span style="color: #000000;">bin.index 
.</span>/mysql-bin.<span style="color: #800080;">000003</span><span style="color: #000000;">
.</span>/mysql-bin.<span style="color: #800080;">000004</span><span style="color: #000000;">
.</span>/mysql-bin.<span style="color: #800080;">000005</span><span style="color: #000000;">
.</span>/mysql-bin.<span style="color: #800080;">000006</span></pre>
</div>
<p class="01">也可以在mysql环境中使用&nbsp;<span class="cnblogs_code">show {binary | master} logs</span>&nbsp;来查看。binary和master是同义词。</p>
<div class="cnblogs_code"><div class="cnblogs_code_toolbar"><span class="cnblogs_code_copy"><a href="javascript:void(0);" onclick="copyCnblogsCode(this)" title="复制代码"><img src="//common.cnblogs.com/images/copycode.gif" alt="复制代码"></a></span></div>
<pre>mysql<span style="color: #808080;">&gt;</span> show <span style="color: #0000ff;">binary</span><span style="color: #000000;"> logs;
</span><span style="color: #808080;">+</span><span style="color: #008080;">--</span><span style="color: #008080;">----------------+-----------+</span>
<span style="color: #808080;">|</span> Log_name         <span style="color: #808080;">|</span> File_size <span style="color: #808080;">|</span>
<span style="color: #808080;">+</span><span style="color: #008080;">--</span><span style="color: #008080;">----------------+-----------+</span>
<span style="color: #808080;">|</span> mysql<span style="color: #808080;">-</span>bin.<span style="color: #800000; font-weight: bold;">000003</span> <span style="color: #808080;">|</span>       <span style="color: #800000; font-weight: bold;">167</span> <span style="color: #808080;">|</span>
<span style="color: #808080;">|</span> mysql<span style="color: #808080;">-</span>bin.<span style="color: #800000; font-weight: bold;">000004</span> <span style="color: #808080;">|</span>       <span style="color: #800000; font-weight: bold;">785</span> <span style="color: #808080;">|</span>
<span style="color: #808080;">|</span> mysql<span style="color: #808080;">-</span>bin.<span style="color: #800000; font-weight: bold;">000005</span> <span style="color: #808080;">|</span>      <span style="color: #800000; font-weight: bold;">1153</span> <span style="color: #808080;">|</span>
<span style="color: #808080;">|</span> mysql<span style="color: #808080;">-</span>bin.<span style="color: #800000; font-weight: bold;">000006</span> <span style="color: #808080;">|</span>       <span style="color: #800000; font-weight: bold;">602</span> <span style="color: #808080;">|</span>
<span style="color: #808080;">+</span><span style="color: #008080;">--</span><span style="color: #008080;">----------------+-----------</span></pre>
<div class="cnblogs_code_toolbar"><span class="cnblogs_code_copy"><a href="javascript:void(0);" onclick="copyCnblogsCode(this)" title="复制代码"><img src="//common.cnblogs.com/images/copycode.gif" alt="复制代码"></a></span></div></div>
<p><a name="blog5.2.3"></a></p>
<h3 id="auto_id_9">5.2.3 show binlog events</h3>
<p class="01"><span style="color: #ff0000;"><strong>该语句用于查看日志中进行了哪些操作。</strong></span></p>
<div class="cnblogs_code">
<pre>mysql<span style="color: #808080;">&gt;</span> show binlog events <span style="color: #808080;">in</span> <span style="color: #ff0000;">'</span><span style="color: #ff0000;">mysql-bin.000005</span><span style="color: #ff0000;">'</span>;</pre>
</div>
<p><img src="https://images2018.cnblogs.com/blog/733013/201805/733013-20180507091129596-1182363918.png" alt=""></p>
<p class="01">可以指定起始位置。同样，起始位置必须指定正确，不能指定不存在的位置。</p>
<div class="cnblogs_code"><div class="cnblogs_code_toolbar"><span class="cnblogs_code_copy"><a href="javascript:void(0);" onclick="copyCnblogsCode(this)" title="复制代码"><img src="//common.cnblogs.com/images/copycode.gif" alt="复制代码"></a></span></div>
<pre>mysql<span style="color: #808080;">&gt;</span> show binlog events <span style="color: #808080;">in</span> <span style="color: #ff0000;">'</span><span style="color: #ff0000;">mysql-bin.000005</span><span style="color: #ff0000;">'</span> <span style="color: #0000ff;">from</span> <span style="color: #800000; font-weight: bold;">961</span><span style="color: #000000;">;
</span><span style="color: #808080;">+</span><span style="color: #008080;">--</span><span style="color: #008080;">----------------+------+------------+-----------+-------------+--------------------------------+</span>
<span style="color: #808080;">|</span> Log_name         <span style="color: #808080;">|</span> Pos  <span style="color: #808080;">|</span> Event_type <span style="color: #808080;">|</span> Server_id <span style="color: #808080;">|</span> End_log_pos <span style="color: #808080;">|</span> Info                           <span style="color: #808080;">|</span>
<span style="color: #808080;">+</span><span style="color: #008080;">--</span><span style="color: #008080;">----------------+------+------------+-----------+-------------+--------------------------------+</span>
<span style="color: #808080;">|</span> mysql<span style="color: #808080;">-</span>bin.<span style="color: #800000; font-weight: bold;">000005</span> <span style="color: #808080;">|</span>  <span style="color: #800000; font-weight: bold;">961</span> <span style="color: #808080;">|</span> Table_map  <span style="color: #808080;">|</span>         <span style="color: #800000; font-weight: bold;">1</span> <span style="color: #808080;">|</span>        <span style="color: #800000; font-weight: bold;">1019</span> <span style="color: #808080;">|</span> table_id: <span style="color: #800000; font-weight: bold;">98</span> (test.student)    <span style="color: #808080;">|</span>
<span style="color: #808080;">|</span> mysql<span style="color: #808080;">-</span>bin.<span style="color: #800000; font-weight: bold;">000005</span> <span style="color: #808080;">|</span> <span style="color: #800000; font-weight: bold;">1019</span> <span style="color: #808080;">|</span> Write_rows <span style="color: #808080;">|</span>         <span style="color: #800000; font-weight: bold;">1</span> <span style="color: #808080;">|</span>        <span style="color: #800000; font-weight: bold;">1075</span> <span style="color: #808080;">|</span> table_id: <span style="color: #800000; font-weight: bold;">98</span> flags: STMT_END_F <span style="color: #808080;">|</span>
<span style="color: #808080;">|</span> mysql<span style="color: #808080;">-</span>bin.<span style="color: #800000; font-weight: bold;">000005</span> <span style="color: #808080;">|</span> <span style="color: #800000; font-weight: bold;">1075</span> <span style="color: #808080;">|</span> Xid        <span style="color: #808080;">|</span>         <span style="color: #800000; font-weight: bold;">1</span> <span style="color: #808080;">|</span>        <span style="color: #800000; font-weight: bold;">1106</span> <span style="color: #808080;">|</span> <span style="color: #0000ff;">COMMIT</span> <span style="color: #008080;">/*</span><span style="color: #008080;"> xid=129 </span><span style="color: #008080;">*/</span>           <span style="color: #808080;">|</span>
<span style="color: #808080;">|</span> mysql<span style="color: #808080;">-</span>bin.<span style="color: #800000; font-weight: bold;">000005</span> <span style="color: #808080;">|</span> <span style="color: #800000; font-weight: bold;">1106</span> <span style="color: #808080;">|</span> Rotate     <span style="color: #808080;">|</span>         <span style="color: #800000; font-weight: bold;">1</span> <span style="color: #808080;">|</span>        <span style="color: #800000; font-weight: bold;">1153</span> <span style="color: #808080;">|</span> mysql<span style="color: #808080;">-</span>bin.<span style="color: #800000; font-weight: bold;">000006</span>;pos<span style="color: #808080;">=</span><span style="color: #800000; font-weight: bold;">4</span>         <span style="color: #808080;">|</span>
<span style="color: #808080;">+</span><span style="color: #008080;">--</span><span style="color: #008080;">----------------+------+------------+-----------+-------------+--------------------------------+</span>&nbsp;</pre>
<div class="cnblogs_code_toolbar"><span class="cnblogs_code_copy"><a href="javascript:void(0);" onclick="copyCnblogsCode(this)" title="复制代码"><img src="//common.cnblogs.com/images/copycode.gif" alt="复制代码"></a></span></div></div>
<p><a name="blog5.2.4"></a></p>
<h3 id="auto_id_10">5.2.4 show master status</h3>
<p class="01">该语句用于显示主服务器中的二进制日志信息。如果是主从结构，它只会显示主从结构中主服务器的二进制日志信息。</p>
<div>
<div class="cnblogs_code">
<pre>mysql<span style="color: #808080;">&gt;</span><span style="color: #000000;"> show master status;    
</span><span style="color: #808080;">+</span><span style="color: #008080;">--</span><span style="color: #008080;">----------------+----------+--------------+------------------+-------------------+</span>
<span style="color: #808080;">|</span> <span style="color: #0000ff;">File</span>             <span style="color: #808080;">|</span> Position <span style="color: #808080;">|</span> Binlog_Do_DB <span style="color: #808080;">|</span> Binlog_Ignore_DB <span style="color: #808080;">|</span> Executed_Gtid_Set <span style="color: #808080;">|</span>
<span style="color: #808080;">+</span><span style="color: #008080;">--</span><span style="color: #008080;">----------------+----------+--------------+------------------+-------------------+</span>
<span style="color: #808080;">|</span> mysql<span style="color: #808080;">-</span>bin.<span style="color: #800000; font-weight: bold;">000006</span> <span style="color: #808080;">|</span>      <span style="color: #800000; font-weight: bold;">602</span> <span style="color: #808080;">|</span>              <span style="color: #808080;">|</span>                  <span style="color: #808080;">|</span>                   <span style="color: #808080;">|</span>
<span style="color: #808080;">+</span><span style="color: #008080;">--</span><span style="color: #008080;">----------------+----------+--------------+------------------+-------------------+</span></pre>
</div>
</div>
<p class="01">可以查看到当前正在使用的日志及下一事件记录的开始位置，还能查看到哪些数据库需要记录二进制日志，哪些数据库不记录二进制日志。</p>
<p><a name="blog5.3"></a></p>
<h2 id="auto_id_11">5.3 删除二进制日志</h2>
<p class="01">删除二进制日志有几种方法。不管哪种方法，都会将删除后的信息同步到二进制index文件中。</p>
<p class="01"><strong>1.reset master</strong><strong>将会删除所有日志，并让日志文件重新从000001</strong><strong>开始。</strong></p>
<div>
<div class="cnblogs_code">
<pre>mysql<span style="color: #808080;">&gt;</span> reset master;</pre>
</div>
</div>
<p class="01"><strong>2.PURGE { BINARY | MASTER } LOGS { TO 'log_name' | BEFORE datetime_expr }</strong></p>
<p class="01">&nbsp;<span class="cnblogs_code">purge master logs <span style="color: #0000ff;">to</span> "binlog_name.00000X"</span>&nbsp;将会清空00000X之前的所有日志文件。例如删除000006之前的日志文件。</p>
<div>
<div class="cnblogs_code">
<pre>mysql<span style="color: #808080;">&gt;</span> purge master logs <span style="color: #0000ff;">to</span> "mysql<span style="color: #808080;">-</span>bin.<span style="color: #800000; font-weight: bold;">000006</span><span style="color: #000000;">";
mysql</span><span style="color: #808080;">&gt;</span> purge <span style="color: #0000ff;">binary</span> logs <span style="color: #0000ff;">to</span> "mysql<span style="color: #808080;">-</span>bin.<span style="color: #800000; font-weight: bold;">000006</span>";</pre>
</div>
<p class="022">master和binary是同义词</p>
</div>
<p class="01">&nbsp;<span class="cnblogs_code">purge master logs before <span style="color: #ff0000;">'</span><span style="color: #ff0000;">yyyy-mm-dd hh:mi:ss</span><span style="color: #ff0000;">'</span></span>&nbsp;将会删除指定日期之前的所有日志。但是若指定的时间处在正在使用中的日志文件中，将无法进行purge。</p>
<div>
<div class="cnblogs_code"><div class="cnblogs_code_toolbar"><span class="cnblogs_code_copy"><a href="javascript:void(0);" onclick="copyCnblogsCode(this)" title="复制代码"><img src="//common.cnblogs.com/images/copycode.gif" alt="复制代码"></a></span></div>
<pre>mysql<span style="color: #808080;">&gt;</span> purge master logs before <span style="color: #ff0000;">'</span><span style="color: #ff0000;">2017-03-29 07:36:40</span><span style="color: #ff0000;">'</span><span style="color: #000000;">;

mysql</span><span style="color: #808080;">&gt;</span><span style="color: #000000;"> show warnings;
</span><span style="color: #808080;">+</span><span style="color: #008080;">--</span><span style="color: #008080;">-------+------+---------------------------------------------------------------------------+</span>
<span style="color: #808080;">|</span> <span style="color: #0000ff;">Level</span>   <span style="color: #808080;">|</span> Code <span style="color: #808080;">|</span> Message                                                                   <span style="color: #808080;">|</span>
<span style="color: #808080;">+</span><span style="color: #008080;">--</span><span style="color: #008080;">-------+------+---------------------------------------------------------------------------+</span>
<span style="color: #808080;">|</span> Warning <span style="color: #808080;">|</span> <span style="color: #800000; font-weight: bold;">1868</span> <span style="color: #808080;">|</span> <span style="color: #0000ff;">file</span> .<span style="color: #808080;">/</span>mysql<span style="color: #808080;">-</span>bin.<span style="color: #800000; font-weight: bold;">000003</span> was <span style="color: #808080;">not</span> purged because it <em><strong><span style="color: #ff0000;">is the active log file</span></strong></em>. <span style="color: #808080;">|</span>
<span style="color: #808080;">+</span><span style="color: #008080;">--</span><span style="color: #008080;">-------+------+---------------------------------------------------------------------------+</span></pre>
<div class="cnblogs_code_toolbar"><span class="cnblogs_code_copy"><a href="javascript:void(0);" onclick="copyCnblogsCode(this)" title="复制代码"><img src="//common.cnblogs.com/images/copycode.gif" alt="复制代码"></a></span></div></div>
</div>
<p class="01"><strong>3.</strong><strong>使用--expire_logs_days=N</strong><strong>选项指定过了多少天日志自动过期清空。</strong></p>
<p><a name="blog5.4"></a></p>
<h2 id="auto_id_12">5.4 二进制日志的记录格式</h2>
<p>在MySQL 5.1之前，MySQL只有一种基于语句statement形式的日志记录格式。即将所有的相关操作记录为SQL语句形式。但是这样的记录方式对某些特殊信息无法同步记录，例如uuid，now()等这样动态变化的值。</p>
<p>从MySQL 5.1开始，MySQL支持statement、row、mixed三种形式的记录方式。row形式是基于行来记录，也就是将相关行的每一列的值都在日志中保存下来，这样的结果会导致日志文件变得非常大，但是保证了动态值的确定性。还有一种mixed形式，表示如何记录日志由MySQL自己来决定。</p>
<p class="01">日志的记录格式由变量&nbsp;<span class="cnblogs_code">binlog_format</span>&nbsp;来指定。其值有：row,statement,mixed。innodb引擎的创始人之一在博客上推荐使用row格式。</p>
<p class="01">下面将记录格式改为row。</p>
<div>
<div class="cnblogs_code">
<pre>mysql<span style="color: #808080;">&gt;</span> <span style="color: #0000ff;">alter</span> <span style="color: #0000ff;">table</span> student <span style="color: #0000ff;">add</span> birthday <span style="color: #0000ff;">datetime</span> <span style="color: #0000ff;">default</span><span style="color: #000000;">  now();
mysql</span><span style="color: #808080;">&gt;</span><span style="color: #000000;"> flush logs;
mysql</span><span style="color: #808080;">&gt;</span> <span style="color: #0000ff;">set</span> binlog_format<span style="color: #808080;">=</span><span style="color: #ff0000;">'</span><span style="color: #ff0000;">row</span><span style="color: #ff0000;">'</span><span style="color: #000000;">;
mysql</span><span style="color: #808080;">&gt;</span> <span style="color: #0000ff;">insert</span> <span style="color: #0000ff;">into</span> student <span style="color: #0000ff;">values</span>(<span style="color: #800000; font-weight: bold;">7</span>,<span style="color: #ff0000;">'</span><span style="color: #ff0000;">xiaowoniu</span><span style="color: #ff0000;">'</span>,<span style="color: #ff0000;">'</span><span style="color: #ff0000;">female</span><span style="color: #ff0000;">'</span>,now());  </pre>
</div>
</div>
<p class="01">查看产生的日志。</p>
<div class="cnblogs_code"><div class="cnblogs_code_toolbar"><span class="cnblogs_code_copy"><a href="javascript:void(0);" onclick="copyCnblogsCode(this)" title="复制代码"><img src="//common.cnblogs.com/images/copycode.gif" alt="复制代码"></a></span></div>
<pre><span style="color: #ff0000;">[</span><span style="color: #ff0000;">root@xuexi data</span><span style="color: #ff0000;">]</span># mysqlbinlog mysql<span style="color: #808080;">-</span>bin.<span style="color: #800000; font-weight: bold;">000005</span><span style="color: #000000;">
...前面固定部分省略...
</span><span style="color: #000000;">'/*!*/;
# at 120
#170329  8:06:24 server id 1  end_log_pos 200 CRC32 0x0ac02649  Query   thread_id=1     exec_time=0     error_code=0
SET TIMESTAMP=1490745984/*!*/;
SET @@session.pseudo_thread_id=1/*!*/;
SET @@session.foreign_key_checks=1, @@session.sql_auto_is_null=0, @@session.unique_checks=1, @@session.autocommit=1/*!*/;
SET @@session.sql_mode=1075838976/*!*/;
SET @@session.auto_increment_increment=1, @@session.auto_increment_offset=1/*!*/;
/*!\C utf8 *//*!*/;
SET @@session.character_set_client=33,@@session.collation_connection=33,@@session.collation_server=8/*!*/;
SET @@session.time_zone='SYSTEM'/*!*/;
SET @@session.lc_time_names=0/*!*/;
SET @@session.collation_database=DEFAULT/*!*/;
BEGIN
/*!*/;
# at 200
#170329  8:06:24 server id 1  end_log_pos 258 CRC32 0xb8cdfd09  Table_map: `test`.`student` mapped to number 94
# at 258
#170329  8:06:24 server id 1  end_log_pos 314 CRC32 0x8ce6f72c  Write_rows: table id 94 flags: STMT_END_F

BINLOG '
gPraWBMBAAAAOgAAAAIBAAAAAF4AAAAAAAEABHRlc3QAB3N0dWRlbnQABAMP/hIFHgD3AQAMCf3N
uA==
gPraWB4BAAAAOAAAADoBAAAAAF4AAAAAAAEAAgAE//AHAAAACXhpYW93b25pdQGZnDqBmCz35ow=
'/*!*/;
# at 314
#170329  8:06:24 server id 1  end_log_pos 345 CRC32 0x7a48c057  Xid = 114
COMMIT/*!*/;
DELIMITER ;
...后面固定部分省略...</span></pre>
<div class="cnblogs_code_toolbar"><span class="cnblogs_code_copy"><a href="javascript:void(0);" onclick="copyCnblogsCode(this)" title="复制代码"><img src="//common.cnblogs.com/images/copycode.gif" alt="复制代码"></a></span></div></div>
<p>发现是一堆看不懂的东西，使用-vv可将这些显示出来。可以看出，结果中记录的非常详细，这也是为什么基于row记录日志会导致日志文件极速变大。</p>
<div class="cnblogs_code"><div class="cnblogs_code_toolbar"><span class="cnblogs_code_copy"><a href="javascript:void(0);" onclick="copyCnblogsCode(this)" title="复制代码"><img src="//common.cnblogs.com/images/copycode.gif" alt="复制代码"></a></span></div>
<pre><span style="color: #ff0000;">[</span><span style="color: #ff0000;">root@xuexi data</span><span style="color: #ff0000;">]</span># mysqlbinlog mysql<span style="color: #808080;">-</span>bin.<span style="color: #800000; font-weight: bold;">000005</span> <span style="color: #808080;">-</span><span style="color: #000000;">vv
...前面省略...
BINLOG </span><span style="color: #ff0000;">'</span><span style="color: #ff0000;">
gPraWBMBAAAAOgAAAAIBAAAAAF4AAAAAAAEABHRlc3QAB3N0dWRlbnQABAMP/hIFHgD3AQAMCf3N
uA==
gPraWB4BAAAAOAAAADoBAAAAAF4AAAAAAAEAAgAE//AHAAAACXhpYW93b25pdQGZnDqBmCz35ow=
</span><span style="color: #ff0000;">'</span><span style="color: #008080;">/*</span><span style="color: #008080;">!</span><span style="color: #008080;">*/</span><span style="color: #000000;">;
### </span><span style="color: #0000ff;">INSERT</span> <span style="color: #0000ff;">INTO</span><span style="color: #000000;"> `test`.`student`
### </span><span style="color: #0000ff;">SET</span><span style="color: #000000;">
###   </span><span style="color: #008000;">@1</span><span style="color: #808080;">=</span><span style="color: #800000; font-weight: bold;">7</span> <span style="color: #008080;">/*</span><span style="color: #008080;"> INT meta=0 nullable=0 is_null=0 </span><span style="color: #008080;">*/</span><span style="color: #000000;">
###   </span><span style="color: #008000;">@2</span><span style="color: #808080;">=</span><span style="color: #ff0000;">'</span><span style="color: #ff0000;">xiaowoniu</span><span style="color: #ff0000;">'</span> <span style="color: #008080;">/*</span><span style="color: #008080;"> VARSTRING(30) meta=30 nullable=0 is_null=0 </span><span style="color: #008080;">*/</span><span style="color: #000000;">
###   </span><span style="color: #008000;">@3</span><span style="color: #808080;">=</span><span style="color: #800000; font-weight: bold;">1</span> <span style="color: #008080;">/*</span><span style="color: #008080;"> ENUM(1 byte) meta=63233 nullable=1 is_null=0 </span><span style="color: #008080;">*/</span><span style="color: #000000;">
###   </span><span style="color: #008000;">@4</span><span style="color: #808080;">=</span><span style="color: #ff0000;">'</span><span style="color: #ff0000;">2017-03-29 08:06:24</span><span style="color: #ff0000;">'</span> <span style="color: #008080;">/*</span><span style="color: #008080;"> DATETIME(0) meta=0 nullable=1 is_null=0 </span><span style="color: #008080;">*/</span><span style="color: #000000;">
# at </span><span style="color: #800000; font-weight: bold;">314</span><span style="color: #000000;">
...后面省略...</span></pre>
<div class="cnblogs_code_toolbar"><span class="cnblogs_code_copy"><a href="javascript:void(0);" onclick="copyCnblogsCode(this)" title="复制代码"><img src="//common.cnblogs.com/images/copycode.gif" alt="复制代码"></a></span></div></div>
<p>还有一种mixed模式。这种模式下默认会采用statement的方式记录，只有以下几种情况会采用row的形式来记录日志。<br>
1.表的存储引擎为NDB，这时对表的DML操作都会以row的格式记录。<br>
2.使用了uuid()、user()、current_user()、found_rows()、row_count()等不确定函数。但测试发现对now()函数仍会以statement格式记录，而sysdate()函数会以row格式记录。<br>
3.使用了insert delay语句。<br>
4.使用了临时表。</p>
<p><a name="blog5.5"></a></p>
<h2 id="auto_id_13">5.5 二进制日志相关的变量</h2>
<p>注意：在配置binlog相关变量的时候，相关变量名总是搞混，因为有的是binlog，有的是log_bin，当他们分开的时候，log在前，当它们一起的时候，bin在前。在配置文件中也同样如此。</p>
<ul>
<li>log_bin = {on | off | base_name} #指定是否启用记录二进制日志或者指定一个日志路径(路径不能加.否则.后的被忽略)</li>
<li>sql_log_bin ={ on | off } #指定是否启用记录二进制日志，只有在log_bin开启的时候才有效</li>
<li>expire_logs_days = #指定自动删除二进制日志的时间，即日志过期时间</li>
<li>binlog_do_db =  #明确指定要记录日志的数据库</li>
<li>binlog_ignore_db =   #指定不记录二进制日志的数据库</li>
<li>log_bin_index =  #指定mysql-bin.index文件的路径</li>
<li>binlog_format = { mixed | row | statement } #指定二进制日志基于什么模式记录</li>
<li>binlog_rows_query_log_events = { 1|0 }  # MySQL5.6.2添加了该变量，当binlog format为row时，默认不会记录row对应的SQL语句，设置为1或其他true布尔值时会记录，但需要使用mysqlbinlog -v查看，这些语句是被注释的，恢复时不会被执行。</li>
<li>max_binlog_size = #指定二进制日志文件最大值，超出指定值将自动滚动。但由于事务不会跨文件，所以并不一定总是精确。</li>
<li>binlog_cache_size = 32768 #<span style="color: #ff0000;"><strong>基于事务类型的日志会先记录在缓冲区</strong></span>，当达到该缓冲大小时这些日志会写入磁盘</li>
<li>max_binlog_cache_size =  #指定二进制日志缓存最大大小，硬限制。默认4G，够大了，建议不要改</li>
<li>binlog_cache_use：使用缓存写二进制日志的次数(这是一个实时变化的统计值)</li>
<li>binlog_cache_disk_use:使用临时文件写二进制日志的次数，当日志超过了binlog_cache_size的时候会使用临时文件写日志，如果该变量值不为0，则考虑增大binlog_cache_size的值</li>
<li>binlog_stmt_cache_size = 32768  #一般等同于且决定binlog_cache_size大小，所以修改缓存大小时只需修改这个而不用修改binlog_cache_size</li>
<li>binlog_stmt_cache_use：使用缓存写二进制日志的次数</li>
<li>binlog_stmt_cache_disk_use: 使用临时文件写二进制日志的次数，当日志超过了binlog_cache_size的时候会使用临时文件写日志，如果该变量值不为0，则考虑增大binlog_cache_size的值</li>
<li>sync_binlog = { 0 | n }  #这个参数直接影响mysql的性能和完整性
<ul>
<li>sync_binlog=0:不同步，日志何时刷到磁盘由FileSystem决定，这个性能最好。</li>
<li>sync_binlog=n:每写n次二进制日志事件(不是事务)，MySQL将执行一次磁盘同步指令fdatasync()将缓存日志刷新到磁盘日志文件中。Mysql中默认的设置是sync_binlog=0，即不同步，这时性能最好，但风险最大。一旦系统奔溃，缓存中的日志都会丢失。</li>

















</ul>

















</li>

















</ul>
<p><strong><span style="color: #ff0000;">在innodb的主从复制结构中，如果启用了二进制日志(几乎都会启用)，要保证事务的一致性和持久性的时候，必须将sync_binlog的值设置为1，因为每次事务提交都会写入二进制日志，设置为1就保证了每次事务提交时二进制日志都会写入到磁盘中，从而立即被从服务器复制过去。</span></strong></p>
<p><a name="blog5.6"></a></p>
<h2 id="auto_id_14">5.6 二进制日志定点还原数据库</h2>
<p class="01">只需指定二进制日志的起始位置（可指定终止位置）并将其保存到sql文件中，由mysql命令来载入恢复即可。当然直接通过管道送给mysql命令也可。</p>
<p class="01">至于是基于位置来恢复还是基于时间点来恢复，这两种行为都可以。选择时间点来恢复比较直观些，并且跨日志文件恢复时更方便。</p>
<div class="cnblogs_code">
<pre>mysqlbinlog --stop-datetime=<span style="color: #800000;">"</span><span style="color: #800000;">2014-7-2 15:27:48</span><span style="color: #800000;">"</span> /tmp/mysql-bin.<span style="color: #800080;">000008</span> | mysql -u user -p password</pre>
</div>
<p class="01">恢复多个二进制日志文件时：</p>
<div>
<div class="cnblogs_code">
<pre>mysqlbinlog mysql-bin.[*] | mysql -uroot -p password</pre>
</div>
</div>
<p class="01">或者将它们导入到一个文件中后恢复。</p>
<div class="cnblogs_code">
<pre>mysqlbinlog mysql-bin.<span style="color: #800080;">000001</span> &gt; /tmp/<span style="color: #000000;">a.sql
mysqlbinlog mysql</span>-bin.<span style="color: #800080;">000002</span> &gt;&gt;/tmp/<span style="color: #000000;">a.sql
mysql </span>-u root -p password -e <span style="color: #800000;">"</span><span style="color: #800000;">source /tmp/a.sql</span><span style="color: #800000;">"</span></pre>
</div>
<p>&nbsp;</p><p><strong><span style="font-size: 18px;">转载请务必在文章<span style="color: red;">最开头</span>标明原文地址<br>本文原创地址：骏马金龙<a href="https://www.cnblogs.com/f-ck-need-u/p/9001061.html">https://www.cnblogs.com/f-ck-need-u/p/9001061.html</a></span></strong><br></p><p><strong><span style="font-size: 18px;">如果觉得文章不错，不妨给个<font color="#0000ff" size="6">打赏</font>，写作不易，各位的支持，能激发和鼓励我更大的写作热情。谢谢！</span></strong><br></p><img src="https://files.cnblogs.com/files/f-ck-need-u/wealipay.bmp"><p></p><p></p></div>
<div id="MySignature" style="display: block;"><div>作者：<a href="https://www.cnblogs.com/f-ck-need-u/" target="_blank">骏马金龙</a></div>
<div>出处：<a href="https://www.cnblogs.com/f-ck-need-u/" target="_blank">http://www.cnblogs.com/f-ck-need-u/</a></div>
<div>Linux运维交流群：<a href="https://jq.qq.com/?_wv=1027&amp;k=5zfAfeJ" target="_blank">710427601</a></div>
<p>
<span style="font-size: 14px; font-family: &quot;Microsoft YaHei&quot;; color: #0000ff">
<strong>
<a href="https://www.cnblogs.com/f-ck-need-u/p/7048359.html" target="_blank">
Linux&amp;shell系列文章：http://www.cnblogs.com/f-ck-need-u/p/7048359.html</a>
</strong>
<br>

<strong>
<a href="https://www.cnblogs.com/f-ck-need-u/p/7576137.html" target="_blank">
网站架构系列文章：http://www.cnblogs.com/f-ck-need-u/p/7576137.html</a>
</strong>
<br>

<strong>
<a href="https://www.cnblogs.com/f-ck-need-u/p/7586194.html" target="_blank">
MySQL/MariaDB系列文章：https://www.cnblogs.com/f-ck-need-u/p/7586194.html</a>
</strong>
<br>

<strong>
<a href="https://www.cnblogs.com/f-ck-need-u/p/9512185.html" target="_blank">
Perl系列：https://www.cnblogs.com/f-ck-need-u/p/9512185.html</a>
</strong>
<br>

<strong>
<a href="https://www.cnblogs.com/f-ck-need-u/p/9832538.html" target="_blank">
Go系列：https://www.cnblogs.com/f-ck-need-u/p/9832538.html</a>
</strong>
<br>

<strong>
<a href="https://www.cnblogs.com/f-ck-need-u/p/9832640.html" target="_blank">
Python系列：https://www.cnblogs.com/f-ck-need-u/p/9832640.html</a>
</strong>
<br>

<strong>
<a href="https://www.cnblogs.com/f-ck-need-u/p/10805545.html" target="_blank">
Ruby系列：https://www.cnblogs.com/f-ck-need-u/p/10805545.html</a>
</strong>
<br>

<strong>
<a href="https://www.cnblogs.com/f-ck-need-u/p/10481466.html" target="_blank">
操作系统系列：https://www.cnblogs.com/f-ck-need-u/p/10481466.html</a>
</strong>
<br>
</span>
</p></div>
<div class="clear"></div>
<div id="blog_post_info_block"><div id="BlogPostCategory">
分类: 
<a href="https://www.cnblogs.com/f-ck-need-u/category/1085743.html" target="_blank">数据库系列</a></div>


<div id="blog_post_info">
<div id="green_channel">
<a href="javascript:void(0);" id="green_channel_digg" onclick="DiggIt(9001061,cb_blogId,1);green_channel_success(this,'谢谢推荐！');">好文要顶</a>
<a id="green_channel_follow" onclick="follow('1e7fa90d-decd-e411-b908-9dcfd8948a71');" href="javascript:void(0);">关注我</a>
<a id="green_channel_favorite" onclick="AddToWz(cb_entryId);return false;" href="javascript:void(0);">收藏该文</a>
<a id="green_channel_weibo" href="javascript:void(0);" title="分享至新浪微博" onclick="ShareToTsina()"><img src="https://common.cnblogs.com/images/icon_weibo_24.png" alt=""></a>
<a id="green_channel_wechat" href="javascript:void(0);" title="分享至微信" onclick="shareOnWechat()"><img src="https://common.cnblogs.com/images/wechat.png" alt=""></a>
</div>
<div id="author_profile">
<div id="author_profile_info" class="author_profile_info">
<a href="https://home.cnblogs.com/u/f-ck-need-u/" target="_blank"><img src="https://pic.cnblogs.com/face/733013/20180330130915.png" class="author_avatar" alt=""></a>
<div id="author_profile_detail" class="author_profile_info">
<a href="https://home.cnblogs.com/u/f-ck-need-u/">骏马金龙</a><br>
<a href="https://home.cnblogs.com/u/f-ck-need-u/followees/">关注 - 22</a><br>
<a href="https://home.cnblogs.com/u/f-ck-need-u/followers/">粉丝 - 1408</a>
</div>
</div>
<div class="clear"></div>
<div id="author_profile_honor"></div>
<div id="author_profile_follow">
<a href="javascript:void(0);" onclick="follow('1e7fa90d-decd-e411-b908-9dcfd8948a71');return false;">+加关注</a>
</div>
</div>


<script type="text/javascript">
currentDiggType = 0;
</script></div>
<div class="clear"></div>
<div id="post_next_prev">

<a href="https://www.cnblogs.com/f-ck-need-u/p/8997814.html" class="p_n_p_prefix">« </a> 上一篇：    <a href="https://www.cnblogs.com/f-ck-need-u/p/8997814.html" title="发布于 2018-05-06 12:04">MySQL/MariaDB中的事务和事务隔离级别</a>
<br>
<a href="https://www.cnblogs.com/f-ck-need-u/p/9010872.html" class="p_n_p_prefix">» </a> 下一篇：    <a href="https://www.cnblogs.com/f-ck-need-u/p/9010872.html" title="发布于 2018-05-08 20:49">详细分析MySQL事务日志(redo log和undo log)</a>

</div>
</div>
</div>
<div class="postDesc">posted @ 
<span id="post-date">2018-05-07 09:40</span>&nbsp;<a href="https://www.cnblogs.com/f-ck-need-u/">骏马金龙</a> 阅读(<span id="post_view_count">12267</span>) 评论(<span id="post_comment_count">2</span>) <a href="https://i.cnblogs.com/EditPosts.aspx?postid=9001061" rel="nofollow"> 编辑</a> <a href="javascript:void(0)" onclick="AddToWz(9001061); return false;">收藏</a>
</div>
</div>
<script src="https://common.cnblogs.com/highlight/9.12.0/highlight.min.js"></script>
<script>markdown_highlight();</script>
<script>
var allowComments = true, cb_blogId = 222371, cb_blogApp = 'f-ck-need-u', cb_blogUserGuid = '1e7fa90d-decd-e411-b908-9dcfd8948a71';
var cb_entryId = 9001061, cb_entryCreatedDate = '2018-05-07 09:40', cb_postType = 1; 
loadViewCount(cb_entryId);
</script><a name="!comments"></a>
<div id="blog-comments-placeholder">

<div id="comment_pager_top">

</div>


<div class="feedback_area_title">评论列表</div>
<div class="feedbackNoItems"></div>

<div class="feedbackItem">
<div class="feedbackListSubtitle">
<div class="feedbackManage">
&nbsp;&nbsp;


<span class="comment_actions">




</span>


</div>
<!-- Title -->

<a href="#3968928" class="layer">#1楼</a>
<a name="3968928" id="comment_anchor_3968928"></a>


<!-- PostDate -->

<span class="comment_date">2018-05-07 11:05</span>


<!--NameLink-->


<a id="a_comment_author_3968928" href="https://www.cnblogs.com/cheer-wang/" target="_blank">小饼干</a>

<div class="feedbackCon">

<div id="comment_body_3968928" class="blog_comment_body">
不错，继续。。。。
</div>
<div class="comment_vote">
<span class="comment_error" style="color: red"></span>
<a href="javascript:void(0);" class="comment_digg" onclick="return voteComment(3968928, 'Digg', this.parentElement, false);">
支持(0)
</a>
<a href="javascript:void(0);" class="comment_burry" onclick="return voteComment(3968928, 'Bury', this.parentElement, false);">
反对(0)
</a>
</div>
<span id="comment_3968928_avatar" style="display:none">
https://pic.cnblogs.com/face/u131140.gif?id=19165201
</span>

</div>
</div>
</div>
<div class="feedbackItem">
<div class="feedbackListSubtitle">
<div class="feedbackManage">
&nbsp;&nbsp;


<span class="comment_actions">




</span>


</div>
<!-- Title -->

<a href="#3969146" class="layer">#2楼</a>
<a name="3969146" id="comment_anchor_3969146"></a>

<span id="comment-maxId" style="display:none">3969146</span>
<span id="comment-maxDate" style="display:none">2018/5/7 下午3:18:19</span>

<!-- PostDate -->

<span class="comment_date">2018-05-07 15:18</span>


<!--NameLink-->


<a id="a_comment_author_3969146" href="https://www.cnblogs.com/cxyzt/" target="_blank">程序员杂谈</a>

<div class="feedbackCon">

<div id="comment_body_3969146" class="blog_comment_body">
这么详细，研究一下
</div>
<div class="comment_vote">
<span class="comment_error" style="color: red"></span>
<a href="javascript:void(0);" class="comment_digg" onclick="return voteComment(3969146, 'Digg', this.parentElement, false);">
支持(0)
</a>
<a href="javascript:void(0);" class="comment_burry" onclick="return voteComment(3969146, 'Bury', this.parentElement, false);">
反对(0)
</a>
</div>


</div>
</div>
</div>


<div id="comment_pager_bottom">

</div>
</div>
<script>
var commentManager = new blogCommentManager();
commentManager.renderComments(0);
</script>

<div id="comment_form" class="commentform">
<a name="commentform"></a>
<div id="divCommentShow"></div>
<div id="comment_nav"><span id="span_refresh_tips"></span><a href="javascript:void(0);" onclick="return RefreshCommentList();" id="lnk_RefreshComments" runat="server" clientidmode="Static">刷新评论</a><a href="#" onclick="return RefreshPage();">刷新页面</a><a href="#top">返回顶部</a></div>
<div id="comment_form_container"><div class="login_tips">
注册用户登录后才能发表评论，请 
<a rel="nofollow" href="javascript:void(0);" class="underline" onclick="return login('commentform');">登录</a>
或 
<a rel="nofollow" href="javascript:void(0);" class="underline" onclick="return register();">注册</a>，
<a href="https://www.cnblogs.com/">访问</a> 网站首页。
</div></div>
<div class="ad_text_commentbox" id="ad_text_under_commentbox"></div>
<div id="ad_t2"><a href="http://www.ucancode.com/index.htm" target="_blank" onclick="ga('send', 'event', 'Link', 'click', 'T2-工控')">【推荐】超50万行VC++源码: 大型组态工控、电力仿真CAD与GIS源码库</a><br><a href="https://www.jdcloud.com/cn/activity/year-end?utm_source=DMT_cnblogs&amp;utm_medium=CH&amp;utm_campaign=q4vm&amp;utm_term=Virtual-Machines" target="_blank" onclick="ga('send', 'event', 'Link', 'click', 'T2-京东云')">【活动】京东云服务器_云主机低于1折，低价高性能产品备战双11</a><br><a href="https://www.ctyun.cn/activity/#/201910/ecs?hmsr=%E7%9C%8B%E7%9C%8B-%E5%8D%9A%E5%AE%A2%E5%9B%AD-10.16-10%E6%9C%88%E7%89%B9%E6%83%A0&amp;hmpl=&amp;hmcu=&amp;hmkw=&amp;hmci=?track=|cp:cz_blog|tgdy:46861|ttjh:shouyedingbuyoucefenlan|key:czkk12|pf:pc" target="_blank" onclick="ga('send', 'event', 'Link', 'click', 'T2-天翼云')">【推荐】天翼云双十一提前开抢，1核1G云主机3个月仅需59元</a><br><a href="https://www.huodongxing.com/event/1514061072200" target="_blank" onclick="ga('send', 'event', 'Link', 'click', 'T2-魔程')">【活动】魔程社区技术沙龙训练营—大数据主题专场等你来报名</a><br><a href="https://cloud.tencent.com/act/double11/reserve?fromSource=gwzcw.3000742.3000742.3000742&amp;utm_medium=cpc&amp;utm_id=gwzcw.3000742.3000742.3000742" target="_blank" onclick="ga('send', 'event', 'Link', 'click', 'T2-腾讯云')">【优惠】腾讯云 11.1 1智惠上云，爆款提前购与双11活动同价</a><br><a href="https://www.getui.com/2019devfest?f=5&amp;p=10&amp;c=15&amp;ch=1&amp;hmsr=%E5%8D%9A%E5%AE%A2%E5%9B%AD&amp;hmpl=2019%E5%BC%80%E5%8F%91%E8%80%85%E8%8A%82&amp;hmcu=PC&amp;hmkw=&amp;hmci=" target="_blank">【福利】个推四大热门移动开发SDK全部免费用一年，限时抢！</a><br></div>
<div id="opt_under_post"></div>
<script async="async" src="https://www.googletagservices.com/tag/js/gpt.js"></script>
<script>
var googletag = googletag || {};
googletag.cmd = googletag.cmd || [];
</script>
<script>
googletag.cmd.push(function () {
googletag.defineSlot("/1090369/C1", [300, 250], "div-gpt-ad-1546353474406-0").addService(googletag.pubads());
googletag.defineSlot("/1090369/C2", [468, 60], "div-gpt-ad-1539008685004-0").addService(googletag.pubads());
googletag.pubads().enableSingleRequest();
googletag.enableServices();
});
</script>
<div id="cnblogs_c1" class="c_ad_block">
<div id="div-gpt-ad-1546353474406-0" style="height:250px; width:300px;" data-google-query-id="CJuK7fqMteUCFZJ9vQodJk4Aeg"><div id="google_ads_iframe_/1090369/C1_0__container__" style="border: 0pt none;"><iframe id="google_ads_iframe_/1090369/C1_0" title="3rd party ad content" name="google_ads_iframe_/1090369/C1_0" width="300" height="250" scrolling="no" marginwidth="0" marginheight="0" frameborder="0" srcdoc="" style="border: 0px; vertical-align: bottom;" data-google-container-id="1" data-load-complete="true"></iframe></div></div>
</div>
<div id="under_post_news"><div class="recomm-block"><b>相关博文：</b><br>·  <a title="MySQL二进制日志总结" href="https://www.cnblogs.com/kerrycode/p/6610874.html" target="_blank" onclick="clickRecomItmem(6610874)">MySQL二进制日志总结</a><br>·  <a title="MySQL日志" href="https://www.cnblogs.com/cshaptx4869/p/10482203.html" target="_blank" onclick="clickRecomItmem(10482203)">MySQL日志</a><br>·  <a title="MySQL Logs" href="https://www.cnblogs.com/carl10086/p/5989956.html" target="_blank" onclick="clickRecomItmem(5989956)">MySQL Logs</a><br>·  <a title="Mysql日志管理" href="https://www.cnblogs.com/lhhitnote/p/8600999.html" target="_blank" onclick="clickRecomItmem(8600999)">Mysql日志管理</a><br>·  <a title="mysql  binlog 日志" href="https://www.cnblogs.com/jesper/p/7614890.html" target="_blank" onclick="clickRecomItmem(7614890)">mysql  binlog 日志</a><br>»  <a target="_blank" href="https://recomm.cnblogs.com/r?postId=9001061&amp;index=1">更多推荐...</a></div></div>
<div id="cnblogs_c2" class="c_ad_block">
<div id="div-gpt-ad-1539008685004-0" style="height:60px; width:468px;" data-google-query-id="CJyK7fqMteUCFZJ9vQodJk4Aeg">

<div id="google_ads_iframe_/1090369/C2_0__container__" style="border: 0pt none;"><iframe id="google_ads_iframe_/1090369/C2_0" title="3rd party ad content" name="google_ads_iframe_/1090369/C2_0" width="468" height="60" scrolling="no" marginwidth="0" marginheight="0" frameborder="0" srcdoc="" style="border: 0px; vertical-align: bottom;" data-google-container-id="2" data-load-complete="true"></iframe></div></div>
</div>
<div id="under_post_kb">
<div class="itnews c_ad_block">
<b>最新 IT 新闻</b>:
<br>
·              <a href="//news.cnblogs.com/n/645631/" target="_blank">Twitter第三季度净利润3700万美元 同比下滑95.6%</a>
<br>
·              <a href="//news.cnblogs.com/n/645630/" target="_blank">云上跑个分！阿里云发布国内首个云端数据库测试平台</a>
<br>
·              <a href="//news.cnblogs.com/n/645629/" target="_blank">IDC：助力转型、深入变革：中国云专业服务市场持续升温</a>
<br>
·              <a href="//news.cnblogs.com/n/645628/" target="_blank">金山办公科创板IPO注册获批 雷军为单一最大投票权拥有者</a>
<br>
·              <a href="//news.cnblogs.com/n/645627/" target="_blank">中国移动采购4400套高精度卫星定位基准站，总预算超3.36亿</a>
<br>
» <a href="https://news.cnblogs.com/" title="IT 新闻" target="_blank">更多新闻...</a>
</div></div>
<div id="HistoryToday" class="c_ad_block"></div>
<script type="text/javascript">
fixPostBody();
setTimeout(function () { incrementViewCount(cb_entryId); }, 50);
deliverAdT2();
deliverAdC1();
deliverAdC2();
loadNewsAndKb();
loadBlogSignature();
LoadPostCategoriesTags(cb_blogId, cb_entryId);        LoadPostInfoBlock(cb_blogId, cb_entryId, cb_blogApp, cb_blogUserGuid);
GetPrevNextPost(cb_entryId, cb_blogId, cb_entryCreatedDate, cb_postType);
loadOptUnderPost();
GetHistoryToday(cb_blogId, cb_blogApp, cb_entryCreatedDate);
</script>
</div>    </div>
</div>
</div>
</div>