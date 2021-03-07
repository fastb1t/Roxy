#ifndef _ROXY_ENUMERATOR_H_
#define _ROXY_ENUMERATOR_H_

#include <vector>
#include <algorithm>

namespace Roxy {

    template<typename T>
    class Enumerator {
        std::vector<T> m_data;
        size_t m_current;

    public:
        Enumerator() : m_current(0) {}

        ~Enumerator() {
            Clear();
        }

        void Clear() {
            m_data.clear();
            m_current = 0;
        }

        void New() {
            m_current = 0;
        }
        
        void Next() {
            if (m_data.size() == 0) {
                m_current = 0;
                return;
            }

            m_current++;

            if (m_current >= m_data.size()) {
                m_current = 0;
            }
        }

        void Shuffle() {
            std::random_shuffle(m_data.begin(), m_data.end(), [](int i) {
                return rand() % i;
            });
        }

        void Add(T t) {
            m_data.push_back(t);
        }

        T Get() {
            if (m_data.size() == 0 || m_current >= m_data.size()) {
                return T(0);
            }

            return m_data[m_current];
        }
    };

}

#endif
