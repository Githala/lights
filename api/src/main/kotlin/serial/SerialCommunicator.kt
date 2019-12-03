package serial

import com.fazecast.jSerialComm.*
import java.lang.Thread.sleep
import java.util.*

class SerialCommunicator(private val portName: String) {

    private val port = SerialPort.getCommPorts().find { p -> p.systemPortName.equals(portName) }!!
    private val scanner: Scanner

    init {
        port.setComPortParameters(9600, 8, 1, 0)
        port.setComPortTimeouts(SerialPort.TIMEOUT_READ_SEMI_BLOCKING, 0, 0)
        println("Connecting to Arduino")
        port.openPort()

        scanner = Scanner(port.inputStream)

        var waitingForGreeting = true
        while(waitingForGreeting) {
            print(".")
            if(scanner.hasNextLine()) {
                val nextLine = scanner.nextLine()
                if(!nextLine.equals("HELLO!")) {
                    waitingForGreeting = false;
                    print("\n")
                    println("Received greeting from Arduino.")
                }
            } else {
                Thread.sleep(1000)
            }
        }
    }

    @Synchronized
    fun readMessage(): String? {
        while(!scanner.hasNextLine()) {
            sleep(100)
        }
        return scanner.nextLine();
    }
}