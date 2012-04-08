/*
 * message.c
 *
 */

#include "../includes/message_serialization.h"

Msg_t deserializeMsg(void * stream){

	Msg_t msg = (Msg_t) calloc(1, sizeof(msg_t));

	Msg_t (*_deserialize_t[MSG_T_MAX_COMMANDS])(Msg_t msg, void * stream) = {
			deserialize_contact,
			deserialize_register,
			deserialize_login,
			deserialize_listLeagues,
			deserialize_listTeams,
			deserialize_listTrades,
			deserialize_leagueShow,
			deserialize_teamShow,
			deserialize_tradeShow,
			deserialize_trade,
			deserialize_tradeWithdraw,
			deserialize_tradeAccept,
			deserialize_tradeNegotiate,
			deserialize_logout,
			deserialize_joinLeague,
			deserialize_createLeague,
			deserialize_draft,
			deserialize_draftOut,
			deserialize_choose
	};

	memcpy(&(msg->type), stream, sizeof(int));
	stream += sizeof(int);
	printf("Deserializing a %d message \n", msg->type);
	return _deserialize_t[msg->type](msg, stream);
}

void * serializeMsg(Msg_t msg){
	void * (*_serialize_t[MSG_T_MAX_COMMANDS]) (Msg_t msg) = {
			serialize_contact,
			serialize_register,
			serialize_login,
			serialize_listLeagues,
			serialize_listTeams,
			serialize_listTrades,
			serialize_leagueShow,
			serialize_teamShow,
			serialize_tradeShow,
			serialize_trade,
			serialize_tradeWithdraw,
			serialize_tradeAccept,
			serialize_tradeNegotiate,
			serialize_logout,
			serialize_joinLeague,
			serialize_createLeague,
			serialize_draft,
			serialize_draftOut,
			serialize_choose
	};

	printf("Serializing a %d, message\n", msg->type);
	return _serialize_t[msg->type](msg);
}

void * serialize_s (Msg_s msg){

	printf("Serializing a %d server message\n", msg->responseType);

	void * msgstr;
	void * msgstraux;
	int msgSize;
	int NumEl = msg->msgList->NumEl;
	int * sizes = malloc(NumEl * sizeof(int));
	int msgListSize = 0;
	int i=0;
	Element e;

	// printf("Cantidad de elem en la lista %d (serialize_s)\n", msg->msgList->NumEl);

	FOR_EACH(e, msg->msgList){
		sizes[i] = strlen(e->data)+1;
		// printf("Size del elemento %d (serialize_s_ \n", sizes[i]);
		msgListSize += sizes[i];
		i++;
	}

	msgSize = 3*sizeof(int) + msgListSize;
	msgstr = msgstraux = malloc(msgSize + sizeof(int));

	memcpy(msgstraux, &msgSize, sizeof(int));
	msgstraux += sizeof(int);
	// printf("Size = %d\n", msgSize);

	memcpy(msgstraux, &(msg->responseType), sizeof(int));
	msgstraux += sizeof(int);

	memcpy(msgstraux, &(msg->status), sizeof(int));
	msgstraux += sizeof(int);

	// printf("Status = %d\n", msg->status);

	memcpy(msgstraux, &(msg->msgList->NumEl), sizeof(int));
	msgstraux += sizeof(int);

	// printf("Cant elem = %d\n", NumEl);

	i = 0;

	FOR_EACH(e, msg->msgList){
		strcpy(msgstraux, e->data);
		// printf("Mensaje: %s\n", (char *) e->data);
		msgstraux += sizes[i];

		i++;
	}

	return msgstr;

}
Msg_s deserialize_s (void * stream){

	Msg_s msg = malloc(sizeof(msg_s));
	List l = malloc(sizeof(llist));
	int elemQty;
	int size, i;

	CreateList(l);

	msg->msgList = l;

	memcpy(&(msg->responseType), stream, sizeof(int));
	stream += sizeof(int);

	memcpy(&(msg->status), stream, sizeof(int));
	stream += sizeof(int);

	// printf("status = %d \n", msg->status);

	memcpy(&(elemQty), stream, sizeof(int));
	stream += sizeof(int);

	// printf("Elemqty = %d \n", elemQty);

	for(i=0; i<elemQty; i++){
		char * str = malloc((size = strlen(stream)+1));
		// printf("Stream: %s\n", (char *) stream);
		strcpy(str, stream);
		stream += size;

		AddToList(str, msg->msgList);
	}

	printf("Deserialized a %d server message\n", msg->responseType);
	return msg;

}

