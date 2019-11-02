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
package mapping;

import communication.scom.Command;
import exceptions.NoRouteFoundException;
import exceptions.UnknownAlgorithmException;
import java.io.File;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;
import math.Angle;
import math.Vector;

/**
 *
 * @author deaxuser
 */
public class RouteTest {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        CharMap map = new CharMap(1000,1000, 'Y', 'X');
        map.setPointSquare(1, 1, 5, map.getObsticalMark());
        map.setPoint(3, 3, map.getEmptySpaceMark());
        map.setPoint(4, 3, map.getEmptySpaceMark());
        map.setPoint(5, 3, map.getEmptySpaceMark());
        map.setPoint(6, 3, map.getEmptySpaceMark());
        
        try {
            map.exportToFile(new File("/home/deaxuser/Desktop/Terkep.map"));
        } catch (IOException ex) {
            Logger.getLogger(RouteTest.class.getName()).log(Level.SEVERE, null, ex);
        }
        
        try
        {
            Node[] path = Pathfinder.constructPath("a*", map, new Node(3, 3), new Node(100, 100));
            
            for (Node n : path)
            {
                //System.out.println("["+n.getX()+","+n.getY()+"]");
            }
            
            Route r = new Route(path);
            System.out.println("Route distance: " + r.getDistance() + " cm");
            System.out.println("Route displacement: " + r.getDisplacement() + " cm");
            
            for (Vector k : r.getPath())
            {
                //System.out.println("["+k.getX1()+","+k.getY1()+"]["+k.getX2()+","+k.getY2()+"]");
            }
            
            System.out.println("Commands:");
            Command[] cms = r.getCommands();
            
            for (Command c : cms)
            {
                System.out.println(c.toString());
            }
        }
        catch (UnknownAlgorithmException | NoRouteFoundException ex)
        {
            Logger.getLogger(AstarTest.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public static void turnTo(Angle size)
    {
        System.out.println("turnTo("+size.size()+")");
    }
    
}
