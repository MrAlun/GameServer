#ifndef _SERVER_H_
#define _SERVER_H_

#include "../Thread/Thread.h"

using namespace std;

class CServer : public CThread
{
public:
	CServer(){}
	~CServer(){}

	 bool Init();

	 virtual void Run();
};

#endif//_SERVER_H_