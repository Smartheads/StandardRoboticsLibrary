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
import exceptions.ConnectionClosedException;
import exceptions.ConnectionTimeoutException;
import exceptions.IncompatibleProtocolVersionException;
import java.nio.charset.Charset;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *  Erstellt laut des SCOM 2.0 Protokolls 1204.
 * 
 * @author Robert Hutter
 */
public class Master implements Runnable, SerialPortDataListener
{
    SerialPort port;
    private final Charset charset;
    private short messageId = 0;
    private volatile byte status = SCOM.OK_CONTINUE;
    private volatile Signal lastSentSignal;
    private volatile Signal lastRecievedSignal = null; // Confirmed signal
    private volatile short awaitedSum;
    private volatile byte ABF_attempts = 0;
    
    private volatile long ACK_signal_sent_at;
    private volatile long ABF_signal_sent_at;
    //private volatile long ANT_signal_sent_at;
    private volatile long ANT_signal_recieved_at;
    private volatile long sum_sent_at;
    private volatile long lastMessageSentAt;
    
    private volatile boolean setupComplete = false;
    private volatile Exception setupException;
    
    /**
     * 
     * @param port
     * @param baud
     * @param charset 
     */
    public Master(SerialPort port, int baud, String charset)
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
    public Master(String port, int baud, String charset)
    {
        this(SerialPort.getCommPort(port), baud, charset);
    }
    
    /**
     * 
     * @param port
     * @param baud 
     */
    public Master(String port, int baud)
    {
        this(port, baud, "ASCII");
    }
    
    /**
     * 
     * @param port 
     */
    public Master(String port)
    {
        this(port, 115200);
    }
    
