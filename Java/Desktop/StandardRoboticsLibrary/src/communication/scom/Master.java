/*
 * Copyright (C) 2019 Robert Hutter
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
import com.fazecast.jSerialComm.SerialPortDataListener;
import com.fazecast.jSerialComm.SerialPortEvent;
import java.nio.ByteBuffer;
import java.nio.charset.Charset;
import java.util.Arrays;

/**
 *  Master.java - SCOM Communication master.
 * 
 * @see SCOM1204
 * @since 2019.10.26
 * @author deaxuser - Robert Hutter
 */
public final class Master implements SerialPortDataListener
{
    SerialPort port;
    
    private volatile int status;
    private volatile int awaitedSum;
    private volatile short lastSignal;
    private volatile int failedAttempts;
    
    private volatile int lastSlaveSignal;
    
    private final Charset charset;
    
    private final static int WAITING_FOR_SLAVE_SIGNAL = 1;
    private final static int WAITING_FOR_SLAVE_RESPONCE = 2;
    private final static int OK_CONTINUE = 0;
    
    private final static int MAX_ATTEMPTS = 100;
    private final static int VERSION = 1204;
    private final static byte SOH = 1;
    private final static byte EOT = 4;
    private final static byte ACK = 5;
    private final static byte ETB = 23;
    
    /**
     *  Constructor for SCOM Master
     * 
     * @param portDesc  Serial port name.
     * @param baudRate  Serial port baud rate.
     * @param charset   Serial port communication character set.
     */
    public Master(String portDesc, int baudRate, String charset)
    {
        this.failedAttempts = 0;
        port = SerialPort.getCommPort(portDesc);
        port.setBaudRate(baudRate);
        this.charset = Charset.forName(charset);
    }
    
    /**
     *  Constructor for SCOM Master.
     * 
     * @param port  Serial port.
     * @param baudRate Serial port baud rate.
     * @param charset   Serial port communication character set.
     */
    public Master(SerialPort port, int baudRate, String charset)
    {
        this.failedAttempts = 0;
        this.port = port;
        this.port.setBaudRate(baudRate);
        this.charset = Charset.forName(charset);
    }
    
    /**
     *  Opens serial communication.
     * 
     */
    public void open()
    {
        port.openPort();
        port.addDataListener(this);
        
        sendSignal(SOH);
        while(status != OK_CONTINUE) {}
        
        sendSignal(VERSION);
        while(status != OK_CONTINUE) {}
        
        status = WAITING_FOR_SLAVE_SIGNAL;
    }
    
    /**
     *  Closes serial communication.
     */
    public void close()
    {
        writeInt16(EOT);
        port.closePort();
    }
    
    /**
     *  Calculates the ASCII sum of the input String.
     * 
     * @see SCOM1204
     * @param str String to calculate ASCII sum of.
     * @return 
     */
    private int calculateSum(String str)
    {
        byte[] b = str.getBytes(charset);
        
        int sum = 0;
        for (byte k : b)
        {
            sum += k;
        }
        
        return sum;
    }
    
    /**
     * Sends 2 bytes of data over on serial in form of short.
     * 
     * @param i 2 Byte variable to send.
     */
    private void writeInt16(int i)
    {
        byte[] b = ByteBuffer.allocate(2).putShort((short)i).array();
        System.out.println("Sent: "+Arrays.toString(b));
        port.writeBytes(b, 2);
    }
    
    /**
     * Sends a signal to the Slave.
     * 
     * @see SCOM1204
     * @param i The signal identifier.
     */
    private void sendSignal(int i)
    {
       lastSignal = (short)i;
       status = WAITING_FOR_SLAVE_RESPONCE;
       awaitedSum = calculateSum(Integer.toString(i));
       writeInt16(i);
    }

    @Override
    public int getListeningEvents() {
        return SerialPort.LISTENING_EVENT_DATA_RECEIVED;
    }

    @Override
    public void serialEvent(SerialPortEvent event) {
        System.out.println("Received: "+Arrays.toString(event.getReceivedData()));
        
        if (status == WAITING_FOR_SLAVE_RESPONCE)
        {
            if (event.getReceivedData().length >= 2)
            {
               ByteBuffer bb = ByteBuffer.allocate(2);
               bb.put(event.getReceivedData()[0]);
               bb.put(event.getReceivedData()[1]);
               short responce = bb.getShort(0);
               System.out.println("Awaited: "+awaitedSum);
               System.out.println("Recived: "+responce);
               
               if (responce == awaitedSum)
               {
                   // Send ACK signal
                   writeInt16(ACK);
                   status = OK_CONTINUE;
                   failedAttempts = 0;
                   System.out.println("SUCCESS");
               }
               else
               {
                   if (failedAttempts < MAX_ATTEMPTS)
                   {
                        writeInt16(lastSignal);
                        failedAttempts++;
                   }
                   else
                   {
                       close();
                   }
               }
            }
        }
    }
}
