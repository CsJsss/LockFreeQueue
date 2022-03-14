//
// Created by Jsss on 2022/3/12.
//

#ifndef LOCKFREEQUEUE_LOCKFREEQUEUE_H
#define LOCKFREEQUEUE_LOCKFREEQUEUE_H

#include <atomic>

using std::atomic;


template <typename T>
class LockFreeQueue{

public:
	struct Node {
		T val;
		atomic<Node*> next;
		Node () : val(T()) {
			std::atomic_init(&next, nullptr);
		}
		Node (const T& _v) : val(_v) {
			std::atomic_init(&next, nullptr);
		}
		Node (const T& _v, atomic<Node*> _next) : val(_v), next(_next) {}
	};


	LockFreeQueue () {
		Node* dummy = new Node;
		head.store(dummy);
		tail.store(dummy);
	};

	// 入队操作
	void enqueue (T _val) {
		// 插入结点为p
		Node* p = new Node(_val);
		Node *cur, *next;
		Node* null = nullptr;
		while (true) {
			// cur 指针为尾结点的快照
			cur = tail.load();
			// next 指针为为尾结点下一个结点
			next = cur -> next;
			// 如果尾指针被移动, 则重新获取cur和next (可能被其他线程移动了, 重新获取以减少CAS操作次数)
			if (cur != tail.load())
				continue;
			if (next != nullptr) {
				// 当next指针不为空时, 尝试移动tail指针, 防止出现死锁
				tail.compare_exchange_weak(cur, next);
				continue;
			}
			// 实现第一步操作: tail -> next = p
			if (cur->next.compare_exchange_weak(null, p) == true)
				break;
		}
		// 实现第二步操作: tail = p
		tail.compare_exchange_weak(cur, p);
	};

	// version 1
//	bool dequeue (T& _ret) {
//		Node *nowHead, *next;
//
//		while (true) {
//			nowHead = head.load();
//			next = nowHead->next;
//			// 如果头指针被移动, 则重新获取其快照 (可能被其他线程移动了, 重新获取以减少CAS操作次数)
//			if (nowHead != head.load())
//				continue;
//			// 队列为空, 出队失败
//			if (next == nullptr)
//				return false;
//			// 在CAS操作前记录返回值, 如果在CAS操作之后则该结点有可能被其他线程free
//			_ret = next->val;
//			// 实现出队的操作. head = head -> next
//			if (head.compare_exchange_weak(nowHead, next))
//				break;
//		}
//		// 释放原头结点
//		delete nowHead;
//		return true;
//	}

	// version 2
	bool dequeue (T& _ret) {
		Node *nowHead, *nowTail, *next;

		while (true) {
			nowHead = head.load(), nowTail = tail.load();
			next = nowHead -> next;
			// 如果头指针被移动, 则重新获取其快照 (可能被其他线程移动了, 重新获取以减少CAS操作次数)
			if (nowHead != head.load())
				continue;
			// 队列为空, 出队失败
			if (nowHead == nowTail and next == nullptr)
				return false;
			// tail指针未正确取值, 尝试移动tail
			if (nowHead == nowTail and next != nullptr) {
				tail.compare_exchange_weak(nowTail, next);
				continue;
			}
			// 在CAS操作前记录返回值, 如果在CAS操作之后则该结点有可能被其他线程free
			_ret = next->val;
			// 实现出队的操作. head = head -> next
			if (head.compare_exchange_weak(nowHead, next))
				break;
		}
		// 释放原头结点
		delete nowHead;
		return true;
	}

	// 删除拷贝构造函数和拷贝赋值函数
	LockFreeQueue (const LockFreeQueue& ) = delete;
	LockFreeQueue operator = (const LockFreeQueue&) = delete;


private:
	atomic<Node*> head, tail;
};


#endif //LOCKFREEQUEUE_LOCKFREEQUEUE_H
