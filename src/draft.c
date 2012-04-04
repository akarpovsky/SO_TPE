/*
 * draft.c
 *
 *  Created on: 03/04/2012
 *      Author: neon
 */


draft()
{
	if( ! draft_empezo)
	{
		jugadores_listos++;
		if(jugadores_listos == total_jugadores)
			crear_thread_coordinador()
			hacer_draft()
	}
	else
	{
		hacer_draft()
	}
}
hacer_draft()
{
	while(draft_not_over)
	{
		if(myTurn)
		{
			if(cliente_me_contesto)
				setear_respuesta
				if(cliente_quiere_salir)
					return;
			sleep(1);
		}
		else{
			if(cliente_quiere_salir)
				return;
		}
	}
}

thread_coordinador(){
	turno=1
			while(draft_isnt_over)
					time=60
					while (time--)
					{
						if(respuesta[turno] esta seteada)
						{
							asignar_jugador_a_equipo
							turno=el_que_sigue
						}
						sleep(1)
					}
	asignar_jugador_random;
	turno=el_que_sigue
}
