### 1.sql调用流程
sql_connect.cc-do_handle_one_connection[调用do_command]
    sql_parse.cc-do_command[调用dispatch_command]
    sql_parse.cc-dispatch_command[执行具体sql逻辑]
    sql_parse.mysql_execute_command
    sql_insert.mysql_insert.651
        lock.cc.299
    insert_precheck
    open_and_lock_tables
### 2.debug日志
```cassandraql

atement
T@6    : | | | >Protocol::send_ok
T@6    : | | | | >net_send_ok
T@6    : | | | | | info: affected_rows: 0  id: 0  status: 2  warning_count: 0
T@6    : | | | | | packet_header: Memory: 0x70000d3cc5ec  Bytes: (4)
07 00 00 01 
T@6    : | | | | | >net_flush
T@6    : | | | | | | >vio_is_blocking
T@6    : | | | | | | | exit: 0
T@6    : | | | | | | <vio_is_blocking 203
T@6    : | | | | | | >net_real_write
T@6    : | | | | | | | >Query_cache::insert
T@6    : | | | | | | | <Query_cache::insert 901
T@6    : | | | | | | | >my_malloc
T@6    : | | | | | | | | my: size: 18  my_flags: 16
T@6    : | | | | | | | | exit: ptr: 0x7fdddc286eb0
T@6    : | | | | | | | <my_malloc 66
T@6    : | | | | | | | >my_compress
T@6    : | | | | | | | | note: Packet too short: Not compressed
T@6    : | | | | | | | <my_compress 56
T@6    : | | | | | | | >vio_write
T@6    : | | | | | | | | enter: sd: 34  buf: 0x7fdddc286eb0  size: 18
T@6    : | | | | | | | | exit: 18
T@6    : | | | | | | | <vio_write 136
T@6    : | | | | | | | >my_free
T@6    : | | | | | | | | my: ptr: 0x7fdddc286eb0
T@6    : | | | | | | | <my_free 135
T@6    : | | | | | | <net_real_write 709
T@6    : | | | | | <net_flush 342
T@6    : | | | | | info: OK sent, so no more error sending allowed
T@6    : | | | | <net_send_ok 257
T@6    : | | | <Protocol::send_ok 545
T@6    : | | <Protocol::end_statement 524
T@6    : | | >Query_cache::end_of_result
T@6    : | | <Query_cache::end_of_result 997
T@6    : | | >log_slow_statement
T@6    : | | | proc_info: /Users/wuhao/data/code/github/coral-learning/mysql-debug/mysql-5.5.62/sql/sql_parse.cc:1516  logging slow query
T@6    : | | | >PROFILING::status_change
T@6    : | | | <PROFILING::status_change 351
T@6    : | | <log_slow_statement 1531
T@6    : | | proc_info: /Users/wuhao/data/code/github/coral-learning/mysql-debug/mysql-5.5.62/sql/sql_parse.cc:1472  cleaning up
T@6    : | | >PROFILING::status_change
T@6    : | | <PROFILING::status_change 351
T@6    : | | proc_info: /Users/wuhao/data/code/github/coral-learning/mysql-debug/mysql-5.5.62/sql/sql_parse.cc:1476  (null)
T@6    : | | >PROFILING::status_change
T@6    : | | <PROFILING::status_change 348
T@6    : | | >free_root
T@6    : | | | enter: root: 0x7fdddb874028  flags: 1
T@6    : | | <free_root 383
T@6    : | | >PROFILING::finish_current_profile
T@6    : | | <PROFILING::finish_current_profile 437
T@6    : | <dispatch_command 1493
T@6    : <do_command 779
T@6    : >do_command
T@6    : | >my_net_set_read_timeout
T@6    : | | enter: timeout: 28800
T@6    : | | >vio_timeout
T@6    : | | <vio_timeout 749
T@6    : | <my_net_set_read_timeout 1176
T@6    : | >clear_error
T@6    : | <clear_error 2475
T@6    : | >reset_diagnostics_area
T@6    : | <reset_diagnostics_area 343
T@6    : | >vio_is_blocking
T@6    : | | exit: 0
T@6    : | <vio_is_blocking 203
T@6    : | >vio_read
T@6    : | | enter: sd: 34  buf: 0x7fdddb874400  size: 7
T@6    : | | vio_error: Got error 35 during read
T@6    : | | exit: -1
T@6    : | <vio_read 65
T@6    : | >vio_is_blocking
T@6    : | | exit: 0
T@6    : | <vio_is_blocking 203
T@6    : | info: vio_read returned -1  errno: 35
T@6    : | >vio_blocking
T@6    : | | enter: set_blocking_mode: 1  old_mode: 0
T@6    : | | exit: 0
T@6    : | <vio_blocking 193
T@6    : | >vio_read
T@6    : | | enter: sd: 34  buf: 0x7fdddb874400  size: 7
T@5    : | | exit: 7
T@5    : | <vio_read 65
T@5    : | packet_header: Memory: 0x7fddda085e00  Bytes: (4)
35 00 00 00 
T@5    : | >vio_read
T@5    : | | enter: sd: 33  buf: 0x7fddda085e00  size: 53
T@5    : | | exit: 53
T@5    : | <vio_read 65
T@5    : | >vio_blocking
T@5    : | | enter: set_blocking_mode: 0  old_mode: 1
T@5    : | | exit: 0
T@5    : | <vio_blocking 193
T@5    : | >my_uncompress
T@5    : | <my_uncompress 136
T@5    : | info: Command on TCP/IP (33) = 3 (Query)
T@5    : | >my_net_set_read_timeout
T@5    : | | enter: timeout: 30
T@5    : | | >vio_timeout
T@5    : | | <vio_timeout 749
T@5    : | <my_net_set_read_timeout 1176
T@5    : | >dispatch_command
T@5    : | | info: packet: ''; command: 3
T@5    : | | >PROFILING::start_new_query
T@5    : | | <PROFILING::start_new_query 380
T@5    : | | >alloc_root
T@5    : | | | enter: root: 0x7fddda068428
T@5    : | | | exit: ptr: 0x7fddda08a010
T@5    : | | <alloc_root 240
T@5    : | | query: INSERT INTO `test` (`id`, `bane`)
VALUES
	(2, 1)
T@5    : | | >PROFILING::set_query_source
T@5    : | | <PROFILING::set_query_source 507
T@5    : | | >alloc_root
T@5    : | | | enter: root: 0x7fddda068428
T@5    : | | | exit: ptr: 0x7fddda08a0a8
T@5    : | | <alloc_root 240
T@5    : | | >mysql_parse
T@5    : | | | >lex_start
T@5    : | | | | >alloc_root
T@5    : | | | | | enter: root: 0x7fddda068428
T@5    : | | | | | exit: ptr: 0x7fddda08a0e0
T@5    : | | | | <alloc_root 240
T@5    : | | | <lex_start 454
T@5    : | | | >mysql_reset_thd_for_next_command
T@5    : | | | | >clear_error
T@5    : | | | | <clear_error 2475
T@5    : | | | | >reset_diagnostics_area
T@5    : | | | | <reset_diagnostics_area 343
T@5    : | | | | >reset_current_stmt_binlog_format_row
T@5    : | | | | | debug: temporary_tables: no, in_sub_stmt: no, system_thread: NON_SYSTEM_THREAD
T@5    : | | | | | >clear_current_stmt_binlog_format_row
T@5    : | | | | | <clear_current_stmt_binlog_format_row 2637
T@5    : | | | | <reset_current_stmt_binlog_format_row 2668
T@5    : | | | | debug: is_current_stmt_binlog_format_row(): 0
T@5    : | | | <mysql_reset_thd_for_next_command 5582
T@5    : | | | >Query_cache::send_result_to_client
T@5    : | | | <Query_cache::send_result_to_client 1806
T@5    : | | | >alloc_root
T@5    : | | | | enter: root: 0x7fddda068428
T@5    : | | | | exit: ptr: 0x7fddda08a0f0
T@5    : | | | <alloc_root 240
T@5    : | | | >convert_string
T@5    : | | | | >alloc_root
T@5    : | | | | | enter: root: 0x7fddda068428
T@5    : | | | | | exit: ptr: 0x7fddda08a0f8
T@5    : | | | | <alloc_root 240
T@5    : | | | <convert_string 1850
T@5    : | | | >alloc_root
T@5    : | | | | enter: root: 0x7fddda068428
T@5    : | | | | exit: ptr: 0x7fddda08a108
T@5    : | | | <alloc_root 240
T@5    : | | | >add_table_to_list
T@5    : | | | | >alloc_root
T@5    : | | | | | enter: root: 0x7fddda068428
T@5    : | | | | | exit: ptr: 0x7fddda08a130
T@5    : | | | | <alloc_root 240
T@5    : | | | | >alloc_root
T@5    : | | | | | enter: root: 0x7fddda068428
T@5    : | | | | | exit: ptr: 0x7fddda08a138
T@5    : | | | | <alloc_root 240
T@5    : | | | | >alloc_root
T@5    : | | | | | enter: root: 0x7fddda068428
T@5    : | | | | | exit: ptr: 0x7fddda08a660
T@5    : | | | | <alloc_root 240
T@5    : | | | <add_table_to_list 6214
T@5    : | | | >set_lock_for_tables
T@5    : | | | | enter: lock_type: 7  for_update: 1
T@5    : | | | <set_lock_for_tables 6452
T@5    : | | | >alloc_root
T@5    : | | | | enter: root: 0x7fddda068428
T@5    : | | | | exit: ptr: 0x7fddda08a668
T@5    : | | | <alloc_root 240
T@5    : | | | >convert_string
T@5    : | | | | >alloc_root
T@5    : | | | | | enter: root: 0x7fddda068428
T@5    : | | | | | exit: ptr: 0x7fddda08a670
T@5    : | | | | <alloc_root 240
T@5    : | | | <convert_string 1850
T@5    : | | | >alloc_root
T@5    : | | | | enter: root: 0x7fddda068428
T@5    : | | | | exit: ptr: 0x7fddda08a678
T@5    : | | | <alloc_root 240
T@5    : | | | >alloc_root
T@5    : | | | | enter: root: 0x7fddda068428
T@5    : | | | | exit: ptr: 0x7fddda08a760
T@5    : | | | <alloc_root 240
T@5    : | | | >alloc_root
T@5    : | | | | enter: root: 0x7fddda068428
T@5    : | | | | exit: ptr: 0x7fddda08a770
T@5    : | | | <alloc_root 240
T@5    : | | | >convert_string
T@5    : | | | | >alloc_root
T@5    : | | | | | enter: root: 0x7fddda068428
T@5    : | | | | | exit: ptr: 0x7fddda08a778
T@5    : | | | | <alloc_root 240
T@5    : | | | <convert_string 1850
T@5    : | | | >alloc_root
T@5    : | | | | enter: root: 0x7fddda068428
T@5    : | | | | exit: ptr: 0x7fddda08a788
T@5    : | | | <alloc_root 240
T@5    : | | | >alloc_root
T@5    : | | | | enter: root: 0x7fddda068428
T@5    : | | | | exit: ptr: 0x7fddda08a870
T@5    : | | | <alloc_root 240
T@5    : | | | >alloc_root
T@5    : | | | | enter: root: 0x7fddda068428
T@5    : | | | | exit: ptr: 0x7fddda08a880
T@5    : | | | <alloc_root 240
T@5    : | | | >alloc_root
T@5    : | | | | enter: root: 0x7fddda068428
T@5    : | | | | exit: ptr: 0x7fddda08a898
T@5    : | | | <alloc_root 240
T@5    : | | | >alloc_root
T@5    : | | | | enter: root: 0x7fddda068428
T@5    : | | | | exit: ptr: 0x7fddda08a8a0
T@5    : | | | <alloc_root 240
T@5    : | | | >alloc_root
T@5    : | | | | enter: root: 0x7fddda068428
T@5    : | | | | exit: ptr: 0x7fddda08a920
T@5    : | | | <alloc_root 240
T@5    : | | | >alloc_root
T@5    : | | | | enter: root: 0x7fddda068428
T@5    : | | | | exit: ptr: 0x7fddda08a930
T@5    : | | | <alloc_root 240
T@5    : | | | >alloc_root
T@5    : | | | | enter: root: 0x7fddda068428
T@5    : | | | | exit: ptr: 0x7fddda08a938
T@5    : | | | <alloc_root 240
T@5    : | | | >alloc_root
T@5    : | | | | enter: root: 0x7fddda068428
T@5    : | | | | exit: ptr: 0x7fddda08a9b8
T@5    : | | | <alloc_root 240
T@5    : | | | >alloc_root
T@5    : | | | | enter: root: 0x7fddda068428
T@5    : | | | | exit: ptr: 0x7fddda08a9c8
T@5    : | | | <alloc_root 240
T@5    : | | | >mysql_execute_command
T@5    : | | | | >free_root
T@5    : | | | | | enter: root: 0x7fddda068460  flags: 0
T@5    : | | | | <free_root 383
T@5    : | | | | >deny_updates_if_read_only_option
T@5    : | | | | <deny_updates_if_read_only_option 803
T@5    : | | | | >stmt_causes_implicit_commit
T@5    : | | | | <stmt_causes_implicit_commit 200
T@5    : | | | | >insert_precheck
T@5    : | | | | | >check_access
T@5    : | | | | | | enter: db: test_1  want_access: 2  master_access: 536870911
T@5    : | | | | | | proc_info: /Users/wuhao/data/code/github/coral-learning/mysql-debug/mysql-5.5.62/sql/sql_parse.cc:4837  checking permissions
T@5    : | | | | | | >PROFILING::status_change
T@5    : | | | | | | <PROFILING::status_change 351
T@5    : | | | | | <check_access 4906
T@5    : | | | | | >check_grant
T@5    : | | | | | <check_grant 5201
T@5    : | | | | <insert_precheck 7152
T@5    : | | | | >mysql_insert
T@5    : | | | | | >open_and_lock_tables
T@5    : | | | | | | enter: derived handling: 1
T@5    : | | | | | | >open_tables
T@5    : | | | | | | | >init_alloc_root
T@5    : | | | | | | | | enter: root: 0x70000d387298
T@5    : | | | | | | | <init_alloc_root 72
T@5    : | | | | | | | proc_info: /Users/wuhao/data/code/github/coral-learning/mysql-debug/mysql-5.5.62/sql/sql_base.cc:4896  Opening tables
T@5    : | | | | | | | >PROFILING::status_change
T@5    : | | | | | | | <PROFILING::status_change 351
T@5    : | | | | | | | >my_hash_free
T@5    : | | | | | | | | enter: hash: 0x70000d386f20
T@5    : | | | | | | | <my_hash_free 139
T@5    : | | | | | | | >open_and_process_table
T@5    : | | | | | | | | tcache: opening table: 'test_1'.'test'  item: 0x7fddda08a138
T@5    : | | | | | | | | >open_table
T@5    : | | | | | | | | | >my_hash_first_from_hash_value
T@5    : | | | | | | | | | <my_hash_first_from_hash_value 290
T@5    : | | | | | | | | | >get_table_share_with_create
T@5    : | | | | | | | | | | >get_table_share
T@5    : | | | | | | | | | | | >my_hash_first_from_hash_value
T@5    : | | | | | | | | | | | | exit: found key at 33
T@5    : | | | | | | | | | | | <my_hash_first_from_hash_value 278
T@5    : | | | | | | | | | | | exit: share: 0x7fdddc28c7e0  ref_count: 2
T@5    : | | | | | | | | | | <get_table_share 656
T@5    : | | | | | | | | | <get_table_share_with_create 704
T@5    : | | | | | | | | | >release_table_share
T@5    : | | | | | | | | | | enter: share: 0x7fdddc28c7e0  table: test_1.test  ref_count: 2  version: 1
T@5    : | | | | | | | | | <release_table_share 794
T@5    : | | | | | | | | | >column_bitmaps_signal
T@5    : | | | | | | | | | | info: read_set: 0x7fddda08eee8  write_set: 0x7fddda08ef08
T@5    : | | | | | | | | | <column_bitmaps_signal 2681
T@5    : | | | | | | | | <open_table 3165
T@5    : | | | | | | | | >free_root
T@5    : | | | | | | | | | enter: root: 0x70000d387298  flags: 1
T@5    : | | | | | | | | <free_root 383
T@5    : | | | | | | | <open_and_process_table 4660
T@5    : | | | | | | | proc_info: /Users/wuhao/data/code/github/coral-learning/mysql-debug/mysql-5.5.62/sql/sql_base.cc:5088  (null)
T@5    : | | | | | | | >PROFILING::status_change
T@5    : | | | | | | | <PROFILING::status_change 348
T@5    : | | | | | | | >free_root
T@5    : | | | | | | | | enter: root: 0x70000d387298  flags: 0
T@5    : | | | | | | | <free_root 383
T@5    : | | | | | | | open_tables: returning: 0
T@5    : | | | | | | <open_tables 5096
T@5    : | | | | | | >lock_tables
T@5    : | | | | | | | >alloc_root
T@5    : | | | | | | | | enter: root: 0x7fddda068428
T@5    : | | | | | | | | exit: ptr: 0x7fddda08a9d8
T@5    : | | | | | | | <alloc_root 240
T@5    : | | | | | | | >mysql_lock_tables
T@5    : | | | | | | | | >lock_tables_check
T@5    : | | | | | | | | <lock_tables_check 214
T@5    : | | | | | | | | >get_lock_data
T@5    : | | | | | | | | | info: count 1
T@5    : | | | | | | | | | >my_malloc
T@5    : | | | | | | | | | | my: size: 48  my_flags: 0
T@5    : | | | | | | | | | | exit: ptr: 0x7fdddc17ae80
T@5    : | | | | | | | | | <my_malloc 66
T@5    : | | | | | | | | | >innobase_trx_init
T@5    : | | | | | | | | | <innobase_trx_init 1548
T@5    : | | | | | | | | | info: sql_lock->table_count 1 sql_lock->lock_count 1
T@5    : | | | | | | | | <get_lock_data 746
T@5    : | | | | | | | | proc_info: /Users/wuhao/data/code/github/coral-learning/mysql-debug/mysql-5.5.62/sql/lock.cc:299  System lock
T@5    : | | | | | | | | >PROFILING::status_change
T@5    : | | | | | | | | <PROFILING::status_change 351
T@5    : | | | | | | | | info: thd->proc_info System lock
T@5    : | | | | | | | | >lock_external
T@5    : | | | | | | | | | info: count 1
T@5    : | | | | | | | | | >handler::ha_external_lock
T@5    : | | | | | | | | | | >ha_innobase::external_lock
T@5    : | | | | | | | | | | | enter: lock_type: 3
T@5    : | | | | | | | | | | | >innobase_trx_init
T@5    : | | | | | | | | | | | <innobase_trx_init 1548
T@5    : | | | | | | | | | | | >trans_register_ha
T@5    : | | | | | | | | | | | | enter: stmt
T@5    : | | | | | | | | | | | <trans_register_ha 1073
T@5    : | | | | | | | | | | <ha_innobase::external_lock 9417
T@5    : | | | | | | | | | <handler::ha_external_lock 5162
T@5    : | | | | | | | | <lock_external 376
T@5    : | | | | | | | | proc_info: /Users/wuhao/data/code/github/coral-learning/mysql-debug/mysql-5.5.62/sql/lock.cc:326  (null)
T@5    : | | | | | | | | >PROFILING::status_change
T@5    : | | | | | | | | <PROFILING::status_change 348
T@5    : | | | | | | | <mysql_lock_tables 339
T@5    : | | | | | | <lock_tables 5862
T@5    : | | | | | | >THD::decide_logging_format
T@5    : | | | | | | | info: query: INSERT INTO `test` (`id`, `bane`)
VALUES
	(2, 1)
T@5    : | | | | | | | info: variables.binlog_format: 1
T@5    : | | | | | | | >get_stmt_unsafe_flags
T@5    : | | | | | | | <get_stmt_unsafe_flags 1407
T@5    : | | | | | | | info: lex->get_stmt_unsafe_flags(): 0x0
T@5    : | | | | | | | >Rpl_filter::db_ok
T@5    : | | | | | | | <Rpl_filter::db_ok 154
T@5    : | | | | | | | info: decision: no logging since mysql_bin_log.is_open() = 0 and (options & OPTION_BIN_LOG) = 0x40000 and binlog_format = 1 and binlog_filter->db_ok(db) = 1
T@5    : | | | | | | <THD::decide_logging_format 4724
T@5    : | | | | | <open_and_lock_tables 5605
T@5    : | | | | | proc_info: /Users/wuhao/data/code/github/coral-learning/mysql-debug/mysql-5.5.62/sql/sql_insert.cc:719  init
T@5    : | | | | | >PROFILING::status_change
T@5    : | | | | | <PROFILING::status_change 351
T@5    : | | | | | >mysql_prepare_insert
T@5    : | | | | | | enter: table_list 0x7fddda08a138, table 0x0, view 0
T@5    : | | | | | | >mysql_prepare_insert_check_table
T@5    : | | | | | | | >setup_tables
T@5    : | | | | | | | | >setup_natural_join_row_types
T@5    : | | | | | | | | <setup_natural_join_row_types 7758
T@5    : | | | | | | | <setup_tables 8104
T@5    : | | | | | | <mysql_prepare_insert_check_table 1252
T@5    : | | | | | | >setup_fields
T@5    : | | | | | | | info: thd->mark_used_columns: 1
T@5    : | | | | | | | info: thd->mark_used_columns: 1
T@5    : | | | | | | <setup_fields 7974
T@5    : | | | | | | >setup_fields
T@5    : | | | | | | | info: thd->mark_used_columns: 2
T@5    : | | | | | | | >find_field_in_table_ref
T@5    : | | | | | | | | enter: table: 'test'  field name: 'id'  item name: 'id'  ref 0x7fddda08a768
T@5    : | | | | | | | | >find_field_in_table
T@5    : | | | | | | | | | enter: table: 'test', field name: 'id'
T@5    : | | | | | | | | | >update_field_dependencies
T@5    : | | | | | | | | | <update_field_dependencies 6106
T@5    : | | | | | | | | <find_field_in_table 6398
T@5    : | | | | | | | <find_field_in_table_ref 6589
T@5    : | | | | | | | >find_field_in_table_ref
T@5    : | | | | | | | | enter: table: 'test'  field name: 'bane'  item name: 'bane'  ref 0x7fddda08a878
T@5    : | | | | | | | | >find_field_in_table
T@5    : | | | | | | | | | enter: table: 'test', field name: 'bane'
T@5    : | | | | | | | | | >update_field_dependencies
T@5    : | | | | | | | | | <update_field_dependencies 6106
T@5    : | | | | | | | | <find_field_in_table 6398
T@5    : | | | | | | | <find_field_in_table_ref 6589
T@5    : | | | | | | | info: thd->mark_used_columns: 1
T@5    : | | | | | | <setup_fields 7974
T@5    : | | | | | | >check_key_in_view
T@5    : | | | | | | <check_key_in_view 1826
T@5    : | | | | | | >setup_fields
T@5    : | | | | | | | info: thd->mark_used_columns: 1
T@5    : | | | | | | | info: thd->mark_used_columns: 1
T@5    : | | | | | | <setup_fields 7974
T@5    : | | | | | | >find_dup_table
T@5    : | | | | | | | enter: table alias: test
T@5    : | | | | | | | info: real table: test_1.test
T@5    : | | | | | | <find_dup_table 1955
T@5    : | | | | | <mysql_prepare_insert 1438
T@5    : | | | | | proc_info: /Users/wuhao/data/code/github/coral-learning/mysql-debug/mysql-5.5.62/sql/sql_insert.cc:804  update
T@5    : | | | | | >PROFILING::status_change
T@5    : | | | | | <PROFILING::status_change 351
T@5    : | | | | | >column_bitmaps_signal
T@5    : | | | | | | info: read_set: 0x7fddda08eee8  write_set: 0x7fddda08ef08
T@5    : | | | | | <column_bitmaps_signal 2681
T@5    : | | | | | >fill_record
T@5    : | | | | | <fill_record 8623
T@5    : | | | | | >write_record
T@5    : | | | | | | >handler::ha_write_row
T@5    : | | | | | | | >ha_innobase::write_row
T@5    : | | | | | | | | >handler::update_auto_increment
T@5    : | | | | | | | | <handler::update_auto_increment 2515
T@5    : | | | | | | | <ha_innobase::write_row 5336
T@5    : | | | | | | <handler::ha_write_row 5203
T@5    : | | | | | | >handler::print_error
T@5    : | | | | | | | enter: error: 121
T@5    : | | | | | | | >handler::get_dup_key
T@5    : | | | | | | | | >info
T@5    : | | | | | | | | | >innobase_trx_init
T@5    : | | | | | | | | | <innobase_trx_init 1548
T@5    : | | | | | | | | <info 8425
T@5    : | | | | | | | <handler::get_dup_key 3217
T@5    : | | | | | | | >key_unpack
T@5    : | | | | | | | | >field_unpack
T@5    : | | | | | | | | | >my_malloc
T@5    : | | | | | | | | | | my: size: 16  my_flags: 16
T@5    : | | | | | | | | | | exit: ptr: 0x7fdddc287640
T@5    : | | | | | | | | | <my_malloc 66
T@5    : | | | | | | | | <field_unpack 375
T@5    : | | | | | | | | >my_free
T@5    : | | | | | | | | | my: ptr: 0x7fdddc287640
T@5    : | | | | | | | | <my_free 135
T@5    : | | | | | | | <key_unpack 419
T@5    : | | | | | | | >my_printf_error
T@5    : | | | | | | | | my: nr: 1062  MyFlags: 0  errno: 0  Format: Duplicate entry '%-.64s' for key '%-.192s'
T@5    : | | | | | | | | >my_message_sql
T@5    : | | | | | | | | | error: error: 1062  message: 'Duplicate entry '2' for key 'PRIMARY''
T@5    : | | | | | | | | | >THD::raise_condition
T@5    : | | | | | | | | | | >set_error_status
T@5    : | | | | | | | | | | <set_error_status 441
T@5    : | | | | | | | | | | >query_cache_abort
T@5    : | | | | | | | | | | <query_cache_abort 961
T@5    : | | | | | | | | | | >alloc_root
T@5    : | | | | | | | | | | | enter: root: 0x7fddda068460
T@5    : | | | | | | | | | | | >my_malloc
T@5    : | | | | | | | | | | | | my: size: 2016  my_flags: 1040
T@5    : | | | | | | | | | | | | exit: ptr: 0x7fddd9840600
T@5    : | | | | | | | | | | | <my_malloc 66
T@5    : | | | | | | | | | | | exit: ptr: 0x7fddd9840610
T@5    : | | | | | | | | | | <alloc_root 240
T@5    : | | | | | | | | | | >alloc_root
T@5    : | | | | | | | | | | | enter: root: 0x7fddda068460
T@5    : | | | | | | | | | | | exit: ptr: 0x7fddd9840788
T@5    : | | | | | | | | | | <alloc_root 240
T@5    : | | | | | | | | | | >alloc_root
T@5    : | | | | | | | | | | | enter: root: 0x7fddda068460
T@5    : | | | | | | | | | | | exit: ptr: 0x7fddd98407b0
T@5    : | | | | | | | | | | <alloc_root 240
T@5    : | | | | | | | | | <THD::raise_condition 1174
T@5    : | | | | | | | | <my_message_sql 2845
T@5    : | | | | | | | <my_printf_error 125
T@5    : | | | | | | <handler::print_error 2860
T@5    : | | | | | | >column_bitmaps_signal
T@5    : | | | | | | | info: read_set: 0x7fddda08eee8  write_set: 0x7fddda08ef08
T@5    : | | | | | | <column_bitmaps_signal 2681
T@5    : | | | | | <write_record 1784
T@5    : | | | | | proc_info: /Users/wuhao/data/code/github/coral-learning/mysql-debug/mysql-5.5.62/sql/sql_insert.cc:1047  end
T@5    : | | | | | >PROFILING::status_change
T@5    : | | | | | <PROFILING::status_change 351
T@5    : | | | | <mysql_insert 1110
T@5    : | | | | proc_info: /Users/wuhao/data/code/github/coral-learning/mysql-debug/mysql-5.5.62/sql/sql_parse.cc:4516  query end
T@5    : | | | | >PROFILING::status_change
T@5    : | | | | <PROFILING::status_change 351
T@5    : | | | | >trans_rollback_stmt
T@5    : | | | | | >ha_rollback_trans
T@5    : | | | | | | >innobase_rollback
T@5    : | | | | | | | trans: aborting transaction
T@5    : | | | | | | | >innobase_trx_init
T@5    : | | | | | | | <innobase_trx_init 1548
T@5    : | | | | | | <innobase_rollback 2932
T@5    : | | | | | | >free_root
T@5    : | | | | | | | enter: root: 0x7fddda066f60  flags: 1
T@5    : | | | | | | <free_root 383
T@5    : | | | | | <ha_rollback_trans 1514
T@5    : | | | | <trans_rollback_stmt 396
T@5    : | | | | >st_select_lex_unit::cleanup
T@5    : | | | | | >st_select_lex::cleanup()
T@5    : | | | | | <st_select_lex::cleanup() 882
T@5    : | | | | <st_select_lex_unit::cleanup 768
T@5    : | | | | proc_info: /Users/wuhao/data/code/github/coral-learning/mysql-debug/mysql-5.5.62/sql/sql_parse.cc:4568  closing tables
T@5    : | | | | >PROFILING::status_change
T@5    : | | | | <PROFILING::status_change 351
T@5    : | | | | >close_thread_tables
T@5    : | | | | | tcache: table: 'test'  query_id: 0
T@5    : | | | | | >THD::binlog_flush_pending_rows_event
T@5    : | | | | | <THD::binlog_flush_pending_rows_event 5101
T@5    : | | | | | >THD::binlog_flush_pending_rows_event
T@5    : | | | | | <THD::binlog_flush_pending_rows_event 5101
T@5    : | | | | | >mysql_unlock_tables
T@5    : | | | | | | >unlock_external
T@5    : | | | | | | | >handler::ha_external_lock
T@5    : | | | | | | | | >ha_innobase::external_lock
T@5    : | | | | | | | | | enter: lock_type: 2
T@5    : | | | | | | | | | >innobase_trx_init
T@5    : | | | | | | | | | <innobase_trx_init 1548
T@5    : | | | | | | | | <ha_innobase::external_lock 9457
T@5    : | | | | | | | <handler::ha_external_lock 5162
T@5    : | | | | | | <unlock_external 651
T@5    : | | | | | | >my_free
T@5    : | | | | | | | my: ptr: 0x7fdddc17ae80
T@5    : | | | | | | <my_free 135
T@5    : | | | | | <mysql_unlock_tables 388
T@5    : | | | | | info: thd->open_tables: 0x7fddda08ee00
T@5    : | | | | | >close_thread_table
T@5    : | | | | | | >ha_reset
T@5    : | | | | | | | >free_io_cache
T@5    : | | | | | | | <free_io_cache 947
T@5    : | | | | | | <ha_reset 5185
T@5    : | | | | | <close_thread_table 1625
T@5    : | | | | <close_thread_tables 1568
T@5    : | | | | proc_info: /Users/wuhao/data/code/github/coral-learning/mysql-debug/mysql-5.5.62/sql/sql_parse.cc:4570  (null)
T@5    : | | | | >PROFILING::status_change
T@5    : | | | | <PROFILING::status_change 348
T@5    : | | | | >stmt_causes_implicit_commit
T@5    : | | | | <stmt_causes_implicit_commit 200
T@5    : | | | | >MDL_context::release_transactional_locks
T@5    : | | | | | >MDL_context::release_locks_stored_before
T@5    : | | | | | | info: found lock to release ticket=0x7fdddc170830
T@5    : | | | | | | >MDL_context::release_lock
T@5    : | | | | | | | enter: db= name=
T@5    : | | | | | | <MDL_context::release_lock 2582
T@5    : | | | | | <MDL_context::release_locks_stored_before 2630
T@5    : | | | | | >MDL_context::release_locks_stored_before
T@5    : | | | | | | info: found lock to release ticket=0x7fdddc170150
T@5    : | | | | | | >MDL_context::release_lock
T@5    : | | | | | | | enter: db=test_1 name=test
T@5    : | | | | | | | >my_hash_delete
T@5    : | | | | | | | <my_hash_delete 584
T@5    : | | | | | | <MDL_context::release_lock 2582
T@5    : | | | | | <MDL_context::release_locks_stored_before 2630
T@5    : | | | | <MDL_context::release_transactional_locks 2770
T@5    : | | | <mysql_execute_command 4617
T@5    : | | | proc_info: /Users/wuhao/data/code/github/coral-learning/mysql-debug/mysql-5.5.62/sql/sql_parse.cc:5844  freeing items
T@5    : | | | >PROFILING::status_change
T@5    : | | | <PROFILING::status_change 351
T@5    : | | | >lex_end
T@5    : | | | | enter: lex: 0x7fddda067288
T@5    : | | | <lex_end 473
T@5    : | | | >Query_arena::free_items
T@5    : | | | | >Item_field::cleanup
T@5    : | | | | | >Item_ident::cleanup
T@5    : | | | | | | >Item::cleanup
T@5    : | | | | | | <Item::cleanup 519
T@5    : | | | | | <Item_ident::cleanup 653
T@5    : | | | | <Item_field::cleanup 4888
T@5    : | | | | >Item_field::cleanup
T@5    : | | | | | >Item_ident::cleanup
T@5    : | | | | | | >Item::cleanup
T@5    : | | | | | | <Item::cleanup 519
T@5    : | | | | | <Item_ident::cleanup 653
T@5    : | | | | <Item_field::cleanup 4888
T@5    : | | | <Query_arena::free_items 3014
T@5    : | | <mysql_parse 5852
T@5    : | | info: query ready
T@5    : | | >Protocol::end_statement
T@5    : | | | >Protocol::send_error
T@5    : | | | | >send_error_packet
T@5    : | | | | <send_error_packet 402
T@5    : | | | | >net_write_command
T@5    : | | | | | enter: length: 45
T@5    : | | | | | >net_flush
T@5    : | | | | | | >vio_is_blocking
T@5    : | | | | | | | exit: 0
T@5    : | | | | | | <vio_is_blocking 203
T@5    : | | | | | | >net_real_write
T@5    : | | | | | | | >Query_cache::insert
T@5    : | | | | | | | <Query_cache::insert 901
T@5    : | | | | | | | >my_malloc
T@5    : | | | | | | | | my: size: 57  my_flags: 16
T@5    : | | | | | | | | exit: ptr: 0x7fdddc047850
T@5    : | | | | | | | <my_malloc 66
T@5    : | | | | | | | >my_compress
T@5    : | | | | | | | | >my_malloc
T@5    : | | | | | | | | | my: size: 72  my_flags: 16
T@5    : | | | | | | | | | exit: ptr: 0x7fddd9600370
T@5    : | | | | | | | | <my_malloc 66
T@5    : | | | | | | | | >my_free
T@5    : | | | | | | | | | my: ptr: 0x7fddd9600370
T@5    : | | | | | | | | <my_free 135
T@5    : | | | | | | | | note: Packet got longer on compression; Not compressed
T@5    : | | | | | | | <my_compress 52
T@5    : | | | | | | | >vio_write
T@5    : | | | | | | | | enter: sd: 33  buf: 0x7fdddc047850  size: 57
T@5    : | | | | | | | | exit: 57
T@5    : | | | | | | | <vio_write 136
T@5    : | | | | | | | >my_free
T@5    : | | | | | | | | my: ptr: 0x7fdddc047850
T@5    : | | | | | | | <my_free 135
T@5    : | | | | | | <net_real_write 709
T@5    : | | | | | <net_flush 342
T@5    : | | | | <net_write_command 484
T@5    : | | | <Protocol::send_error 574
T@5    : | | <Protocol::end_statement 524
T@5    : | | >Query_cache::end_of_result
T@5    : | | <Query_cache::end_of_result 997
T@5    : | | >log_slow_statement
T@5    : | | | proc_info: /Users/wuhao/data/code/github/coral-learning/mysql-debug/mysql-5.5.62/sql/sql_parse.cc:1516  logging slow query
T@5    : | | | >PROFILING::status_change
T@5    : | | | <PROFILING::status_change 351
T@5    : | | <log_slow_statement 1531
T@5    : | | proc_info: /Users/wuhao/data/code/github/coral-learning/mysql-debug/mysql-5.5.62/sql/sql_parse.cc:1472  cleaning up
T@5    : | | >PROFILING::status_change
T@5    : | | <PROFILING::status_change 351
T@5    : | | proc_info: /Users/wuhao/data/code/github/coral-learning/mysql-debug/mysql-5.5.62/sql/sql_parse.cc:1476  (null)
T@5    : | | >PROFILING::status_change
T@5    : | | <PROFILING::status_change 348
T@5    : | | >free_root
T@5    : | | | en

```