**client_priv.h**

提供mysql客户端链接功能如下代码：
```c
MYSQL *mysql_connect_ssl_check(MYSQL *mysql_arg, const char *host,
                               const char *user, const char *passwd,
                               const char *db, uint port,
                               const char *unix_socket, ulong client_flag,
                               my_bool ssl_required MY_ATTRIBUTE((unused)))
{
  MYSQL *mysql= mysql_real_connect(mysql_arg, host, user, passwd, db, port,
                                   unix_socket, client_flag);
#if defined(HAVE_OPENSSL) && !defined(EMBEDDED_LIBRARY)
  if (mysql &&                                   /* connection established. */
      ssl_required &&                            /* --ssl-mode=REQUIRED. */
      !mysql_get_ssl_cipher(mysql))              /* non-SSL connection. */
  {
    NET *net= &mysql->net;
    net->last_errno= CR_SSL_CONNECTION_ERROR;
    strmov(net->last_error, "--ssl-mode=REQUIRED option forbids non SSL connections");
    strmov(net->sqlstate, "HY000");
    return NULL;
  }
#endif
  return mysql;
}
```

CMakeLists.txt

**completion_hash.cc
completion_hash.h**
提供hashtable的实现
```c
#include <sys/types.h>
#include <my_sys.h>

typedef struct _entry {
	char *str;
	struct _entry *pNext;
} entry;

typedef struct bucket
{
  uint h;					/* Used for numeric indexing */
  char *arKey;
  uint nKeyLength;
  uint count;
  entry *pData;
  struct bucket *pNext;
} Bucket;

typedef struct hashtable {
  uint nTableSize;
  uint initialized;
  MEM_ROOT mem_root;
  uint(*pHashFunction) (const char *arKey, uint nKeyLength);
  Bucket **arBuckets;
} HashTable;

extern int completion_hash_init(HashTable *ht, uint nSize);
extern int completion_hash_update(HashTable *ht, char *arKey, uint nKeyLength, char *str);
extern int hash_exists(HashTable *ht, char *arKey);
extern Bucket *find_all_matches(HashTable *ht, const char *str, uint length, uint *res_length);
extern Bucket *find_longest_match(HashTable *ht, char *str, uint length, uint *res_length);
extern void add_word(HashTable *ht,char *str);
extern void completion_hash_clean(HashTable *ht);
extern int completion_hash_exists(HashTable *ht, char *arKey, uint nKeyLength);
extern void completion_hash_free(HashTable *ht);

#endif /* _HASH_ */
```


**echo.c**
打印内容
```c

```


**get_password.c**
** Ask for a password from tty
** This is an own file to avoid conflicts with curses
```c
static void get_password(char *to,uint length,int fd, my_bool echo)
{
  char *pos=to,*end=to+length;

  for (;;)
  {
    char tmp;
    if (my_read(fd,&tmp,1,MYF(0)) != 1)
      break;
    if (tmp == '\b' || (int) tmp == 127)
    {
      if (pos != to)
      {
	if (echo)
	{
	  fputs("\b \b",stderr);
	  fflush(stderr);
	}
	pos--;
	continue;
      }
    }
    if (tmp == '\n' || tmp == '\r' || tmp == 3)
      break;
    if (iscntrl(tmp) || pos == end)
      continue;
    if (echo)
    {
      fputc('*',stderr);
      fflush(stderr);
    }
    *(pos++) = tmp;
  }
  while (pos != to && isspace(pos[-1]) == ' ')
    pos--;					/* Allow dummy space at end */
  *pos=0;
  return;
}

```

my_readline.h
```c

/* readline for batch mode */

typedef struct st_line_buffer
{
  File file;
  char *buffer;			/* The buffer itself, grown as needed. */
  char *end;			/* Pointer at buffer end */
  char *start_of_line,*end_of_line;
  uint bufread;			/* Number of bytes to get with each read(). */
  uint eof;
  ulong max_size;
  ulong read_length;		/* Length of last read string */
  int error;
  bool truncated;
} LINE_BUFFER;

extern LINE_BUFFER *batch_readline_init(ulong max_size,FILE *file);
extern LINE_BUFFER *batch_readline_command(LINE_BUFFER *buffer, char * str);
extern char *batch_readline(LINE_BUFFER *buffer, bool binary_mode);
extern void batch_readline_end(LINE_BUFFER *buffer);

#endif /* CLIENT_MY_READLINE_INCLUDED */

```

readline.cc
对my——readline。h实现
批量读取的实现from 文件或者文本（字符串）
```c

```


mysqladmin.cc
```c

```

mysqlbinlog.cc
```c

```

mysql.cc
/* mysql command tool
 * Commands compatible with mSQL by David J. Hughes
 * 提供mysql命令工具
 **/

```c
static COMMANDS commands[] = {
  { "?",      '?', com_help,   1, "Synonym for `help'." },
  { "clear",  'c', com_clear,  0, "Clear the current input statement."},
  { "connect",'r', com_connect,1,
    "Reconnect to the server. Optional arguments are db and host." },
  { "delimiter", 'd', com_delimiter,    1,
    "Set statement delimiter." },
#ifdef USE_POPEN
  { "edit",   'e', com_edit,   0, "Edit command with $EDITOR."},
#endif
  { "ego",    'G', com_ego,    0,
    "Send command to mysql server, display result vertically."},
  { "exit",   'q', com_quit,   0, "Exit mysql. Same as quit."},
  { "go",     'g', com_go,     0, "Send command to mysql server." },
  { "help",   'h', com_help,   1, "Display this help." },
#ifdef USE_POPEN
  { "nopager",'n', com_nopager,0, "Disable pager, print to stdout." },
#endif
  { "notee",  't', com_notee,  0, "Don't write into outfile." },
#ifdef USE_POPEN
  { "pager",  'P', com_pager,  1, 
    "Set PAGER [to_pager]. Print the query results via PAGER." },
#endif
  { "print",  'p', com_print,  0, "Print current command." },
  { "prompt", 'R', com_prompt, 1, "Change your mysql prompt."},
  { "quit",   'q', com_quit,   0, "Quit mysql." },
  { "rehash", '#', com_rehash, 0, "Rebuild completion hash." },
  { "source", '.', com_source, 1,
    "Execute an SQL script file. Takes a file name as an argument."},
  { "status", 's', com_status, 0, "Get status information from the server."},
