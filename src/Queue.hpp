/*
 * Queue.hpp
 *
 *  Created on: Feb 19, 2015
 *      Author: gamberini
 */

/*
 * TransmissionQueue.h
 *
 *  Created on: Sep 17, 2014
 *      Author: gamberini
 */

#ifndef QUEUE_HPP_
#define QUEUE_HPP_


#include <queue>
#include <boost/thread.hpp>
#include "Types.hpp"

template <class T>
class Queue {
private:
	boost::mutex m_mtx;
	std::queue<T> q;
	bool empty;
	T * ret;

public:
	Queue() {
		ret = NULL;
		empty = false;
	}

	void Enque(T& toEnque) {
		m_mtx.lock();
		q.push(toEnque);
		m_mtx.unlock();
	}

	T * Deque() {
		do {
			m_mtx.lock();
			empty = q.empty();
			m_mtx.unlock();
			if(empty) {
				boost::this_thread::sleep(boost::posix_time::microseconds(1));
			}
		} while(empty);
		m_mtx.lock();
		ret = & q.front();
		m_mtx.unlock();
		return ret;
	}

	void Pop() {
		m_mtx.lock();
		q.pop();
		m_mtx.unlock();
	}

	bool IsEmpty() {
		return q.empty();
	}
};

#endif /* QUEUE_HPP_ */
