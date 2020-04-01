/*
 * Copyright (C) 2020 Robert Hutter
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package communication.scom;

import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortEvent;
import java.nio.ByteBuffer;
import java.nio.charset.Charset;
import java.util.Arrays;

/**
 * Erstellt laut des SCOM 2.0 Protokolls.
 * Contains general functions to support SCOM.
 * 
 * @author Robert Hutter
 */
public final class SCOM
{
    final static short VERSION = 1206;
    
    // Signal messages
    final static short SOH = 0x01;
    final static short STX = 0x02;
    final static short ETX = 0x03;
    final static short EOT = 0x04;
    final static short ABF = 0x05;
    final static short ACK = 0x06;
    final static short ANT = 0x07;
    
    // Timing values
    final static long SIGNAL_TIMEOUT = 10000L;
    final static long TIMEOUT = 1000L;
    final static long ABF_INTERVAL = 1000L;
    /**
     * Time that must be waited after sending an ACK signal.
     */
    final static long ACK_WAIT = 550L;
    /**
     * Time waited before sending a ABF signal if ACK does'nt arrive.
     */
    final static long ACK_TIMEOUT = 400L;
    final static long REFRESH_INTERVAL = 10L;
    
    // Status values
    final static byte OK_CONTINUE = 0x00;
    final static byte WAITING_FOR_ASCII_SUM = 0x01;
    final static byte IN_ACK_TIMEOUT_BUFFER = 0x02;
    final static byte WAITING_FOR_INFO_MESSAGE = 0x03;
    //final static byte WAITING_FOR_MORE_ABF = 0x04;
    final static byte WAITING_FOR_ACK = 0x05;
    final static byte WAITING_FOR_REP_MESSAGE = 0x06;
    final static byte WAITING_FOR_ANT = 0x07;
    final static byte CONNECTION_CLOSED = 0x08;
    final static byte WAITING_FOR_MORE_ANT = 0x09;
    final static byte WAITING_FOR_SIGNAL = 0x10;
    
    final static byte MAX_ABF_ATTEMPTS = 10;
    
    /**
     *  Calculates the ASCII sum of the input String.
     * 
     * @param charset
     * @see SCOM 2.0 1204
     * @param str String to calculate ASCII sum of.
     * @return 
     */
    public static short calculateSum(String str, Charset charset)
    {
        byte[] b = str.getBytes(charset);
        
        short sum = 0;
        for (byte k : b)
        {
            sum += k;
        }
        
        return sum;
    }
    
    /**
     * 
     * @param s
     * @param charset
     * @return 
     */
    public static short calculateSum(short s, Charset charset)
    {
        return calculateSum(Short.toString(s), charset);
    }
    
    /**
     *  Sends a Signal over Serial.
     * 
     * @param sig
     * @param port
     */
    public static void writeSignal(Signal sig, SerialPort port)
    {
        port.writeBytes(sig.getBytes(), sig.length);
        System.out.println("Sent: "+Arrays.toString(sig.getBytes()));
    }
    
    /**
     * Writes an info message to serial.
     * 
     * @param message
     * @param charset
     * @param port 
     */
    public static void writeInfoMessage(String message, Charset charset, SerialPort port)
    {
        port.writeBytes(message.getBytes(charset), message.getBytes().length);
    }
    
    /**
     * Reads a signal from the serial buffer.
     * 
     * @param event
     * @return 
     */
    public static Signal readSignal(SerialPortEvent event)
    {
        ByteBuffer bb = ByteBuffer.allocate(event.getReceivedData().length);
        bb.put(event.getReceivedData());
        System.out.println("Recieved: "+Arrays.toString(event.getReceivedData()));
        return new Signal(bb.getShort(2), bb.getShort(0));
    }
}