#ifdef USE_POPEN
  { "system", '!', com_shell,  1, "Execute a system shell command."},
#endif
  { "tee",    'T', com_tee,    1, 
    "Set outfile [to_outfile]. Append everything into given outfile." },
  { "use",    'u', com_use,    1,
    "Use another database. Takes database name as argument." },
  { "charset",    'C', com_charset,    1,
    "Switch to another charset. Might be needed for processing binlog with multi-byte charsets." },
  { "warnings", 'W', com_warnings,  0,
    "Show warnings after every statement." },
  { "nowarning", 'w', com_nowarnings, 0,
    "Don't show warnings after every statement." },
  /* Get bash-like expansion for some commands */
  { "create table",     0, 0, 0, ""},
  { "create database",  0, 0, 0, ""},
  { "show databases",   0, 0, 0, ""},
  { "show fields from", 0, 0, 0, ""},
  { "show keys from",   0, 0, 0, ""},
  { "show tables",      0, 0, 0, ""},
  { "load data from",   0, 0, 0, ""},
  { "alter table",      0, 0, 0, ""},
  { "set option",       0, 0, 0, ""},
  { "lock tables",      0, 0, 0, ""},
  { "unlock tables",    0, 0, 0, ""},
  /* generated 2006-12-28.  Refresh occasionally from lexer. */
  { "ACTION", 0, 0, 0, ""},
  { "ADD", 0, 0, 0, ""},
  { "AFTER", 0, 0, 0, ""},
  { "AGAINST", 0, 0, 0, ""},
  { "AGGREGATE", 0, 0, 0, ""},
  { "ALL", 0, 0, 0, ""},
  { "ALGORITHM", 0, 0, 0, ""},
  { "ALTER", 0, 0, 0, ""},
  { "ANALYZE", 0, 0, 0, ""},
  { "AND", 0, 0, 0, ""},
  { "ANY", 0, 0, 0, ""},
  { "AS", 0, 0, 0, ""},
  { "ASC", 0, 0, 0, ""},
  { "ASCII", 0, 0, 0, ""},
  { "ASENSITIVE", 0, 0, 0, ""},
  { "AUTO_INCREMENT", 0, 0, 0, ""},
  { "AVG", 0, 0, 0, ""},
  { "AVG_ROW_LENGTH", 0, 0, 0, ""},
  { "BACKUP", 0, 0, 0, ""},
  { "BDB", 0, 0, 0, ""},
  { "BEFORE", 0, 0, 0, ""},
  { "BEGIN", 0, 0, 0, ""},
  { "BERKELEYDB", 0, 0, 0, ""},
  { "BETWEEN", 0, 0, 0, ""},
  { "BIGINT", 0, 0, 0, ""},
  { "BINARY", 0, 0, 0, ""},
  { "BINLOG", 0, 0, 0, ""},
  { "BIT", 0, 0, 0, ""},
  { "BLOB", 0, 0, 0, ""},
  { "BOOL", 0, 0, 0, ""},
  { "BOOLEAN", 0, 0, 0, ""},
  { "BOTH", 0, 0, 0, ""},
  { "BTREE", 0, 0, 0, ""},
  { "BY", 0, 0, 0, ""},
  { "BYTE", 0, 0, 0, ""},
  { "CACHE", 0, 0, 0, ""},
  { "CALL", 0, 0, 0, ""},
  { "CASCADE", 0, 0, 0, ""},
  { "CASCADED", 0, 0, 0, ""},
  { "CASE", 0, 0, 0, ""},
  { "CHAIN", 0, 0, 0, ""},
  { "CHANGE", 0, 0, 0, ""},
  { "CHANGED", 0, 0, 0, ""},
  { "CHAR", 0, 0, 0, ""},
  { "CHARACTER", 0, 0, 0, ""},
  { "CHARSET", 0, 0, 0, ""},
  { "CHECK", 0, 0, 0, ""},
  { "CHECKSUM", 0, 0, 0, ""},
  { "CIPHER", 0, 0, 0, ""},
  { "CLIENT", 0, 0, 0, ""},
  { "CLOSE", 0, 0, 0, ""},
  { "CODE", 0, 0, 0, ""},
  { "COLLATE", 0, 0, 0, ""},
  { "COLLATION", 0, 0, 0, ""},
  { "COLUMN", 0, 0, 0, ""},
  { "COLUMNS", 0, 0, 0, ""},
  { "COMMENT", 0, 0, 0, ""},
  { "COMMIT", 0, 0, 0, ""},
  { "COMMITTED", 0, 0, 0, ""},
  { "COMPACT", 0, 0, 0, ""},
  { "COMPRESSED", 0, 0, 0, ""},
  { "CONCURRENT", 0, 0, 0, ""},
  { "CONDITION", 0, 0, 0, ""},
  { "CONNECTION", 0, 0, 0, ""},
  { "CONSISTENT", 0, 0, 0, ""},
  { "CONSTRAINT", 0, 0, 0, ""},
  { "CONTAINS", 0, 0, 0, ""},
  { "CONTINUE", 0, 0, 0, ""},
  { "CONVERT", 0, 0, 0, ""},
  { "CREATE", 0, 0, 0, ""},
  { "CROSS", 0, 0, 0, ""},
  { "CUBE", 0, 0, 0, ""},
  { "CURRENT_DATE", 0, 0, 0, ""},
  { "CURRENT_TIME", 0, 0, 0, ""},
  { "CURRENT_TIMESTAMP", 0, 0, 0, ""},
  { "CURRENT_USER", 0, 0, 0, ""},
  { "CURSOR", 0, 0, 0, ""},
  { "DATA", 0, 0, 0, ""},
  { "DATABASE", 0, 0, 0, ""},
  { "DATABASES", 0, 0, 0, ""},
  { "DATE", 0, 0, 0, ""},
  { "DATETIME", 0, 0, 0, ""},
  { "DAY", 0, 0, 0, ""},
  { "DAY_HOUR", 0, 0, 0, ""},
  { "DAY_MICROSECOND", 0, 0, 0, ""},
  { "DAY_MINUTE", 0, 0, 0, ""},
  { "DAY_SECOND", 0, 0, 0, ""},
  { "DEALLOCATE", 0, 0, 0, ""},     
  { "DEC", 0, 0, 0, ""},
  { "DECIMAL", 0, 0, 0, ""},
  { "DECLARE", 0, 0, 0, ""},
  { "DEFAULT", 0, 0, 0, ""},
  { "DEFINER", 0, 0, 0, ""},
  { "DELAYED", 0, 0, 0, ""},
  { "DELAY_KEY_WRITE", 0, 0, 0, ""},
  { "DELETE", 0, 0, 0, ""},
  { "DESC", 0, 0, 0, ""},
  { "DESCRIBE", 0, 0, 0, ""},
  { "DES_KEY_FILE", 0, 0, 0, ""},
  { "DETERMINISTIC", 0, 0, 0, ""},
  { "DIRECTORY", 0, 0, 0, ""},
  { "DISABLE", 0, 0, 0, ""},
  { "DISCARD", 0, 0, 0, ""},
  { "DISTINCT", 0, 0, 0, ""},
  { "DISTINCTROW", 0, 0, 0, ""},
  { "DIV", 0, 0, 0, ""},
  { "DO", 0, 0, 0, ""},
  { "DOUBLE", 0, 0, 0, ""},
  { "DROP", 0, 0, 0, ""},
  { "DUAL", 0, 0, 0, ""},
  { "DUMPFILE", 0, 0, 0, ""},
  { "DUPLICATE", 0, 0, 0, ""},
  { "DYNAMIC", 0, 0, 0, ""},
  { "EACH", 0, 0, 0, ""},
  { "ELSE", 0, 0, 0, ""},
  { "ELSEIF", 0, 0, 0, ""},
  { "ENABLE", 0, 0, 0, ""},
  { "ENCLOSED", 0, 0, 0, ""},
  { "END", 0, 0, 0, ""},
  { "ENGINE", 0, 0, 0, ""},
  { "ENGINES", 0, 0, 0, ""},
  { "ENUM", 0, 0, 0, ""},
  { "ERRORS", 0, 0, 0, ""},
  { "ESCAPE", 0, 0, 0, ""},
  { "ESCAPED", 0, 0, 0, ""},
  { "EVENTS", 0, 0, 0, ""},
  { "EXECUTE", 0, 0, 0, ""},
  { "EXISTS", 0, 0, 0, ""},
  { "EXIT", 0, 0, 0, ""},
  { "EXPANSION", 0, 0, 0, ""},
  { "EXPLAIN", 0, 0, 0, ""},
  { "EXTENDED", 0, 0, 0, ""},
  { "FALSE", 0, 0, 0, ""},
  { "FAST", 0, 0, 0, ""},
  { "FETCH", 0, 0, 0, ""},
  { "FIELDS", 0, 0, 0, ""},
  { "FILE", 0, 0, 0, ""},
  { "FIRST", 0, 0, 0, ""},
  { "FIXED", 0, 0, 0, ""},
  { "FLOAT", 0, 0, 0, ""},
  { "FLOAT4", 0, 0, 0, ""},
  { "FLOAT8", 0, 0, 0, ""},
  { "FLUSH", 0, 0, 0, ""},
  { "FOR", 0, 0, 0, ""},
  { "FORCE", 0, 0, 0, ""},
  { "FOREIGN", 0, 0, 0, ""},
  { "FOUND", 0, 0, 0, ""},
  { "FROM", 0, 0, 0, ""},
  { "FULL", 0, 0, 0, ""},
  { "FULLTEXT", 0, 0, 0, ""},
  { "FUNCTION", 0, 0, 0, ""},
  { "GEOMETRY", 0, 0, 0, ""},
  { "GEOMETRYCOLLECTION", 0, 0, 0, ""},
  { "GET_FORMAT", 0, 0, 0, ""},
  { "GLOBAL", 0, 0, 0, ""},
  { "GRANT", 0, 0, 0, ""},
  { "GRANTS", 0, 0, 0, ""},
  { "GROUP", 0, 0, 0, ""},
  { "HANDLER", 0, 0, 0, ""},
  { "HASH", 0, 0, 0, ""},
  { "HAVING", 0, 0, 0, ""},
  { "HELP", 0, 0, 0, ""},
  { "HIGH_PRIORITY", 0, 0, 0, ""},
  { "HOSTS", 0, 0, 0, ""},
  { "HOUR", 0, 0, 0, ""},
  { "HOUR_MICROSECOND", 0, 0, 0, ""},
  { "HOUR_MINUTE", 0, 0, 0, ""},
  { "HOUR_SECOND", 0, 0, 0, ""},
  { "IDENTIFIED", 0, 0, 0, ""},
  { "IF", 0, 0, 0, ""},
  { "IGNORE", 0, 0, 0, ""},
  { "IMPORT", 0, 0, 0, ""},
  { "IN", 0, 0, 0, ""},
  { "INDEX", 0, 0, 0, ""},
  { "INDEXES", 0, 0, 0, ""},
  { "INFILE", 0, 0, 0, ""},
  { "INNER", 0, 0, 0, ""},
  { "INNOBASE", 0, 0, 0, ""},
  { "INNODB", 0, 0, 0, ""},
  { "INOUT", 0, 0, 0, ""},
  { "INSENSITIVE", 0, 0, 0, ""},
  { "INSERT", 0, 0, 0, ""},
  { "INSERT_METHOD", 0, 0, 0, ""},
  { "INT", 0, 0, 0, ""},
  { "INT1", 0, 0, 0, ""},
  { "INT2", 0, 0, 0, ""},
  { "INT3", 0, 0, 0, ""},
  { "INT4", 0, 0, 0, ""},
  { "INT8", 0, 0, 0, ""},
  { "INTEGER", 0, 0, 0, ""},
  { "INTERVAL", 0, 0, 0, ""},
  { "INTO", 0, 0, 0, ""},
  { "IO_THREAD", 0, 0, 0, ""},
  { "IS", 0, 0, 0, ""},
  { "ISOLATION", 0, 0, 0, ""},
  { "ISSUER", 0, 0, 0, ""},
  { "ITERATE", 0, 0, 0, ""},
  { "INVOKER", 0, 0, 0, ""},
  { "JOIN", 0, 0, 0, ""},
  { "KEY", 0, 0, 0, ""},
  { "KEYS", 0, 0, 0, ""},
  { "KILL", 0, 0, 0, ""},
  { "LANGUAGE", 0, 0, 0, ""},
  { "LAST", 0, 0, 0, ""},
  { "LEADING", 0, 0, 0, ""},
  { "LEAVE", 0, 0, 0, ""},
  { "LEAVES", 0, 0, 0, ""},
  { "LEFT", 0, 0, 0, ""},
  { "LEVEL", 0, 0, 0, ""},
  { "LIKE", 0, 0, 0, ""},
  { "LIMIT", 0, 0, 0, ""},
  { "LINES", 0, 0, 0, ""},
  { "LINESTRING", 0, 0, 0, ""},
  { "LOAD", 0, 0, 0, ""},
  { "LOCAL", 0, 0, 0, ""},
  { "LOCALTIME", 0, 0, 0, ""},
  { "LOCALTIMESTAMP", 0, 0, 0, ""},
  { "LOCK", 0, 0, 0, ""},
  { "LOCKS", 0, 0, 0, ""},
  { "LOGS", 0, 0, 0, ""},
  { "LONG", 0, 0, 0, ""},
  { "LONGBLOB", 0, 0, 0, ""},
  { "LONGTEXT", 0, 0, 0, ""},
  { "LOOP", 0, 0, 0, ""},
  { "LOW_PRIORITY", 0, 0, 0, ""},
  { "MASTER", 0, 0, 0, ""},
  { "MASTER_CONNECT_RETRY", 0, 0, 0, ""},
  { "MASTER_HOST", 0, 0, 0, ""},
  { "MASTER_LOG_FILE", 0, 0, 0, ""},
  { "MASTER_LOG_POS", 0, 0, 0, ""},
  { "MASTER_PASSWORD", 0, 0, 0, ""},
  { "MASTER_PORT", 0, 0, 0, ""},
  { "MASTER_SERVER_ID", 0, 0, 0, ""},
  { "MASTER_SSL", 0, 0, 0, ""},
  { "MASTER_SSL_CA", 0, 0, 0, ""},
  { "MASTER_SSL_CAPATH", 0, 0, 0, ""},
  { "MASTER_SSL_CERT", 0, 0, 0, ""},
  { "MASTER_SSL_CIPHER", 0, 0, 0, ""},
  { "MASTER_SSL_KEY", 0, 0, 0, ""},
  { "MASTER_USER", 0, 0, 0, ""},
  { "MATCH", 0, 0, 0, ""},
  { "MAX_CONNECTIONS_PER_HOUR", 0, 0, 0, ""},
  { "MAX_QUERIES_PER_HOUR", 0, 0, 0, ""},
  { "MAX_ROWS", 0, 0, 0, ""},
  { "MAX_UPDATES_PER_HOUR", 0, 0, 0, ""},
  { "MAX_USER_CONNECTIONS", 0, 0, 0, ""},
  { "MEDIUM", 0, 0, 0, ""},
  { "MEDIUMBLOB", 0, 0, 0, ""},
  { "MEDIUMINT", 0, 0, 0, ""},
  { "MEDIUMTEXT", 0, 0, 0, ""},
  { "MERGE", 0, 0, 0, ""},
  { "MICROSECOND", 0, 0, 0, ""},
  { "MIDDLEINT", 0, 0, 0, ""},
  { "MIGRATE", 0, 0, 0, ""},
  { "MINUTE", 0, 0, 0, ""},
  { "MINUTE_MICROSECOND", 0, 0, 0, ""},
  { "MINUTE_SECOND", 0, 0, 0, ""},
  { "MIN_ROWS", 0, 0, 0, ""},
  { "MOD", 0, 0, 0, ""},
  { "MODE", 0, 0, 0, ""},
  { "MODIFIES", 0, 0, 0, ""},
  { "MODIFY", 0, 0, 0, ""},
  { "MONTH", 0, 0, 0, ""},
  { "MULTILINESTRING", 0, 0, 0, ""},
  { "MULTIPOINT", 0, 0, 0, ""},
  { "MULTIPOLYGON", 0, 0, 0, ""},
  { "MUTEX", 0, 0, 0, ""},
  { "NAME", 0, 0, 0, ""},
  { "NAMES", 0, 0, 0, ""},
  { "NATIONAL", 0, 0, 0, ""},
  { "NATURAL", 0, 0, 0, ""},
  { "NDB", 0, 0, 0, ""},
  { "NDBCLUSTER", 0, 0, 0, ""},
  { "NCHAR", 0, 0, 0, ""},
  { "NEW", 0, 0, 0, ""},
  { "NEXT", 0, 0, 0, ""},
  { "NO", 0, 0, 0, ""},
  { "NONE", 0, 0, 0, ""},
  { "NOT", 0, 0, 0, ""},
  { "NO_WRITE_TO_BINLOG", 0, 0, 0, ""},
  { "NULL", 0, 0, 0, ""},
  { "NUMERIC", 0, 0, 0, ""},
  { "NVARCHAR", 0, 0, 0, ""},
  { "OFFSET", 0, 0, 0, ""},
  { "OLD_PASSWORD", 0, 0, 0, ""},
  { "ON", 0, 0, 0, ""},
  { "ONE", 0, 0, 0, ""},
  { "ONE_SHOT", 0, 0, 0, ""},
  { "OPEN", 0, 0, 0, ""},
  { "OPTIMIZE", 0, 0, 0, ""},
  { "OPTION", 0, 0, 0, ""},
  { "OPTIONALLY", 0, 0, 0, ""},
  { "OR", 0, 0, 0, ""},
  { "ORDER", 0, 0, 0, ""},
  { "OUT", 0, 0, 0, ""},
  { "OUTER", 0, 0, 0, ""},
  { "OUTFILE", 0, 0, 0, ""},
  { "PACK_KEYS", 0, 0, 0, ""},
  { "PARTIAL", 0, 0, 0, ""},
  { "PASSWORD", 0, 0, 0, ""},
  { "PHASE", 0, 0, 0, ""},
  { "POINT", 0, 0, 0, ""},
  { "POLYGON", 0, 0, 0, ""},
  { "PRECISION", 0, 0, 0, ""},
  { "PREPARE", 0, 0, 0, ""},
  { "PREV", 0, 0, 0, ""},
  { "PRIMARY", 0, 0, 0, ""},
  { "PRIVILEGES", 0, 0, 0, ""},
  { "PROCEDURE", 0, 0, 0, ""},
  { "PROCESS", 0, 0, 0, ""},
  { "PROCESSLIST", 0, 0, 0, ""},
  { "PURGE", 0, 0, 0, ""},
  { "QUARTER", 0, 0, 0, ""},
  { "QUERY", 0, 0, 0, ""},
  { "QUICK", 0, 0, 0, ""},
  { "READ", 0, 0, 0, ""},
  { "READS", 0, 0, 0, ""},
  { "REAL", 0, 0, 0, ""},
  { "RECOVER", 0, 0, 0, ""},
  { "REDUNDANT", 0, 0, 0, ""},
  { "REFERENCES", 0, 0, 0, ""},
  { "REGEXP", 0, 0, 0, ""},
  { "RELAY_LOG_FILE", 0, 0, 0, ""},
  { "RELAY_LOG_POS", 0, 0, 0, ""},
  { "RELAY_THREAD", 0, 0, 0, ""},
  { "RELEASE", 0, 0, 0, ""},
  { "RELOAD", 0, 0, 0, ""},
  { "RENAME", 0, 0, 0, ""},
  { "REPAIR", 0, 0, 0, ""},
  { "REPEATABLE", 0, 0, 0, ""},
  { "REPLACE", 0, 0, 0, ""},
  { "REPLICATION", 0, 0, 0, ""},
  { "REPEAT", 0, 0, 0, ""},
  { "REQUIRE", 0, 0, 0, ""},
  { "RESET", 0, 0, 0, ""},
  { "RESTORE", 0, 0, 0, ""},
  { "RESTRICT", 0, 0, 0, ""},
  { "RESUME", 0, 0, 0, ""},
  { "RETURN", 0, 0, 0, ""},
  { "RETURNS", 0, 0, 0, ""},
  { "REVOKE", 0, 0, 0, ""},
  { "RIGHT", 0, 0, 0, ""},
  { "RLIKE", 0, 0, 0, ""},
  { "ROLLBACK", 0, 0, 0, ""},
  { "ROLLUP", 0, 0, 0, ""},
  { "ROUTINE", 0, 0, 0, ""},
  { "ROW", 0, 0, 0, ""},
  { "ROWS", 0, 0, 0, ""},
  { "ROW_FORMAT", 0, 0, 0, ""},
  { "RTREE", 0, 0, 0, ""},
  { "SAVEPOINT", 0, 0, 0, ""},
  { "SCHEMA", 0, 0, 0, ""},
  { "SCHEMAS", 0, 0, 0, ""},
  { "SECOND", 0, 0, 0, ""},
  { "SECOND_MICROSECOND", 0, 0, 0, ""},
  { "SECURITY", 0, 0, 0, ""},
  { "SELECT", 0, 0, 0, ""},
  { "SENSITIVE", 0, 0, 0, ""},
  { "SEPARATOR", 0, 0, 0, ""},
  { "SERIAL", 0, 0, 0, ""},
  { "SERIALIZABLE", 0, 0, 0, ""},
  { "SESSION", 0, 0, 0, ""},
  { "SET", 0, 0, 0, ""},
  { "SHARE", 0, 0, 0, ""},
  { "SHOW", 0, 0, 0, ""},
  { "SHUTDOWN", 0, 0, 0, ""},
  { "SIGNED", 0, 0, 0, ""},
  { "SIMPLE", 0, 0, 0, ""},
  { "SLAVE", 0, 0, 0, ""},
  { "SNAPSHOT", 0, 0, 0, ""},
  { "SMALLINT", 0, 0, 0, ""},
  { "SOME", 0, 0, 0, ""},
  { "SONAME", 0, 0, 0, ""},
  { "SOUNDS", 0, 0, 0, ""},
  { "SPATIAL", 0, 0, 0, ""},
  { "SPECIFIC", 0, 0, 0, ""},
  { "SQL", 0, 0, 0, ""},
  { "SQLEXCEPTION", 0, 0, 0, ""},
  { "SQLSTATE", 0, 0, 0, ""},
  { "SQLWARNING", 0, 0, 0, ""},
  { "SQL_BIG_RESULT", 0, 0, 0, ""},
  { "SQL_BUFFER_RESULT", 0, 0, 0, ""},
  { "SQL_CACHE", 0, 0, 0, ""},
  { "SQL_CALC_FOUND_ROWS", 0, 0, 0, ""},
  { "SQL_NO_CACHE", 0, 0, 0, ""},
  { "SQL_SMALL_RESULT", 0, 0, 0, ""},
  { "SQL_THREAD", 0, 0, 0, ""},
  { "SQL_TSI_SECOND", 0, 0, 0, ""},
  { "SQL_TSI_MINUTE", 0, 0, 0, ""},
  { "SQL_TSI_HOUR", 0, 0, 0, ""},
  { "SQL_TSI_DAY", 0, 0, 0, ""},
  { "SQL_TSI_WEEK", 0, 0, 0, ""},
  { "SQL_TSI_MONTH", 0, 0, 0, ""},
  { "SQL_TSI_QUARTER", 0, 0, 0, ""},
  { "SQL_TSI_YEAR", 0, 0, 0, ""},
  { "SSL", 0, 0, 0, ""},
  { "START", 0, 0, 0, ""},
  { "STARTING", 0, 0, 0, ""},
  { "STATUS", 0, 0, 0, ""},
  { "STOP", 0, 0, 0, ""},
  { "STORAGE", 0, 0, 0, ""},
  { "STRAIGHT_JOIN", 0, 0, 0, ""},
  { "STRING", 0, 0, 0, ""},
  { "STRIPED", 0, 0, 0, ""},
  { "SUBJECT", 0, 0, 0, ""},
  { "SUPER", 0, 0, 0, ""},
  { "SUSPEND", 0, 0, 0, ""},
  { "TABLE", 0, 0, 0, ""},
  { "TABLES", 0, 0, 0, ""},
  { "TABLESPACE", 0, 0, 0, ""},
  { "TEMPORARY", 0, 0, 0, ""},
  { "TEMPTABLE", 0, 0, 0, ""},
  { "TERMINATED", 0, 0, 0, ""},
  { "TEXT", 0, 0, 0, ""},
  { "THEN", 0, 0, 0, ""},
  { "TIME", 0, 0, 0, ""},
  { "TIMESTAMP", 0, 0, 0, ""},
  { "TIMESTAMPADD", 0, 0, 0, ""},
  { "TIMESTAMPDIFF", 0, 0, 0, ""},
  { "TINYBLOB", 0, 0, 0, ""},
  { "TINYINT", 0, 0, 0, ""},
  { "TINYTEXT", 0, 0, 0, ""},
  { "TO", 0, 0, 0, ""},
  { "TRAILING", 0, 0, 0, ""},
  { "TRANSACTION", 0, 0, 0, ""},
  { "TRIGGER", 0, 0, 0, ""},
  { "TRIGGERS", 0, 0, 0, ""},
  { "TRUE", 0, 0, 0, ""},
  { "TRUNCATE", 0, 0, 0, ""},
  { "TYPE", 0, 0, 0, ""},
  { "TYPES", 0, 0, 0, ""},
  { "UNCOMMITTED", 0, 0, 0, ""},
  { "UNDEFINED", 0, 0, 0, ""},
  { "UNDO", 0, 0, 0, ""},
  { "UNICODE", 0, 0, 0, ""},
  { "UNION", 0, 0, 0, ""},
  { "UNIQUE", 0, 0, 0, ""},
  { "UNKNOWN", 0, 0, 0, ""},
  { "UNLOCK", 0, 0, 0, ""},
  { "UNSIGNED", 0, 0, 0, ""},
  { "UNTIL", 0, 0, 0, ""},
  { "UPDATE", 0, 0, 0, ""},
  { "UPGRADE", 0, 0, 0, ""},
  { "USAGE", 0, 0, 0, ""},
  { "USE", 0, 0, 0, ""},
  { "USER", 0, 0, 0, ""},
  { "USER_RESOURCES", 0, 0, 0, ""},
  { "USE_FRM", 0, 0, 0, ""},
  { "USING", 0, 0, 0, ""},
  { "UTC_DATE", 0, 0, 0, ""},
  { "UTC_TIME", 0, 0, 0, ""},
  { "UTC_TIMESTAMP", 0, 0, 0, ""},
  { "VALUE", 0, 0, 0, ""},
  { "VALUES", 0, 0, 0, ""},
  { "VARBINARY", 0, 0, 0, ""},
  { "VARCHAR", 0, 0, 0, ""},
  { "VARCHARACTER", 0, 0, 0, ""},
  { "VARIABLES", 0, 0, 0, ""},
  { "VARYING", 0, 0, 0, ""},
  { "WARNINGS", 0, 0, 0, ""},
  { "WEEK", 0, 0, 0, ""},
  { "WHEN", 0, 0, 0, ""},
  { "WHERE", 0, 0, 0, ""},
  { "WHILE", 0, 0, 0, ""},
  { "VIEW", 0, 0, 0, ""},
  { "WITH", 0, 0, 0, ""},
  { "WORK", 0, 0, 0, ""},
  { "WRITE", 0, 0, 0, ""},
  { "X509", 0, 0, 0, ""},
  { "XOR", 0, 0, 0, ""},
  { "XA", 0, 0, 0, ""},
  { "YEAR", 0, 0, 0, ""},
  { "YEAR_MONTH", 0, 0, 0, ""},
  { "ZEROFILL", 0, 0, 0, ""},
  { "ABS", 0, 0, 0, ""},
  { "ACOS", 0, 0, 0, ""},
  { "ADDDATE", 0, 0, 0, ""},
  { "ADDTIME", 0, 0, 0, ""},
  { "AES_ENCRYPT", 0, 0, 0, ""},
  { "AES_DECRYPT", 0, 0, 0, ""},
  { "AREA", 0, 0, 0, ""},
  { "ASIN", 0, 0, 0, ""},
  { "ASBINARY", 0, 0, 0, ""},
  { "ASTEXT", 0, 0, 0, ""},
  { "ASWKB", 0, 0, 0, ""},
  { "ASWKT", 0, 0, 0, ""},
  { "ATAN", 0, 0, 0, ""},
  { "ATAN2", 0, 0, 0, ""},
  { "BENCHMARK", 0, 0, 0, ""},
  { "BIN", 0, 0, 0, ""},
  { "BIT_COUNT", 0, 0, 0, ""},
  { "BIT_OR", 0, 0, 0, ""},
  { "BIT_AND", 0, 0, 0, ""},
  { "BIT_XOR", 0, 0, 0, ""},
  { "CAST", 0, 0, 0, ""},
  { "CEIL", 0, 0, 0, ""},
  { "CEILING", 0, 0, 0, ""},
  { "BIT_LENGTH", 0, 0, 0, ""},
  { "CENTROID", 0, 0, 0, ""},
  { "CHAR_LENGTH", 0, 0, 0, ""},
  { "CHARACTER_LENGTH", 0, 0, 0, ""},
  { "COALESCE", 0, 0, 0, ""},
  { "COERCIBILITY", 0, 0, 0, ""},
  { "COMPRESS", 0, 0, 0, ""},
  { "CONCAT", 0, 0, 0, ""},
  { "CONCAT_WS", 0, 0, 0, ""},
  { "CONNECTION_ID", 0, 0, 0, ""},
  { "CONV", 0, 0, 0, ""},
  { "CONVERT_TZ", 0, 0, 0, ""},
  { "COUNT", 0, 0, 0, ""},
  { "COS", 0, 0, 0, ""},
  { "COT", 0, 0, 0, ""},
  { "CRC32", 0, 0, 0, ""},
  { "CROSSES", 0, 0, 0, ""},
  { "CURDATE", 0, 0, 0, ""},
  { "CURTIME", 0, 0, 0, ""},
  { "DATE_ADD", 0, 0, 0, ""},
  { "DATEDIFF", 0, 0, 0, ""},
  { "DATE_FORMAT", 0, 0, 0, ""},
  { "DATE_SUB", 0, 0, 0, ""},
  { "DAYNAME", 0, 0, 0, ""},
  { "DAYOFMONTH", 0, 0, 0, ""},
  { "DAYOFWEEK", 0, 0, 0, ""},
  { "DAYOFYEAR", 0, 0, 0, ""},
  { "DECODE", 0, 0, 0, ""},
  { "DEGREES", 0, 0, 0, ""},
  { "DES_ENCRYPT", 0, 0, 0, ""},
  { "DES_DECRYPT", 0, 0, 0, ""},
  { "DIMENSION", 0, 0, 0, ""},
  { "DISJOINT", 0, 0, 0, ""},
  { "ELT", 0, 0, 0, ""},
  { "ENCODE", 0, 0, 0, ""},
  { "ENCRYPT", 0, 0, 0, ""},
  { "ENDPOINT", 0, 0, 0, ""},
  { "ENVELOPE", 0, 0, 0, ""},
  { "EQUALS", 0, 0, 0, ""},
  { "EXTERIORRING", 0, 0, 0, ""},
  { "EXTRACT", 0, 0, 0, ""},
  { "EXP", 0, 0, 0, ""},
  { "EXPORT_SET", 0, 0, 0, ""},
  { "FIELD", 0, 0, 0, ""},
  { "FIND_IN_SET", 0, 0, 0, ""},
  { "FLOOR", 0, 0, 0, ""},
  { "FORMAT", 0, 0, 0, ""},
  { "FOUND_ROWS", 0, 0, 0, ""},
  { "FROM_DAYS", 0, 0, 0, ""},
  { "FROM_UNIXTIME", 0, 0, 0, ""},
  { "GET_LOCK", 0, 0, 0, ""},
  { "GEOMETRYN", 0, 0, 0, ""},
  { "GEOMETRYTYPE", 0, 0, 0, ""},
  { "GEOMCOLLFROMTEXT", 0, 0, 0, ""},
  { "GEOMCOLLFROMWKB", 0, 0, 0, ""},
  { "GEOMETRYCOLLECTIONFROMTEXT", 0, 0, 0, ""},
  { "GEOMETRYCOLLECTIONFROMWKB", 0, 0, 0, ""},
  { "GEOMETRYFROMTEXT", 0, 0, 0, ""},
  { "GEOMETRYFROMWKB", 0, 0, 0, ""},
  { "GEOMFROMTEXT", 0, 0, 0, ""},
  { "GEOMFROMWKB", 0, 0, 0, ""},
  { "GLENGTH", 0, 0, 0, ""},
  { "GREATEST", 0, 0, 0, ""},
  { "GROUP_CONCAT", 0, 0, 0, ""},
  { "GROUP_UNIQUE_USERS", 0, 0, 0, ""},
  { "HEX", 0, 0, 0, ""},
  { "IFNULL", 0, 0, 0, ""},
  { "INET_ATON", 0, 0, 0, ""},
  { "INET_NTOA", 0, 0, 0, ""},
  { "INSTR", 0, 0, 0, ""},
  { "INTERIORRINGN", 0, 0, 0, ""},
  { "INTERSECTS", 0, 0, 0, ""},
  { "ISCLOSED", 0, 0, 0, ""},
  { "ISEMPTY", 0, 0, 0, ""},
  { "ISNULL", 0, 0, 0, ""},
  { "IS_FREE_LOCK", 0, 0, 0, ""},
  { "IS_USED_LOCK", 0, 0, 0, ""},
  { "LAST_INSERT_ID", 0, 0, 0, ""},
  { "ISSIMPLE", 0, 0, 0, ""},
  { "LAST_DAY", 0, 0, 0, ""},
  { "LCASE", 0, 0, 0, ""},
  { "LEAST", 0, 0, 0, ""},
  { "LENGTH", 0, 0, 0, ""},
  { "LN", 0, 0, 0, ""},
  { "LINEFROMTEXT", 0, 0, 0, ""},
  { "LINEFROMWKB", 0, 0, 0, ""},
  { "LINESTRINGFROMTEXT", 0, 0, 0, ""},
  { "LINESTRINGFROMWKB", 0, 0, 0, ""},
  { "LOAD_FILE", 0, 0, 0, ""},
  { "LOCATE", 0, 0, 0, ""},
  { "LOG", 0, 0, 0, ""},
  { "LOG2", 0, 0, 0, ""},
  { "LOG10", 0, 0, 0, ""},
  { "LOWER", 0, 0, 0, ""},
  { "LPAD", 0, 0, 0, ""},
  { "LTRIM", 0, 0, 0, ""},
  { "MAKE_SET", 0, 0, 0, ""},
  { "MAKEDATE", 0, 0, 0, ""},
  { "MAKETIME", 0, 0, 0, ""},
  { "MASTER_POS_WAIT", 0, 0, 0, ""},
  { "MAX", 0, 0, 0, ""},
  { "MBRCONTAINS", 0, 0, 0, ""},
  { "MBRDISJOINT", 0, 0, 0, ""},
  { "MBREQUAL", 0, 0, 0, ""},
  { "MBRINTERSECTS", 0, 0, 0, ""},
  { "MBROVERLAPS", 0, 0, 0, ""},
  { "MBRTOUCHES", 0, 0, 0, ""},
  { "MBRWITHIN", 0, 0, 0, ""},
  { "MD5", 0, 0, 0, ""},
  { "MID", 0, 0, 0, ""},
  { "MIN", 0, 0, 0, ""},
  { "MLINEFROMTEXT", 0, 0, 0, ""},
  { "MLINEFROMWKB", 0, 0, 0, ""},
  { "MPOINTFROMTEXT", 0, 0, 0, ""},
  { "MPOINTFROMWKB", 0, 0, 0, ""},
  { "MPOLYFROMTEXT", 0, 0, 0, ""},
  { "MPOLYFROMWKB", 0, 0, 0, ""},
  { "MONTHNAME", 0, 0, 0, ""},
  { "MULTILINESTRINGFROMTEXT", 0, 0, 0, ""},
  { "MULTILINESTRINGFROMWKB", 0, 0, 0, ""},
  { "MULTIPOINTFROMTEXT", 0, 0, 0, ""},
  { "MULTIPOINTFROMWKB", 0, 0, 0, ""},
  { "MULTIPOLYGONFROMTEXT", 0, 0, 0, ""},
  { "MULTIPOLYGONFROMWKB", 0, 0, 0, ""},
  { "NAME_CONST", 0, 0, 0, ""},
  { "NOW", 0, 0, 0, ""},
  { "NULLIF", 0, 0, 0, ""},
  { "NUMGEOMETRIES", 0, 0, 0, ""},
  { "NUMINTERIORRINGS", 0, 0, 0, ""},
  { "NUMPOINTS", 0, 0, 0, ""},
  { "OCTET_LENGTH", 0, 0, 0, ""},
  { "OCT", 0, 0, 0, ""},
  { "ORD", 0, 0, 0, ""},
  { "OVERLAPS", 0, 0, 0, ""},
  { "PERIOD_ADD", 0, 0, 0, ""},
  { "PERIOD_DIFF", 0, 0, 0, ""},
  { "PI", 0, 0, 0, ""},
  { "POINTFROMTEXT", 0, 0, 0, ""},
  { "POINTFROMWKB", 0, 0, 0, ""},
  { "POINTN", 0, 0, 0, ""},
  { "POLYFROMTEXT", 0, 0, 0, ""},
  { "POLYFROMWKB", 0, 0, 0, ""},
  { "POLYGONFROMTEXT", 0, 0, 0, ""},
  { "POLYGONFROMWKB", 0, 0, 0, ""},
  { "POSITION", 0, 0, 0, ""},
  { "POW", 0, 0, 0, ""},
  { "POWER", 0, 0, 0, ""},
  { "QUOTE", 0, 0, 0, ""},
  { "RADIANS", 0, 0, 0, ""},
  { "RAND", 0, 0, 0, ""},
  { "RELEASE_LOCK", 0, 0, 0, ""},
  { "REVERSE", 0, 0, 0, ""},
  { "ROUND", 0, 0, 0, ""},
  { "ROW_COUNT", 0, 0, 0, ""},
  { "RPAD", 0, 0, 0, ""},
  { "RTRIM", 0, 0, 0, ""},
  { "SEC_TO_TIME", 0, 0, 0, ""},
  { "SESSION_USER", 0, 0, 0, ""},
  { "SUBDATE", 0, 0, 0, ""},
  { "SIGN", 0, 0, 0, ""},
  { "SIN", 0, 0, 0, ""},
  { "SHA", 0, 0, 0, ""},
  { "SHA1", 0, 0, 0, ""},
  { "SLEEP", 0, 0, 0, ""},
  { "SOUNDEX", 0, 0, 0, ""},
  { "SPACE", 0, 0, 0, ""},
  { "SQRT", 0, 0, 0, ""},
  { "SRID", 0, 0, 0, ""},
  { "STARTPOINT", 0, 0, 0, ""},
  { "STD", 0, 0, 0, ""},
  { "STDDEV", 0, 0, 0, ""},
  { "STDDEV_POP", 0, 0, 0, ""},
  { "STDDEV_SAMP", 0, 0, 0, ""},
  { "STR_TO_DATE", 0, 0, 0, ""},
  { "STRCMP", 0, 0, 0, ""},
  { "SUBSTR", 0, 0, 0, ""},
  { "SUBSTRING", 0, 0, 0, ""},
  { "SUBSTRING_INDEX", 0, 0, 0, ""},
  { "SUBTIME", 0, 0, 0, ""},
  { "SUM", 0, 0, 0, ""},
  { "SYSDATE", 0, 0, 0, ""},
  { "SYSTEM_USER", 0, 0, 0, ""},
  { "TAN", 0, 0, 0, ""},
  { "TIME_FORMAT", 0, 0, 0, ""},
  { "TIME_TO_SEC", 0, 0, 0, ""},
  { "TIMEDIFF", 0, 0, 0, ""},
  { "TO_DAYS", 0, 0, 0, ""},
  { "TOUCHES", 0, 0, 0, ""},
  { "TRIM", 0, 0, 0, ""},
  { "UCASE", 0, 0, 0, ""},
  { "UNCOMPRESS", 0, 0, 0, ""},
  { "UNCOMPRESSED_LENGTH", 0, 0, 0, ""},
  { "UNHEX", 0, 0, 0, ""},
  { "UNIQUE_USERS", 0, 0, 0, ""},
  { "UNIX_TIMESTAMP", 0, 0, 0, ""},
  { "UPPER", 0, 0, 0, ""},
  { "UUID", 0, 0, 0, ""},
  { "VARIANCE", 0, 0, 0, ""},
  { "VAR_POP", 0, 0, 0, ""},
  { "VAR_SAMP", 0, 0, 0, ""},
  { "VERSION", 0, 0, 0, ""},
  { "WEEKDAY", 0, 0, 0, ""},
  { "WEEKOFYEAR", 0, 0, 0, ""},
  { "WITHIN", 0, 0, 0, ""},
  { "X", 0, 0, 0, ""},
  { "Y", 0, 0, 0, ""},
  { "YEARWEEK", 0, 0, 0, ""},
  /* end sentinel */
  { (char *)NULL,       0, 0, 0, ""}
};


```

mysqlcheck.c
```c

```

mysql_config_editor.cc
```c

```

mysqldump.c
```c

```

mysqlimport.c
```c

```

mysql_plugin.c
```c

```

mysqlshow.c
```c

```

mysqlslap.c
```c

```

mysqltest.cc
```c

```

mysql_upgrade.c
```c

```


sql_string.cc
```c

```

sql_string.h
```c

```