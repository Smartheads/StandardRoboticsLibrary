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

import java.util.ArrayList;
import java.util.List;

/**
 *  Node.java - A traversable point on a map.
 * 
 * @since 2018.06.24
 * @author deaxuser - Robert Hutter
 */
public class Node extends math.Coord
{
    protected Node parent;
    protected double f;
    protected double g;
    protected double cost;

    /**
     * Constructor for creating a new Node2D.
     * 
     * @param x Position x
     * @param y Position y
     */
    public Node (int x, int y)
    {
        this(x, y, 1);
    }
    
    /**
     *  Creates a new Node.
     * 
     * @param x
     * @param y
     * @param cost
     */
    public Node (int x, int y, int cost)
    {
        this(x, y, 1, cost);
    }
    
    /**
     * Creates a new Node.
     * 
     * @param x
     * @param y
     * @param z
     * @param cost
     */
    public Node (int x, int y, int z, int cost)
    {
        super(x, y, z);
        this.cost = cost;
        this.g = cost;
    }
    
    /**
     *  Return Node2D array of neighbors,that are traversable.
     * 
    * @param map
    * @return 
     */
    public Node[] getNeighbors(Map2D map)
    {
        List<Node> neighbors = new ArrayList<>();

        /* XOX
         * XXX
         * XXX
         */
        if (this.getY() - 1 >= 1)
        {
            if (map.isTraversable(this.getX(), this.getY() - 1))
            {
                neighbors.add(new Node (this.getX(), this.getY() - 1));
            }
        }
        
        /* XXX
         * XXX
         * XOX
         */
        if (map.getHeight() >= this.getY() + 1)
        {
            if (map.isTraversable(this.getX(), this.getY() + 1))
            {
                neighbors.add(new Node(this.getX(), this.getY() + 1));
            }
        }

        /* XXX
         * XXO
         * XXX
         */
        if (map.getWidth() >= this.getX() + 1)
        {
            if (map.isTraversable(this.getX() + 1, this.getY()))
            {
                neighbors.add(new Node(this.getX() + 1, this.getY()));
            }
        }

        /* XXX
         * OXX
         * XXX
         */
        if (this.getX() - 1 >= 1)
        {
            if (map.isTraversable(this.getX() - 1, this.getY()))
            {
                neighbors.add(new Node(this.getX() - 1, this.getY()));
            }
        }

        neighbors.forEach(node -> node.setParent(this));

        return neighbors.toArray(new Node[neighbors.size()]);
    }
    
    /**
     * Compare two nodes
     * 
     * @param o Node2D to compare
     * @return
     */
    public boolean equals (Node o)
    {
        return (o.getX() == this.getX()) && (o.getY() == this.getY());
    }
    
    /**
     * Get the node with the lowest F from a collection.
     * 
     * @param list Collection of nodes to check
     * @return
     */
    public static Node getLowestF(Node[] list)
    {
        Node i = list[0];
        for (Node n : list)
        {
            if (n.getF() < i.getF())
            {
                i = n;
            }
        }

        return i;
    }
    
    /**
     * Returns index of node in list. (see @param) Returns -1 if node not contained within list.
     * 
     * @param list Collection of Nodes
     * @param node Node to look for
     * @return 
     */
    public static int containsNodeIndex (Node[] list, Node node)
    {
        for (int i = 0; i < list.length; i++)
        {
            if (node.equals(list[i]))
            {
                return i;
            }
        }
        
        return -1;
    }

    /**
     * Check to see if a collection contains a node or not.
     * 
     * @param list Collection of Nodes
     * @param node Node2D to look for
     * @return
     */
    public static boolean containsNode (Node[] list, Node node)
    {
        return containsNodeIndex (list, node) != -1;
    }

    /**
     *
     * @return
     */
    public Node getParent() {
            return parent;
    }

    /**
     *
     * @param parent
     */
    public void setParent(Node parent) {
            this.parent = (Node) parent;
    }

    /**
     *
     * @return
     */
    public double getF() {
            return f;
    }

    /**
     *
     * @param f
     */
    public void setF(double f) {
            this.f = f;
    }

    /**
     *
     * @return
     */
    public double getG() {
            return g;
    }

    /**
     *
     * @param g
     */
    public void setG(double g) {
            this.g = g;
    }

    /**
     *
     * @return
     */
    public double getCost() {
            return cost;
    }

    /**
     *
     * @param cost
     */
    public void setCost(int cost) {
            this.cost = cost;
    }

    @Override
    public int getZ() {
        return 1;
    }
}
