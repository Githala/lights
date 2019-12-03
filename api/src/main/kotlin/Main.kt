import com.fazecast.jSerialComm.SerialPort
import serial.SerialCommunicator
import spark.Spark.get
import java.util.*


fun main(args: Array<String>) {
    val serialCommunicator = SerialCommunicator("cu.wchusbserial14340")

    get("/hello") { req, res -> "Hello World" }
    get("/message") { req, res -> serialCommunicator.readMessage() }
}