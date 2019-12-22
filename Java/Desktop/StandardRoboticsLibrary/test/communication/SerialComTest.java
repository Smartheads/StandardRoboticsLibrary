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
package communication;

import communication.scom.Command;
import communication.scom.Master;
import exceptions.ConnectionTimeoutException;
import exceptions.IncompatibleProtocolVersionException;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author deaxuser
 */
public class SerialComTest {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        Master master = new Master("ttyACM0", 115200, "ascii");
        try {
            master.open();
            master.sendCommand(new Command("helloWorld"));
        } catch (IncompatibleProtocolVersionException | ConnectionTimeoutException ex) {
            Logger.getLogger(SerialComTest.class.getName()).log(Level.SEVERE, null, ex);
            System.exit(1);
        }
    }
}
