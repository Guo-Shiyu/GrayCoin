# 增删改查程序生成器 
(程序语言设计课程设计生成器)  
能够根据给定的文法描述的元数据, 生成对应 C 语言的程序. 进而直接满足某些单子的需求. 即便是出现不能直接满足需求的情况, 也应当尽量减少修改所需要的代价.

## 需求
+ 某种基础元素 element, 例如图书管理系统中的图书, 包含一些数据 data
+ 容纳 element 的容器 container, 例如所有图书的链表/数组/二叉树等, 是一个可以进行增删改查的数据结构
+ 菜单页的显示文本与选项映射关系 menu
+ 输入校验 check

## 分析
elem 是 container 的容纳类型, 也是增删改查的基础元素, 因此一个增删改查程序需要的元素仅仅是
+ 容器的数据结构与基础元素.  
    确定了容器的数据结构, 则增删改查.


## 文法定义
~~~ f#
    
~~~


## 例子: 图书管理系统  
书籍包括书名, 编号, 作者, 附加信息四个字段, 图书馆中所有书作为一个系统存储在单链表中, 完成增删改查操作.

~~~   
    target C 

    element Book
        |id int 
        |name str 
        |author str 
        |appendix str

    container AllBook of Book order by id incr
        + add
        + find by id, name, author
        + delete by id
        
       
~~~