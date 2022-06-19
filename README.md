# Easy_UART_STM32
En la mayoria de mis desarrollos suelo implementar la misma dinamica para gestionar la comunicacion serie, por esto me decidi a armar mi propia libreria para tal fin.
Easy UART es una libreria para implementar de manera sencilla la comunicacion serie para los microncontroladores STM.
Puede funcionar con varios puertos en simultaneo, obtener las lineas  nuevas (separadas por cualquier caracter de comando) e ir almacenandolas en un buffer. Tambien puede descomponer en palabras las lineas separandolas por espacios.

_In most of my projets I usually implement the same dynamics to manage serial communication, for this reason I decided to build my own library for this purpose.
Easy UART is a library to easily implement serial communication for STM microcontrollers.
It can work with several ports simultaneously, get the new lines (separated by any command character) and store them in a buffer. You can also break lines into words by separating them with spaces._

#   Example of implementation
## Single port
```C
int main(){
    EU_init(&huart1);									//initialize port interrupt
    while(1){
	  if(EU_lineAvailable() == 1){        				//Return true if a newline is available
		  uint8_t number_of_words = EU_getNWords(); 	//Returns the number of words that this line has
		  uint16_t size_w; 
		  uint8_t *buffer_w = EU_getNextWord(&size_w);	//Returns the pointer to the start of the next word and the size of the word
		  uint16_t size_l;
		  uint8_t *buffer_l = EU_getLine(&size_l); 		//Return the pointer to the beginning of the last line (and update the line being worked on) and the size of the line
	  }
    }
}
```

## Multi port

```C
int main(){
    EUM_init(0, &huart1);				//initialize port interrupt, n is the port number
    EUM_init(1, &huart2);
	//... EUM_init(N, &huartN);
    while(1){
	  if(EUM_lineAvailable(n) == 1){        //Return true if a newline is available, n is the port number
		  uint8_t number_of_words = EUM_getNWords(n);    //Returns the number of words that this line has, n is the port number
		  uint16_t size_w; 
		  uint8_t *buffer_w = EUM_getNextWord(n, &size_w);//Returns the pointer to the start of the next word and the size of the word, n is the port number
		  uint16_t size_l;
		  uint8_t *buffer_l = EUM_getLine(n, &size_l); //Return the pointer to the beginning of the last line (and update the line being worked on) and the size of the line, n is the port number
	  }
    }
}
```
# Configurations
## in EasyUART.h
```C
#define MULTI_PORT 0 			//0:false, 1:true, defines if multiport mode is compiled
#define N_MULTI_PORT 3			//numbers of ports implemented
#define SIZE_BUFFERS 256		//size of all line in buffers
#define BUFFER_LINES 8			//numbers of lines stored for each port (circular buffer of the lines)

```