void * serialize_register (Msg_t msg){
	int msgSize;
	void * msgstraux;
	void * msgstr;
	int passSize, userSize;

	passSize = strlen(msg->data.register_t.pass)+1;
	userSize = strlen(msg->data.register_t.user)+1;
	msgSize = sizeof(int)+passSize+userSize;

	msgstraux = msgstr = malloc(msgSize+sizeof(int));

	memcpy(msgstraux, &(msgSize), sizeof(int));
	msgstraux += sizeof(int);
	memcpy(msgstraux, &(msg->type), sizeof(int));
	msgstraux += sizeof(int);
	strcpy(msgstraux, msg->data.register_t.user);
	msgstraux += userSize;
	strcpy(msgstraux, msg->data.register_t.pass);

	return msgstr;
}

void * serialize_login (Msg_t msg){
	int msgSize;
	void * msgstraux;
	void * msgstr;
	int passSize, userSize;

	passSize = strlen(msg->data.login_t.pass)+1;
	userSize = strlen(msg->data.login_t.user)+1;
	// printf("User size = %d\n", userSize);
	// printf("Pass size = %d\n", passSize);
	msgSize = sizeof(int)+passSize+userSize;

	msgstraux = msgstr = calloc(1, msgSize+sizeof(int));

	memcpy(msgstraux, &(msgSize), sizeof(int));
	msgstraux += sizeof(int);
	memcpy(msgstraux, &(msg->type), sizeof(int));
	msgstraux += sizeof(int);
	msg->data.login_t.user[userSize-1] = '\0';
	strcpy(msgstraux, msg->data.login_t.user);
	msgstraux += userSize;
	msg->data.login_t.pass[passSize-1] = '\0';
	strcpy(msgstraux, msg->data.login_t.pass);
	// printf("\t Pass = %s\n", (char *)msgstraux);

	return msgstr;
}

void * serialize_listLeagues (Msg_t msg){
	int msgSize;
	void * msgstraux;
	void * msgstr;

	msgSize = sizeof(int);

	msgstraux = msgstr = malloc(msgSize+sizeof(int));

	memcpy(msgstraux, &(msgSize), sizeof(int));
	msgstraux += sizeof(int);
	memcpy(msgstraux, &(msg->type), sizeof(int));

	return msgstr;
}

void * serialize_listTeams (Msg_t msg){
	int msgSize;
	void * msgstraux;
	void * msgstr;

	msgSize = sizeof(int);

	msgstraux = msgstr = malloc(msgSize+sizeof(int));

	memcpy(msgstraux, &(msgSize), sizeof(int));
	msgstraux += sizeof(int);
	memcpy(msgstraux, &(msg->type), sizeof(int));

	return msgstr;
}

void * serialize_listTrades (Msg_t msg){
	int msgSize;
	void * msgstraux;
	void * msgstr;

	msgSize = sizeof(int);

	msgstraux = msgstr = malloc(msgSize+sizeof(int));

	memcpy(msgstraux, &(msgSize), sizeof(int));
	msgstraux += sizeof(int);
	memcpy(msgstraux, &(msg->type), sizeof(int));

	return msgstr;
}

void * serialize_leagueShow (Msg_t msg){
	int msgSize;
	void * msgstraux;
	void * msgstr;

	msgSize = 2*sizeof(int);

	msgstraux = msgstr = malloc(msgSize+sizeof(int));

	memcpy(msgstraux, &(msgSize), sizeof(int));
	msgstraux += sizeof(int);
	memcpy(msgstraux, &(msg->type), sizeof(int));
	msgstraux += sizeof(int);
	memcpy(msgstraux, &(msg->data.show_t.ID), sizeof(int));

	return msgstr;
}

