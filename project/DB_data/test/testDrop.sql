drop table book;
create table book(id int primary key, name varchar(30) unique);
insert into book values(1, 'LYF');
insert into book values(2, 'LYF');
drop index bookname on book;
insert into book values(3, 'LYF');

create index bookname on book(name);
insert into book values (4, 'LYF');
