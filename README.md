# Complete the Challenge
fork from [Kerno's C++ Linux Engineering Challenge](https://github.com/serikaron/cpp-challenge-24)

## How to Use
* Build the image

```bash
docker buildx build -t cpp-challenge --load --platform linux/x86_64 .
```

* Run

```bash
docker run --rm -it --name cpp -v <output/path>:/tmp/challenge cpp-challenge
```
    
Replace <output/path> with your host path.

You can check entrypoint.sh, which generates /tmp/challenge/out.csv inside the docker image.

Upon successful execution, you will find out.csv in <output/path>.


Alternatively, you can execute:
```bash
docker run --rm -it --name cpp --entrypoint='' cpp-challenge /bin/bash
```
to enter the container and then execute:
```bash
/out/bin/generator | /out/bin/processor -o <filepath>
```

## Code Structure
Model (core directory) primarily consists of `Broker` and `Persistor` for logic processing, and `Mapping` for data storage.

* Broker is responsible for receiving messages produced by `generator`, converting them into corresponding HTTP requests or responses, and storing them in Mapping.
* Persistor retrieves paired Request Paths and Response Codes from Mapping and saves them to the specified file.
* Broker and Persistor execute in their respective threads, accessing the shared Mapping with thread lock.

The Processor in the main directory acts as a controller, coordinating actions between Broker and Persistor. Tests in the test directory implement simple tests for Broker and Persistor.

## Some food for thought (a.k.a., nice to have's)

* **What would happen if we added messages in another protocol / format?** 
    
Based on the principle of avoiding over-design, the current codebase has concrete Broker and Persistor. However, extending it is straightforward. Suppose we need to subscribe to messages from a Redis channel. This could be implemented in another Model, requiring only an Adapter to be implemented in the application's relevant code (currently under the main directory) to use the specific Broker.

It's also possible to implement within the current Model. Renaming the existing Broker to PipelineBroker, then implementing a RedisBroker, and extracting a common base class Broker would suffice.

Similarly, for Persistor, if it needs to go to a database or publish to a Redis channel, the same approach can be used for extension.

* **What would happen if the message rate exceeded your CPU capacity?**

I'm not entirely clear on this issue. Does it refer to messages produced by the generator exceeding the consuming of my program? If so, there might be several solutions:
  * The simplest would be hardware upgrades. lol
  * If it's a code issue, identifying which part of the code is consuming significant time using benchmarks and then addressing it.
  * If it's an architectural problem, placing messages received by Broker into shared memory or Kafka, and processing them with multiple threads/processes/machines might help.
  * If even reading messages from the generator is overwhelming, there might not be many other options. It's hard to optimize without specific knowledge of the generator's implementation.


* **What would happen if some messages never got a response?**

Upon receiving a request, record the current time, and after a certain period, treat it as an error.