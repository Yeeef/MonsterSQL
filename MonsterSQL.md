# MonsterSQL 👹

## 实验要求

- 实现一个DBMS
	1. 表的建立、删除
	2. 索引的建立、删除
	3. 记录的插入、删除、查找

## 需求概述

- 数据类型
	+ `int`
	+ `char(n)` 1 <= n <= 255
	+ `float`

- 表定义
	+ 最多**32**个属性
	+ **UNIQUE**
	+ **单属性主键**

- 索引的建立和删除
	+ 主键 **自动建立B+树索引**
	+ UNIQUE 可以建立或者删除B+树索引
	+ B+树索引为**单属性、单值**

- 记录的查找
	+ 等值查找
	+ 范围查找
	+ and连接的多条件查询

- 记录的插入和删除
	+ **单条记录**的插入
	+ **单条或多条记录**的删除

## 语法说明

- 支持标准 SQL 语句
	+ 分号结尾
	+ 一条SQL语句可以多行
	+ 关键字均为**小写**？
	+ 创建表语句
	+ 删除表语句
	+ 创建索引语句
	+ 删除索引语句 
	+ 选择语句
	+ 插入记录语句
	+ 删除记录语句
	+ 退出系统语句
	+ ==执行SQL脚本语句==

```sql
create table student(
		sno char(8),
		sname char(16) unique,
		sage int,
		sgender char(1),
		primary key(sno)
);

drop table student;

create index stunameid on student( sname );

select * from student where sage > 20 and sgender = 'F';
```

## 体系结构

### Interpreter

- 用户交互层

- 一些教程关于 `Lex` `Yacc`
    + [Lex Wiki](https://en.wikipedia.org/wiki/Lex_%28software%29)
	+ [Yacc Wiki](https://en.wikipedia.org/wiki/Yacc)
	+ [Lex与Yacc](https://www.ibm.com/developerworks/cn/linux/sdk/lex/)
	+ [tutorial](https://www.epaperpress.com/lexandyacc/)

### API

- Application Program Interface 是一些预先定义的函数，目的是提供应用程序与开发人员基于某软件或硬件得以访问一组例程的能力，而又无需访问源码，或理解内部工作机制的细节。

- 提供执行 SQL 语句的接口
	+ 输入`Interpreter`层解释生成的命令内部表示？
	+ 根据`Catalog Manager`提供的信息生成执行规则
	+ 调用`Record Manager`、`Index Manager`和`Catalog Manager`提供的相应接口进行执行
	+ 返回结果给`Interpreter`

- ==很重要==

### Catalog Manager

- 管理数据库的所有 **模式信息**
	+ 表定义（表名、列数、主键、索引等）
	+ 属性的定义信息(数据类型、是否唯一等)
	+ 索引的定义信息(所属表、索引建立在哪个属性上等)
	+ 提供访问及操作上述信息的接口

- 需要以文件形式存储

### Record Manager

- 管理记录表中数据的 **数据文件**
	+ 数据文件的创建
	+ 数据文件的删除 
	+ 记录的插入 
	+ 记录的删除 
	+ 记录的查询 
	+ ***提供相应接口***

- 数据文件
	+ 数据文件由一个或多个数据块（block)组成，**块大小应与缓冲区块大小相同**
	+ 一个数据块包含一条至多条记录
		* 支持定长的记录即可
		* 不要求支持记录的跨块存储
	+ 一个数据块能存储多少条记录可以根据块大小和存储的每 条记录的长度计算出来

### Index Manager

- 负责 **B+树索引** 的实现
	+ B+树索引的创建
	+ B+树索引的删除 
	+ 等值查询 
	+ 插入键值 
	+ 删除键值 
	+ ***提供相应的接口***

- 索引文件
	+ B+树中 **节点大小应与缓冲区的块大小相同**
	+ B+树中节点中的数据按照顺序存储
	+ B+树的 **叉数由节点大小与索引键大小计算得到**

### Buffer Manager

- 负责 **缓冲区** 的管理
	+ 数据块的写入、写出
	+ 实现缓冲区的 **替换算法(e.g. LRU)**
	+ 记录块状态(是否被修改过) 
	+ 锁定缓冲区某块，不允许替换出去

- 缓冲区与文件系统 **交互的单位是块** ，一般可定为4KB或8KB

### DB Files

- 数据文件
- 索引文件
- Catalog 文件








