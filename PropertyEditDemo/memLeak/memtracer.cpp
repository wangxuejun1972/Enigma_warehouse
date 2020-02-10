#include "memtracer.h"

#ifdef _DEBUG

#include <cstdlib>
#include <iostream>
#include <fstream>
using std::ofstream;

MemTracer   CustomTracer;
bool MemTracer::Ready = false;

void * operator new (size_t _size, const char* _file, long _line)
{
	void * p = malloc(_size);

	if (MemTracer::Ready)
		CustomTracer.add(p, _file, _line);

	return p;
}

//void * operator new (size_t _size)
//{
//	void * p = malloc(_size);
//
//	if (MemTracer::Ready)
//		CustomTracer.add(p, "Unknow", -1);
//
//	return p;
//}

void operator delete (void *_p)
{
	if (MemTracer::Ready)
		CustomTracer.remove(_p);
	free(_p);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

MemTracer::TraceInfo::TraceInfo(const char* _file, long _line) : m_file(_file), m_line(_line)
{
}

const char* MemTracer::TraceInfo::File() const
{
	return m_file;
}

long MemTracer::TraceInfo::Line() const
{
	return m_line;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

MemTracer::MemTracer() : m_lockcount(0)
{
	MemTracer::Ready = true;
}

MemTracer::~MemTracer()
{
	MemTracer::Ready = false;
	dump();
}

void MemTracer::add(void* _p, const char* _file, long _line)
{
	if (m_lockcount > 0)
		return;
	Lock  lock(*this);
	m_tracer_infos[_p] = TraceInfo(_file, _line);
	
}

void MemTracer::remove(void* _p)
{
	if (m_lockcount > 0)
		return;
	Lock  lock(*this);
	auto it = m_tracer_infos.find(_p);
	if (it != m_tracer_infos.end())
	{
		m_tracer_infos.erase(it);
	}
}

void MemTracer::dump()
{
	/*for (auto trace_info : m_tracer_infos)
	{
		std::cout << "0x" << trace_info.first << ":\t" << trace_info.second.File() << ":\tIn line" << trace_info.second.Line() << std::endl;
	}*/
	ofstream dumpfile("dumpfile.txt");
	if (dumpfile.is_open())
	{
		for (auto trace_info : m_tracer_infos)
		{
			std::cout << "0x" << trace_info.first << ":\t" << trace_info.second.File() << ":\tIn line" << trace_info.second.Line() << std::endl;
			dumpfile << "0x" << trace_info.first << ":\t" << trace_info.second.File() << ":\tIn line" << trace_info.second.Line() << "\n";
		}
		dumpfile.close();
	}
}

#endif //!_DEBUG