    /**
     * 
     * @throws Exception 
     */
    public void waitUntilSetupComplete() throws Exception
    {
        while(!setupComplete)
        {
            try
            {
                Thread.sleep(100);
            }
            catch (InterruptedException ex)
            {
                Logger.getLogger(Master.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
        
        if (status == SCOM.CONNECTION_CLOSED)
            throw setupException;
        
    }
    
    /**
     * Closes the SCOM communication
     */
    public void closeConnection()
    {
        
    }
    
    /**
     * Opens the Serial port and starts communication.
     */
    @Override
    public void run()
    {
        try
        {
            port.openPort();
            port.addDataListener(this);
            
            // Wait for Slave to turn on if activated by opening of serial port
            try
            {
                Thread.sleep(500);
            }
            catch (InterruptedException ex)
            {
                Logger.getLogger(Master.class.getName()).log(Level.SEVERE, null, ex);
            }
            
            // Send SOH
            sendSignal(SCOM.SOH);
            while(status != SCOM.OK_CONTINUE) {updateSCOM();}
            
            // Recieve Slave version
            status = SCOM.WAITING_FOR_SIGNAL;
            while(status != SCOM.OK_CONTINUE || lastRecievedSignal == null) {updateSCOM();}
            short slaveVersion = lastRecievedSignal.getMessage();
            
            // Compare versions
            if (SCOM.VERSION != slaveVersion)
            {
                // Shut down: send EOT
                sendSignal(SCOM.EOT);
                while(status != SCOM.OK_CONTINUE) {updateSCOM();}
                throw new exceptions.IncompatibleProtocolVersionException();
            }
            else
            {
                lastRecievedSignal = null;
                // Send ETX
                sendSignal(SCOM.ETX);
                while(status != SCOM.OK_CONTINUE) {updateSCOM();}
            }
        }
        catch (ConnectionTimeoutException | IncompatibleProtocolVersionException
                | ConnectionClosedException ex
              )
        {
            Logger.getLogger(Master.class.getName()).log(Level.SEVERE, null, ex);
            status = SCOM.CONNECTION_CLOSED;
            setupException = ex;
            port.closePort();
        }
        setupComplete = true;
    }
    
    /**
     * 
     */
    void updateSCOM(SerialPortEvent event) throws ConnectionTimeoutException, ConnectionClosedException
    {
        // Check if new data has arrived
        if(event != null)
        {
            // New data has arrived
            if (event.getReceivedData().length >= Signal.length ||
                    (status == SCOM.WAITING_FOR_INFO_MESSAGE && event.getReceivedData().length > 0)
                )
            {
                Signal recievedSignal = null;
                
                if (status != SCOM.WAITING_FOR_INFO_MESSAGE)
                {
                    recievedSignal = SCOM.readSignal(event);
                    
                    // Preprocess status
                    switch (status)
                    {
                        case SCOM.WAITING_FOR_SIGNAL:
                            status = SCOM.OK_CONTINUE;
                        break;
                        
                        case SCOM.WAITING_FOR_REP_MESSAGE:
                            status = SCOM.OK_CONTINUE;
                        break;
                        
                        case SCOM.WAITING_FOR_MORE_ANT:
                            status = SCOM.OK_CONTINUE;
                        break;
                        
                        default:
                            break;
                    }
                }
                else
                {
                    // TODO: Implement info message
                }
                
                // A message has arrived
                switch (status)
                {
                    // A message has arrived
                    case SCOM.OK_CONTINUE:
                        switch (recievedSignal.getMessage())
                        {
                            case SCOM.ANT:
                                status = SCOM.WAITING_FOR_MORE_ANT;
                                ANT_signal_recieved_at = System.currentTimeMillis();
                            break;
                            
                            case SCOM.ABF:
                                sendSignal(SCOM.ANT);
                            break;
                            
                            default:
                                sendSum(recievedSignal.getSum(charset));
                                lastRecievedSignal = new Signal(recievedSignal);
                            break;
                        }
                    break;
                    
                    // ACK, ABF or resent message has arrived
                    case SCOM.WAITING_FOR_ACK:
                        switch (recievedSignal.getMessage())
                        {
                            case SCOM.ACK:
                                status = SCOM.OK_CONTINUE;
                            break;
                            
                            case SCOM.ABF:
                                sendSignal(SCOM.ANT);
                            break;
                            
                            default:
                                sendSum(recievedSignal.getSum(charset));
                                lastRecievedSignal = new Signal(recievedSignal);
                            break;
                        }
                    break;
                    
                    // Confirmation sum has arrived
                    case SCOM.WAITING_FOR_ASCII_SUM:
                        // If ASCII sum is correct
                        if (recievedSignal.getMessage() == awaitedSum)
                        {
                            lastSentSignal = null;
                            sendSignal(SCOM.ACK);
                        }
                        else
                        {
                            sendSignal(lastSentSignal.getMessage());
                        }
                    break;
                        
                    // ABF has arrived
                    case SCOM.IN_ACK_TIMEOUT_BUFFER:
                        if (recievedSignal.getMessage() == SCOM.ABF)
                        {
                            // Deal with ACK timeout
                            sendSignal(SCOM.ANT);
                        }
                        else
                        {
                // TEMP
                            sendSum(recievedSignal.getSum(charset));
                                lastRecievedSignal = new Signal(recievedSignal);
                        }
                    break;
                    
                    // ANT or ABF has arrived
                    case SCOM.WAITING_FOR_ANT:
                        switch(recievedSignal.getMessage())
                        {
                            case SCOM.ANT:
                                status = SCOM.WAITING_FOR_MORE_ANT;
                                ANT_signal_recieved_at = System.currentTimeMillis();
                            break;
                            
                            case SCOM.ABF:
                                sendSignal(SCOM.ANT);
                            break;
                            
                            default:
                                sendSignal(SCOM.ABF);
                            break;
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
                case SCOM.WAITING_FOR_ACK:
                    if (System.currentTimeMillis() >= sum_sent_at + SCOM.ACK_TIMEOUT)
                    {
                        // Did'nt recieved ACK
                        sendSignal(SCOM.ABF);
                    }
                break;
                
                case SCOM.WAITING_FOR_ASCII_SUM:
                    if (System.currentTimeMillis() >= lastMessageSentAt + SCOM.TIMEOUT)
                    {
                        // Timeout detected
                        sendSignal(SCOM.ABF);
                    }
                break;
                
                case SCOM.IN_ACK_TIMEOUT_BUFFER:
                    if (System.currentTimeMillis() >= ACK_signal_sent_at + SCOM.ACK_WAIT)
                    {
                        // ACK recieved, set status to ok_continue
                        status = SCOM.OK_CONTINUE;
                    }
                break;
                
                case SCOM.WAITING_FOR_ANT:
                    if (System.currentTimeMillis() >= ABF_signal_sent_at + SCOM.ABF_INTERVAL)
                    {
                        // Send new ABF
                        sendSignal(SCOM.ABF);
                    }
                break;
                
                case SCOM.WAITING_FOR_MORE_ANT:
                    if (System.currentTimeMillis() >= ANT_signal_recieved_at + SCOM.REFRESH_INTERVAL)
                    {
                        // All ANTs have arrived
                        ABF_attempts = 0;
                        if (lastSentSignal != null)
                        {
                            sendSignal(lastSentSignal.getMessage()); // Resend last message
                        }
                        else
                        {
                            status = SCOM.WAITING_FOR_REP_MESSAGE;
                        }
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
    void updateSCOM() throws ConnectionTimeoutException, ConnectionClosedException
    {
        updateSCOM(null);
    }
    
    /**
     * Sends a sign_ACK_TIMEOUT_BUFFERal.
     * 
     * @param message 
     */
    private void sendSignal(short message) throws ConnectionTimeoutException
    {
        switch (message)
        {
            case SCOM.ACK:
                status = SCOM.IN_ACK_TIMEOUT_BUFFER;
                ACK_signal_sent_at = System.currentTimeMillis();
                SCOM.writeSignal(new Signal(messageId++, message), port);
            break;
            
            case SCOM.ABF:
                if (ABF_attempts++ >= SCOM.MAX_ABF_ATTEMPTS)
                {
                    throw new exceptions.ConnectionTimeoutException();
                }
                status = SCOM.WAITING_FOR_ANT;
                ABF_signal_sent_at = System.currentTimeMillis();
                SCOM.writeSignal(new Signal(messageId++, message), port);
            break;
            
            case SCOM.ANT:
                if (lastRecievedSignal != null)
                {
                    status = SCOM.WAITING_FOR_REP_MESSAGE;
                }
                else
                {
                    status = SCOM.WAITING_FOR_MORE_ANT;
                    ANT_signal_recieved_at/*ANT_signal_sent_at*/ = System.currentTimeMillis();
                }
                SCOM.writeSignal(new Signal(messageId++, message), port);
            break;
            
            default:
                lastSentSignal = new Signal(messageId++, message);
                status = SCOM.WAITING_FOR_ASCII_SUM;
                awaitedSum = lastSentSignal.getSum(charset);
                lastMessageSentAt = lastSentSignal.createdAt;
                SCOM.writeSignal(lastSentSignal, port);
            break;
        }
    }
    
    /**
     * Sends an ASCII sum over serial.
     * 
     * @param sum 
     */
    private void sendSum(short sum)
    {
        status = SCOM.WAITING_FOR_ACK;
        sum_sent_at = System.currentTimeMillis();
        SCOM.writeSignal(new Signal(messageId++, sum), port);
    }

    /**
     * 
     * @param event
     */
    @Override
    public void serialEvent(SerialPortEvent event)
    {
        try
        {
            this.updateSCOM(event);
        }
        catch (ConnectionTimeoutException | ConnectionClosedException ex)
        {
            Logger.getLogger(Master.class.getName()).log(Level.SEVERE, null, ex);
            status = SCOM.CONNECTION_CLOSED;
            setupComplete = true;
            port.closePort();
        }
    }
    
    @Override
    public int getListeningEvents()
    {
        return SerialPort.LISTENING_EVENT_DATA_RECEIVED;
    }
}
