## Record Manager 设计报告 👹

> by 李易非



###  一、设计简介

​	Record Manager 具体管理各个表的记录，依据 API 提供的需求来进行相应数据文件的更新，因为之前 File Manager 的实现，Record Manager 的实现被大大化简，从物理层操作上升到了逻辑层操作。

​	Record Manager 具有的功能如下 ( 简化版 ) ：

``` c++
class RecordManager
{
    bool create_table(表名);
    bool drop_table(表名);
    int insert(表名, rawdata);
    int select(表名, 属性名, 条件, 操作数);
    int Delete(表名, 属性名, 条件, 操作数);
    const char * GetRecordByAddr(表名, 地址); 
    bool DeleteRecordByAddr(表名, 地址);
};
```



### 二、具体实现

- `bool create_table(表名)`

  - 调用 Catalog Manager 获得对应表名的 Record Length;
  - 创建数据区文件 `data/表名`

  ``` c++
  get Record Length from Catalog Manager;
  CreateFile("data/表名", 记录长度)
  ```

  ​

- `bool drop_table(表名)`

  - 删除数据区文件 `data/表名`



- `int insert(表名, rawdata)`

  - 通过表名构建一个 File Manager, 调用 File Manager 的 `add_record(rawdata)` 函数

  ``` c++
  FileManager file(表名)；
  file.add_record(rawdata);
  ```

  ​

- `int select(表名, 属性名, 条件, 操作数)`

  - 调用 Catalog Manager 通过表名获得表信息；
  - 遍历文件，判断每一条记录是否符合条件；若符合，打印对应记录；
  - 返回符合条件的记录总数；

  ``` c++
  Table = CatalogManager.get_table(表名)；
  FileManager file("data/" + 表名)；

  int NumOfRecord = 0;
  while file.getNextRecord != -1
      if Table.isSatisfyAllCondition(记录rawdata, 属性名, 条件, 操作数);
  		print the record;
  		NumOfRecord++;

  return NumOfRecord;
  ```

  ​

- `int Delete(表名, 属性名, 条件, 操作数)`

  - 调用 Catalog Manager 通过表名获得表信息；
  - 遍历文件，判断每一条记录是否符合条件；若符合，删除记录与对应的索引；
  - 返回删除的记录总数；

  ``` c++
  Table = CatalogManager.get_table(表名)；
  Attris = Table.GetAttriIndexed(); // 获得所有有索引的属性
  FileManager file("data/" + 表名)；

  int NumOfRecord = 0;

  while file.getNextRecord != -1
      if Table.isSatisfyAllCondition(记录rawdata, 属性名, 条件, 操作数);
  		file.DeleteRecord;
  		for all attributes that is indexed
  			delete the index data;
  		NumOfRecord++;

  return NumOfRecord;
  ```



- `const char * GetRecordByAddr(表名, 地址)`

  - 构建 File Manager, 调用 File Manager 的 GetRecordByAddr 函数，并返回记录的 rawdata;

  ``` c++
  FileManager file("data/" + 表名)；
  return file.GetRecordByAddr(地址)；
  ```



- `bool DeleteRecordByAddr(表名, 地址)`

  - 构建 File Manager, 调用 File Manager 的 DeleteRecordByAddr 函数;

  ``` c++
  FileManager file("data/" + 表名)；
  return file.DeleteRecordByAddr(地址)；
  ```

  ​