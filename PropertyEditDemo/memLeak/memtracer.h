#ifndef _H_MEMTRACER_H__
#define _H_MEMTRACER_H__

#include <map>

#ifdef _DEBUG

void* operator new (size_t _size, const char* _file, long _line);
//void * operator new (size_t _size);
void operator delete (void *_p);

class MemTracer
{
	class TraceInfo
	{
	public:
		TraceInfo(const char* _file = nullptr, long _line = 0);
		const char* File() const;
		long Line() const;
	private:
		const char* m_file;
		long  m_line;
	};

	class Lock
	{
	public :
		Lock(MemTracer & _tracer) : m_tracer(_tracer)
		{
			m_tracer.m_lockcount++;
		}
		~Lock()
		{
			m_tracer.m_lockcount--;
		}
	private:
		MemTracer		&m_tracer;
	};

public :
	MemTracer();
	virtual ~MemTracer();
public:
	void add(void* _p, const char* _file, long _line);
		
	void remove(void* _p);

	void dump();
public:
	static bool Ready;
private:
	std::map <void* , TraceInfo>	m_tracer_infos;
	long							m_lockcount;
};

extern MemTracer   CustomTracer;

#endif //!_DEBUG

#endif //!_H_MEMTRACER_H__