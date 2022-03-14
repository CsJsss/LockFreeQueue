//
// Created by Jsss on 2022/3/13.
//


#include <iostream>
#include <vector>
#include <atomic>
#include "setting.h"
#include <thread>
#include <chrono>
#include <queue>
#include <mutex>

using std::vector;
using std::thread;
using std::cout;
using std::atomic;
using std::queue;
using std::mutex;

int all = producerWorks * producerNums;

// 记录生产者和消费者完成数目
atomic<int> prodFinish(0);

queue<int> qu;

mutex mtx_prod, mtx_cons;

void produce (int _id) {
	// 确定生产者生产编号的范围, 保证每个生产者生产的物品唯一
	int L = _id * producerNums, R = (_id + 1) * producerNums - 1;

	for (int u = L; u <= R; u ++ ) {
		// lock_guard是一个互斥量包装程序，它提供了一种方便的RAII（Resource acquisition is initialization ）风格的机制来在作用域块的持续时间内拥有一个互斥量。
		std::lock_guard<mutex> lock(mtx_prod);
		qu.push(u);
	}

	// 生产完毕则, finish个数 + 1
	prodFinish.fetch_add(1);
}

void consume () {
	// 必须使用CAS操作判断, 如果prodFinish的值不为producerWorks的数目, 即生产者还在生产, 就继续尝试消费
	int cur;
//	while (prodFinish.load() < producerWorks) {
	while (prodFinish.compare_exchange_weak(producerWorks, producerWorks) == false) {
		std::lock_guard<mutex> lock(mtx_cons);
		if (qu.size() > 0)
			qu.pop();
	}
}

void lastConsume () {
	while (qu.empty() == false)
		qu.pop();
}

void epochRunMutex() {
	vector<thread> prod(producerWorks), cons(consumerWorks);

	for (int i = 0; i < producerWorks; i ++ )
		prod[i] = thread(produce, i);

	for (auto& t : cons)
		t = thread(consume);

	for (auto& t : prod)
		t.join();

	for (auto& t : cons)
		t.join();

	// 当生产者生产完毕后, 添加最后一个消费者, 该消费者保证能够将剩下的物品消费完毕
	lastConsume();
}

int main() {

	std::chrono::time_point<std::chrono::system_clock> start_time = std::chrono::system_clock::now();

	for (int i = 0; i < epochs; i ++ )
		epochRunMutex();

	std::chrono::time_point<std::chrono::system_clock> end_time = std::chrono::system_clock::now();
	std::chrono::milliseconds time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

	cout << "\nepoch cost average time = " << time.count() / epochs << " ms\n";
	return 0;
}
