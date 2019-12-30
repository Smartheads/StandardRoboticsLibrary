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
import exceptions.ConnectionTimeoutException;
import exceptions.IncompatibleProtocolVersionException;
import java.nio.ByteBuffer;
import java.nio.charset.Charset;
import java.util.Arrays;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *  Master.java - SCOM Communication master.
 * 
 * @see SCOM1204
 * @since 2019.10.26
 * @author deaxuser - Robert Hutter
 */
public /*final*/ class Master implements SerialPortDataListener
{
    SerialPort port;
    
    private volatile int status;
    private volatile int awaitedSum;
    private volatile short lastSignal;
    private volatile Command lastCommand;
    private volatile int failedAttempts;
    
    private volatile short lastSlaveSignal;
    private volatile long messageId;
    
    private final Charset charset;
    
    private final static int COMMUNICATION_ENDED = -2;
    private final static int NONE = -1;
    private final static int WAITING_FOR_SLAVE_SIGNAL = 1;
    private final static int WAITING_FOR_SLAVE_RESPONCE = 2;
    private final static int RECOVERING_WRONG_SLAVE_RESPONCE = 3;
    private final static int OK_CONTINUE = 0;
    
    private final static int MAX_ATTEMPTS = 10;
    private final static short VERSION = 1204;
    private final static short SOH = 1;
    private final static short STX = 2;
    private final static short ETX = 3;
    private final static short EOT = 4;
    private final static short ACK = 5;
    private final static short ETB = 23;
    
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
        port.openPort();
        port.addDataListener(this);
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
     *  Sends a coammnd over Serial,
     * 
     * @param command
     * @throws exceptions.ConnectionTimeoutException
     * @see SCOM Protocol
     */
    public void sendCommand(Command command) throws ConnectionTimeoutException
    {
        sendSignal(STX);
        while(status != OK_CONTINUE) {watchTimeout();}
        
        writeInfoMessage(command.toString());
        awaitedSum = calculateSum(command.toString());
        
        // Wait for responce
        status = WAITING_FOR_SLAVE_RESPONCE;
        lastSignal = NONE;
        while(status != OK_CONTINUE) {watchTimeout();}
        
        sendSignal(ETX);
        while(status != OK_CONTINUE) {watchTimeout();}
    }
    
    /**
     *  Opens serial communication.
     * 
     * @throws exceptions.ConnectionTimeoutException
     * @see SCOM Protocol
     * @throws exceptions.IncompatibleProtocolVersionException
     */
    public void open() throws IncompatibleProtocolVersionException, ConnectionTimeoutException
    {
        port.openPort();
        port.addDataListener(this);
        
        sendSignal(SOH);
        do {watchTimeout();} while(status != OK_CONTINUE);
        
        sendSignal(VERSION);
        do {watchTimeout();} while(status != OK_CONTINUE);
        
        status = WAITING_FOR_SLAVE_SIGNAL;
        do {watchTimeout();} while(status != OK_CONTINUE);
        
        if (lastSlaveSignal != VERSION)
        {
            sendSignal(EOT);
            while(status != OK_CONTINUE) {watchTimeout();}
            throw new exceptions.IncompatibleProtocolVersionException();
        }
        else
        {
            sendSignal(ETB);
            while(status != OK_CONTINUE) {watchTimeout();}
        }
    }
    
    /**
     *  Closes serial communication.
     */
    public void close()
    {
        status = COMMUNICATION_ENDED;
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
        //System.out.println("Sent: "+Arrays.toString(b));
        port.writeBytes(b, 2);
    }
    
    /**
     * Sends a string over serial.
     * 
     * @param str 
     */
    private void writeInfoMessage(String str)
    {
        messageId = System.nanoTime();
        byte[] buff = str.getBytes(charset);
        this.port.writeBytes(buff, buff.length);
    }
    
    /**
     * Sends a signal to the Slave.
     * 
     * @see SCOM1204
     * @param i The signal identifier.
     */
    private void sendSignal(short i)
    {
       lastSignal = i;
       messageId =  System.nanoTime();
       status = WAITING_FOR_SLAVE_RESPONCE;
       awaitedSum = calculateSum(Integer.toString(i));
       writeInt16(i);
       System.out.println("Sent signal: ["+i+"]");
    }

    @Override
    public int getListeningEvents() {
        return SerialPort.LISTENING_EVENT_DATA_RECEIVED;
    }

    @Override
    public void serialEvent(SerialPortEvent event) {
        System.out.println("Received: "+Arrays.toString(event.getReceivedData()));
        
        if (event.getReceivedData().length > 1)
        {
            if (status == WAITING_FOR_SLAVE_RESPONCE)
            {
                // Clear buffer by reading all bytes
                ByteBuffer cb = ByteBuffer.allocate(event.getReceivedData().length);
                cb.put(event.getReceivedData());
                
                // Take first two bytes and compile a signal
                ByteBuffer bb = ByteBuffer.allocate(2);
                bb.put(cb.get(0));
                bb.put(cb.get(1));
                
                short responce = bb.getShort(0);
                System.out.println("Recived responce: ["+responce+"]");
                
                if (responce == ACK)
                {
                    status = OK_CONTINUE;
                    System.out.println("SUCCESS");
                    return;
                }

                if (responce == awaitedSum)
                {
                    // Send ACK signal
                    writeInt16(ACK);
                    messageId = System.nanoTime();
                    status = OK_CONTINUE;
                    failedAttempts = 0;
                    System.out.println("SUCCESS");
                }
                else
                {
                    // Kill current timeout handling function
                    status = RECOVERING_WRONG_SLAVE_RESPONCE;
                    try
                    {
                        Thread.sleep(20);
                    }
                    catch (InterruptedException ex)
                    {
                        Logger.getLogger(Master.class.getName()).log(Level.SEVERE, null, ex);
                    }
                    status = WAITING_FOR_SLAVE_RESPONCE;
                    
                    if (failedAttempts < MAX_ATTEMPTS)
                    {
                        if (lastSignal != NONE) // NONE if last message was an info message.
                        {
                            writeInt16(lastSignal);
                        }
                        else
                        {
                            writeInfoMessage(lastCommand.toString());
                        }
                        
                        // Run timeout handler
                        try
                        {
                            watchTimeout();
                        }
                        catch (ConnectionTimeoutException ex)
                        {
                            Logger.getLogger(Master.class.getName()).log(Level.SEVERE, null, ex);
                        }
                        
                        failedAttempts++;
                    }
                    else
                    {
                        close();
                    }
                }
            }
            else if (status == WAITING_FOR_SLAVE_SIGNAL)
            {
                // Parse recived bytes
                ByteBuffer bb = ByteBuffer.allocate(2);
                bb.put(event.getReceivedData()[0]);
                bb.put(event.getReceivedData()[1]);
                short signal = bb.getShort(0);
                
                lastSlaveSignal = signal;
                
                // Send responce
                lastSignal = (short) calculateSum(Short.toString(signal));
                messageId =  System.nanoTime();
                writeInt16(lastSignal);
                System.out.println(lastSignal);
                status = WAITING_FOR_SLAVE_RESPONCE;
            }
            else
            {
                // Read signal
                ByteBuffer bb = ByteBuffer.allocate(2);
                bb.put(event.getReceivedData()[0]);
                bb.put(event.getReceivedData()[1]);
                short signal = bb.getShort(0);
                
                // If sent within timeout buffer
                if (System.nanoTime() <= messageId + 250)
                {
                    if (signal == awaitedSum)
                    {
                        // Send ACK signal
                        writeInt16(ACK);
                        messageId = System.nanoTime();
                        status = OK_CONTINUE;
                        failedAttempts = 0;
                        System.out.println("SUCCESS");
                    }
                }
            }
        }
    }

    /**
     *  Handels timeouts and other time based operations
     * 
     * @throws exceptions.ConnectionTimeoutException
     */
    private void watchTimeout() throws ConnectionTimeoutException
    {
        long tracedMessage = messageId;
        for (int i = 0; i < MAX_ATTEMPTS; i++)
        {
            // Wait one second, but constantly check for responce meanwhile
            for (int x = 0; x < 100; x++)
            {
                try
                {
                    Thread.sleep(10);
                }
                catch (InterruptedException e)
                {
                    Logger.getLogger(Master.class.getName()).log(Level.SEVERE, "Error sleeping thread.");
                }

                // Return if no longer waiting for responce
                if (status != WAITING_FOR_SLAVE_RESPONCE)
                {
                    return;
                }
            }
            
            // Resend message
            if (tracedMessage == messageId)
            {
                if (lastSignal == NONE)
                {
                    writeInfoMessage(lastCommand.toString());
                }
                else
                {
                    sendSignal(lastSignal);
                }
                tracedMessage = messageId;
            }
            else
            {
                // Return if already moved onto new message
                return;
            }
        }
        throw new ConnectionTimeoutException();
    }
}
