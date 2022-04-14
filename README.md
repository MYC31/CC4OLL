## Lab5-1

**实验要求：** 用lock coupling或者乐观锁的并发控制方法实现一个有序链表

**实验说明：** 要求实现四个功能：添加一个节点（Add）；删除一个节点（Remove）；查找一个节点（Contain）；输出整个链表（Print）

**实验指导**

- Lock coupling即通过锁住链表的前驱节点和后继节点，以实现多线程并行操作链表
- 乐观锁：只有在寻找到要加锁位置的时候才加锁（先加锁，再进行验证是否被修改）

**实验内容：**

1. 多个线程插入节点（不加锁）——观察1-100中是否有乱序出现（Test Case 1）
2. 多个线程插入节点（加锁）——观察1-100中是正确插入（Test Case 2）
3. 多个线程删除节点——观察1-100中是否全部删除（Test Case 2）
4. 多个线程插入和删除节点——观察奇数/偶数是否被正确插入/删除（Test Case 3）
5. 多个线程查询和删除节点——观察奇数/偶数是否正确查找/删除（Test Case 4）
