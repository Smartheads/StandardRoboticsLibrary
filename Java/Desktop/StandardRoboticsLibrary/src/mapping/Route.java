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
import java.util.ArrayList;
import math.Angle;
import math.Vector;

/**
 *  Route.java - Class for dealing with a route constructed with pathfinder.
 * 
 * @since 2019.10.25.
 * @author deaxuser - Robert Hutter
 */
public class Route {
    Vector[] path;
    
    /**
     *  Creates a new route from a node array.
     * 
     * @param nodes
     */
    public Route (Node[] nodes)
    {
        path = new Vector[nodes.length -1];
        for (int i = 0; i < nodes.length - 1; i++)
        {
            path[i] = new Vector(nodes[i].getX(), nodes[i].getY(), nodes[i].getZ(), nodes[i+1].getX(), nodes[i+1].getY(), nodes[i+1].getZ());
        }
    }
    
    /**
     * Creates a new route from a vector array.
     * 
     * @param path
     */
    public Route (Vector[] path)
    {
        this.path = path;
    }
    
    /**
     * Returns the displacement of the route. 
     * 
     * @return
     */
    public double getDisplacement()
    {
        return Vector.addition(path).getLength();
    }
    
    /**
     *  Returns the routes total distance.
     * 
     * @return
     */
    public double getDistance()
    {
        double d = 0.0;
        for (Vector v : path)
        {
            d += v.getLength();
        }
        return d;
    }
    
    public Vector[] getPath()
    {
        return this.path;
    }
    
    /**
     *  Returns an array of commands.
     * 
     * @return
     */
    public Command[] getCommands()
    {
        // Generate list of commands
        ArrayList<Command> commands = new ArrayList<>();
        Angle dir = new Angle(0);
            
        for (Vector v : path) {
            if (v.getX2() > v.getX1())
            {
                if (dir.size() != 90)
                {
                    commands.add(new Command("turnTo", "90"));
                   dir.setAngle(90);
                }
            }
            else if (v.getX2() < v.getX1())
            {
                if (dir.size() != 270)
                {
                    commands.add(new Command("turnTo", "270"));
                    dir.setAngle(270);
                }
            }

            double xM = Math.abs(v.getX1()-v.getX2());
            if (xM > 0)
                commands.add(new Command("forward", Double.toString(xM)));

            if (v.getY2() > v.getY1())
            {
                if(dir.size() != 180)
                {
                    commands.add(new Command("turnTo", "180"));
                    dir.setAngle(180);
                }
            }
            else if (v.getY2() < v.getY1())
            {
                if (dir.size() != 0)
                {
                    commands.add(new Command("turnTo", "0"));
                    dir.setAngle(0);
                }
            }

            double yM = Math.abs(v.getY1()-v.getY2());
            if (yM > 0)
                commands.add(new Command("forward", Double.toString(yM)));
        }
        
        return simplifyCommands(commands.toArray(new Command[commands.size()]));
    }
    
    /**
     * Simplifies a command array. I.e. forward(1.0); and forward(1.0) will become
     * forward(2.0);
     * 
     * @param c Command array to simplify.
     * @return 
     */
    private Command[] simplifyCommands(Command[] c)
    {
        ArrayList<Command> sc = new ArrayList<>();
        boolean done = true;
        
        for (int i = 0; i < c.length - 1; i++)
        {
            if (c[i].getBody().equals(c[i+1].getBody()) && c[i].getBody().equals("forward"))
            {
                double arg1 = Double.parseDouble(c[i].getArg(0));
                double arg2 = Double.parseDouble(c[i+1].getArg(0));
                sc.add(new Command(c[i].getBody(), Double.toString(arg1+arg2)));
                i++;
                done = false;
            }
            else
            {
                sc.add(c[i]);
            }
            
            if (i == c.length - 2)
            {
                sc.add(c[i+1]);
            }
        }
        
        if (c.length == 1)
        {
            sc.add(c[0]);
        }
        
        if (done)
        {
            return sc.toArray(new Command[sc.size()]);
        }
        
        return this.simplifyCommands(sc.toArray(new Command[sc.size()]));
    }
}