void * serialize_teamShow (Msg_t msg){
	int msgSize;
	void * msgstraux;
	void * msgstr;

	msgSize = 2*sizeof(int);

	msgstraux = msgstr = malloc(msgSize+sizeof(int));

	memcpy(msgstraux, &(msgSize), sizeof(int));
	msgstraux += sizeof(int);
	memcpy(msgstraux, &(msg->type), sizeof(int));
	msgstraux += sizeof(int);
	memcpy(msgstraux, &(msg->data.show_t.ID), sizeof(int));

	return msgstr;
}

void * serialize_tradeShow (Msg_t msg){
	int msgSize;
	void * msgstraux;
	void * msgstr;

	msgSize = 2*sizeof(int);

	msgstraux = msgstr = malloc(msgSize+sizeof(int));

	memcpy(msgstraux, &(msgSize), sizeof(int));
	msgstraux += sizeof(int);
	memcpy(msgstraux, &(msg->type), sizeof(int));
	msgstraux += sizeof(int);
	memcpy(msgstraux, &(msg->data.show_t.ID), sizeof(int));

	return msgstr;
}

void * serialize_trade (Msg_t msg){
	int msgSize;
	void * msgstraux;
	void * msgstr;
	int fromSize, toSize;

	fromSize = strlen(msg->data.trade_t.from)+1;
	toSize = strlen(msg->data.trade_t.to)+1;
	msgSize = 2*sizeof(int) + fromSize + toSize;
	msgstraux = msgstr = malloc(msgSize+sizeof(int));
	memcpy(msgstraux, &(msgSize), sizeof(int));
	msgstraux += sizeof(int);
	memcpy(msgstraux, &(msg->type), sizeof(int));
	msgstraux += sizeof(int);
	strcpy(msgstraux, msg->data.trade_t.from);
	msgstraux += fromSize;
	strcpy(msgstraux, msg->data.trade_t.to);
	msgstraux += toSize;
	memcpy(msgstraux, &(msg->data.trade_t.teamID), sizeof(int));

	return msgstr;
}

void * serialize_tradeWithdraw (Msg_t msg){
	int msgSize;
	void * msgstraux;
	void * msgstr;

	msgSize = 2*sizeof(int);

	msgstraux = msgstr = malloc(msgSize+sizeof(int));

	memcpy(msgstraux, &(msgSize), sizeof(int));
	msgstraux += sizeof(int);
	memcpy(msgstraux, &(msg->type), sizeof(int));
	msgstraux += sizeof(int);
	memcpy(msgstraux, &(msg->data.trade_t.tradeID), sizeof(int));

	return msgstr;
}

void * serialize_tradeAccept (Msg_t msg){
	int msgSize;
	void * msgstraux;
	void * msgstr;

	msgSize = 2*sizeof(int);

	msgstraux = msgstr = malloc(msgSize+sizeof(int));

	memcpy(msgstraux, &(msgSize), sizeof(int));
	msgstraux += sizeof(int);
	memcpy(msgstraux, &(msg->type), sizeof(int));
	msgstraux += sizeof(int);
	memcpy(msgstraux, &(msg->data.trade_t.tradeID), sizeof(int));

	return msgstr;
}

void * serialize_tradeNegotiate (Msg_t msg){
	int msgSize;
	void * msgstraux;
	void * msgstr;
	int fromSize, toSize;

	fromSize = strlen(msg->data.trade_t.from)+1;
	toSize = strlen(msg->data.trade_t.to)+1;
	msgSize = 2*sizeof(int) + fromSize + toSize;

	msgstraux = msgstr = malloc(msgSize+sizeof(int));

	memcpy(msgstraux, &(msgSize), sizeof(int));
	msgstraux += sizeof(int);
	memcpy(msgstraux, &(msg->type), sizeof(int));
	msgstraux += sizeof(int);
	memcpy(msgstraux, msg->data.trade_t.from, fromSize);
	msgstraux += fromSize;
	memcpy(msgstraux, msg->data.trade_t.to, toSize);
	msgstraux += toSize;
	memcpy(msgstraux, &(msg->data.trade_t.tradeID), sizeof(int));

	return msgstr;
}

