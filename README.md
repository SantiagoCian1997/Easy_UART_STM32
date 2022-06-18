# Easy_UART_STM32
En la mayoria de mis desarrollos suelo implementar la misma dinamica para gestionar la comunicacion serie, por esto me decidi a armar mi propia libreria para tal fin
Easy UART es una libreria para implementar de manera sencilla la comunicacion serie para los microncontroladores STM.
Puede funcionar con varios puertos en simultaneo, obtener las lineas  nuevas (separadas por cualquier caracter de comando) e ir almacenandolas en un buffer. Tambien puede descomponer en palabras las lineas separandolas por espacios.

_In most of my developments I usually implement the same dynamics to manage serial communication, for this reason I decided to build my own library for this purpose
Easy UART is a library to easily implement serial communication for STM microcontrollers.
It can work with several ports simultaneously, get the new lines (separated by any command character) and store them in a buffer. You can also break lines into words by separating them with spaces._

#   Example of implementation
## Single port
int main(){
    EU_init(&huart1);
    while(1){
	  if(EU_lineAvailable() == 1){        //Consulta si hay una nueva linea disponible
		  uint8_t number_of_words = EU_getNWords();    //Retorna la cantidad de palabras que posee esta linea
		  uint16_t size;
		  uint8_t *word=EU_getNextWord(&size); //Retorna el punero al inicio de la siguiente palabra y el tamaño de la misma
		  uint8_t *line =EU_getLine(); //Retorna el inicio de la ultima linea (y actualiza la linea que se esta trabajando)
	  }
    }
}

## Multi port

//in future
