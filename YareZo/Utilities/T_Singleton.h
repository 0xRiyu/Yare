#ifndef YAREZO_T_SINGLETON_H
#define YAREZO_T_SINGLETON_H

#include "src/Core.h"
#include <mutex>

namespace Yarezo {
	namespace Utilities {

		template <class T>
		class T_Singleton {
		public:
			// Provide global access to the only instance of this class
			static T* instance() {
				if (!m_Instance) {
					// to prevent multiple threads initialising multiple instances of the class
					// we lock it as soon as its being created 
					std::lock_guard<std::mutex> mutex_lock(m_Constructed);
					// Check to see if another thread has already initialised an instance in the time it took to acquire a lock
					if (!m_Instance) {
						m_Instance = new T();
					}
				}
				return m_Instance;
			}

			// Provide global access to release/delete this class
			static void release() {
				std::lock_guard<std::mutex> mutex_lock(m_Constructed);
				if (m_Instance) {
					delete m_Instance;
					m_Instance = nullptr;
				}
			}

		protected:
			// TSingleton should only be constructed by itself
			T_Singleton() {}
			~T_Singleton() {}

			static T* m_Instance;
			static std::mutex m_Constructed;

		private:
			// No copying allowed
			NONCOPYABLE(T_Singleton);
		};

		// Ensure that the instance is initialised to NULL at the start of the program
		template <class T> std::mutex T_Singleton<T>::m_Constructed;
		template <class T> T* T_Singleton<T>::m_Instance = nullptr;
	}
}

#endif //YAREZO_T_SINGLETON_H