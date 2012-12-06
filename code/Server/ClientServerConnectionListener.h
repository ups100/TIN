///////////////////////////////////////////////////////////
//  ClientServerConnectionListener.h
//  Implementation of the Class ClientServerConnectionListener
//  Created on:      07-gru-2012 00:33:34
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_95DC1A3C_38E6_4071_B16C_8876A967096B__INCLUDED_)
#define EA_95DC1A3C_38E6_4071_B16C_8876A967096B__INCLUDED_

class ClientServerConnectionListener
{

public:
	ClientServerConnectionListener();
	virtual ~ClientServerConnectionListener();

	virtual void onConnectToAlias(QString name, Password password) =0;
	virtual void onCreateAlias(QString name, Password password) =0;
	virtual void onRemoveAlias(QString name, Password password) =0;

};
#endif // !defined(EA_95DC1A3C_38E6_4071_B16C_8876A967096B__INCLUDED_)
