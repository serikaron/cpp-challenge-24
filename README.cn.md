# Complete the Challenge
fork from [Kerno's C++ Linux Engineering Challenge](https://github.com/serikaron/cpp-challenge-24)

## 怎样使用
* 生成镜像

```bash
docker buildx build -t cpp-challenge --load --platform linux/x86_64 .
```

* 运行

```bash
docker run --rm -it --name cpp -v <output/path>:/tmp/challenge cpp-challenge
```
    
替换对应的`output/path`

可以查看entrypoint.sh，会在docker image里生成/tmp/challenge/out.csv。

成功执行后会在`output/path`里看到out.csv

也可以执行
```bash
docker run --rm -it --name cpp --entrypoint='' cpp-challenge /bin/bash
```
直接进入container里面，再执行命令
```bash
/out/bin/generator | /out/bin/processor -o <filepath>
```

## 代码结构
Model(core目录) 主要由逻辑处理的`Broker`和`Persistor`，数据存储的`Mapping`
* Broker负责接收`generator`生产的消息，再转成对应的Http Request或者Http Response，存储到Mapping里。
* Persistor负责从Mapping取出已配对的Request Path和Response Code，保存到指定文件里
* Broker和Persistor在各自的线程里循环执行，共同访问的Mapping加了锁。

main目录中的`Processor`是类似controller的角色，协调Broker和Persistor动作。test目录实现了Broker和Persistor的简单测试。

## 几个问题的回答
* **What would happen if we added messages in another protocol / format?** 
    
基于不要过度设计的原则，现在的代码里Broker和Persistor都是固定的。但是要扩展也很容易。假设需要从Redis里subscribe channel获得消息。
这可能是在另外的Model里实现，只需要在使用方(现在main目录下的相关代码)实现一个Adapter，应用具体的Broker就可以了。

当然也可以在现在的Model中实现。将现的的Broker改名成PipelineBroker，另外实现一个RedisBroker，再提取公共基类Broker就可以了。

然后Persistor也是类似，如果不是存储到文件，可能存到数据库或者Publish到redis channel，也可以用同样手段来扩展。


* **What would happen if the message rate exceeded your CPU capacity?**

这个问题我想得不是太清楚。 指的是generator生产的消息快过我程序的消费能力？如果是这样可能有下面几种解决办法
  * 最简单当然是升级硬件：）
  * 如果是代码问题，需要用类似bench mark的手段定位哪部分代码耗时严重，再来解决。
  * 如果是架构问题，Broker收到消息后放到共享内存或者Kafka中，由多线程/多进程/多机器去消费处理
  * 如果是直接从generator里读取消息也处理不过来，似乎没有其它办法了。不清楚具体generator的实现，似乎很难针对优化。


* **What would happen if some messages never got a response?**

在收到请求时记下当前时间，在一定时间后当作出错处理。