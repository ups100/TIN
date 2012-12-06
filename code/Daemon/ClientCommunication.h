///////////////////////////////////////////////////////////
//  ClientCommunication.h
//  Implementation of the Class ClientCommunication
//  Created on:      07-gru-2012 00:33:33
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_12038366_02C4_491b_8F39_E9A6E75F2374__INCLUDED_)
#define EA_12038366_02C4_491b_8F39_E9A6E75F2374__INCLUDED_

class ClientCommunication
{

public:
	ClientCommunication();
	virtual ~ClientCommunication();

	Utilities::Message getMessage();

};
#endif // !defined(EA_12038366_02C4_491b_8F39_E9A6E75F2374__INCLUDED_)
