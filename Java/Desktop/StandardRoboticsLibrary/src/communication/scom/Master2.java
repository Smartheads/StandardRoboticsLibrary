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
import com.fazecast.jSerialComm.SerialPortDataListener;
import com.fazecast.jSerialComm.SerialPortEvent;
import java.nio.charset.Charset;

/**
 *  Erstellt laut des SCOM 2.0 Protokolls 1204.
 * 
 * @author Robert Hutter
 */
public class Master2 implements Runnable, SerialPortDataListener
{
    SerialPort port;
    private final Charset charset;
    private short messageId = 0;
    private volatile byte status = SCOM2.OK_CONTINUE;
    private volatile Signal lastSentSignal;
    private volatile short awaitedSum;
    
    private volatile long ACK_signal_sent_at;
    private volatile long ANT_signal_sent_at;
    
    /**
     * 
     * @param port
     * @param baud
     * @param charset 
     */
    public Master2(SerialPort port, int baud, String charset)
    {
        this.port = port;
        this.port.setBaudRate(baud);
        this.charset = Charset.forName(charset);
    }
    
    /**
     * 
     * @param port
     * @param baud
     * @param charset 
     */
    public Master2(String port, int baud, String charset)
    {
        this(SerialPort.getCommPort(port), baud, charset);
    }
    
    /**
     * 
     * @param port
     * @param baud 
     */
    public Master2(String port, int baud)
    {
        this(port, baud, "ASCII");
    }
    
    /**
     * 
     * @param port 
     */
    public Master2(String port)
    {
        this(port, 115200);
    }
    
    /**
     * Opens the Serial port and starts communication.
     */
    @Override
    public void run()
    {
        port.openPort();
        port.addDataListener(this);
        
        sendSignal(SCOM2.SOH);
        while(status != SCOM2.OK_CONTINUE) {updateSCOM();}
    }
    
    /**
     * 
     */
    void updateSCOM(SerialPortEvent event)
    {
        // Check if new data has arrived
        if(event != null)
        {
            // New data has arrived
            if (event.getReceivedData().length >= Signal.length ||
                    (status == SCOM2.WAITING_FOR_INFO_MESSAGE && event.getReceivedData().length > 0)
                )
            {
                Signal recievedSignal = null;
                
                if (status != SCOM2.WAITING_FOR_INFO_MESSAGE)
                {
                    recievedSignal = SCOM2.readSignal(event);
                }
                
                // A message has arrived
                switch (status)
                {
                    case SCOM2.WAITING_FOR_ASCII_SUM:
                        // If ASCII sum is correct
                        if (recievedSignal.getMessage() == awaitedSum)
                        {
                            sendSignal(SCOM2.ACK);
                        }
                        break;
                        
                    case SCOM2.IN_ACK_TIMEOUT_BUFFER:
                        if (recievedSignal.getMessage() == SCOM2.ABF)
                        {
                            // Deal with ACK timeout
                            sendSignal(SCOM2.ANT);
                        }
                        break;
                        
                    default:
                        break;
                }
            }
        }
        else
        {
            switch(status)
            {
                case SCOM2.IN_ACK_TIMEOUT_BUFFER:
                    if (System.currentTimeMillis() >= ACK_signal_sent_at + SCOM2.ACK_WAIT)
                    {
                        // ACK recived, set status to ok_continue
                        status = SCOM2.OK_CONTINUE;
                    }
                break;
                
                case SCOM2.WAITING_FOR_MORE_ABF:
                    if (System.currentTimeMillis() >= ANT_signal_sent_at + SCOM2.REFRESH_INTERVAL)
                    {
                        // Resend original message
                        sendSignal(lastSentSignal.getMessage());
                    }
                break;
                
                default:
                break;
            }
        }
    }
    
    /**
     * 
     */
    void updateSCOM()
    {
        updateSCOM(null);
    }
    
    /**
     * Sends a signal.
     * 
     * @param message 
     */
    private void sendSignal(short message)
    {
        switch (message)
        {
            case SCOM2.ACK:
                SCOM2.writeSignal(new Signal(messageId++, message), port);
                status = SCOM2.IN_ACK_TIMEOUT_BUFFER;
                ACK_signal_sent_at = System.currentTimeMillis();
            break;
            
            case SCOM2.ANT:
                SCOM2.writeSignal(new Signal(messageId++, message), port);
                status = SCOM2.WAITING_FOR_MORE_ABF;
                ANT_signal_sent_at = System.currentTimeMillis();
            break;
            
            default:
                lastSentSignal = new Signal(messageId++, message);
                SCOM2.writeSignal(lastSentSignal, port);
                status = SCOM2.WAITING_FOR_ASCII_SUM;
                awaitedSum = lastSentSignal.getSum(charset);
            break;
        }
    }

    /**
     * 
     * @param event
     */
    @Override
    public void serialEvent(SerialPortEvent event)
    {
        this.updateSCOM(event);
    }
    
    @Override
    public int getListeningEvents()
    {
        return SerialPort.LISTENING_EVENT_DATA_RECEIVED;
    }
}