void * serialize_joinLeague (Msg_t msg){
	int msgSize;
	void * msgstraux;
	void * msgstr;

	msgSize = 2 * sizeof(int);

	msgstr = msgstraux = malloc(msgSize+sizeof(int));

	memcpy(msgstraux, &(msgSize), sizeof(int));
	msgstraux += sizeof(int);
	memcpy(msgstraux, &(msg->type), sizeof(int));
	msgstraux += sizeof(int);
	memcpy(msgstraux, &(msg->data.ID), sizeof(int));

	return msgstr;
}

void * serialize_createLeague (Msg_t msg){
	int msgSize;
	void * msgstraux;
	void * msgstr;
	int nameSize;

	nameSize = strlen(msg->data.name)+1;
	msgSize = sizeof(int)+nameSize;

	msgstr = msgstraux = malloc(msgSize+sizeof(int));

	memcpy(msgstraux, &(msgSize), sizeof(int));
	msgstraux += sizeof(int);
	memcpy(msgstraux, &(msg->type), sizeof(int));
	msgstraux += sizeof(int);
	strcpy(msgstraux, msg->data.name);

	return msgstr;
}

void * serialize_draft (Msg_t msg){
	int msgSize;
	void * msgstraux;
	void * msgstr;

	msgSize = 2 * sizeof(int);

	msgstr = msgstraux = malloc(msgSize+sizeof(int));

	memcpy(msgstraux, &(msgSize), sizeof(int));
	msgstraux += sizeof(int);
	memcpy(msgstraux, &(msg->type), sizeof(int));
	msgstraux += sizeof(int);
	memcpy(msgstraux, &(msg->data.ID), sizeof(int));

	return msgstr;
}

void * serialize_draftOut (Msg_t msg){
	int msgSize;
	void * msgstraux;
	void * msgstr;

	msgSize = sizeof(int);

	msgstraux = msgstr = malloc(msgSize+sizeof(int));

	memcpy(msgstraux, &(msgSize), sizeof(int));
	msgstraux += sizeof(int);
	memcpy(msgstraux, &(msg->type), sizeof(int));

	return msgstr;
}

void * serialize_logout (Msg_t msg){
	int msgSize;
	void * msgstraux;
	void * msgstr;

	msgSize = sizeof(int);

	msgstraux = msgstr = malloc(msgSize+sizeof(int));

	memcpy(msgstraux, &(msgSize), sizeof(int));
	msgstraux += sizeof(int);
	memcpy(msgstraux, &(msg->type), sizeof(int));

	return msgstr;
}

Msg_t deserialize_register (Msg_t msg, void * stream){
	int size;

	msg->data.register_t.user = malloc((size = strlen(stream)+1));
	strcpy(msg->data.register_t.user, stream);
	stream += size;
	msg->data.register_t.pass = malloc(strlen(stream)+1);
	strcpy(msg->data.register_t.pass, stream);

	return msg;
}

Msg_t deserialize_login (Msg_t msg, void * stream){
	int size;
	msg->data.login_t.user = (char *) calloc(1, (size = strlen(stream)+1));
	strcpy(msg->data.login_t.user, stream);
	// printf("Deserialize login user size = %d - user = %s\n", size, msg->data.login_t.user);
	stream += size;
	msg->data.login_t.pass = (char * ) calloc(1, (strlen(stream)+1));
	strcpy(msg->data.login_t.pass, stream);
	// printf("Deserialize login pass size = %d - pass = %s\n", strlen(stream)+1, msg->data.login_t.pass);
	return msg;
}

