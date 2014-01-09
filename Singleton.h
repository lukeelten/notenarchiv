#ifndef SINGLETON_H
#define SINGLETON_H

template<class T>
class Singleton {
protected:
    static T* m_pSingleton;

public:
    virtual ~Singleton () throw() { }

    inline static T* Get() throw() {
        if (!m_pSingleton)
            m_pSingleton = new T;

        return m_pSingleton;
    }

    static void Del () throw() {
        if (m_pSingleton) {
            delete m_pSingleton;
            m_pSingleton = nullptr;
        }
    }

};

template<class T>
T* Singleton<T>::m_pSingleton = nullptr;

#endif // SINGLETON_H
