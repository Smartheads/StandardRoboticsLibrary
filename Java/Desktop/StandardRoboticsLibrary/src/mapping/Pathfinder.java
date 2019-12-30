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

import exceptions.NoRouteFoundException;
import exceptions.UnknownAlgorithmException;
import java.util.ArrayList;
import java.util.List;

/**
 *  Pathfinder.java - Help construct pathes on maps.
 * 
 * @since 2019.10.25.
 * @author deaxuser - Robert Hutter
 */
public final class Pathfinder
{

    /**
     *  Constructs a new route from Node a, to Node b.
     * 
     * 
     * @param alg Algorithm to use.
     *              Supported algorithms:
     *                  - "A*": A* algorithm
     * @param map Map to construct path on
     * @param start Start node
     * @param goal Goal/end node
     * @return
     * @throws exceptions.UnknownAlgorithmException
     * @throws exceptions.NoRouteFoundException
     */
    public static Route constructRoute(String alg, Map2D map, Node start, Node goal) throws UnknownAlgorithmException, NoRouteFoundException
    {
        return new Route(constructPath(alg, map, start, goal));
    }
    
    /**
     *
     * Constructs a path on a 2D plane from the start node to the goal node.
     * 
     * @param alg Algorithm to use.
     *              Supported algorithms:
     *                  - "A*": A* algorithm
     * @param map Map to construct path on
     * @param start Start node
     * @param goal Goal/end node
     * @return
     * @throws exceptions.UnknownAlgorithmException
     * @throws exceptions.NoRouteFoundException
     */
    public static Node[] constructPath(String alg, Map2D map, Node start, Node goal) throws UnknownAlgorithmException, NoRouteFoundException
    {
        if (!map.isTraversable(goal.getX(), goal.getY()))
        {
            throw new NoRouteFoundException();
        }
        
        if (alg.toLowerCase().equals("a*"))
        {
            // Pathfind using A* algorithm
            List<Node> openList = new ArrayList<>();
            List<Node> closedList = new ArrayList<>();

            start.setG(0);
            start.setF(start.getG() + getHeuristic(start, goal));
            openList.add(start);

            while (!openList.isEmpty())
            {
                Node current = Node.getLowestF(openList.toArray(new Node[openList.size()]));

                if (goal.equals(current))
                {
                    try
                    {
                        goal.setParent(closedList.get(closedList.size() - 1));
                    }
                    catch (IndexOutOfBoundsException e)
                    {
                        throw new NoRouteFoundException();
                    }
                    
                    return  recontructPath(goal);
                }

                openList.remove(current);
                closedList.add(current);

                for (Node n : current.getNeighbors(map))
                {
                    if (!Node.containsNode (closedList.toArray(new Node[closedList.size()]), n))
                    {
                        n.setF(n.getG() + getHeuristic(n, goal));

                        if (!Node.containsNode(openList.toArray(new Node[openList.size()]), n))
                        {
                            openList.add(n);
                        }
                        else
                        {
                            Node openNeighbor;
                            openNeighbor = openList.get(Node.containsNodeIndex(
                                    openList.toArray(new Node[openList.size()]), n));
                            if (n.getG() < openNeighbor.getG())
                            {
                                openNeighbor.setG(n.getG());
                                openNeighbor.setParent(n.getParent());
                            }
                        }
                    }
                }
            }
            
            // No route found
            throw new exceptions.NoRouteFoundException();
        }
        else
        {
            throw new exceptions.UnknownAlgorithmException(alg);
        }
    }
    
    /**
     * Construct a path after using a pathfinder algorithm.
     * Retraces the route through the last element.
     * 
     * @param goal The last node in a pathfinder algorithm
     * @return
     */
    private static Node[] recontructPath(Node goal)
    {
        // Retrace steps
        List<Node> path = new ArrayList<>();
        
        Node n = goal;
        while (n != null)
        {
            path.add(n);
            n = n.getParent();
        }
        
        // Flip route
        Node[] flipped = new Node[path.size()];
        
        for (int i = path.size(); i > 0; i--)
        {
            flipped[path.size() - i] = path.get(i - 1);
        }

        return flipped;
    }
    
    /**
     * Estimates the heuristic of the node with the Manhattan method.
     * 
     * @param start
     * @param goal
     * @return
     */
    private static int getHeuristic(Node start, Node goal)
    {
        return Math.abs(start.getX() - goal.getX()) + Math.abs(start.getY() - goal.getY()) + Math.abs(start.getZ() - goal.getZ());
    }
}
