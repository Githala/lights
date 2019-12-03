class SerialMessageService {
	public:
		bool hasMessage();
		void getMessage();
	
};



/*
A: HELLO
P: HELLO

S: PING
A: PONG

S: NODE xxxx <COMMANDS>

COMMAND: <MODE> | BRIGHTNESS [0-100]

MODE:
	SOLID [R,G,B]   (RGB = [0-255])
	BLINK [R,G,B]
	RAINBOW
	OFF

A: NODE xxxx <MODE> BRIGHTNESS [0-255]

S: NODE <ID> STATE
ID: xxxx | ALL

A: NODE xxxx <MODE> BRIGHTNESS [0-255]
*/