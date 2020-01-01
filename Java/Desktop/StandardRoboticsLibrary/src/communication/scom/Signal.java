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

import java.nio.ByteBuffer;
import java.nio.charset.Charset;

/**
 * Erstellt laut des SCOM 2.0. Protokolls 1204.
 * 
 * Signal.java - Class for an SCOM Signal.
 * 
 * @author Robert Hutter
 */
public final class Signal
{
    private short id;
    private short message;
    public static final int length = 4;
    public final long createdAt;
    
    {
        createdAt = System.currentTimeMillis();
    }
    
    /**
     * 
     * @param id
     * @param message 
     */
    public Signal(short id, short message)
    {
        this.id = id;
        this.message = message;
    }
    
    /**
     * 
     * @param sig 
     */
    public Signal(Signal sig)
    {
        this.id = sig.getId();
        this.message = sig.getMessage();
    }
    
    /**
     * 
     * @param charset
     * @return 
     */
    public short getSum(Charset charset)
    {
        return SCOM.calculateSum(message, charset);
    }
    
    /**
     * Returns the Signal as a byte array.
     * 
     * @return 
     */
    public byte[] getBytes()
    {
        ByteBuffer idb = ByteBuffer.allocate(4).putShort(this.message).putShort(this.id);
        return idb.array();
    }

    public short getId() {
        return id;
    }

    public void setId(short id) {
        this.id = id;
    }

    public short getMessage() {
        return message;
    }

    public void setMessage(short message) {
        this.message = message;
    }
}
