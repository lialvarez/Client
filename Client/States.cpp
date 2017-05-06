#include "States.h"

//ST_ReceiveFirstAck
genericState * ST_ReceiveFirstAck::on_Ack(genericEvent * ev)
{
	//sendData();	//TODO
	return (genericState*) new ST_ReceiveAck;
}

genericState * ST_ReceiveFirstAck::on_Error(genericEvent * ev)
{
	return (genericState*) new ST_Idle;
}

genericState * ST_ReceiveFirstAck::on_Timeout(genericEvent * ev)
{
	//sendWRQ();	//TODO
	return nullptr;
}

genericState * ST_ReceiveFirstAck::on_ConnectionFailed(genericEvent * ev)
{
	//y aca qe verga hacemo? //TODO
	return nullptr;
}


//ST_ReceiveAck
genericState * ST_ReceiveAck::on_Ack(genericEvent * ev)
{
	//dataReceived();	//TODO	recien cuando llega el ack de la data enviada se mueve el FILE* para apuntar ala siguiente data
	//if(sendData())	//TODO
	//	return (genericState*) new ST_ReceiveLastData;
	return nullptr;
}

genericState * ST_ReceiveAck::on_Error(genericEvent * ev)
{
	return (genericState*) new ST_Idle;
}

genericState * ST_ReceiveAck::on_Timeout(genericEvent * ev)
{
	//sendData();	//TODO. Como no se invoca a la funcion dataReceived, se sigue apuntando al mismo paquete que antes
	return nullptr;
}

genericState * ST_ReceiveAck::on_ConnectionFailed(genericEvent * ev)
{
	return nullptr;
}

genericState * ST_ReceiveAck::on_LastData(genericEvent * ev)
{
	return (genericState*) new ST_ReceiveLastAck;
}

//ST_ReceiveLastAck
genericState * ST_ReceiveLastAck::on_Ack(genericEvent * ev)
{
	//TODO: mostrar que es ya se termino de enviar el mensaje
	return (genericState*) new ST_Idle;
}

genericState * ST_ReceiveLastAck::on_Error(genericEvent * ev)
{
	//TODO: mostrar error
	return (genericState*) new ST_Idle;
}

genericState * ST_ReceiveLastAck::on_Timeout(genericEvent * ev)
{
	//sendData()	//TODO
	return nullptr;
}

genericState * ST_ReceiveLastAck::on_ConnectionFailed(genericEvent * ev)
{
	return nullptr;
}

//ST_ReceiveFirstData
genericState * ST_ReceiveFirstData::on_Data(genericEvent * ev)
{
	//sendAck();
	return (genericState*) new ST_ReceiveData;
}

genericState * ST_ReceiveFirstData::on_Error(genericEvent * ev)
{
	return (genericState*) new ST_Idle;
}

genericState * ST_ReceiveFirstData::on_Timeout(genericEvent * ev)
{
	//sendRRQ()	//TODO
	return nullptr;
}

genericState * ST_ReceiveFirstData::on_ConnectionFailed(genericEvent * ev)
{
	return nullptr;
}

//ST_ReceiveData
genericState * ST_ReceiveData::on_Data(genericEvent * ev)
{
	//sendAck() //TODO
	return nullptr;
}

genericState * ST_ReceiveData::on_Error(genericEvent * ev)
{
	return (genericState*) new ST_Idle;
}

genericState * ST_ReceiveData::on_Timeout(genericEvent * ev)
{
	//sendAck() //TODO
	return nullptr;
}

genericState * ST_ReceiveData::on_ConnectionFailed(genericEvent * ev)
{
	return nullptr;
}

genericState * ST_ReceiveData::on_LastData(genericEvent * ev)
{
	return (genericState*) new ST_Idle;
}