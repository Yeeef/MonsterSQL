create table student(
sno varchar(8),
sname varchar(16) unique,
sage int,
sgender varchar(1),
primary key(sno));

create index stunameid on student(sname);

insert into student values('123','wy',22,'M');

select * from student;

select * from student where sgender = 'M';

select * from student where sage = 22;
