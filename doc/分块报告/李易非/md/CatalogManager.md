## Catalog Manager 模块设计报告 👹

> by 李易非



### 一、设计简介

​	Catalog Manager 管理数据库的 Meta Data，在数据库启动阶段，Catalog Manager 将读取 Meta Data并把它们存入内存当中，这样做的理由如下：

- minisql 应用场景：表/索引的数量不会很多，且表的属性最多只有32个；

- 在程序中我们需要大量访问数据 Meta Data，直接存入内存，可以加快访问速度；

  File Manager 具有的功能如下 ( 简化版 ) ：

  ``` c++
  class CatalogManager
  {
      unordered_map <string, Table *> Name2Table; // 表名到表的hash
      unordered_map <string, Index *> Name2Index; // 索引名到索引的hash

      CatalogManager();

      bool create_table(表名, 属性集);      	// 创建表，更新Meta data
      bool drop_table(表名);				  // 删除表，更新Meta data
      const Table* get_table(表名);			  // 通过表名获得表
      bool create_index(索引名, 表名, 属性名);  // 创建索引，更新Meta data
      bool drop_index(索引名, 表名); 			// 删除索引，更新Meta data

  };
  ```



### 二、具体说明



#### 1、成员变量说明

- `Name2Table`: 表名字到具体表的 `unordered_map` ，可以通过表名直接找到对应表的实体对象；
- `Name2Index`: 索引名字到具体索引的 `unordered_map` ，可以通过索引名直接找到对应索引的实体对象；



#### 2、成员函数说明

- `CatalogManager()`

  - 读取 `TableMeta/` 、`IndexMeta/` 中的元信息，构建 Table，Index对象，并加入 `Name2Table` 中；

- `create_table(表名，属性集)`

  - 首先检查该表是否已经存在，若存在，抛出错误；
  - 保证属性集中没有重复的属性，若有重复属性，抛出错误；
  - 保证属性集没有超过32个属性，若超过32个属性，抛出错误；
  - 通过属性信息，构建一个 Table 实体并加入 `Name2Table` 中；
  - 更新 `TableMeta/tables` 信息；
  - 创建 `TableMeta/表名` 文件，并写好第一个Block ( 该文件对应的元信息 )；

  ```c++
  if table already exists
  	throw err
  if attribute_set is duplicated
  	throw err
  if there are more than 32 attributes
  	throw err
  Name2Table.insert({表名, 表实体});
  Update Metadata;
  CreateFile("TableMeta/表名");
  ```

  ​

- `drop_table(表名)`

  - 检查要删除的表是否存在，若不存在，抛出错误；
  - 更新 `Name2Table`
  - 更新 `TableMeta/tables` 文件
  - 删除 `TableMeta/表名` 文件

  ``` c++
  if table does not exist
      throw err;
  Name2Table.erase(表名);
  Update Metadata;
  DeleteFile("TableMeta/表名");
  ```

  ​

- `const Table* get_table(表名)`

  - 首先检查该表是否存在，若不存在，抛出错误
  - 通过 `Name2Table` 直接返回对应表实体；

  ``` c++
  if table does not exist
  	throw err;

  return Name2Table(表名)
  ```

  ​

- `bool create_index(索引名, 表名, 属性名)`

  - 首先检查该索引是否存在，若已经存在，抛出错误；
  - 再检查需要建索引的属性是否时 Unique，若不是，抛出错误；
  - 更新 `Name2Index`
  - 更新 `IndexMeta/indices`

  ``` c++
  if index already exists
  	throw err;
  if attribute is not Unique
   	throw err;
  Name2Index.insert({索引名, 索引实体})
  Update Metadata;
  ```

  ​

- `bool drop_index(索引名, 表名)`

  - 首先检查该索引是否存在，若不存在，抛出错误；
  - 更新 `Name2Index`
  - 更新 `IndexMeta/indices`

  ``` c++
  if index does not exist
      throw err;
  Name2Index.erase(索引名);
  Update Metadata;
  ```

  ​