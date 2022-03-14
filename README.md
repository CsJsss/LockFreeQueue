# LockFreeQueue

使用cpp实现无锁队列. 并进行了正确性检查以及和mutex队列的性能比较和分析. 详细内容可以查看博客.

## 运行

```bash
$ ./build.sh
```

## 结果

```bash
running check LockFreeQueue

produce 1 finish.
produce 3 finish.
produce 0 finish.
produce 2 finish.

error in count = 0
error out count = 0

cost time = 1261 ms
finish check LockFreeQueue

=====================================================================

running benchmark LockFreeQueue


epoch cost average time = 1066 ms

finish benchmark LockFreeQueue

=====================================================================
running benchmark mutexQueue


epoch cost average time = 381 ms

finish benchmark mutexQueue

=====================================================================
```


## 链接博客
[无锁队列的原理和实现](https://csjsss.github.io/2022/03/13/ConcurrencyAndDistribution/%E6%97%A0%E9%94%81%E9%98%9F%E5%88%97%E7%9A%84%E5%8E%9F%E7%90%86%E5%92%8C%E5%AE%9E%E7%8E%B0/)