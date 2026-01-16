# cpp-cache
使用c++实现的缓存系统小项目，主要是LRU，LFU和ARC以及它们的拓展
### LRU
#### LRU基础版本
最基础的LRU可能是Leetcode那道题目[146.LRU 缓存](https://leetcode.cn/problems/lru-cache/description/?envType=study-plan-v2&envId=top-100-liked)

，我采用了智能指针管理内存，然后是使用比较规范的c++语法写的，下一步会对其进行工程级别的优化。
#### LRU_k
朴素的LRU算法已经能够满足缓存的要求了，但是还是有一些不足。当热点数据较多时，有较高的命中率，但是如果有偶发性的批量操作，会使得热点数据被非热点数据挤出容器，使得缓存受到了“污染”。
所以设置k参数，只有最近出现次数超过k的才进入cache，不然在history等着，使用两个双链表加上哈希map实现
### LFU
#### LFU基础版本
参考[460. LFU 缓存](https://leetcode.cn/problems/lfu-cache/description/) ,基础设计考虑了智能指针以及污染问题，但是还是有一些小问题存在。
