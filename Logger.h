
#ifndef LOGGER_H
#define LOGGER_H


#pragma once
#include<iostream>
using namespace std;
#include <stdio.h>
#include <mutex>
#define LOG_FILE "log.txt"
#include <fstream>
#pragma warning(push, 0)
// Some include(s) with unfixable warnings
#pragma warning(pop)
enum LogPriority
{
	TracePriority, DebugPriority, InfoPriority, WarnPriority, ErrorPriority, CriticalPriority
};

ofstream file(LOG_FILE,ios::app);


class Logger
{
private:
	static LogPriority priority;



public:
	static void set_priority(LogPriority new_priority)
	{
		priority = new_priority;
	}

	template<typename... Args>
	static void Trace(string name,int line,const char* message, Args... args)
	{
		if (priority <= TracePriority)
		{
	
			printf("[Trace]\t");
			printf(message, args...);
			printf("[%s] [%s] [%s] [line:%d] ",__TIME__,__FILE__,__func__,__LINE__);
			printf("\n");
			file<<"\tDATE:"<<__DATE__<<"\tTIME:"<<__TIME__<<"\tFILENAME:"<<name<<"\tFUNCTIONNAME:"<<__func__<<"\t[line]:"<<line<<"\t"<<message<<endl;
		}
	}

	template<typename... Args>
	static void Debug(string name,int line,const char* message, Args... args)
	{
		if (priority <= DebugPriority)
		{
		
			printf("[Debug]\t");
			printf(message, args...);
			printf("[%s] [%s] [%s] [line:%d] ",__TIME__,__FILE__,__func__,__LINE__);
			printf("\n");
			file<<"\tDATE:"<<__DATE__<<"\tTIME:"<<__TIME__<<"\tFILENAME:"<<name<<"\tFUNCTIONNAME:"<<__func__<<"\t[line]:"<<line<<"\t"<<message<<endl;
		}
	}

	template<typename... Args>
	static void Info(string name,int line,const char* message, Args... args)
	{
		if (priority <= InfoPriority)
		{

			printf("[Info]\t");
			//printf(message, args...);
			printf("[%s] [%s] [%s] [line:%d] ",__TIME__,__FILE__,__func__,__LINE__);
			printf("\n");
			file<<"\tDATE:"<<__DATE__<<"\tTIME:"<<__TIME__<<"\tFILENAME:"<<name<<"\tFUNCTIONNAME:"<<__func__<<"\t[line]:"<<line<<"\t"<<message<<endl;
			cout<<endl;
		}
	}

	template<typename... Args>
	static void Warn(string name,int line,const char* message, Args... args)
	{
		if (priority <= WarnPriority)
		{
			
			printf("[Warn]\t");
			printf(message, args...);
			printf("[%s] [%s] [%s] [line:%d] ",__TIME__,__FILE__,__func__,__LINE__);
			printf("\n");
			file<<"\tDATE:"<<__DATE__<<"\tTIME:"<<__TIME__<<"\tFILENAME:"<<name<<"\tFUNCTIONNAME:"<<__func__<<"\t[line]:"<<line<<"\t"<<message<<endl;
		}
	}

	template<typename... Args>
	static void Error(string name,int line,const char* message, Args... args)
	{
		if (priority <= ErrorPriority)
		{
		
			printf("[Error]\t");
			//printf(message, args...);
			printf("[%s] [%s] [%s] [line:%d] ",__TIME__,__FILE__,__func__,__LINE__);
			printf("\n");
			file<<"\tDATE:"<<__DATE__<<"\tTIME:"<<__TIME__<<"\tFILENAME:"<<name<<"\tFUNCTIONNAME:"<<__func__<<"\t[line]:"<<line<<"\t"<<message<<endl;
		}
	}

	template<typename... Args>
	static void Critical(string name,int line,const char* message, Args... args)
	{
		if (priority <= CriticalPriority)
		{
		
			printf("[Critical]\t");
			printf(message, args...);
			printf("[%s] [%s] [%s] [line:%d] ",__TIME__,__FILE__,__func__,__LINE__);
			printf("\n");
			file<<"\tDATE:"<<__DATE__<<"\tTIME:"<<__TIME__<<"\tFILENAME:"<<name<<"\tFUNCTIONNAME:"<<__func__<<"\t[line]:"<<line<<"\t"<<message<<endl;
		}
	}
};

LogPriority Logger::priority = InfoPriority;
#endif
