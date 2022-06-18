/*
 * EasyUART.c
 *
 *  Created on: Jun 17, 2022
 *      Author: Santiago Cian, santiagocian97@gmail.com
 *      		https://github.com/SantiagoCian1997/Easy_UART_STM32
 */
#include "EasyUART.h"


extern UART_HandleTypeDef huart1;//borrar


#if MULTI_PORT
	struct port_str ports[N_MULTI_PORT];
#else
	struct port_str ports[1];
#endif

struct port_str * port_selected=ports;

/* Description: Create interruption for listen each port
 *
 * Parameters:
 * 				UART_HandleTypeDef *huart: struct of port UART (see HAL library)
 * Return:
 * 				none
 * Notes:
 * 				hem
 */
void EU_init(UART_HandleTypeDef *huart){

	(*port_selected).port=huart;
	HAL_UART_Receive_IT((*port_selected).port, &(*port_selected).inChar, 1);
	(*port_selected).actualLine=0;
	(*port_selected).lastLine=0;
	(*port_selected).indexBuffer=0;


}

/* Description: Confirm if there is aline to read
 *
 * Parameters:
 * 				none
 * Return:
 * 				1: if exist a new line for read
 * 				0: if don´t exist
 * Notes:
 *
 */
uint8_t EU_lineAvailable(){
	if((*port_selected).lastLine!=(*port_selected).actualLine){
		return (1);
	}
	return (0);
}


/* Description: If exist line for read, return pointer of buffer and modifies the line of the buffer that is read
 *
 * Parameters:
 * 				none
 * Return:
 * 				uint8_t *: pointer of buffer
 * 				if no exist line return NULL=0;
 * Notes:
 *
 */
uint8_t * EU_getLine(){
	if(EU_lineAvailable()){
		uint8_t *ret;
		ret=((*port_selected).buffer[(*port_selected).lastLine]);
		(*port_selected).lastLine=(*port_selected).actualLine;
		return(ret);
	}
	return(0);
}

/* Description: Returns number of word existin in buffer line (last line)
 *
 * Parameters:
 * 				none
 * Return:
 * 				uint16_t : number of word existin
 * Notes:
 *
 */
uint16_t EU_getNWords(){
	uint16_t indice=0;
	uint16_t NWords=0;
	(*port_selected).initActualWordsInLine[(*port_selected).lastLine]=0;
	while((*port_selected).buffer[(*port_selected).lastLine][indice]>' '){
		NWords++;
		while((*port_selected).buffer[(*port_selected).lastLine][indice++]>' ');
	}
	(*port_selected).NWordsInLine[(*port_selected).lastLine]=NWords;
	return (NWords);
}


/* Description: Returns pointer to init word and length of this
 *
 * Parameters:
 * 				uint16_t *size: returns here the length of the word
 * Return:
 * 				uint16_t *: pointer to init word
 * Notes:
 *
 */
uint8_t *EU_getNextWord(uint16_t *size){
	uint16_t inicio=(*port_selected).initActualWordsInLine[(*port_selected).lastLine];
	uint16_t fin=inicio;
	while ((*port_selected).buffer[(*port_selected).lastLine][fin]>' ')fin++;;
	//DEBUG
	printf("inicio:%i, fin:%i\n",inicio,fin);
	//

	(*size)=fin-inicio;
	(*port_selected).initActualWordsInLine[(*port_selected).lastLine]=fin+1;
	return (&(*port_selected).buffer[(*port_selected).lastLine][inicio]);
}

/** HAL library, interruption for UART
 *
  * @brief  Rx Transfer completed callbacks.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
#if MULTI_PORT
	for(uint8_t i=0;i<N_MULTI_PORT;i++) if(huart==ports[i].huart) port_selected=&ports[i];
#endif
	if((*port_selected).inChar<' '){
		//DEBUG
		//printf("fin line\n");
		//printf("Actual: %i, Size_line:%i\n",(*port_selected).actualLine,(*port_selected).indexBuffer);
		//printf("String: %s\n",(*port_selected).buffer[(*port_selected).actualLine]);
		//END eso
		(*port_selected).sizeLine[(*port_selected).actualLine]=(*port_selected).indexBuffer;
		(*port_selected).indexBuffer=0;
		(*port_selected).actualLine++;
		if((*port_selected).actualLine>=BUFFER_LINES)(*port_selected).actualLine=0;
	}else{
		if((*port_selected).indexBuffer<SIZE_BUFFERS-1){
			(*port_selected).buffer[(*port_selected).actualLine][(*port_selected).indexBuffer++]=(*port_selected).inChar;
			(*port_selected).buffer[(*port_selected).actualLine][(*port_selected).indexBuffer]=0;
		}
	}
	HAL_UART_Receive_IT((*port_selected).port, &(*port_selected).inChar, 1);
}
