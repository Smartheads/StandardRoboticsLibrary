
import java.nio.ByteBuffer;

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
/**
 *
 * @author deaxuser
 */
public class Bytes {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        int i = 100;
        byte[] bytes = ByteBuffer.allocate(4).putInt(i).array();
        
        byte[] x = new byte[2];
        x[0] = bytes[2];
        x[1] = bytes[3];
        
        
        
        for (byte k : x)
        {
            System.out.println(new Byte(k).intValue());
        }
    }
    
}
