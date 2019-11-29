Create table(
    id int primary key,
	name varchar(50),
	age INT);//错误：没有表名

Create table student(
    id int primary key,
	name varchar(50),
	age);//错误：age没给类型
	
Create table student(
    id int primary key,
	name varchar(50) unique,
	age INT);//建表并测试unique属性
	
create index person on student;//错误反例

create index person in student(id);//错误反例in

create index person on student(id);//创建索引

drop index person;//删除索引反例
drop index person on student;//删除索引

insert into student values(5, 'lyf');

insert into student values(5, 'lyf', 18);//单条记录插入

select * from student where id = 5;//等值查询

select * from student where id > 4;//范围查询

select * from student where id = 0 and name = 'k';//and连接的多条件查询

delete from student where id = 1;//找不到需要删除的

delete from student where id = 5;//删除记录

drop table student;//删除表