Msg_t deserialize_listLeagues (Msg_t msg, void * stream){

	/*
	 * XXX: Nothing to do here.
	 */

	return msg;
}

Msg_t deserialize_listTeams (Msg_t msg, void * stream){

	/*
	 * XXX: Nothing to do here.
	 */

	return msg;
}

Msg_t deserialize_listTrades (Msg_t msg, void * stream){

	/*
	 * XXX: Nothing to do here.
	 */

	return msg;
}

Msg_t deserialize_leagueShow (Msg_t msg, void * stream){

	memcpy(&(msg->data.show_t.ID), stream, sizeof(int));

	return msg;
}

Msg_t deserialize_teamShow (Msg_t msg, void * stream){

	memcpy(&(msg->data.show_t.ID), stream, sizeof(int));

	return msg;
}

Msg_t deserialize_tradeShow (Msg_t msg, void * stream){

	memcpy(&(msg->data.show_t.ID), stream, sizeof(int));

	return msg;
}

Msg_t deserialize_trade (Msg_t msg, void * stream){
	int size;

	msg->data.trade_t.from = malloc((size = strlen(stream)+1));
	strcpy(msg->data.trade_t.from, stream);
	stream += size;
	msg->data.trade_t.to = malloc((size = strlen(stream)+1));
	strcpy(msg->data.trade_t.to, stream);
	stream += size;
	memcpy(&(msg->data.trade_t.teamID), stream, sizeof(int));

	return msg;
}

Msg_t deserialize_tradeWithdraw (Msg_t msg, void * stream){

	memcpy(&(msg->data.trade_t.tradeID), stream, sizeof(int));

	return msg;
}

Msg_t deserialize_tradeAccept (Msg_t msg, void * stream){

	memcpy(&(msg->data.trade_t.tradeID), stream, sizeof(int));

	return msg;
}

Msg_t deserialize_tradeNegotiate (Msg_t msg, void * stream){
	int size;

	msg->data.trade_t.from = malloc((size = strlen(stream)+1));
	strcpy(msg->data.trade_t.from, stream);
	stream += size;

	msg->data.trade_t.to = malloc((size = strlen(stream)+1));
	strcpy(msg->data.trade_t.to, stream);
	stream +=size;

	memcpy(&(msg->data.trade_t.tradeID), stream, sizeof(int));

	return msg;
}

Msg_t deserialize_joinLeague (Msg_t msg, void * stream){

	memcpy(&(msg->data.ID), stream, sizeof(int));

	return msg;
}

Msg_t deserialize_createLeague (Msg_t msg, void * stream){
	int size;

	msg->data.name = malloc((size = strlen(stream)+1));

	strcpy(msg->data.name, stream);

	return msg;
}

Msg_t deserialize_draft (Msg_t msg, void * stream){

	memcpy(&(msg->data.ID), stream, sizeof(int));

	return msg;
}

Msg_t deserialize_draftOut (Msg_t msg, void * stream){

	/*
	 * XXX: Nothing to do here.
	 */

	return msg;
}

Msg_t deserialize_logout (Msg_t msg, void * stream){

	/*
	 * XXX: Nothing to do here.
	 */

	return msg;
}

Msg_t deserialize_choose (Msg_t msg, void * stream)
{
	int size;

	msg->data.name = malloc((size = strlen(stream)+1));

	strcpy(msg->data.name, stream);

	return msg;
}

void * serialize_choose (Msg_t msg)
{
	int msgSize;
	void * msgstraux;
	void * msgstr;
	int nameSize;

	nameSize = strlen(msg->data.name)+1;
	msgSize = sizeof(int)+nameSize;

	msgstr = msgstraux = malloc(msgSize+sizeof(int));

	memcpy(msgstraux, &(msgSize), sizeof(int));
	msgstraux += sizeof(int);
	memcpy(msgstraux, &(msg->type), sizeof(int));
	msgstraux += sizeof(int);
	strcpy(msgstraux, msg->data.name);

	return msgstr;
}
