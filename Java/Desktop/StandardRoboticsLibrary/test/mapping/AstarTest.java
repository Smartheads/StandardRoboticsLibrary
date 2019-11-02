/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package mapping;

import exceptions.NoRouteFoundException;
import exceptions.UnknownAlgorithmException;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author deaxuser
 */
public class AstarTest {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        CharMap map = new CharMap(100,100, 'Y', 'X');
        map.setPoint(1, 2, map.getObsticalMark());
        
        try
        {
            Node[] path = Pathfinder.constructPath("a*", map, new Node(1, 1), new Node(10, 10));
            
            for (Node n : path)
            {
                System.out.println("["+n.getX()+","+n.getY()+"]");
            }
            
            Route r = new Route(path);
            System.out.println("Route distance: " + r.getDistance() + " cm");
            System.out.println("Route displacement: " + r.getDisplacement() + " cm");
        }
        catch (UnknownAlgorithmException | NoRouteFoundException ex)
        {
            Logger.getLogger(AstarTest.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
}
